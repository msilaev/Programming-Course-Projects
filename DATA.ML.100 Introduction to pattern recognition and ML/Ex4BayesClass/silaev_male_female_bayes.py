#weight test error 0.6834170854271358
#height test error 0.8525963149078727
#weight+height test error 0.8726968174204355
# p(h|male)*p(w|male)* p(male)


from matplotlib import pyplot as plt
import numpy as np
import random

class Classifiers:

    def __init__(self, train_y, test_y, train_X, test_X):

        self.train_y = train_y
        self.test_y = test_y
        self.train_X = train_X
        self.test_X = test_X   
    
    def hist_height_male(self):
        bins_height = np.linspace(80, 220, 11) 
        data_X_train_height = self.train_X[:, 0]                
        data_X_train_height_male = np.array([i for num,
                                             i in enumerate(data_X_train_height)
                                            if self.train_y[num] == 0])        
                  
        hist_train_height_male, bins_height = np.histogram(data_X_train_height_male,
                                                           bins=bins_height)
        return hist_train_height_male, bins_height
    
    def hist_height_female(self):
        bins_height = np.linspace(80, 220, 11)  
        data_X_train_height = self.train_X[:, 0]             
        data_X_train_height_female = np.array([i for num,
                                               i in enumerate(data_X_train_height)
                                              if self.train_y[num] == 1])
        hist_train_height_female, bins_height = np.histogram(data_X_train_height_female,
                                                             bins=bins_height)
        return hist_train_height_female, bins_height
    
    def hist_weight(self):
        bins_weight = np.linspace(30, 180, 11)
        data_X_train_weight = self.train_X[:, 1]                
        hist_train_weight, bins_weight = np.histogram(data_X_train_weight,
                                                      bins=bins_weight)
        return hist_train_weight, bins_weight
        
    def hist_height(self):
        bins_height = np.linspace(80, 220, 11) 
        data_X_train_height = self.train_X[:, 0]                
        hist_train_height, bins_height = np.histogram(data_X_train_height,
                                                      bins=bins_height)    
        
        return hist_train_height, bins_height     
                     
    def hist_weight_male(self):
        bins_weight = np.linspace(30, 180, 11)
        data_X_train_weight = self.train_X[:, 1]
        data_X_train_weight_male = np.array([i for num, i in enumerate(data_X_train_weight)
                                            if self.train_y[num] == 0])    
        
        hist_train_weight_male, bins_weight = np.histogram(data_X_train_weight_male,
                                                           bins=bins_weight)
        
        return hist_train_weight_male, bins_weight
    
    def hist_weight_female(self):
        bins_weight = np.linspace(30, 180, 11)
        data_X_train_weight = self.train_X[:, 1]        
        data_X_train_weight_female = np.array([i for num, i in enumerate(data_X_train_weight)
                                              if self.train_y[num] == 1])
        
        hist_train_weight_female, bins_weight = np.histogram(data_X_train_weight_female,
                                                           bins=bins_weight)
        
        return hist_train_weight_female, bins_weight
           
    def prior_prob(self):
        count_1 = len([i for i in self.train_y if i == 0])
        return (count_1/len(self.train_y), 1 - count_1/len(self.train_y))    
    
    def baies_class_weight(self):

        p_prior_male, p_prior_female = self.prior_prob()
        hist_train_weight, bins_weight = self.hist_weight()
        hist_train_weight_female, bins_weight = self.hist_weight_female()
        hist_train_weight_male, bins_weight = self.hist_weight_male()

        p_weight = hist_train_weight/sum(hist_train_weight)
        p_weight_female = hist_train_weight_female/sum(hist_train_weight_female)
        p_weight_male = hist_train_weight_male/sum(hist_train_weight_male)

        bins_weight_centers = [(bins_weight[n] + bins_weight[n+1])/2 for n in range(len(bins_weight)-1)]

        pred_y = []
        for weight in self.test_X[:, 1] :
            bins_weight_centers_diff = [abs(i - weight) for i in bins_weight_centers]
            ind_w = np.argmin(bins_weight_centers_diff)

            p_weight_min = p_weight[ind_w]
            p_weight_female_min = p_weight_female[ind_w] 
            p_weight_male_min = p_weight_male[ind_w]

            # Use Bayes theorem p(male|w)p(2) = p(w|male)p(male)
            p_male = p_weight_male_min*p_prior_male/p_weight_min
            p_female = p_weight_female_min*p_prior_female/p_weight_min   
                                   
            if p_male >= p_female: 
                pred_y.append(0)
            else:
                pred_y.append(1)
        
        return pred_y               
    
    def baies_class_height(self):
        p_prior_male, p_prior_female = self.prior_prob()

        hist_train_height, bins_height = self.hist_height()
        hist_train_height_female, bins_height = self.hist_height_female()
        hist_train_height_male, bins_height = self.hist_height_male()

        p_height = hist_train_height/sum(hist_train_height)
        p_height_female = hist_train_height_female/sum(hist_train_height_female)
        p_height_male = hist_train_height_male/sum(hist_train_height_male)

        bins_height_centers = [(bins_height[n] + bins_height[n+1])/2 for n in range(len(bins_height)-1)]

        pred_y = []
        for height in self.test_X[:, 0]:
            bins_height_centers_diff = [abs(i- height) for i in bins_height_centers]
            ind_w = np.argmin(bins_height_centers_diff)

            p_height_min = p_height[ind_w]
            p_height_female_min = p_height_female[ind_w] 
            p_height_male_min = p_height_male[ind_w]            
        
            # Use Bayes theorem p(male|h)p(h) = p(h|male)p(male) 
            p_male = p_height_male_min*p_prior_male/p_height_min
            p_female = p_height_female_min*p_prior_female/p_height_min   
            
            if p_male >= p_female: 
                pred_y.append(0)
            else:
                pred_y.append(1)

        return pred_y

    def baies_class_height_weight(self):

        p_prior_male, p_prior_female = self.prior_prob()
        hist_train_weight, bins_weight = self.hist_weight()
        hist_train_weight_female, bins_weight = self.hist_weight_female()
        hist_train_weight_male, bins_weight = self.hist_weight_male()

        p_weight = hist_train_weight/sum(hist_train_weight)
        p_weight_female = hist_train_weight_female/sum(hist_train_weight_female)
        p_weight_male = hist_train_weight_male/sum(hist_train_weight_male)

        bins_weight_centers = [(bins_weight[n] + bins_weight[n+1])/2 for n in range(len(bins_weight)-1)]      
          
        hist_train_height, bins_height = self.hist_height()
        hist_train_height_female, bins_height = self.hist_height_female()
        hist_train_height_male, bins_height = self.hist_height_male()

        p_height = hist_train_height/sum(hist_train_height)
        p_height_female = hist_train_height_female/sum(hist_train_height_female)
        p_height_male = hist_train_height_male/sum(hist_train_height_male)

        bins_height_centers = [(bins_height[n] + bins_height[n+1])/2
                               for n in range(len(bins_height)-1)]

        pred_y = []
        for x in self.test_X:
            weight = x[1]
            height = x[0]
            bins_weight_centers_diff = [abs(i - weight) 
                                        for i in bins_weight_centers]
            ind_w = np.argmin(bins_weight_centers_diff)

            bins_height_centers_diff = [abs(i - height) 
                                        for i in bins_height_centers]
            ind_h = np.argmin(bins_height_centers_diff)

            p_weight_female_min = p_weight_female[ind_w] 
            p_weight_male_min = p_weight_male[ind_w]

            p_height_female_min = p_height_female[ind_h] 
            p_height_male_min = p_height_male[ind_h]

            # Use naive Bayes approach assuming that h and w are independent 
            # so that p(male|wh)~ p(male)p(h|male)p(w|male) 
            p_male = p_prior_male*p_weight_male_min*p_height_male_min
            p_female = p_prior_female*p_weight_female_min*p_height_female_min

            if p_male >= p_female: 
                pred_y.append(0)
            else:
                pred_y.append(1)
        
        return pred_y   
        
    def accuracy_score(self, y_pred):
        a = [1 for num, i in enumerate(y_pred)
             if y_pred[num] == self.test_y[num]]
        return sum(a)/len(y_pred)

data_X_test = np.loadtxt("male_female_X_test.txt")
data_y_test = np.loadtxt("male_female_y_test.txt")
data_X_train = np.loadtxt("male_female_X_train.txt")
data_y_train = np.loadtxt("male_female_y_train.txt")

c = Classifiers(data_y_train, data_y_test, data_X_train, data_X_test)
p_prior_male, p_prior_female = c.prior_prob()

print(f"prior probabilities: p_male= {p_prior_male}, p_female= {p_prior_female}")

y_pred = c.baies_class_height()
print("Accuracy of classifier using height: ", 
      c.accuracy_score(y_pred))

y_pred = c.baies_class_weight()
print("Accuracy of classifier using weight: ", 
      c.accuracy_score(y_pred))

y_pred = c.baies_class_height_weight()
print("Accuracy of classifier using height and weight: ", 
      c.accuracy_score(y_pred))