# RUN IN inst/data_sets/svm FOLDER!

# 2e
svm.twocircles.dataset <- data.frame(read.table(system.file("data_sets", "svm","circles.data", package="gmum.r")));
save(svm.twocircles.dataset, file="svm_two_circles_dataset.RData", compress=TRUE);

# 2e
svm.twoellipsoids.dataset <- data.frame(read.table(system.file("data_sets", "svm","2e.data", package="gmum.r")));
save(svm.twoellipsoids.dataset, file="svm_two_ellipsoids_dataset.RData", compress=TRUE);

# Breast Cancer
svm.breastcancer.dataset <- data.frame(read.table(system.file("data_sets", "svm","breast_cancer.data", package="gmum.r"),  quote="\""));
save(svm.breastcancer.dataset, file="svm_breast_cancer_dataset.RData", compress=TRUE);


# Transduction

system(system.file("data_sets", "svm", "download_transduction.sh",
                   mustWork=TRUE, package="gmum.r"))

train.transduction <- read.matrix.csr(system.file("data_sets", "svm", 
                                                  "transductive", "train_transduction.dat",
                                                  mustWork=TRUE, package="gmum.r"))
train.induction <- read.matrix.csr(system.file("data_sets", "svm", 
                                               "transductive", "train_induction.dat",
                                               mustWork=TRUE, package="gmum.r"))

# Rest zeros out
train.transduction$x <- train.transduction$x[,1:9253]
train.induction$x <- train.induction$x[,1:9253]
test <- read.matrix.csr(system.file("data_sets", "svm", 
                                    "transductive", "test.dat",
                                    mustWork=TRUE, package="gmum.r"))

# Now standarize labels (this ugly code is because read.matrix.csr fails)
train.induction$y <- as.factor(as.numeric(train.induction$y))
train.transduction$y <- as.numeric(train.transduction$y)
train.transduction$y[train.transduction$y == 1] = 0
train.transduction$y  <- as.factor(train.transduction$y)
test$y <- (as.numeric(test$y))
test$y[test$y == 1] = 3
test$y[test$y == 2] = 1
test$y[test$y == 3] = 2
test$y <- as.factor(test$y)

# Save on space
test$x <- NULL

transduction.test.data <- list(tr=train.transduction, ind=train.induction, test=test)

save(transduction.test.data, file="transduction.test.data.RData")

