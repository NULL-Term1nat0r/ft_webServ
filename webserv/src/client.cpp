#include "../includes/server.hpp"

server::client::client(int clientSocket, serverConf &serverConfig, int serverIndex) : clientSocket(clientSocket), serverConfig(serverConfig), serverIndex(serverIndex) {
	this->lastActivity = time(0);
	this->clientGetRequest = NULL;
	this->clientPostRequest = NULL;
	this->clientDeleteRequest = NULL;
	this->clientCgiRequest = NULL;
	this->clientResponse = NULL;
	this->baseRequest = NULL;

}

server::client::~client() {
	 if (this->clientGetRequest != NULL){
		 delete this->clientGetRequest;
		 this->clientGetRequest = NULL;
	 }
	 if (this->clientPostRequest != NULL){
		 delete this->clientPostRequest;
		 this->clientPostRequest = NULL;
	 }
	 if (this->clientDeleteRequest != NULL){
		 delete this->clientDeleteRequest;
		 this->clientDeleteRequest = NULL;
	 }
	 if (this->clientCgiRequest != NULL){
		 delete this->clientCgiRequest;
		 this->clientCgiRequest = NULL;
	 }
	 if (this->clientResponse != NULL){
		 delete this->clientResponse;
		 this->clientResponse = NULL;
	 }
}

void server::client::executeClientRequest(){
	std::vector<uint8_t> _request(serverConfig._buffSize);
	int dataRecieved = recv(this->clientSocket, &_request[0], serverConfig._buffSize, 0);
	if (dataRecieved < 0)
		throw std::runtime_error("recv failed");
	else if (dataRecieved == 0)
		throw std::runtime_error("client disconnected");
	createNewRequest(_request);
//	try {
		if (checkGetRequest())
			return;
		else if (checkPostRequest(_request))
			return;
		else if (checkDeleteRequest())
			return;
		else {
			std::cout << green <<  "no valid request\n" << reset << std::endl;
			return;
		}
//	}
//	catch (std::exception &e) {
//		std::cout << "caught exception of clientRequest" << e.what() << std::endl;
//	}
}

bool server::client::checkPostRequest(std::vector<uint8_t> _request) {
	if  (this->clientPostRequest != NULL) {
		if (!clientPostRequest->getAllChunksSent()) {
			try {
				clientPostRequest->writeBinaryToFile(_request);
			}
			catch (std::exception &e) {
				std::cout << "caught exception of post Request " << e.what() << std::endl;
			}
			if (clientPostRequest->getAllChunksSent()) {
				if (clientPostRequest->_baseRequest->_cgi) {
					cgiRequest newCgiRequest = cgiRequest(clientPostRequest->_baseRequest, serverConfig, serverIndex);
					newCgiRequest.executeCgi();
					response *newResponse = new response(newCgiRequest.getFilePath(), newCgiRequest.statusCode, serverConfig);
					clientResponse = newResponse;
					delete this->baseRequest;
					this->baseRequest = NULL;
					delete clientPostRequest;
					clientPostRequest = NULL;
				}
				else {
					response *newResponse = new response("./html_files/uploadSuccessful.html", 201, serverConfig);
					clientResponse = newResponse;
					delete this->baseRequest;
					this->baseRequest = NULL;
					delete clientPostRequest;
					clientPostRequest = NULL;
				}

			}
		}
		return true;
	}
	return false;
}

bool server::client::checkGetRequest() {
	if (this->clientGetRequest != NULL) {
		if (clientGetRequest->_baseRequest->_cgi) {
			cgiRequest newCgiRequest = cgiRequest(clientGetRequest->_baseRequest, serverConfig, serverIndex);
			newCgiRequest.executeCgi();
			response *newResponse = new response(newCgiRequest.getFilePath(), newCgiRequest.statusCode, serverConfig);
			clientResponse = newResponse;
			delete this->baseRequest;
			this->baseRequest = NULL;
			delete clientGetRequest;
			clientGetRequest = NULL;
		}
		else {
			response *newResponse = new response(clientGetRequest->getFilePath(), clientGetRequest->statusCode, serverConfig);
			clientResponse = newResponse;
			delete this->baseRequest;
			this->baseRequest = NULL;
			delete clientGetRequest;
			clientGetRequest = NULL;
		}
		return true;
	}
	return false;
}

