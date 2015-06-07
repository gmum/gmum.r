"""
Support Vector Machines module
"""

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

import gmumpy.core

from numpy.core.numeric import asfortranarray
from scipy.sparse.csr import csr_matrix

from gmumpy.base import ClassifierMixin, BaseEstimator


class SVC(ClassifierMixin, BaseEstimator):
    """Support Vector Classifier

    Parameters
    ----------
    C : float, optional (default=1.0)
        Penalty parameter. Trade-off between training error and margin

    cache_size : float, optional (default=200)
        Size of cache for kernel evaluations in MB.

    class_weight : dict, optional
        Set the parameter C of class i to class_weight[i]*C for SVC. If not
        given, all classes are supposed to have weight one.

    coef0 : float, optional (default=0.0)
        Independent term in kernel function.

    core : string, optional (default='libsvm')
        Support Vector Machine library to use in learning.
        Available are: 'libsvm', 'svmlight'.
        If none is given, 'libsvm' will be used.

    degree : optional (default=3)
        Degree of the polynomial kernel function ('poly').
        Ignored by all other kernels.

    gamma : float, optional (default=0.0)
        Kernel coefficient for 'rbf', 'poly' and 'sigmoid'.
        If gamma is 0.0 then 1/n_features will be used instead.

    kernel : string, optional (default='rbf')
        Specifies the kernel type to be used in the algorithm.
        It must be one of 'linear', 'poly', 'rbf' or 'sigmoid'.
        If none is given, 'rbf' will be used.

    max_iter : int, optional (default=-1)
        Hard limit on iterations within solver, or -1 for no limit.

    tol : float, optional (default=1e-3)
        Tolerance for stopping criterion.
        Currently, supported only with 'libsvm' core.

    verbose : bool, optional (default: False)
        Enable verbose output.
        If True, sets verbosity to 6. False sets verbosity to 0 unless other
        given as a verbosity parameter.

    Attributes
    ----------
    support_vectors_ : array-like, shape = [n_SV, n_features]
        Support vectors.

    configuration_ : SVMConfiguration
        Current representation of the SVM. GMUM core class object.

    client_ : SVMClient
        Core libraries wrapper and GMUM core prediction implementation.

    Examples
    --------
    >>> from sklearn.datasets import load_svmlight_file
    >>> X, y = load_svmlight_file('../inst/data_sets/svm/breast_cancer.data')
    >>> clf = SVC()
    >>> clf.fit(X, y) #doctest: +NORMALIZE_WHITESPACE
    SVC(C=1.0, cache_size=200, class_weight=None, coef0=0.0, core='libsvm',
        degree=3, gamma=0.0, kernel='rbf', max_iter=-1, random_state=None,
        tol=0.001, verbose=False)
    >>> print(clf.predict([[-1, -1, -1, -1, -1, -1, -0.5, -1, -1, -1]]))
    [ 2.]

    """

    # TODO: class_weight = 'auto'
    # TODO: kernel: 'precomputed' and callable
    # TODO: Add probability (?)
    # TODO: Implement random_state
    # TODO: Add shrinking (?)

    def __init__(self, C=1.0, cache_size=200, class_weight=None, coef0=0.0,
                 core='libsvm', degree=3, gamma=0.0, kernel='rbf', max_iter=-1,
                 random_state=None, tol=1e-3, verbose=False):

        # Parameter exceptions

        if random_state:
            print 'WARINIG: random_state parameter currently not implemented.'

        # Parameter collection

        self.C = C
        self.cache_size = cache_size
        self.class_weight = class_weight
        self.coef0 = coef0
        self.core = core
        self.degree = degree
        self.gamma = gamma
        self.kernel = kernel
        self.max_iter = max_iter
        self.random_state = random_state
        self.tol = tol
        self.verbose = verbose

        # Additional parameters

        if verbose:
            self.verbosity = 6
        else:
            self.verbosity = 0

        # Flags

        self.fitted = False

        # Attributes

        self.configuration_ = SVMConfiguration()
        self.client_ = SVMClient(self.configuration_)


        self.client_.setC(C)
        self.client_.setCacheSize(cache_size)
        # TODO: wrap in SVMClient
        if class_weight:
            self.client_.getConfiguration().setClassWeights(class_weight)
        self.client_.setCoef0(coef0)
        self.client_.setLibrary(core)
        self.client_.setDegree(degree)
        self.client_.setGamma(gamma)
        self.client_.setKernel(kernel)
        # TODO: wrap in SVMClient
        self.client_.getConfiguration().max_iter = max_iter
        # TODO: random_state
        self.client_.setEps(tol)
        self.client_.getConfiguration().set_verbosity(self.verbosity)

    def fit(self, X, y, sample_weight=None):
        """Fit the SVM model according to the given training data.

        Parameters
        ----------
        X : {array-like, sparse matrix}, shape (n_samples, n_features)
            Array of training samples, where n_samples is the number of samples
            and n_features is the number of features.

        y : array-like, shape (n_samples,)
            Target values.

        sample_weight : array-like, shape (n_samples,)
            Per-sample weights.

        Returns
        -------
        self : object
            Returns self.
        """
        # TODO: sample_weight
        if sample_weight:
            print 'WARINIG: sample_weight parameter currently not implemented.'

        self.configuration_.setData(X)
        self.configuration_.setTarget(y)
        self.client_.setConfiguration(self.configuration_)
        self.client_.train()
        self.fitted = True

        return self

    def predict(self, X):
        """Perform prediction on samples in X.

        Parameters
        ----------
        X : {array-like, sparse matrix}, shape (n_samples, n_features)
            Array of prediction samples, where n_samples is the number of
            samples and n_features is the number of features.

        Returns
        -------
        y_pred : array, shape (n_samples,)
        """
        X = self._validate_for_predict(X)

        self.configuration_.setPrediction(True)
        self.client_.setConfiguration(self.configuration_)
        self.client_.predict(X)
        return self.client_.getConfiguration().result

    def decision_function(self, X):
        """Distance of the samples X to the separating hyperplane.

        Parameters
        ----------
        X : array-like, shape = [n_samples, n_features]

        Returns
        -------
        X : array-like, shape = [n_samples, n_class * (n_class-1) / 2]
            Returns the decision function of the sample for each class
            in the model.
        """
        X = self._validate_for_predict(X)

        # TODO: Implement in C++
        raise NotImplementedError(
            "decision_function is currently not implemented in core")

    def _validate_for_predict(self, X):
        """Validate data and SVC if they are good to perform a predict()"""
        if not self.fitted:
            raise ValueError("You must use fit() before trying to predict()")
        return X


