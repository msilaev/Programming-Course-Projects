import pickle
import numpy as np
import matplotlib.pyplot as plt

import os
os.environ['TF_CPP_MIN_LOG_LEVEL'] = '3'

import tensorflow as tf
from tensorflow import keras
from tensorflow.keras import layers
from tensorflow.keras.callbacks import EarlyStopping
from tensorflow.keras.regularizers import l2

def unpickle(file):
    with open(file, 'rb') as f:
        dict = pickle.load(f, encoding="latin1")
    return dict

#path = r"C:\Users\xdmisi\Documents\STUDY\MLandPatternRecogniiton\project\Ex5NN\cifar-10-batches-py\data_batch_1"
#datadict = unpickle(path)

#x_train = datadict["data"]
#y_train = datadict["labels"]

path = r"C:\Users\xdmisi\Documents\STUDY\MLandPatternRecogniiton\project\Ex5NN\cifar-10-batches-py\test_batch"
datadict = unpickle(path)

x_test = datadict["data"]
y_test = datadict["labels"]

# Collecting data from several batches
ind = 0
for fname_ind in range(1,5):
    path = r"C:\Users\xdmisi\Documents\STUDY\MLandPatternRecogniiton\project\Ex5NN\cifar-10-batches-py\data_batch_" + str(fname_ind)
    datadict = unpickle(path)

    x_train_1 = datadict["data"]
    y_train_1 = datadict["labels"]

    x_train_1 = x_train_1.reshape(-1, 32 * 32 * 3).astype('float32') / 255.0
    y_train_1 = keras.utils.to_categorical(y_train_1, 10)
    
    if ind == 0:
        x_train = x_train_1
        y_train = y_train_1
    else:
        x_train = np.concatenate((x_train, x_train_1), axis=0)
        y_train = np.concatenate((y_train, y_train_1), axis=0)

    ind = ind + 1

x_test = x_test.reshape(-1, 32 * 32 * 3).astype('float32') / 255.0
y_test = keras.utils.to_categorical(y_test, 10)

# Define early stopping
early_stopping = EarlyStopping(
    monitor='val_loss',    # Metric to monitor (e.g., validation loss)
    patience=5,             # Number of epochs with no improvement before stopping
    restore_best_weights=True  # Restore model weights from the epoch with the best value of the monitored metric
)

user_choice = int(input(
    "Choose an option:\n"
    "  1. Model with a small number (5) of neurons in the hidden layer.\n"
    "  2. Model with a large number (400) of neurons in the hidden layer and no regularization.\n"
    "  3. Model with a large number (400) of neurons in the hidden layer and dropout regularization to reduce overfitting.\n"
    "Enter the number of your choice: ")      )

# We check three models : 
# Model 1: with small number (5) of neurons in hidden layer.
# Model 2: with large number (400)  of neurons in hidden layer and no regularization.
# Model 3: with large number (400)  of neurons in hidden layer and droput regularization to reduce overfitting. 

# Compile the model
# tested different learning rates, 
# learning_rate = 0.0001 -> Model 3 test accuracy 0.4528
# learning_rate = 0.0005 -> Model 3 test accuracy 0.42370 
# learning_rate = 0.001 -> Model 3 test accuracy 0.3727 

if (user_choice == 1): 

    model1 = keras.Sequential([
        layers.Dense(5, input_dim = 3*32*32, activation='relu'), 
        layers.Dense(10, activation='softmax')  # Output layer with 10 neurons (CIFAR-10 classes) and softmax activation
    ])

    learning_rate = 0.0005
    opt = keras.optimizers.Adam(learning_rate=learning_rate)
    model1.compile(optimizer=opt, loss='categorical_crossentropy', metrics=['accuracy'])

    # Train the model
    history1 = model1.fit(x_train, y_train, epochs=200,  
        validation_split=0.2, callbacks=[early_stopping], verbose = 0)