bool server::client::checkDeleteRequest() {
	if (this->clientDeleteRequest != NULL) {
		try {
			clientDeleteRequest->deleteFile();
			response *newResponse = new response("./html_files/deleteSuccessful.html", clientDeleteRequest->statusCode, serverConfig);
			clientResponse = newResponse;
		}
		catch (std::exception &e) {
			std::cout << "caught exception of delete Request" << std::endl;
			response *newResponse = new response("./html_files/deleteFailed.html", clientDeleteRequest->statusCode, serverConfig);
			clientResponse = newResponse;
		}
		delete this->baseRequest;
		this->baseRequest = NULL;
		delete clientDeleteRequest;
		clientDeleteRequest = NULL;
		return true;
	}
	return false;
}

//bool server::client::checkCgiRequest() {
//	if (this->clientCgiRequest != NULL) {
//		clientCgiRequest->executeCgi();
//		response *newResponse = new response(clientCgiRequest->getFilePath(), clientCgiRequest->statusCode, serverConfig);
//		clientResponse = newResponse;
//		delete this->baseRequest;
//		this->baseRequest = NULL;
//		delete clientCgiRequest;
//		clientCgiRequest = NULL;
//		return true;
//	}
//	return false;
//}

void server::client::createNewRequest(std::vector<uint8_t> _request){

	if (this->clientGetRequest == NULL && this->clientPostRequest == NULL && this->clientDeleteRequest == NULL && this->clientCgiRequest == NULL) {
		this->baseRequest = new request(_request, serverConfig, serverIndex);
		if (!this->baseRequest->methodIsValid && this->baseRequest->page != "") {
			response *newResponse = new response(serverConfig.getErrorPage(serverIndex, 405), 405, serverConfig);
			delete this->baseRequest;
			clientResponse = newResponse;
			return;
		}
		else if (this->baseRequest->getPostMethod()){
			createNewPostRequest();
		}
		else if (this->baseRequest->getGetMethod()){
			createNewGetRequest();
		}
		else if (this->baseRequest->getDeleteMethod()){
			createNewDeleteRequest();
		}
	}
}

void server::client::createNewGetRequest(){
	this->clientGetRequest = new getRequest(baseRequest, serverConfig, serverIndex);
}

void server::client::createNewPostRequest(){

	this->clientPostRequest = new postRequest(baseRequest, serverConfig, serverIndex);

	if (this->clientPostRequest->getContentLength() > serverConfig._server[serverIndex].bodySize) {
		invalidRequest = true;
		response *newResponse = new response(serverConfig.getErrorPage(serverIndex, 413), 413, serverConfig);
		clientResponse = newResponse;
		delete this->baseRequest;
		this->baseRequest = NULL;
		delete clientPostRequest;
		clientPostRequest = NULL;
	}
	else if (!serverConfig.checkFileType(this->clientPostRequest->getFileExtension())) {
		invalidRequest = true;
		response *newResponse = new response(serverConfig.getErrorPage(serverIndex, 415), 415, serverConfig);
		clientResponse = newResponse;
		delete this->baseRequest;
		this->baseRequest = NULL;
		delete clientPostRequest;
		clientPostRequest = NULL;
	}
}

void server::client::createNewDeleteRequest(){
	this->clientDeleteRequest = new deleteRequest(baseRequest);
}

void server::client::createNewCgiRequest(){
	this->clientCgiRequest = new cgiRequest(baseRequest, serverConfig, serverIndex);
}

void server::client::executeClientResponse(){
	if (this->clientResponse != NULL){
		if (this->clientResponse->_allChunkSent) {
			delete this->clientResponse;
			this->clientResponse = NULL;
		}
		else {
			try {
				std::string chunk = this->clientResponse->getChunk(serverConfig._buffSize);
//				usleep(1000);
				if (send(this->clientSocket, &chunk[0], chunk.length(), 0) < 0)
					throw std::runtime_error("send failed");
			}
			catch (std::exception &e) {
				std::cout << green << e.what() << reset << std::endl;
				std::cout << "caught exception of clientResponse" << std::endl;
			}
		}
	}
}
//
//void server::client::cleanRequest(request *clientRequest) {
//	if (dynamic_cast<getRequest *>(clientRequest) != NULL) {
//		delete dynamic_cast<getRequest *>(clientRequest);
//	}
//	else if (dynamic_cast<postRequest *>(clientRequest) != NULL) {
//		delete dynamic_cast<postRequest *>(clientRequest);
//	}
//	else (dynamic_cast<deleteRequest *>(clientRequest) != NULL) {
//		delete dynamic_cast<deleteRequest *>(clientRequest);
//	}
//}
//
