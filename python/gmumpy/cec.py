"""
Cross Entropy Clustering module
"""

import numpy as np

import gmumpy.core

from gmumpy.base import BaseEstimator, ClusterMixin, TransformerMixin

class CEC(BaseEstimator, ClusterMixin, TransformerMixin):
    """Cross-Entropy Clustering

    Parameters
    ----------
    n_clusters : int, optional (default=1)
                 The number of clusters to form as well as the number of centroids to generate.

    method_type : {'standard', 'diagonal', 'sphere'} 
                  Type of clustering (Gauss family).
                  'standard' - We divide dataset into ellipsoid-like clusters without any preferences (default)
                  'diagonal' - Gaussians with diagonal covariance. The clustering will try to divide the data into ellipsoid with radiuses parallel to coordinate axes
                  'sphere' - The clustering will try to divide the data into circles of arbitrary sizes

    init : {'random', 'kmeans++'} 
            Method for initialization, defaults to 'kmeans++':
            'kmeans++' : selects initial cluster centers for k-mean clustering in a smart way to speed up convergence
            'random': choose k observations (rows) at random from data for the initial centroids

    n_init : int, optional (default=10)
             Number of time the CEC algorithm will be run with different centroid seeds. The final results will be the best output of n_init consecutive runs in terms of inertia

    tol: float, optional (default=0.05)
                  Percentage size threshold from range (0, 1) for killing cluster

    max_iter: int, optional (default=25)
                    Maximum number of iterations.
                    If set to 0, it will iterate until converge.

    log_energy : int, optional (default=True)
                 If set to True, algorithm will log energy from all iterations

    log_ncluster : int, optional (default=True)
                   If set to True, algorithm will log number of cluster from all iterations

    algorithm : {'hartigan'} 
                Type of algorithm
                'hartigan' - Hartigan algorithm

    Attributes
    ----------

    TODO

    Examples
    --------
    >>>> from gmumpy.datasets import load_mouse1
    >>>> mouse1 = load_mouse1()
    >>>> cec = CEC(n_clusters=3)
    >>>> clustering = cec.fit_predict(mouse1.data)
    >>>> print cec.energy()
    """

    def __init__(self, n_clusters=1, method_type='standard',
            init='kmeans++', n_init=10, tol=0.05,
            max_iter=25, log_energy=True, log_ncluster=True, algorithm='hartigan'):

        available_method_types={'standard', 'diagonal', 'sphere'}
        if method_type not in available_method_types:
            raise Exception('Invalid method type, available: %s' % available_method_types)

        available_init_methods={'kmeans++', 'random'}
        if init not in available_init_methods:
            raise Exception('Invalid init, available: %s' % available_init_methods)

        available_algorithms={'hartigan'}
        if algorithm not in available_algorithms:
            raise Exception('Invalid algorithm, available: %s' % available_algorithms)

        self.n_clusters=n_clusters
        self.method_type=method_type
        self.init=init
        self.n_init=n_init
        self.tol=tol
        self.max_iter=max_iter
        self.log_energy=log_energy
        self.log_ncluster=log_ncluster
        self.algorithm=algorithm
        self.model=None

    def fit(self, X):
        """Compute CEC clustering.

        Parameters
        ----------
        X : array-like matrix, shape=(n_samples, n_features)
            Array of training samples, where n_samples is the number of samples
            and n_features is the number of features.
           
        """
        cfg = CecConfiguration()
        cfg.set_dataset(X)
        cfg.set_eps(self.tol)
        cfg.set_nclusters(self.n_clusters)
        cfg.set_log_energy(self.log_energy)
        cfg.set_log_cluster(self.log_ncluster)
        cfg.set_nstart(self.n_init)
        cfg.set_method_type(self.method_type)
        cfg.set_method_init(self.init)
        cfg.set_it_max(self.max_iter)
        cfg.set_algorithm(self.algorithm)
        self.model = CecModel(cfg)
        return self

    def fit_predict(self, X):
        """Compute cluster centers and predict cluster index for each sample.
           Convenience method; equivalent to calling fit(X) followed by predict(X).
        """
        self.fit(X)
        return self.model.get_assignment()

    def predict(self, X):
        """Predict the closest cluster each sample in X belongs to.

        Parameters
        ----------
        X : array-like matrix, shape=(n_samples, n_features)
            Array of training samples, where n_samples is the number of samples
            and n_features is the number of features.

        Returns
        -------
        y : array, shape (n_samples,)
           
        """
        if self.model is None:
            return None
        return [self.model.predict(x) for x in X]

    def clustering(self):
        """Returns clustering computed by fit method

        Returns
        -------
        y : array, shape (n_samples,)
        """
        if self.model is not None:
            return self.model.get_assignment()
        else:
            return None

    def energy(self):
        """Returns energy computed by fit method

        Returns
        -------
        energy : float
        """
        if self.model is not None:
            return self.model.get_energy()
        else:
            return None

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

