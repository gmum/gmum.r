"""
================
CEC SVM Pipeline
================
Exemplary usage of Pipeline that runs cross entropy clustering and then
support vector classification. Regular SVC provided for comparison.
"""
print(__doc__)

from gmumpy.cec import CEC
from gmumpy.svm import SVC
from sklearn.datasets import samples_generator
from sklearn.pipeline import Pipeline

# Generate some data to play with
X, y = samples_generator.make_classification(
    n_informative=5, n_redundant=0, random_state=42)

# Regular SVC
svc = SVC(kernel='linear')
svc.fit(X, y)

# CEC SVM
cec = CEC(n_clusters=20, method_type='standard')
clf = SVC(kernel='linear')
cec_svm = Pipeline([('cec', cec), ('svc', clf)])

cec_svm.fit(X, y)

print 'SVC score:', svc.score(X, y)
print 'CEC SVM score:', cec_svm.score(X, y)
