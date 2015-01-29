# 2e
svm.twocircles.dataset <- data.frame(read.table(system.file("data_sets", "svm","circles.data", package="gmum.r")));
save(svm.twocircles.dataset, file="~/gmum.r/data/svm_two_circles_dataset.RData", compress=TRUE);

# 2e
svm.twoellipsoids.dataset <- data.frame(read.table(system.file("data_sets", "svm","2e.data", package="gmum.r")));
save(svm.twoellipsoids.dataset, file="~/gmum.r/data/svm_two_ellipsoids_dataset.RData", compress=TRUE);

# Breast Cancer
svm.breastcancer.dataset <- data.frame(read.table(system.file("data_sets", "svm","breast_cancer.data", package="gmum.r"),  quote="\""));
save(svm.breastcancer.dataset, file="~/gmum.r/data/svm_breast_cancer_dataset.RData", compress=TRUE);


