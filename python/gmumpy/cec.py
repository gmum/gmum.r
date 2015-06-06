"""
Cross Entropy Clustering module
"""

import numpy as np

import gmumpy.core

from gmumpy.base import BaseEstimator, ClusterMixin, TransformerMixin

class CEC_METHOD_TYPE(object):
    STANDARD='standard'
    DIAGONAL='diagonal'
    SPHERE='sphere'

class CEC_METHOD_INIT(object):
    KMEANS='kmeans++'
    RANDOM='random'

class CEC_ALGORITHM(object):
    HARTIGAN='hartigan'

class CEC(BaseEstimator, ClusterMixin, TransformerMixin):
    """Cross-Entropy Clustering

    Parameters
    ----------
    k : int, optional (default=1)
        Number of clusters / features

    method_type : CEC_METHOD_TYPE, optional (default=STANDARD)
                  Type of clustering (Gauss family).
                  DIAGONAL - Gaussians with diagonal covariance. The clustering will try to divide the data into ellipsoid with radiuses parallel to coordinate axes
                  STANDARD - We divide dataset into ellipsoid-like clusters without any preferences (default)
                  SPHERE - The clustering will try to divide the data into circles of arbitrary sizes

    method_init : CEC_METHOD_INIT, optional (default=RANDOM)
                  Initial clustering
                  RANDOM - Starts with random clustering
                  KMEANS - Uses kmeans++ algorithm to initialize clustering

    control_nstart : int, optional (default=10)
                     Number of times CEC algorithm is to be performed. The best outcome is chosen as result.

    control_eps : float, optional (default=0.05)
                  Percentage size threshold from range (0, 1) for killing cluster

    control_itmax : int, optional (default=25)
                    Maximum number of iterations.
                    If set to 0, it will iterate until converge.

    log_energy : int, optional (default=True)
                 If set to True, algorithm will log energy from all iterations

    log_ncluster : int, optional (default=True)
                   If set to True, algorithm will log number of cluster from all iterations

    algorithm : CEC_ALGORITHM, (default=HARTIGAN)
                Type of algorithm

    Attributes
    ----------

    TODO
    """

    def __init__(self, x, k=1, method_type=CEC_METHOD_TYPE.STANDARD,
            method_init=CEC_METHOD_INIT.RANDOM, control_nstart=10, control_eps=0.05,
            control_itmax=25, log_energy=True, log_ncluster=True, algorithm=CEC_ALGORITHM.HARTIGAN):
        self.cfg = CecConfiguration()
        self.cfg.set_dataset(x)
        self.cfg.set_eps(control_eps)
        self.cfg.set_nclusters(k)
        self.cfg.set_log_energy(log_energy)
        self.cfg.set_log_cluster(log_ncluster)
        self.cfg.set_nstart(control_nstart)
        self.cfg.set_method_type(method_type)
        self.cfg.set_method_init(method_init)
        self.cfg.set_it_max(control_itmax)
        self.cfg.set_algorithm(algorithm)
        self.model = CecModel(self.cfg)

    def clustering(self):
        return self.model.get_assignment()

    def energy(self):
        return self.model.get_energy()

class CecConfiguration(gmumpy.core.CecConfiguration):
    """CEC configuration

    Create your CecConfiguration object like this:

    >>> conf = CecConfiguration()

    Set dataset with any numpy array compatible object:

    >>> data = np.loadtxt('../inst/data_sets/cec/mouse_1/input.txt')
    >>> conf.set_dataset(data)

    Fill remaining parameters with appropriate setters:

    >>> conf.set_nclusters(3)
    >>> conf.set_nstart(5)
    >>> conf.set_method_type('standard')
    >>> conf.set_method_init('random')
    >>> conf.set_algorithm('hartigan')
    """

    def __init__(self):
        super(CecConfiguration, self).__init__()

    def set_dataset(self, X):
        super(CecConfiguration, self).set_dataset(np.asfortranarray(X))


class CecModel(gmumpy.core.CecModel):
    """CEC model

    In order to create a model you must setup your configuration:

    >>> conf = CecConfiguration()
    >>> data = np.loadtxt('../inst/data_sets/cec/mouse_1/input.txt')
    >>> conf.set_dataset(data)
    >>> conf.set_nclusters(3)
    >>> conf.set_nstart(5)
    >>> conf.set_method_type('standard')
    >>> conf.set_method_init('random')
    >>> conf.set_algorithm('hartigan')

    Use your configuration object to create a model:

    >>> model = CecModel(conf)

    print model.get_energy()
    print model.get_assignment()

    """

    def __init__(self, cec_model):
        super(CecModel, self).__init__(cec_model)

