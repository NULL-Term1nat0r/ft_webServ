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
	this->lastActivity = time(0);
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
				setUpResponse("./html_files/uploadSuccessful.html", 201, clientPostRequest);
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
			setUpResponse(clientGetRequest->getFilePath(), 619, clientGetRequest);
			return true;
		}
		else {
			setUpResponse(clientGetRequest->getFilePath(), 200, clientGetRequest);
			return true;
		}
	}
	return false;
}

bool server::client::checkDeleteRequest() {
	if (this->clientDeleteRequest != NULL) {
		std::cout << "hopped into client delete Request handling \n";
		try {
			this->clientDeleteRequest->deleteFile();
		}
		catch (std::exception &e) {
			std::cout << "caught exception of delete Request" << std::endl;
			setUpResponse("./html_files/errorPages/error404.html", 404, clientDeleteRequest);
			return true;
		}
		setUpResponse("./html_files/upload/deleteSuccessful.html", 200, clientDeleteRequest);
		return true;
	}
	return false;
}

bool server::client::checkCgiRequest() {
	if (this->clientCgiRequest != NULL) {
		std::cout << "cgi filePath: " << clientCgiRequest->getFilePath() << std::endl;
		try {
			clientCgiRequest->executeCgi();
		}
		catch (std::exception &e) {
			std::cout << "caught exception of cgi Request" << std::endl;
			setUpResponse(clientCgiRequest->getFilePath(), 500, clientCgiRequest);
		}
		setUpResponse(clientCgiRequest->getFilePath(), 200, clientCgiRequest);
		return true;
	}
	return false;
}

void  server::client::setUpResponse(std::string filePath, int statusCode, request *request){
	try {
		response *newResponse = new response(filePath, statusCode);
		clientResponse = newResponse;
		if (dynamic_cast<postRequest *>(request))
		{
//			std::cout << "delete clientPostRequest\n";
			delete clientPostRequest;
			clientPostRequest = NULL;
		}
		else if (dynamic_cast<getRequest *>(request)){
//			std::cout << "delete clientGetRequest\n";
			delete clientGetRequest;
			clientGetRequest = NULL;
		}
		else if (dynamic_cast<deleteRequest *>(request)){
//			std::cout << "delete clientDeleteRequest\n";
			delete clientDeleteRequest;
			clientDeleteRequest = NULL;
		}
		else if (dynamic_cast<cgiRequest *>(request)){
//			std::cout << "delete clientCgiRequest\n";
			delete clientCgiRequest;
			clientCgiRequest = NULL;
		}
	}
	catch (std::exception &e) {
		std::cout << "caught exception of  creating a response" << std::endl;
	}
}

void server::client::createNewRequest(std::vector<uint8_t> _request){

	if (this->clientGetRequest == NULL && this->clientPostRequest == NULL && this->clientDeleteRequest == NULL && this->clientCgiRequest == NULL) {
		request newRequest = request(_request, serverConfig, serverIndex);
//		std::cout << parsing::vectorToString(_request) << std::endl;

		std::string method  = request::getMethodString(newRequest);
//		std::cout << blue << "page to check: " << newRequest.getStringURL() << reset << std::endl; // "/random
//		std::cout << "method: " << method << std::endl; // "GET", "POST", "DELETE", "CGI
//		std::cout << "boolean method: " << newRequest.getDeleteMethod() << std::endl; // "GET", "POST", "DELETE", "CGI
		if (method == "")
			return;
		if (!request::checkPageMethod(method, newRequest.getStringURL(), serverIndex, serverConfig)) {
//			std::cout << "boolean pageMethod: " << request::checkPageMethod(method, newRequest.getStringURL(), serverIndex, serverConfig) << std::endl; // "GET", "POST", "DELETE", "CGI

			response *newResponse = new response("./html_files/errorPages/error405.html", 405);
			clientResponse = newResponse;
			return;
		}
		if (newRequest.getGetMethod() && newRequest.getCgi()){
			createNewCgiRequest(_request, newRequest);
		}

		else if (newRequest.getPostMethod()){
			createNewPostRequest(_request, newRequest);
		}
		else if (newRequest.getGetMethod()){
			createNewGetRequest(_request, newRequest);
		}
		else if (newRequest.getDeleteMethod()){
			createNewDeleteRequest(_request, newRequest);
		}
		std::cout << " no criteria was met\n";
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
	this->lastActivity = time(0);
	std::cout << "filepath: " << this->clientResponse->filePath << std::endl;
	if (this->clientResponse != NULL){
		if (this->clientResponse->_allChunkSent && this->clientResponse->filePath.find("html_files/tmp_cgi.txt") != std::string::npos) {
			if (this->clientResponse->removeFile(this->clientResponse->filePath.c_str()))
				std::cout << "file removed\n";
			else
				std::cout << "file not removed\n";
		}
		if (this->clientResponse->_allChunkSent) {
			delete this->clientResponse;
			this->clientResponse = NULL;
		}
		else {
			std::string chunk = this->clientResponse->getChunk(serverConfig._buffSize);
			send(this->clientSocket, &chunk[0], chunk.length(), 0);
		}
	}
}
