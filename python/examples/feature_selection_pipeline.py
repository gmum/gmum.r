"""
==================
Pipeline Anova SVM
==================
Simple usage of Pipeline that runs successively a univariate
feature selection with anova and then a C-SVM of the selected features.

This example comes from scikit-learn package.
"""
print(__doc__)

from gmumpy import svm
from sklearn.datasets import samples_generator
from sklearn.feature_selection import SelectKBest
from sklearn.feature_selection import f_regression
from sklearn.pipeline import Pipeline

# generate some data to play with
X, y = samples_generator.make_classification(
    n_informative=5, n_redundant=0, random_state=42)

# Regular SVC
svc = svm.SVC(kernel='linear')
svc.fit(X, y)

# ANOVA SVM-C
anova_filter = SelectKBest(f_regression, k=5)
clf = svm.SVC(kernel='linear')
anova_svm = Pipeline([('anova', anova_filter), ('svc', clf)])
# You can set the parameters using the names issued
# For instance, fit using a k of 10 in the SelectKBest
# and a parameter 'C' of the svm
anova_svm.set_params(anova__k=10, svc__C=.1).fit(X, y)

print 'SVC score:', svc.score(X, y)
print 'ANOVA SVM-C score:', anova_svm.score(X, y)
