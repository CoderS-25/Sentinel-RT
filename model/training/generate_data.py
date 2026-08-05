"""
Sentinel-RT Data Generator
--------------------------
This script generates a synthetic dataset to simulate task scheduling scenarios in 
the μT-Kernel 3.0 RTOS. The generated data will be used to train an AI scheduler 
that predicts the optimal priority for each task and the recommended power state 
for the system.

For a team that might not be familiar with Machine Learning (ML), generating 
synthetic data is a common first step when real data isn't available yet. 
We try to encode the "rules" of what a good schedule looks like so the ML 
model can learn these patterns.
"""

import os
import numpy as np
import pandas as pd

# Set random seed for reproducibility
np.random.seed(42)

def generate_data(num_samples=10000, num_tasks=5):
    # Lists to store our generated features and labels
    features_list = []
    labels_priority_list = []
    labels_power_list = []

    for _ in range(num_samples):
        # 1. Generate Task Features
        task_features = []
        priorities = []
        
        total_cpu = 0
        
        for t in range(num_tasks):
            # cpu_load (0-100%)
            cpu_load = np.random.uniform(0, 100)
            # wait_time_ms (0-50ms)
            wait_time = np.random.uniform(0, 50)
            # deadline_proximity_ratio (0.0-1.0, where 1.0 is imminent)
            deadline_prox = np.random.uniform(0.0, 1.0)
            # context_switch_rate (0-100 switches/sec)
            ctx_switch = np.random.uniform(0, 100)
            # is_blocked (0 or 1)
            is_blocked = np.random.choice([0, 1])
            
            # Append features for this task
            task_features.extend([cpu_load, wait_time, deadline_prox, ctx_switch, is_blocked])
            total_cpu += cpu_load
            
            # 2. Determine ideal priority (Labels)
            # Priority values: 1-7 (1=highest)
            
            # Introduce a base random priority for noise
            prio = np.random.randint(4, 6)
            
            if is_blocked == 1:
                # Blocked tasks get lowest priority (5-7)
                prio = np.random.randint(5, 8)
            elif deadline_prox > 0.7:
                # Imminent deadline gets highest priority (1-2)
                prio = np.random.randint(1, 3)
            elif cpu_load > 60 and deadline_prox < 0.4:
                # High CPU, low deadline gets medium priority (3-4)
                prio = np.random.randint(3, 5)
                
            priorities.append(prio)
            
        # Global feature: total system CPU load 
        # (Could be conceptually > 100% since we sum over tasks, 
        # so we average it out to represent a percentage of max capacity)
        sys_cpu_load = total_cpu / num_tasks
        
        # Combine all features: 5 tasks * 5 features + 1 global = 26 features
        all_features = task_features + [sys_cpu_load]
        features_list.append(all_features)
        labels_priority_list.append(priorities)
        
        # 3. Determine ideal power state (Labels)
        # 0=Active, 1=Light Sleep, 2=Deep Sleep
        if sys_cpu_load < 20:
            power_state = 2 # Deep Sleep
        elif sys_cpu_load < 50:
            power_state = 1 # Light Sleep
        else:
            power_state = 0 # Active
            
        # Add a tiny bit of noise to power state ~5% of the time
        if np.random.rand() < 0.05:
            power_state = np.random.choice([0, 1, 2])
            
        labels_power_list.append(power_state)

    # Convert to pandas DataFrame for easy saving and viewing
    feature_cols = []
    for t in range(num_tasks):
        feature_cols.extend([f'task_{t}_cpu', f'task_{t}_wait', f'task_{t}_deadline', 
                             f'task_{t}_ctx', f'task_{t}_blocked'])
    feature_cols.append('sys_cpu_load')
    
    prio_cols = [f'label_prio_{t}' for t in range(num_tasks)]
    
    df_features = pd.DataFrame(features_list, columns=feature_cols)
    df_priorities = pd.DataFrame(labels_priority_list, columns=prio_cols)
    df_power = pd.DataFrame({'label_power': labels_power_list})
    
    # Concatenate all into one big dataset
    dataset = pd.concat([df_features, df_priorities, df_power], axis=1)
    
    return dataset

if __name__ == '__main__':
    print("Starting data generation...")
    dataset = generate_data(10000, 5)
    
    # Ensure directory exists
    output_dir = r"C:\Users\user\Documents\Sentinel-RT\model\dataset"
    os.makedirs(output_dir, exist_ok=True)
    
    output_path = os.path.join(output_dir, "task_scenarios.csv")
    dataset.to_csv(output_path, index=False)
    
    print(f"Data generation complete! Saved to {output_path}")
    print("\nDataset Summary Statistics:")
    print(dataset.describe())
