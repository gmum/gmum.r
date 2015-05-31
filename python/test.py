#!/usr/bin/env python2

import numpy as np
from sklearn.datasets import load_svmlight_file

from gmumpy.svm import SVMConfiguration

X, y = load_svmlight_file('../inst/data_sets/svm/breast_cancer.data')

sc = SVMConfiguration()