elif (user_choice == 2):
    model2 = keras.Sequential([
        layers.Dense(400, input_dim = 3*32*32, activation='relu'), 
        layers.Dense(10, activation='softmax')  # Output layer with 10 neurons (CIFAR-10 classes) and softmax activation
    ])

    learning_rate = 0.0005


    opt = keras.optimizers.Adam(learning_rate=learning_rate)
    model2.compile(optimizer=opt, loss='categorical_crossentropy', metrics=['accuracy'])

    history2 = model2.fit(x_train, y_train, epochs=200,  
        validation_split=0.2, callbacks=[early_stopping], verbose = 0)

elif (user_choice == 3):
    model3 = keras.Sequential([
    layers.Dense(400, input_dim = 3*32*32, activation='relu'), 
    layers.Dropout(0.3),  # Add a dropout layer with a dropout rate of 0.3 (30% dropout)
    layers.Dense(10, activation='softmax')  # Output layer with 10 neurons (CIFAR-10 classes) and softmax activation
    ])

    learning_rate = 0.0005

    opt = keras.optimizers.Adam(learning_rate=learning_rate)
    model3.compile(optimizer=opt, loss='categorical_crossentropy', metrics=['accuracy'])

    history3 = model3.fit(x_train, y_train, epochs=200,  
        validation_split=0.2, callbacks=[early_stopping], verbose = 0)


#print("evaluate")
if (user_choice == 1): 

    # Evaluate the model on the test data
    #Model 1
    train_loss, train_accuracy = model1.evaluate(x_train, y_train, verbose= 0)
    test_loss, test_accuracy = model1.evaluate(x_test, y_test, verbose= 0)

    print("\n ***************************** \n")
    print(" Model 1: 5 neurons in hidden layer")
    print(f'Test accuracy : {test_accuracy}, Train accuracy: {train_accuracy}')

elif (user_choice == 2): 

    #Model 2
    train_loss, train_accuracy = model2.evaluate(x_train, y_train, verbose= 0)
    test_loss, test_accuracy = model2.evaluate(x_test, y_test, verbose= 0)
    print("\n ***************************** \n")
    print("Model 2: 400 neurons in hidden layer, no regularization")
    print(f'Test accuracy : {test_accuracy}, Train accuracy: {train_accuracy}')

elif (user_choice == 3): 

    #Model 3
    train_loss, train_accuracy = model3.evaluate(x_train, y_train, verbose= 0)
    test_loss, test_accuracy = model3.evaluate(x_test, y_test, verbose= 0)

    print("\n ***************************** \n")
    print("Model 3: 400 neurons in hidden layer, dropout regularization")
    print(f'Test accuracy : {test_accuracy}, Train accuracy: {train_accuracy}')
    print("\n ***************************** \n")

# plot results 
fig, ax = plt.subplots()

if (user_choice == 1): 

    #plt.subplots_adjust(hspace=1)  # Increase vertical spacing

    ax.plot(history1.history['loss'])
    ax.plot(history1.history['val_loss'])
    ax.set_title("Model 1: 5 neurons")
    ax.set_ylabel('loss')
    ax.set_xlabel('epoch')
    ax.legend(['Training Loss', 'Validation Loss'], loc='upper right')
    plt.show()


elif (user_choice == 2):
    ax.plot(history2.history['loss'])
    ax.plot(history2.history['val_loss'])
    ax.set_title("Model 2: 400 neurons,  no regularization")
    ax.set_ylabel('loss')
    ax.set_xlabel('epoch')
    ax.legend(['Training Loss', 'Validation Loss'], loc='upper right')

elif (user_choice == 3):

    ax.plot(history3.history['loss'])
    ax.plot(history3.history['val_loss'])
    ax.set_title("Model 3: 400 neurons,  dropout regularization")
    ax.set_ylabel('loss')
    ax.set_xlabel('epoch')
    ax.legend(['Training Loss', 'Validation Loss'], loc='upper right')


plt.show()
