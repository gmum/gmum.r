"""
Datasets module
"""
import numpy as np
import os
import sklearn.datasets.base

DATA_HOME = os.path.abspath(os.path.realpath(os.path.join(os.path.dirname(__file__), '../../inst/data_sets')))

# TODO: Consider creating `datasets` package TODO that in gmumpy.datasets.base
class Bunch(sklearn.datasets.base.Bunch):

    pass


# TODO
def get_data_home():
    return DATA_HOME


def load_simple1():
    simple1_path = os.path.join(DATA_HOME, 'cec', 'simple_1')
    bunch = Bunch(name='simple1')
    bunch['data'] = np.loadtxt(os.path.join(simple1_path, 'input.txt'))
    bunch['target'] = np.loadtxt(os.path.join(simple1_path, 'cluster.txt'))
    bunch['n_clusters'] = np.unique(bunch.target).size
    return bunch


def load_mouse1():
    mouse1_path = os.path.join(DATA_HOME, 'cec', 'mouse_1')
    bunch = Bunch(name='mouse1')
    bunch['data'] = np.loadtxt(os.path.join(mouse1_path, 'input.txt'))
    bunch['target'] = np.loadtxt(os.path.join(mouse1_path, 'cluster.txt'))
    bunch['n_clusters'] = np.unique(bunch.target).size
    return bunch


def load_ellipse_gauss():
    ellipse_gauss_path = os.path.join(DATA_HOME, 'cec', 'EllipseGauss')
    bunch = Bunch(name='ellipse_gauss')
    bunch['data'] = np.loadtxt(os.path.join(ellipse_gauss_path, 'input.txt'))
    bunch['target'] = np.loadtxt(os.path.join(ellipse_gauss_path, 'cluster.txt'))
    bunch['n_clusters'] = np.unique(bunch.target).size
    return bunch

