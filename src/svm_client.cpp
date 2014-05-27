#include "svm_client.h"

// SVMClient
SVMClient::SVMClient(SVMConfiguration *config) {
	SVMConfiguration current_config = *config;
	this->config = current_config;
}

SVMConfiguration SVMClient::getConfig() {
	return SVMClient::config;
}

arma::vec SVMClient::getResult() {
	return SVMClient::config.result;
}

// Main client function, it uses FlowFactory to get a certain work flow
// and runs processRequest(c,r) function on each block returing last result
void SVMClient::run() {
	SVMClient::createFlow();
	for (std::vector<SVMHandler*>::iterator iter = SVMHandlers.begin();
			iter != SVMHandlers.end(); ++iter) {
		(*iter)->processRequest(config);
	}
}

void SVMClient::createFlow() {
	SVMType svm_type = config.getParams().svm_type;
	Preprocess preprocess = config.getParams().preprocess;
	std::vector<SVMHandler*> handlers;

	//	switch (svm_type) {
	//		case LIBSVM: {
	//			LibSVMRunner runner;
	//			handlers.push_back(&runner);
	//			break;
	//		}
	//		case SVMLIGHT : {	SVMLightRunner runner;		// Wating for svm light runner implementation
	//			handlers.push_back( &runner );
	//			break;
	//		}
	//		default: {
	//			LibSVMRunner runner;				// dafault will be libsvm
	//			handlers.push_back(&runner);
	//			break;
	//		}
	//	}

	switch (preprocess) {
	// case TWOE :	{	TwoeSVMPostprocessor post_runner;
	// 				TwoeSVMPreprocessor pre_runner;
	// 				handles.insert( handlers.bedin(), pre_runner );
	// 				handlers.push_back( post_runner );
	// 				break;
	// 			}

	case VK:
		break; // TODO

	case NONE:
		break;

	case NORM: {
		NormRunner norm_runner;
		handlers.push_back(&norm_runner);
		break;
	}
	default:
		break;
	}

	this->SVMHandlers = handlers;
}

