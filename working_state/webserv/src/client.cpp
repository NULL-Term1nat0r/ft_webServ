#include "../includes/server.hpp"

server::client::client(int clientSocket, serverConf &serverConfig, int serverIndex) : clientSocket(clientSocket), serverConfig(serverConfig), serverIndex(serverIndex) {
	this->lastActivity = time(0);
	this->clientGetRequest = NULL;
	this->clientPostRequest = NULL;
	this->clientDeleteRequest = NULL;
	this->clientCgiRequest = NULL;
	this->clientResponse = NULL;

}

server::client::~client() {
}

void server::client::executeClientRequest(){
	std::vector<uint8_t> _request(serverConfig._buffSize);
	recv(this->clientSocket, &_request[0], serverConfig._buffSize, 0);
//	std::cout << parsing::vectorToString(_request) << std::endl;
	createNewRequest(_request);
//	try {
		if (checkGetRequest())
			return;
		else if (checkPostRequest(_request))
			return;
		else if (checkDeleteRequest())
			return;
		else if (checkCgiRequest())
			return;
		else
			return;
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
				std::cout << "caught exception of post Request" << std::endl;
			}
			if (clientPostRequest->getAllChunksSent()) {
				response *newResponse = new response("./html_files/uploadSuccessful.html", 201);
				clientResponse = newResponse;
				delete this->clientPostRequest;
				this->clientPostRequest = NULL;
			}
		}
		return true;
	}
	return false;
}

bool server::client::checkGetRequest() {
	if (this->clientGetRequest != NULL) {
		std::cout << "filePath: " << clientGetRequest->getFilePath() << std::endl;
		if (this->clientGetRequest->autoIndexListing) {
			std::cout << "response for 619 got created\n";
			response *newResponse = new response(this->clientGetRequest->getFilePath(), 619);
			clientResponse = newResponse;
			delete clientGetRequest;
			clientGetRequest = NULL;
			return true;
		}
		else {
			response *newResponse = new response(clientGetRequest->getFilePath(), 200);
			clientResponse = newResponse;
			delete clientGetRequest;
			clientGetRequest = NULL;
			return true;
		}
	}
	return false;
}

bool server::client::checkDeleteRequest() {
	if (this->clientDeleteRequest != NULL) {
		response *newResponse = new response("./html_files/deleteSuccessful.html", 200);
		clientResponse = newResponse;
		delete clientDeleteRequest;
		clientDeleteRequest = NULL;
		return true;
	}
	return false;
}

bool server::client::checkCgiRequest() {
	if (this->clientCgiRequest != NULL) {
		std::cout << "cgi filePath: " << clientCgiRequest->getFilePath() << std::endl;
		clientCgiRequest->executeCgi();
		response *newResponse = new response(clientCgiRequest->getFilePath(), 200);
		clientResponse = newResponse;
		delete clientCgiRequest;
		clientCgiRequest = NULL;
		return true;
	}
	return false;
}

//void server::client::createNewRequest(std::vector<uint8_t> _request){
//
//	if (this->clientGetRequest == NULL && this->clientPostRequest == NULL && this->clientDeleteRequest == NULL && this->clientCgiRequest == NULL) {
//		request newRequest = request(_request, serverConfig, serverIndex);
//		std::cout << parsing::vectorToString(_request) << std::endl;
//		std::cout << "get cgi method: " << newRequest.getCgi() << std::endl;
//		if (newRequest.getGetMethod() && newRequest.getCgi()) {
//			std::cout << "create new CgiRequest\n";
//			cgiRequest *newCgiRequest = new cgiRequest(_request, serverConfig, serverIndex);
//			this->clientCgiRequest = newCgiRequest;
//		}
//		else if (newRequest.getPostMethod()){
//			std::cout << "create new PostRequest\n";
//			postRequest *newPostRequest = new postRequest(_request, serverConfig, serverIndex);
//			this->clientPostRequest= newPostRequest;
//		}
//		else if (newRequest.getGetMethod()){
//			std::cout << "create new GetRequest\n";
//			getRequest *newGetRequest = new getRequest(_request, serverConfig, serverIndex);
//			this->clientGetRequest = newGetRequest;
//		}
//		else if (newRequest.getDeleteMethod()){
//			deleteRequest *newDeleteRequest = new deleteRequest(_request, serverConfig, serverIndex);
//			this->clientDeleteRequest = newDeleteRequest;
//		}
//	}
//}

void server::client::createNewRequest(std::vector<uint8_t> _request){

	if (this->clientGetRequest == NULL && this->clientPostRequest == NULL && this->clientDeleteRequest == NULL && this->clientCgiRequest == NULL) {
		request newRequest = request(_request, serverConfig, serverIndex);
		std::cout << parsing::vectorToString(_request) << std::endl;

		std::string method  = request::getMethodString(newRequest);
		if (method == "")
			return;
		if (!request::checkPageMethod(method, newRequest.getStringURL(), serverIndex, serverConfig)) {

			response *newResponse = new response("./html_files/errorPages/error405.html", 405);
			clientResponse = newResponse;
			return;
		}
		if (newRequest.getGetMethod() && newRequest.getCgi())
			createNewCgiRequest(_request, newRequest);
		else if (newRequest.getPostMethod()){
			createNewPostRequest(_request, newRequest);
		}
		else if (newRequest.getGetMethod()){
			createNewGetRequest(_request, newRequest);
		}
		else if (newRequest.getDeleteMethod()){
			createNewDeleteRequest(_request, newRequest);
		}
	}
}

void server::client::createNewGetRequest(std::vector<uint8_t> _request, request &newRequest){
	this->clientGetRequest = new getRequest(_request, serverConfig, serverIndex);
}

void server::client::createNewPostRequest(std::vector<uint8_t> _request, request &newRequest){

	this->clientPostRequest = new postRequest(_request, serverConfig, serverIndex);
	if (this->clientPostRequest->getContentLength() > serverConfig._server[serverIndex].bodySize) {
		response *newResponse = new response("./html_files/errorPages/error413.html", 413);
		clientResponse = newResponse;
		delete this->clientPostRequest;
		this->clientPostRequest = NULL;
	}
}

void server::client::createNewDeleteRequest(std::vector<uint8_t> _request, request &newRequest){
	this->clientDeleteRequest = new deleteRequest(_request, serverConfig, serverIndex);
}

void server::client::createNewCgiRequest(std::vector<uint8_t> _request, request &newRequest){
	this->clientCgiRequest = new cgiRequest(_request, serverConfig, serverIndex);
}

void server::client::executeClientResponse(){
	if (this->clientResponse != NULL){
		if (this->clientResponse->_allChunkSent && this->clientResponse->filePath.find("html_files/tmzgugp_cgi") != std::string::npos) {
			if (this->clientResponse->removeFile(this->clientResponse->filePath.c_str()))
				std::cout << "file removed\n";
			else
				std::cout << "file not removed\n";
		}
		else if (this->clientResponse->_allChunkSent) {
			delete this->clientResponse;
			this->clientResponse = NULL;
		}
		else {
			std::string chunk = this->clientResponse->getChunk(serverConfig._buffSize);
			send(this->clientSocket, &chunk[0], chunk.length(), 0);
		}
	}
}
