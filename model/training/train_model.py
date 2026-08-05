"""
Sentinel-RT Model Training
--------------------------
This script trains a 1-Dimensional Convolutional Neural Network (1D-CNN) 
on the synthetic task scheduling dataset. 

A 1D-CNN is often used for sequence data or feature vectors where local 
patterns (like features of a single task) might be useful to extract. 

The model takes the 26 input features and has two "heads" (outputs):
1. Priority Head: Predicts the priority (1-7) for each of the 5 tasks (Regression).
2. Power Head: Predicts the system power state (Classification).
"""

import os
import numpy as np
import pandas as pd
import tensorflow as tf
from tensorflow.keras.models import Model
from tensorflow.keras.layers import Input, Conv1D, MaxPooling1D, GlobalAveragePooling1D, Dense, Reshape
from tensorflow.keras.callbacks import EarlyStopping
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import MinMaxScaler
from sklearn.metrics import confusion_matrix
import matplotlib.pyplot as plt
import seaborn as sns

# Set random seed for reproducibility
tf.random.set_seed(42)
np.random.seed(42)

def train_model():
    dataset_path = r"C:\Users\user\Documents\Sentinel-RT\model\dataset\task_scenarios.csv"
    if not os.path.exists(dataset_path):
        print(f"Error: Dataset not found at {dataset_path}")
        return

    # 1. Load Data
    print("Loading data...")
    df = pd.read_csv(dataset_path)
    
    # 26 features: first 26 columns
    X = df.iloc[:, :26].values
    
    # Labels: next 5 columns are priorities, last column is power state
    y_prio = df.iloc[:, 26:31].values
    y_power = df.iloc[:, 31].values
    
    # Convert power state to one-hot encoding for categorical_crossentropy
    y_power_categorical = tf.keras.utils.to_categorical(y_power, num_classes=3)
    
    # 2. Split Data
    # 80% for training (teaching the model), 20% for testing (evaluating it)
    X_train, X_test, y_prio_train, y_prio_test, y_power_train, y_power_test = train_test_split(
        X, y_prio, y_power_categorical, test_size=0.2, random_state=42)
        
    # 3. Normalize Features
    # Neural networks prefer inputs between 0 and 1.
    scaler = MinMaxScaler()
    X_train_scaled = scaler.fit_transform(X_train)
    X_test_scaled = scaler.transform(X_test)
    
    # Ensure docs directory exists for saving plots
    docs_dir = r"C:\Users\user\Documents\Sentinel-RT\docs"
    os.makedirs(docs_dir, exist_ok=True)
    
    # 4. Build Model
    # Input layer expects 26 features
    inputs = Input(shape=(26,))
    
    # Reshape for Conv1D which expects (timesteps, features) -> (26, 1)
    x = Reshape((26, 1))(inputs)
    
    # First Convolutional Layer
    x = Conv1D(filters=32, kernel_size=3, activation='relu', padding='same')(x)
    x = MaxPooling1D(pool_size=2)(x)
    
    # Second Convolutional Layer
    x = Conv1D(filters=64, kernel_size=3, activation='relu', padding='same')(x)
    
    # Pooling to flatten the spatial dimensions
    x = GlobalAveragePooling1D()(x)
    
    # Shared Dense Layer
    x = Dense(64, activation='relu')(x)
    
    # Output 1: Priority Assignment (Regression - linear activation)
    priority_output = Dense(5, activation='linear', name='priority')(x)
    
    # Output 2: Power State (Classification - softmax activation)
    power_output = Dense(3, activation='softmax', name='power_state')(x)
    
    # Create the Model
    model = Model(inputs=inputs, outputs=[priority_output, power_output])
    
    # Compile Model
    # MSE (Mean Squared Error) is good for predicting numbers (priorities).
    # Categorical Crossentropy is good for classification (power states).
    model.compile(optimizer='adam', 
                  loss={'priority': 'mse', 'power_state': 'categorical_crossentropy'},
                  metrics={'priority': 'mae', 'power_state': 'accuracy'})
                  
    model.summary()
    
    # 5. Train Model
    print("Training model...")
    # Stop training early if validation loss doesn't improve for 10 epochs
    early_stopping = EarlyStopping(monitor='val_loss', patience=10, restore_best_weights=True)
    
    history = model.fit(
        X_train_scaled, 
        {'priority': y_prio_train, 'power_state': y_power_train},
        validation_data=(X_test_scaled, {'priority': y_prio_test, 'power_state': y_power_test}),
        epochs=100,
        batch_size=64,
        callbacks=[early_stopping],
        verbose=1
    )
    
    # 6. Evaluate and Plot
    print("\nEvaluating model on test set...")
    eval_results = model.evaluate(X_test_scaled, {'priority': y_prio_test, 'power_state': y_power_test})
    
    print("\n--- Final Metrics ---")
    print(f"Total Test Loss: {eval_results[0]:.4f}")
    # Keras outputs metrics in a specific order depending on model outputs.
    # Usually [loss, priority_loss, power_state_loss, priority_mae, power_state_accuracy]
    print(f"Priority MAE: {eval_results[3]:.4f}")
    print(f"Power State Accuracy: {eval_results[4]:.4f}")
    
    # Plot Training curves
    plt.figure(figsize=(12, 4))
    
    plt.subplot(1, 2, 1)
    plt.plot(history.history['priority_loss'], label='Train Priority Loss (MSE)')
    plt.plot(history.history['val_priority_loss'], label='Val Priority Loss')
    plt.legend()
    plt.title('Priority Loss')
    
    plt.subplot(1, 2, 2)
    plt.plot(history.history['power_state_accuracy'], label='Train Power Acc')
    plt.plot(history.history['val_power_state_accuracy'], label='Val Power Acc')
    plt.legend()
    plt.title('Power State Accuracy')
    
    loss_plot_path = os.path.join(docs_dir, "training_loss.png")
    plt.savefig(loss_plot_path)
    plt.close()
    print(f"Saved training curves to {loss_plot_path}")
    
    # Confusion Matrix for Power State
    preds = model.predict(X_test_scaled)
    # preds[1] is the power state prediction (softmax output)
    power_preds_classes = np.argmax(preds[1], axis=1)
    power_true_classes = np.argmax(y_power_test, axis=1)
    
    cm = confusion_matrix(power_true_classes, power_preds_classes)
    plt.figure(figsize=(6, 5))
    sns.heatmap(cm, annot=True, fmt='d', cmap='Blues', xticklabels=['Active', 'Light', 'Deep'], yticklabels=['Active', 'Light', 'Deep'])
    plt.xlabel('Predicted')
    plt.ylabel('True')
    plt.title('Power State Confusion Matrix')
    
    cm_plot_path = os.path.join(docs_dir, "confusion_matrix.png")
    plt.savefig(cm_plot_path)
    plt.close()
    print(f"Saved confusion matrix to {cm_plot_path}")
    
    # 7. Save Model
    model_dir = r"C:\Users\user\Documents\Sentinel-RT\model\training"
    os.makedirs(model_dir, exist_ok=True)
    model_path = os.path.join(model_dir, "sentinel_model.h5")
    model.save(model_path)
    print(f"Saved trained model to {model_path}")

if __name__ == '__main__':
    train_model()
