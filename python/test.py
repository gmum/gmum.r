#!/usr/bin/env python2

from gmumpy import core

print dir(core)

print core.Algorithm
print dir(core.CecModel)

print ''
cc = core.CecConfiguration()
print cc

# FIXME
cec = core.CecModel(cc)
print cec

exit(0)

