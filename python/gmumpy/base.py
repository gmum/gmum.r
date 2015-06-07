"""
Base classes for all estimators
"""

# We are wrapping below classes and might want to expand / reimplement them in
# the future.
# pylint: disable=too-few-public-methods

import sklearn.base


class BaseEstimator(sklearn.base.BaseEstimator):
    """Base class for all estimators in gmumpy"""

    pass


class ClassifierMixin(sklearn.base.ClassifierMixin):
    """Mixin class for all classifiers in gmumpy"""

    pass


class ClusterMixin(sklearn.base.ClusterMixin):
    """Mixin class for all cluster estimators in gmumpy"""

    pass


class RegressorMixin(sklearn.base.RegressorMixin):
    """Mixin class for all regression estimators in gmumpy"""

    pass


class TransformerMixin(sklearn.base.TransformerMixin):
    """Mixin class for all transformers in gmumpy"""

    pass

