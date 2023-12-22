import numpy as np
import random


class Classifiers:

    def __init__(self, train_y, test_y, train_X, test_X):

        self.train_y = train_y
        self.test_y = test_y
        self.train_X = train_X
        self.test_X = test_X   

    def rnd_class(self):
        return np.array([random.randint(0, 1)
                         for _ in range(len(self.test_y))])

    def max_lh_class(self):
        count_1 = len([i for i in self.train_y if i == 1])
        count_0 = len([i for i in self.train_y if i == 0])

        if count_1 >= count_0:
            return np.array([1 for _ in range(len(self.test_y))])
        else:
            return np.array([0 for _ in range(len(self.test_y))])

    def accuracy_score(self, y_pred):
        a = [1 for num, i in enumerate(y_pred)
             if y_pred[num] == self.test_y[num]]
        return sum(a)/len(y_pred)

data_X_test= np.loadtxt("male_female_X_test.txt")
data_y_test = np.loadtxt("male_female_y_test.txt")
data_X_train= np.loadtxt("male_female_X_train.txt")
data_y_train = np.loadtxt("male_female_y_train.txt")
c = Classifiers(data_y_train, data_y_test, data_X_train, data_X_test)

y_pred = c.rnd_class()
print("Accuracy of random classifier: ", c.accuracy_score(y_pred))

y_pred = c.max_lh_class()
print("Accuracy of maximum probability classifier: ", c.accuracy_score(y_pred))
