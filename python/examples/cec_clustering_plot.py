#!/usr/bin/env python2

import matplotlib.pyplot as plt
from gmumpy.datasets import load_mouse1
from gmumpy.cec import CEC

mouse = load_mouse1()
cec = CEC(n_clusters=mouse.n_clusters, method_type='sphere')
clustering = cec.fit_predict(mouse.data)
plt.scatter(mouse.data[:,0], mouse.data[:,1], c=clustering)
plt.show()

