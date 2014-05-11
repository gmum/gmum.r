#include "svm_flow_factory.h"
#include <vector>

std::vector<SVMHandler*> SVMFlowFactory::createSVMFlow( SVMConfiguration config ) {

	SVMType svm_type = config.getParams().svm_type;
	Preprocess preprocess = config.getParams().preprocess;
	std::vector<SVMHandler*> handlers;

	switch( svm_type ) {
		case LIBSVM : { LibSVMRunner runner;
					  	handlers.push_back( &runner );
					  	break;
					  }
		// case SVMLIGHT : {	SVMLightRunner runner;		// Wating for svm light runner implementation
		// 			  		handlers.push_back( &runner );
		// 			  		break;
		// 				}

		default :	  {	LibSVMRunner runner;				// dafault will be libsvm
					  	handlers.push_back( &runner );
					  	break;
					  }
	}

	switch ( preprocess ) {
		// case TWOE :	{	TwoeSVMPostprocessor post_runner; 
		// 				TwoeSVMPreprocessor pre_runner;
		// 				handles.insert( handlers.bedin(), pre_runner );
		// 				handlers.push_back( post_runner );
		// 				break;
		// 			}
					
		case VK :	break; // TODO
					
		case NONE : break;
		
		// case NORM : {	NormRunner norm_runner;
		// 				handlers.push_back( &norm_runner );
		// 				break;
		// 			}

		default : break;

	}

	return handlers;
}