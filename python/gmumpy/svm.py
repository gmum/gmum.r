"""
Support Vector Machines module
"""

import numpy as np

import gmumpy.core

from scipy.sparse.csr import csr_matrix

from gmumpy.base import ClassifierMixin, BaseEstimator


class SVC(ClassifierMixin, BaseEstimator):
    """Support Vector Classifier

    Parameters
    ----------

    TODO

    Attributes
    ----------

    TODO
    """

    def __init__(self, C=1.0, cache_size=200, class_weight=None, coef0=0.0,
                 degree=3, gamma=0.0, kernel='rbf', max_iter=-1,
                 probability=False, random_state=None, shrinking=True,
                 tol=1e-3, verbose=False):
        # TODO
        pass


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
        super(SVMConfiguration, self).setData(np.asfortranarray(X))

    def setTarget(self, y):
        if isinstance(y, csr_matrix):
            y = y.toarray()
        super(SVMConfiguration, self).setTarget(np.asfortranarray(y))


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
        return super(SVMClient, self).predict(np.asfortranarray(X))

