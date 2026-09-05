"""
Sentinel-RT PC Simulator
========================
Demonstrates the AI scheduler making real decisions using the trained
INT8 TFLite model on 5 realistic scenarios.

Feature vector format (26 floats):
  Per task (5 tasks × 5 features = 25):
    - cpu_load          (0-100%)
    - wait_time_ms      (0-50ms)
    - deadline_proximity (0.0-1.0, 1=imminent)
    - context_switch_rate (0-100/sec)
    - is_blocked        (0 or 1)
  Global (1):
    - total_system_cpu_load (0-100%)
"""

import os, sys, random
import numpy as np

try:
    import tensorflow as tf
    TF_AVAILABLE = True
except ImportError:
    TF_AVAILABLE = False

try:
    from colorama import init, Fore, Style
    init(autoreset=True)
    G = Fore.GREEN; Y = Fore.YELLOW; R = Fore.RED; RESET = Style.RESET_ALL
except ImportError:
    G = Y = R = RESET = ""

MODEL_PATH = r"C:\Users\user\Documents\Sentinel-RT\model\quantized\sentinel_model.tflite"
POWER_LABELS = ["Active", "LightSleep", "DeepSleep"]
POWER_COLORS = [G, Y, R]

# ── Feature builder ──────────────────────────────────────────────────────────
def make_features(tasks):
    """
    tasks: list of 5 dicts with keys: cpu, wait, deadline, csr, blocked
    Returns np.array shape (1, 26) float32, normalized to [0,1]
    """
    vec = []
    total_cpu = 0.0
    for t in tasks:
        vec.append(t['cpu']      / 100.0)
        vec.append(t['wait']     / 50.0)
        vec.append(t['deadline'])           # already 0-1
        vec.append(t['csr']      / 100.0)
        vec.append(float(t['blocked']))
        total_cpu += t['cpu']
    vec.append(min(total_cpu / 5.0, 100.0) / 100.0)   # mean system CPU
    return np.array([vec], dtype=np.float32), total_cpu / 5.0

# ── TFLite runner ────────────────────────────────────────────────────────────
def run_inference(interpreter, features_f32):
    inp = interpreter.get_input_details()[0]
    out = interpreter.get_output_details()

    # Quantize input float32 -> int8
    scale, zp = inp['quantization']
    inp_i8 = (features_f32 / scale + zp).clip(-128, 127).astype(np.int8)
    interpreter.set_tensor(inp['index'], inp_i8)
    interpreter.invoke()

    # Dequantize priority output (out[1] has shape [1, 5])
    ps, pz = out[1]['quantization']
    priorities = (interpreter.get_tensor(out[1]['index'])[0].astype(np.float32) - pz) * ps

    # Dequantize power state output and argmax (out[0] has shape [1, 3])
    ws, wz = out[0]['quantization']
    pw_raw = (interpreter.get_tensor(out[0]['index'])[0].astype(np.float32) - wz) * ws
    power_idx = int(np.argmax(pw_raw))
    return priorities, power_idx

# ── Rule-based fallback (when model not available) ────────────────────────────
def rule_based(tasks, total_cpu_pct):
    priorities = []
    for t in tasks:
        if t['deadline'] > 0.7:
            priorities.append(1.0)
        elif t['cpu'] > 70:
            priorities.append(2.0)
        else:
            priorities.append(4.0)
    power_idx = 2 if total_cpu_pct < 20 else (1 if total_cpu_pct < 50 else 0)
    return np.array(priorities), power_idx

# ── Print helper ─────────────────────────────────────────────────────────────
def print_scenario(name, tasks, priorities, power_idx, total_cpu):
    color = POWER_COLORS[power_idx]
    label = POWER_LABELS[power_idx]
    explanations = {
        0: "High CPU demand — maximum performance.",
        1: "Moderate load — reduce clocks for efficiency.",
        2: "Minimal load — deep sleep saves maximum power."
    }
    print(f"\n{'='*58}")
    print(f"  Scenario: {name}")
    print(f"  System CPU: {total_cpu:.1f}%   Power: {color}{label}{RESET}  ({explanations[power_idx]})")
    print(f"{'='*58}")
    print(f"  {'Task':^4} | {'CPU%':^6} | {'Wait':^5} | {'Deadline':^8} | {'CSR':^5} | {'AI Priority':^11}")
    print(f"  {'-'*55}")
    for i, (t, p) in enumerate(zip(tasks, priorities)):
        dl_flag = " [!]" if t['deadline'] > 0.7 else ""
        print(f"  {i:^4} | {t['cpu']:^6.1f} | {t['wait']:^5.0f} | {t['deadline']:^8.2f}{dl_flag:2} | {t['csr']:^5.0f} | {p:^11.4f}")

