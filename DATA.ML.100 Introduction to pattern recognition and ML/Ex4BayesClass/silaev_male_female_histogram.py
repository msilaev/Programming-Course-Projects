from matplotlib import pyplot as plt
import numpy as np

data_X_test = np.loadtxt("male_female_X_test.txt")
data_y_test = np.loadtxt("male_female_y_test.txt")

data_X_train = np.loadtxt("male_female_X_train.txt")
data_y_train = np.loadtxt("male_female_y_train.txt")

data_X_train_weight = data_X_train[:, 1]
data_X_train_weight_male = np.array([i for num, i in enumerate(data_X_train_weight) if data_y_train[num] == 0])
data_X_train_weight_female = np.array([i for num, i in enumerate(data_X_train_weight) if data_y_train[num] == 1])

data_X_train_height = data_X_train[:, 0]
data_X_train_height_male = np.array([i for num, i in enumerate(data_X_train_height) if data_y_train[num] == 0])
data_X_train_height_female = np.array([i for num, i in enumerate(data_X_train_height) if data_y_train[num] == 1])

# Creating histogram for height
plt.figure(1)
bins = np.linspace(80, 220, 11) 
hist_train_height_female, bins = np.histogram(data_X_train_height_female, bins=bins)
hist_train_height_male, bins = np.histogram(data_X_train_height_male, bins=bins)

plt.bar(bins[:-1], hist_train_height_female, width=(bins[1] - bins[0]), 
        align="edge", color="red", alpha=0.5, label="female")
plt.bar(bins[:-1], hist_train_height_male, width=(bins[1] - bins[0]), 
        align="edge", color="blue", alpha=0.5, label="male")

#plt.hist( data_X_train_height_female, color = "red", alpha=0.5, bins = bins )
#plt.hist( data_X_train_height_male, color = 'blue', alpha=0.5, bins = bins)

plt.legend()
plt.xlabel('height [m]')
plt.ylabel('amount [num]')
plt.savefig("silaev_male_female_histogram_plot_height.png")

# Creating histogram for weight
plt.figure(2)
bins = np.linspace(30, 180, 11)
hist_train_weight_female, bins = np.histogram(data_X_train_weight_female, bins=bins)
hist_train_weight_male, bins = np.histogram(data_X_train_weight_male, bins=bins)

plt.bar(bins[:-1], hist_train_weight_female, width=(bins[1] - bins[0]), 
        align="edge", color="red", alpha=0.5, label="female")
plt.bar(bins[:-1], hist_train_weight_male, width=bins[1] - bins[0], 
        align="edge", color="blue", alpha=0.5, label="male")

#plt.hist( data_X_train_weight_female, color=( "red"), alpha=0.5, bins = bins)
#plt.hist( data_X_train_weight_male, color=( "blue"), alpha=0.5,  bins = bins)
plt.legend()
plt.xlabel('weight [kg]')
plt.ylabel('amount [num]')
plt.savefig("silaev_male_female_histogram_plot_weight.png")
plt.show()