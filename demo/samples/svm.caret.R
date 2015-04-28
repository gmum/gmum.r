# We support caret, so that you can construct
# easily complex experiments using gmum.R

library(gmum.r)
library(caret)
library(mlbench) # For Sonar dataset
set.seed(777)

# Task to discriminate between sonar signals
# bounced off a metal cylinder or rougly cylindrical rock
data(Sonar)

inTraining <- createDataPartition(Sonar$Class, p = .75, list = FALSE)
training <- Sonar[ inTraining,]
testing  <- Sonar[-inTraining,]

fitControl <- trainControl(method = "cv",
                           ## 10-fold CV...
                           number = 10,
                           ## repeated 2 times
                           repeats = 1,
                           verboseIter=FALSE
)


# Caret support is provided by a set of care methods
# You can choose between: caret.gmumSvmRadial, caret.gmumSvmLinear
# and caret.gmumSvmPoly
model <- train(Class ~ ., data = training,
               method = caret.gmumSvmRadial,
               preProc = c("center", "scale"),
               tuneLength = 8,             
               trControl = fitControl,
               tuneGrid = expand.grid(C=10^(c(-4:4)), gamma=10^(c(-4:4))),
               lib = "libsvm", # gmum.R parameter - pick library
               verbosity = 0 # no outputs
               )

# Print 10-fold CV experiment results 
print(model)

# Seems that gamma=0.01 and and C=100 was best choice
plot(model)
