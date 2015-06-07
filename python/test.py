#!/usr/bin/env python2

import numpy as np
from sklearn.datasets import load_svmlight_file
from gmumpy.datasets import load_mouse1
from gmumpy.svm import SVMConfiguration
from gmumpy.cec import CEC

X, y = load_svmlight_file('../inst/data_sets/svm/breast_cancer.data')

sc = SVMConfiguration()

mouse1 = load_mouse1()
cec = CEC(n_clusters=mouse1.n_clusters)
clustering = cec.fit_predict(mouse1.data)
print cec.predict([[1.0, 2.0]])