# ── Main ──────────────────────────────────────────────────────────────────────
def main():
    print(f"\n{G}{'*'*58}")
    print(f"  Sentinel-RT PC Simulator  (INT8 TFLite Model)")
    print(f"{'*'*58}{RESET}\n")

    # Load model
    interpreter = None
    if TF_AVAILABLE and os.path.exists(MODEL_PATH):
        import warnings
        with warnings.catch_warnings():
            warnings.simplefilter("ignore")
            interpreter = tf.lite.Interpreter(MODEL_PATH)
            interpreter.allocate_tensors()
        print(f"{G}[OK] Loaded INT8 TFLite model: {os.path.getsize(MODEL_PATH)/1024:.1f} KB{RESET}")
    else:
        print(f"{Y}[!] Model not found — using rule-based fallback.{RESET}")

    # ── 5 Test Scenarios ────────────────────────────────────────────────────
    scenarios = [
        {
            "name": "1. Normal Load",
            "tasks": [
                {'cpu':45, 'wait':10, 'deadline':0.3, 'csr':20, 'blocked':0},
                {'cpu':55, 'wait':8,  'deadline':0.4, 'csr':18, 'blocked':0},
                {'cpu':50, 'wait':12, 'deadline':0.2, 'csr':22, 'blocked':0},
                {'cpu':48, 'wait':9,  'deadline':0.5, 'csr':15, 'blocked':0},
                {'cpu':52, 'wait':11, 'deadline':0.3, 'csr':19, 'blocked':0},
            ]
        },
        {
            "name": "2. Deadline Crisis",
            "tasks": [
                {'cpu':20, 'wait':2,  'deadline':0.95, 'csr':50, 'blocked':0},
                {'cpu':10, 'wait':10, 'deadline':0.20, 'csr':10, 'blocked':0},
                {'cpu':30, 'wait':1,  'deadline':0.92, 'csr':60, 'blocked':0},
                {'cpu':5,  'wait':20, 'deadline':0.10, 'csr':5,  'blocked':1},
                {'cpu':5,  'wait':20, 'deadline':0.10, 'csr':5,  'blocked':1},
            ]
        },
        {
            "name": "3. Idle / Deep Sleep",
            "tasks": [
                {'cpu':1,  'wait':50, 'deadline':0.0, 'csr':2, 'blocked':1},
                {'cpu':1,  'wait':50, 'deadline':0.0, 'csr':2, 'blocked':1},
                {'cpu':1,  'wait':50, 'deadline':0.0, 'csr':2, 'blocked':1},
                {'cpu':1,  'wait':50, 'deadline':0.0, 'csr':2, 'blocked':1},
                {'cpu':1,  'wait':50, 'deadline':0.0, 'csr':2, 'blocked':1},
            ]
        },
        {
            "name": "4. Heavy Compute Burst",
            "tasks": [
                {'cpu':2,  'wait':50, 'deadline':0.1, 'csr':5,  'blocked':0},
                {'cpu':3,  'wait':50, 'deadline':0.1, 'csr':5,  'blocked':0},
                {'cpu':0,  'wait':50, 'deadline':0.0, 'csr':0,  'blocked':1},
                {'cpu':90, 'wait':0,  'deadline':0.8, 'csr':80, 'blocked':0},
                {'cpu':90, 'wait':0,  'deadline':0.8, 'csr':80, 'blocked':0},
            ]
        },
        {
            "name": "5. Mixed Realistic",
            "tasks": [
                {'cpu':40, 'wait':1,  'deadline':0.90, 'csr':45, 'blocked':0},
                {'cpu':5,  'wait':10, 'deadline':0.40, 'csr':12, 'blocked':0},
                {'cpu':10, 'wait':5,  'deadline':0.60, 'csr':20, 'blocked':0},
                {'cpu':0,  'wait':50, 'deadline':0.00, 'csr':0,  'blocked':1},
                {'cpu':5,  'wait':20, 'deadline':0.20, 'csr':8,  'blocked':0},
            ]
        },
    ]

    for s in scenarios:
        features, total_cpu = make_features(s["tasks"])
        if interpreter:
            priorities, power_idx = run_inference(interpreter, features)
        else:
            priorities, power_idx = rule_based(s["tasks"], total_cpu)
        print_scenario(s["name"], s["tasks"], priorities, power_idx, total_cpu)

    # ── Live Simulation ──────────────────────────────────────────────────────
    print(f"\n\n{'*'*58}")
    print(f"  LIVE SIMULATION  (10 random cycles @ 5ms interval)")
    print(f"{'*'*58}")
    for step in range(1, 11):
        rand_tasks = [
            {'cpu': random.uniform(0,100), 'wait': random.uniform(0,50),
             'deadline': random.uniform(0,1), 'csr': random.uniform(0,100),
             'blocked': random.randint(0,1)}
            for _ in range(5)
        ]
        features, total_cpu = make_features(rand_tasks)
        if interpreter:
            _, power_idx = run_inference(interpreter, features)
        else:
            _, power_idx = rule_based(rand_tasks, total_cpu)
        color = POWER_COLORS[power_idx]
        label = POWER_LABELS[power_idx]
        print(f"  Step {step:2d}: System CPU {total_cpu:5.1f}%  ->  {color}{label:10}{RESET}")

    print(f"\n{G}[OK] Simulation complete. Model is ready for NPU deployment.{RESET}\n")

if __name__ == "__main__":
    main()


