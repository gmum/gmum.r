"""
Datasets module
"""

# pylint: disable=fixme

import os
from numpy.lib.npyio import loadtxt
from numpy.lib.arraysetops import unique
import sklearn.datasets.base

DATA_HOME = os.path.abspath(\
        os.path.realpath(os.path.join(os.path.dirname(__file__), '../../inst/data_sets')))

# TODO: Consider creating `datasets` package TODO that in gmumpy.datasets.base
class Bunch(sklearn.datasets.base.Bunch):
    """Wrapper on sklean Bunch class
    """

    pass


# TODO
def get_data_home():
    """Returns absolute path to data directory
    """
    return DATA_HOME


def load_simple1():
    """Loads and returns simple1 dataset
    """
    simple1_path = os.path.join(DATA_HOME, 'cec', 'simple_1')
    bunch = Bunch(name='simple1')
    bunch['data'] = loadtxt(os.path.join(simple1_path, 'input.txt'))
    bunch['target'] = loadtxt(os.path.join(simple1_path, 'cluster.txt'))
    bunch['n_clusters'] = len(unique(bunch['target']))
    return bunch


def load_mouse1():
    """Loads and returns mouse1 dataset
    """
    mouse1_path = os.path.join(DATA_HOME, 'cec', 'mouse_1')
    bunch = Bunch(name='mouse1')
    bunch['data'] = loadtxt(os.path.join(mouse1_path, 'input.txt'))
    bunch['target'] = loadtxt(os.path.join(mouse1_path, 'cluster.txt'))
    bunch['n_clusters'] = len(unique(bunch['target']))
    return bunch


def load_ellipse_gauss():
    """Loads and returns ellipse gauss dataset
    """
    ellipse_gauss_path = os.path.join(DATA_HOME, 'cec', 'EllipseGauss')
    bunch = Bunch(name='ellipse_gauss')
    bunch['data'] = loadtxt(os.path.join(ellipse_gauss_path, 'input.txt'))
    bunch['target'] = loadtxt(os.path.join(ellipse_gauss_path, 'cluster.txt'))
    bunch['n_clusters'] = len(unique(bunch['target']))
    return bunch
