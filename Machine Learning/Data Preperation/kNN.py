# Part 2 - Task B

from sklearn.base import BaseEstimator, ClassifierMixin
from scipy.spatial import distance
import numpy as np
from scipy import stats
class kNN(BaseEstimator, ClassifierMixin): 
  def __init__(self, n_neighbors:int = 3): 
    """
    Initialize the kNN classifier.

    Parameters:
    - n_neighbors: int, optional (default=3)
      The number of nearest neighbors to consider for classification.
    """
    self.n_neighbors = n_neighbors 
    self.X_train = None
    self.y_train = None

  def fit(self, X, y): 
    """
    Fit the kNN classifier to the training data.

    Parameters:
    - X: array-like, shape (n_samples, n_features)
      The training input samples.
    - y: array-like, shape (n_samples,)
      The target values.

    Returns:
    - self: object
      Returns the instance itself.
    """
    self.X_train = X.copy()
    self.y_train = y.copy()
    return self 

  def predict(self, X): 
    """
    Predict the class labels for the input samples.

    Parameters:
    - X: array-like, shape (n_samples, n_features)
      The input samples.

    Returns:
    - predictions: array-like, shape (n_samples,)
      The predicted class labels for the input samples.
    """
    # Calculate the distance between each test sample and all training samples
    dist_matrix = distance.cdist(X, self.X_train)

    # Get the indices of the k nearest neighbors for each test sample
    n_neighbors_indexes = np.argpartition(dist_matrix, self.n_neighbors, axis=1)[:, :self.n_neighbors]
    
    # Get the labels of the k nearest neighbors for each test sample
    knn_labels = np.array(self.y_train)[n_neighbors_indexes]

    # Predict the class labels based on the majority vote of the k nearest neighbors
    predictions = stats.mode(knn_labels, axis=1)[0].ravel()
    return predictions