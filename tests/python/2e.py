import matplotlib
matplotlib.use('Agg')
from matplotlib import pyplot as plt
import numpy as np
from sklearn.covariance import EmpiricalCovariance
from sklearn.svm import SVC
from scipy import linalg as la
from sklearn.datasets import load_svmlight_file
from sklearn.metrics import accuracy_score
import seaborn
from math import ceil


def split_data(X, y, p=0.66):
        m = len(y)
        train_end = int(round(m * p))
        return X[0:train_end], X[train_end+1:-1], y[0:train_end], y[train_end+1:-1]

class MahalanobisClassifier(object):

    def __init__(self, C=1, method='mahalanobis', estimator=EmpiricalCovariance()):
        self.method = method
        self.estimator = estimator
        self.clf = SVC(kernel='linear', C=C, verbose=True)

    def fit(self, X, y):

        self.positive_label = min(y)
        self.negative_label = max(y)

        if self.method == 'mahalanobis':

            self.estimator.fit(X)
            self.cov = self.estimator.covariance_

        elif self.method == '2e':

            X_parts = {l : X[y==l] for l in (self.positive_label, self.negative_label)}

            self.estimator.fit(X_parts[self.positive_label])
            self.cov = self.estimator.covariance_

            self.estimator.fit(X_parts[self.negative_label])
            self.cov += self.estimator.covariance_
        else:
            print 'WARNING: NO MAHALANOBIS USED'
            self.cov = np.eye(X.shape[1])

        self.S = la.sqrtm(la.inv(self.cov))

        P = X.dot(self.S)

        self.clf.fit(P, y)

        if self.method == '2e':

            projected = self.clf.decision_function(P) + self.clf.intercept_ 
            std = {l : np.std(projected[y==l]) for l in (self.positive_label, self.negative_label)}

            self.b = self.clf.intercept_ + (std[self.positive_label] - std[self.negative_label])/(std[self.positive_label] + std[self.negative_label])

        else:
    
            self.b = self.clf.intercept_

        return self

    def predict(self, X):
        return np.array([self.positive_label if self.clf.decision_function(x) + self.clf.intercept_ < self.b else self.negative_label for x in X.dot(self.S)])





import sys
import glob

import os

from cStringIO import StringIO
import sys

if __name__ == '__main__':
    data_dir = "../../data_local/"
    for file_name in os.listdir(data_dir):
        for method in ['2e']:
            for C in (0.1, 1, 10):
                print 'Testing method', method, 'on', file_name
                X, y = load_svmlight_file(data_dir + file_name)
                X = X.toarray()
                xtr, xtst, ytr, ytst = split_data(X, y)
                clf = MahalanobisClassifier(C=C, method=method)
                clf.fit(xtr, ytr)
                print "Data size: ", len(xtr)
                pred_tr = clf.predict(xtr)
                pred_tst = clf.predict(xtst)
                print "SCORE: %s %f %f %d %f" % (file_name, accuracy_score(pred_tr, ytr), accuracy_score(pred_tst, ytst), len(clf.clf.support_vectors_), C)


