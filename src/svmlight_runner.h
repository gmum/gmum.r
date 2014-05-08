#ifndef SVMLIGHT_RUNNER_H
#define SVMLIGHT_RUNNER_H

#include "svm_handler.h"
#include "svm_basic.h"


class SVMLightRunner : public SVMHandler {
public :
    SVMResult processRequest(SVMConfiguration, SVMResult);
    bool canHandle(SVMConfiguration);
};

#endif
