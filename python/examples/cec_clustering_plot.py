#!/usr/bin/env python2
"""
==================
CEC clustering plot
==================
Simple usage of CEC class and plot results
"""
print __doc__

import matplotlib.pyplot as plt
from gmumpy.datasets import load_mouse1
from gmumpy.cec import CEC

def main():
    """
    Main function for this example
    """
    mouse = load_mouse1()
    data = mouse['data']
    n_clusters = mouse['n_clusters']
    cec = CEC(n_clusters=3, method_type='sphere')
    clustering = cec.fit_predict(data)
    plt.scatter(data[:, 0], data[:, 1], c=clustering)
    plt.show()
    print 'energy', cec.energy_
    print 'iters', cec.n_iter_
    print 'centers', cec.cluster_centers_
    print 'energy history', cec.energy_history_
    print 'ncluster history', cec.ncluster_history_
    #print 'labels': cec.labels_ 

if __name__ == '__main__':
    main()
