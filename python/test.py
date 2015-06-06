#!/usr/bin/env python2

import numpy as np
from sklearn.datasets import load_svmlight_file
from gmumpy.datasets import load_mouse1
from gmumpy.svm import SVMConfiguration
from gmumpy.cec import CEC

X, y = load_svmlight_file('../inst/data_sets/svm/breast_cancer.data')

sc = SVMConfiguration()

mouse1 = load_mouse1()
cec = CEC(x=mouse1.data, k=mouse1.k)
print cec.energy()