class SVMConfiguration(gmumpy.core.SVMConfiguration):
    """SVM configuration

    Create your SVMConfiguration object like this:

    >>> sc = SVMConfiguration()

    Fill it with data using setData method:

    >>> from sklearn.datasets import load_svmlight_file
    >>> X, y = load_svmlight_file('../inst/data_sets/svm/breast_cancer.data')
    >>> sc.setData(X)
    >>> sc.setTarget(y)

    Note that so created SVM configuration contains default params.
    """

    def setData(self, X):
        if isinstance(X, csr_matrix):
            X = X.toarray()
        super(SVMConfiguration, self).setData(asfortranarray(X))

    def setTarget(self, y):
        if isinstance(y, csr_matrix):
            y = y.toarray()
        super(SVMConfiguration, self).setTarget(
            asfortranarray(y).astype(float)
        )


class SVMClient(gmumpy.core.SVMClient):
    """SVM client

    You must pass SVMConfiguration object when creating a new SVMClient. Let's
    create for example:

    >>> sc = SVMConfiguration()
    >>> from sklearn.datasets import load_svmlight_file
    >>> X, y = load_svmlight_file('../inst/data_sets/svm/breast_cancer.data')
    >>> sc.setData(X)
    >>> sc.setTarget(y)
    >>> sc.setKernel('rbf')

    In order to train your SVM you must turn off the prediction flag:

    >>> sc.setPrediction(False)

    Then you can simply pass the object to the constructor:

    >>> svm = SVMClient(sc)
    >>> svm.run()

    After that, we can try a prediction:

    >>> sc.setPrediction(True)
    >>> svm = SVMClient(sc)
    >>> svm.predict(X)
    >>> import sklearn.metrics
    >>> assert(sklearn.metrics.accuracy_score(sc.result, y) > 0.6)

    """

    def predict(self, X):
        if isinstance(X, csr_matrix):
            X = X.toarray()
        return super(SVMClient, self).predict(asfortranarray(X))

