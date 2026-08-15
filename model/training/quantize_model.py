"""
Sentinel-RT Model Quantization
------------------------------
This script converts the trained 32-bit floating point Keras model into 
an 8-bit integer (int8) TensorFlow Lite (TFLite) format.

Why quantize? 
Microcontrollers and NPUs (like the STM32N657) often have limited memory 
and perform math operations much faster using integers instead of floats. 
By converting the weights and activations to int8, we drastically reduce 
the model size (usually by 4x) and speed up inference, with very little 
loss in accuracy.
"""

import os
import numpy as np
import pandas as pd
import tensorflow as tf
from sklearn.preprocessing import MinMaxScaler

def quantize_model():
    model_path = r"C:\Users\user\Documents\Sentinel-RT\model\training\sentinel_model.keras"
    dataset_path = r"C:\Users\user\Documents\Sentinel-RT\model\dataset\task_scenarios.csv"
    
    if not os.path.exists(model_path):
        print(f"Error: Model not found at {model_path}")
        return
        
    print("Loading original Keras model...")
    model = tf.keras.models.load_model(model_path)
    
    # Get original model size
    original_size = os.path.getsize(model_path)
    print(f"Original model size: {original_size / 1024:.2f} KB")
    
    print("Loading representative dataset for calibration...")
    # We need a small sample of real-ish data so the converter can observe 
    # the typical range of values passing through the network. This helps it
    # figure out how to scale the floating point numbers into integers (0-255).
    df = pd.read_csv(dataset_path)
    X = df.iloc[:, :26].values
    
    # Normally we load the saved scaler, but for simplicity here we re-fit on a subset
    scaler = MinMaxScaler()
    X_scaled = scaler.fit_transform(X)
    
    # Use 100 samples for calibration
    representative_data = X_scaled[:100].astype(np.float32)
    
    def representative_dataset_gen():
        for i in range(len(representative_data)):
            # TFLite expects data in the format [batch_size, input_features]
            yield [representative_data[i:i+1]]
            
    # Initialize the converter
    converter = tf.lite.TFLiteConverter.from_keras_model(model)
    
    # Enable full integer quantization
    converter.optimizations = [tf.lite.Optimize.DEFAULT]
    converter.representative_dataset = representative_dataset_gen
    
    # Ensure that ALL ops are quantized to int8 (weights and activations)
    converter.target_spec.supported_ops = [tf.lite.OpsSet.TFLITE_BUILTINS_INT8]
    
    # Set the input and output tensors to uint8 or int8 (we'll use int8)
    converter.inference_input_type = tf.int8
    converter.inference_output_type = tf.int8
    
    print("Converting model to TFLite (Full INT8)...")
    tflite_quant_model = converter.convert()
    
    # Save the quantized model
    quantized_dir = r"C:\Users\user\Documents\Sentinel-RT\model\quantized"
    os.makedirs(quantized_dir, exist_ok=True)
    tflite_path = os.path.join(quantized_dir, "sentinel_model.tflite")
    
    with open(tflite_path, 'wb') as f:
        f.write(tflite_quant_model)
        
    quantized_size = os.path.getsize(tflite_path)
    print(f"Quantized model size: {quantized_size / 1024:.2f} KB")
    print(f"Size reduction: {original_size / quantized_size:.2f}x")
    
    if quantized_size < 50 * 1024:
        print("SUCCESS: Quantized model is under the 50KB target!")
    else:
        print("WARNING: Quantized model exceeds 50KB target.")
        
    print("\nValidating quantized model on a single sample...")
    
    # Load TFLite model and allocate tensors
    interpreter = tf.lite.Interpreter(model_content=tflite_quant_model)
    interpreter.allocate_tensors()
    
    input_details = interpreter.get_input_details()
    output_details = interpreter.get_output_details()
    
    # Test on the first sample of our representative data
    test_sample = representative_data[0:1]
    
    # Quantize the input manually since the input tensor expects int8
    input_scale, input_zero_point = input_details[0]['quantization']
    test_sample_quantized = (test_sample / input_scale + input_zero_point).astype(np.int8)
    
    interpreter.set_tensor(input_details[0]['index'], test_sample_quantized)
    interpreter.invoke()
    
    print("Inference successful. TFLite INT8 model is ready for deployment!")
    print(f"Saved to: {tflite_path}")

if __name__ == '__main__':
    quantize_model()
