"""
Cross Entropy Clustering module
"""
# Pylint shows messages like: Module 'numpy' has no 'array' member, which is not true
# pylint: disable=no-member
# Arguments number may differ since we are extending SWIG-generated code
# which implements method params as *args, **kwargs for their own reasons
# pylint: disable=arguments-differ
# pylint: disable=fixme
# We are using sklearn-like argument names: X, y, C etc.
# pylint: disable=invalid-name
# We are using sklearn-like argument number
# pylint: disable=too-many-arguments
# We are using sklearn get_params() and set_params()
# pylint: disable=too-many-instance-attributes
# I've done it like sklearn KMeans
# pylint: disable=unused-argument
# pylint: disable=protected-access

import warnings
import numpy as np

import gmumpy.core
from gmumpy.base import BaseEstimator, ClusterMixin, TransformerMixin

from sklearn.metrics.pairwise import euclidean_distances
from sklearn.utils import check_array
from sklearn.utils.validation import check_is_fitted

class CEC(BaseEstimator, ClusterMixin, TransformerMixin):
    """Cross-Entropy Clustering

    Parameters
    ----------
    n_clusters : int, optional (default=1)
                 The number of clusters to form as well as the number of centroids to generate.

    method_type : {'standard', 'diagonal', 'sphere'}
                  Type of clustering (Gauss family).
                  'standard' - Divides dataset into ellipsoid-like clusters without any preferences
                  'diagonal' - Gaussians with diagonal covariance.
                               The clustering will try to divide the data into ellipsoid
                               with radiuses parallel to coordinate axes
                  'sphere' - Divides the data into circles of arbitrary sizes

    init : {'random', 'kmeans++'}
            Method for initialization, defaults to 'kmeans++':
            'kmeans++' : selects initial cluster centers for k-mean clustering in a smart
                         way to speed up convergence
            'random': choose k observations (rows) at random from data for the initial centroids

    n_init : int, optional (default=10)
             Number of time the CEC algorithm will be run with different centroid seeds.
             The final results will be the best output of n_init consecutive runs
             in terms of inertia

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
    cluster_centers_ : array, [n_clusters, n_features]
        Coordinates of cluster centers

    labels_ :
        Labels of each point

    energy_ :
        Cost of the best assignment

    energy_history_ :
        List of costs from each iteration

    ncluster_history_ :
        List of number or clusters from each iteration

    n_iter_ :
        Total number of iterations

    Examples
    --------
    >>> cec = CEC()
    >>> cec = CEC(n_clusters=3)
    >>> cec = CEC(n_clusters=3, method_type='sphere', init='random', n_init=5, tol=0.01)
    """

    def __init__(self, n_clusters=1, method_type='standard',
                 init='kmeans++', n_init=10, tol=0.05,
                 max_iter=25, log_energy=True, log_ncluster=True, algorithm='hartigan'):

        available_method_types = {'standard', 'diagonal', 'sphere'}
        if method_type not in available_method_types:
            raise Exception('Invalid method type, available: %s' % available_method_types)

        available_init_methods = {'kmeans++', 'random'}
        if init not in available_init_methods:
            raise Exception('Invalid init, available: %s' % available_init_methods)

        available_algorithms = {'hartigan'}
        if algorithm not in available_algorithms:
            raise Exception('Invalid algorithm, available: %s' % available_algorithms)

        self.n_clusters = n_clusters
        self.method_type = method_type
        self.init = init
        self.n_init = n_init
        self.tol = tol
        self.max_iter = max_iter
        self.log_energy = log_energy
        self.log_ncluster = log_ncluster
        self.algorithm = algorithm
        self._model = None
        self.cluster_centers_ = None
        self.n_iter_ = None
        self.labels_ = None
        self.energy_ = None
        self.energy_history_ = None
        self.ncluster_history_ = None

    def _check_fit_data(self, X):
        """Verify that the number of samples given is larger than k"""
        X = check_array(X, accept_sparse='csr', dtype=np.float64)
        if X.shape[0] < self.n_clusters:
            raise ValueError("n_samples=%d should be >= n_clusters=%d" % (
                X.shape[0], self.n_clusters))
        return X

    def _check_test_data(self, X):
        """Verify test data"""
        X = check_array(X, accept_sparse='csr')
        _, n_features = X.shape
        expected_n_features = self.cluster_centers_.shape[1]
        if not n_features == expected_n_features:
            raise ValueError("Incorrect number of features. "
                             "Got %d features, expected %d" % (
                                 n_features, expected_n_features))
        if X.dtype.kind != 'f':
            warnings.warn("Got data type %s, converted to float "
                          "to avoid overflows" % X.dtype,
                          RuntimeWarning, stacklevel=2)
            X = X.astype(np.float)

        return X

    def fit(self, X, y=None):
        """Compute CEC clustering.

        Parameters
        ----------
        X : array-like matrix, shape=(n_samples, n_features)
            Array of training samples, where n_samples is the number of samples
            and n_features is the number of features.

        Examples
        --------
        >>> from gmumpy.datasets import load_mouse1
        >>> mouse = load_mouse1()
        >>> c = CEC(n_clusters=mouse.n_clusters)
        >>> c.fit(mouse.data) #doctest: +NORMALIZE_WHITESPACE
        CEC(algorithm='hartigan', init='kmeans++', log_energy=True, log_ncluster=True,
              max_iter=25, method_type='standard', n_clusters=3, n_init=10, tol=0.05)
        """
        X = self._check_fit_data(X)
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
        self._model = CecModel(cfg)
        self.cluster_centers_ = self._model.centers()
        self.n_iter_ = self._model.iters()
        self.labels_ = self._model.get_assignment()
        self.energy_ = self._model.get_energy()
        if self.log_energy:
            self.energy_history_ = self._model.get_energy_history()
        if self.log_ncluster:
            self.ncluster_history_ = self._model.get_nclusters()
        return self

    def fit_predict(self, X, y=None):
        """Compute cluster centers and predict cluster index for each sample.
           Convenience method; equivalent to calling fit(X) followed by predict(X).

        Examples
        --------
        >>> from gmumpy.datasets import load_mouse1
        >>> mouse = load_mouse1()
        >>> c = CEC(n_clusters=mouse.n_clusters)
        >>> clustering = c.fit_predict(mouse.data)
        """
        return self.fit(X).labels_

    def fit_transform(self, X, y=None):
        """Compute clustering and transform X to cluster-distance space.
        Equivalent to fit(X).transform(X), but more efficiently implemented.
        """
        # Currently, this just skips a copy of the data if it is not in
        # np.array or CSR format already.
        # XXX This skips _check_test_data, which may change the dtype;
        # we should refactor the input validation.
        X = self._check_fit_data(X)
        return self.fit(X)._transform(X)

    def transform(self, X, y=None):
        """Transform X to a cluster-distance space.
        In the new space, each dimension is the distance to the cluster
        centers.  Note that even if X is sparse, the array returned by
        `transform` will typically be dense.
        Parameters
        ----------
        X : {array-like, sparse matrix}, shape = [n_samples, n_features]
            New data to transform.
        Returns
        -------
        X_new : array, shape [n_samples, k]
            X transformed in the new space.
        """
        check_is_fitted(self, 'cluster_centers_')

        X = self._check_test_data(X)
        return self._transform(X)

    def _transform(self, X):
        """guts of transform method; no input validation"""
        return euclidean_distances(X, self.cluster_centers_)

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

        Examples
        --------
        >>> from gmumpy.datasets import load_mouse1
        >>> mouse = load_mouse1()
        >>> c = CEC(n_clusters=mouse.n_clusters)
        >>> c.fit(mouse.data) #doctest: +NORMALIZE_WHITESPACE
        CEC(algorithm='hartigan', init='kmeans++', log_energy=True, log_ncluster=True,
                max_iter=25, method_type='standard', n_clusters=3, n_init=10, tol=0.05)
        >>> cluster_id = c.predict([[1.0, 2.0], [3.0, 4.0]])
        """
        check_is_fitted(self, 'cluster_centers_')

        X = self._check_test_data(X)
        return np.array([self._model.predict(x) for x in X])

class CecConfiguration(gmumpy.core.CecConfiguration):
    """CEC configuration

    Create your CecConfiguration object like this:

    >>> conf = CecConfiguration()

    Set dataset with any numpy array compatible object:

    >>> import numpy as np
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
    >>> import numpy as np
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

