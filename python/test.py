#!/usr/bin/env python2

from gmumpy.core import svm_svm_basic as svm_basic

print dir(svm_basic)

sc = svm_basic.SVMConfiguration()
print dir(sc)

kt = svm_basic.SVMLIGHT
print kt

from gmumpy.core import cec_hartigan as hartigan

print dir(hartigan)

from gmumpy.core import cec_cec_configuration as cec_configuration

print dir(cec_configuration)

