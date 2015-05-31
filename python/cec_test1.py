#!/usr/bin/env python2

from gmumpy.core import CecModel, CecConfiguration
import numpy as np
data = np.asfortranarray(np.loadtxt('../inst/data_sets/cec/mouse_1/input.txt'))
conf = CecConfiguration()
conf.set_dataset(data)
conf.set_nclusters(3)
conf.set_nstart(5)
conf.set_method_type('standard')
conf.set_method_init('random')
conf.set_algorithm('hartigan')

model = CecModel(conf)
print model.get_energy()
print model.get_assignment()
