import numpy as np
from sklearn.metrics import mean_squared_error
from sklearn.preprocessing import StandardScaler
from sklearn.linear_model import LinearRegression
from sklearn import tree
from sklearn import ensemble

X_test = np.loadtxt("disease_X_test.txt")
y_test = np.loadtxt("disease_y_test.txt")
X_train = np.loadtxt("disease_X_train.txt")
y_train = np.loadtxt("disease_y_train.txt")

# center and normalize features 
feature_scaler = StandardScaler()
X_train_normalized = feature_scaler.fit_transform(X_train)
X_test_normalized = feature_scaler.transform(X_test)

# (a) Baseline classifier
y_train_mean = np.mean(y_train)
y_pred_baseline = np.array([y_train_mean for _ in range(len(y_test))]) 
mse_baseline = mean_squared_error(y_test, y_pred_baseline)
print(f"Mean Squared Error for baseline: {mse_baseline:.0f}")

# (b) Linear regression
clf = LinearRegression()
clf.fit(X_train_normalized, y_train)
y_pred_lin_reg = clf.predict(X_test_normalized)
mse_lin_reg = mean_squared_error(y_test, y_pred_lin_reg)
print(f"Mean Squared Error for linear regression: {mse_lin_reg:.0f}")

# (c) Tree
clf = tree.DecisionTreeRegressor()
clf = clf.fit(X_train_normalized, y_train)
y_pred_tree = clf.predict(X_test_normalized)
mse_tree = mean_squared_error(y_test, y_pred_tree)
print(f"Mean Squared Error for tree: {mse_tree:.0f}")

# (d) Random forest
clf = ensemble.RandomForestRegressor()
clf = clf.fit(X_train_normalized, y_train)
y_pred_forest = clf.predict(X_test_normalized)
mse_forest = mean_squared_error(y_test, y_pred_forest)
print(f"Mean Squared Error for random forest: {mse_forest:.0f}")
