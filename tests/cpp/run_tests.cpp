#include "gtest/gtest.h"

#include "GNG.h"

/** Include all tests */

DebugCollector dbg;
int GNGNode::dim =0; 

//#include "basic_convergence.cpp"
#include "graph_tests.cpp"


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
