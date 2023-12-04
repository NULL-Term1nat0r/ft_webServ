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
	std::cout <<" client got destructed\n";
	 if (this->clientGetRequest != NULL)
	 	delete this->clientGetRequest;
	 if (this->clientPostRequest != NULL)
	 	delete this->clientPostRequest;
	 if (this->clientDeleteRequest != NULL)
	 	delete this->clientDeleteRequest;
	 if (this->clientCgiRequest != NULL)
	 	delete this->clientCgiRequest;
	 if (this->clientResponse != NULL)
	 	delete this->clientResponse;
	 if (this->baseRequest != NULL)
	 	delete this->baseRequest;
}

void server::client::executeClientRequest(){
	std::vector<uint8_t> _request(serverConfig._buffSize);
	int lol = recv(this->clientSocket, &_request[0], serverConfig._buffSize, 0);
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
				response *newResponse = new response("./html_files/uploadSuccessful.html", 201, serverConfig);
				clientResponse = newResponse;
				delete this->baseRequest;
				this->baseRequest = NULL;
				delete clientPostRequest;
				clientPostRequest = NULL;

			}
		}
		return true;
	}
	return false;
}

bool server::client::checkGetRequest() {
	if (this->clientGetRequest != NULL) {
		response *newResponse = new response(clientGetRequest->getFilePath(), clientGetRequest->statusCode, serverConfig);
		clientResponse = newResponse;
		delete this->baseRequest;
		this->baseRequest = NULL;
		delete clientGetRequest;
		clientGetRequest = NULL;
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

bool server::client::checkCgiRequest() {
	if (this->clientCgiRequest != NULL) {
		std::cout << "cgi filePath: " << clientCgiRequest->getFilePath() << std::endl;
		clientCgiRequest->executeCgi();
		response *newResponse = new response(clientCgiRequest->getFilePath(), clientCgiRequest->statusCode, serverConfig);
		clientResponse = newResponse;
		delete this->baseRequest;
		this->baseRequest = NULL;
		delete clientCgiRequest;
		clientCgiRequest = NULL;
		return true;
	}
	return false;
}

void server::client::createNewRequest(std::vector<uint8_t> _request){

	if (this->clientGetRequest == NULL && this->clientPostRequest == NULL && this->clientDeleteRequest == NULL && this->clientCgiRequest == NULL) {
//		std::cout << "----------------------------------NEW-----REQUEST-------------------------------------------\n" << parsing::vectorToString(_request, 1500) <<std::endl;
		this->baseRequest = new request(_request, serverConfig, serverIndex);
//		std::cout << yellow << this->baseRequest->getRequestString().substr(0, 500) << reset << std::endl;

		std::string method  = this->baseRequest->getMethodString();
		if (method == "")
			return;
		if (!this->baseRequest->checkPageMethod(method, this->baseRequest->getStringURL(), serverIndex, serverConfig)) {

			std::string test = serverConfig.getErrorPage(serverIndex, 405);
			std::cout << "test: " << test << std::endl;
			response *newResponse = new response(serverConfig.getErrorPage(serverIndex, 405), 405, serverConfig);
			delete this->baseRequest;
			clientResponse = newResponse;
			return;
		}
		if (this->baseRequest->getGetMethod() && this->baseRequest->getCgi())
			createNewCgiRequest();
		else if (this->baseRequest->getPostMethod() && this->baseRequest->getCgi()){
			std::cout << "cgi post request incoming\n";
			createNewCgiRequest();
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
		std::cout << "contentLength: " << this->clientPostRequest->getContentLength() << std::endl;
		std::cout << "bodySize: " << serverConfig._server[serverIndex].bodySize << std::endl;
		std::cout << "file too big " << std::endl;
		response *newResponse = new response("./html_files/errorPages/error413.html", 413, serverConfig);
		clientResponse = newResponse;
		delete this->baseRequest;
		this->baseRequest = NULL;
		delete clientPostRequest;
		clientPostRequest = NULL;
	}
	else if (!serverConfig.checkFileType(this->clientPostRequest->getFileExtension())) {
		std::cout << "fileExtension: " << this->clientPostRequest->getFileExtension() << std::endl;
		invalidRequest = true;
		response *newResponse = new response("./html_files/errorPages/error415.html", 415, serverConfig);
		clientResponse = newResponse;
		delete this->baseRequest;
		this->baseRequest = NULL;
		delete clientPostRequest;
		clientPostRequest = NULL;
	}
}

void server::client::createNewDeleteRequest(){
	this->clientDeleteRequest = new deleteRequest(baseRequest, serverConfig, serverIndex);
}

void server::client::createNewCgiRequest(){
	this->clientCgiRequest = new cgiRequest(baseRequest, serverConfig, serverIndex);
}

void server::client::executeClientResponse(){
	if (this->clientResponse != NULL){
		if (this->clientResponse->_allChunkSent && this->clientResponse->filePath.find("html_files/tmp_cgi.txt") != std::string::npos) {
			if (!this->clientResponse->removeFile(this->clientResponse->filePath.c_str()))
				std::cout << "file removed\n";
			else
				std::cout << "file not removed\n";
			delete this->clientResponse;
			this->clientResponse = NULL;
		}
		else if (this->clientResponse->_allChunkSent) {
			delete this->clientResponse;
			this->clientResponse = NULL;
			// close(this->clientSocket);
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

