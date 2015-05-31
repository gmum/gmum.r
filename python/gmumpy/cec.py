"""
Cross Entropy Clustering module
"""

import numpy as np

import gmumpy.core


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

