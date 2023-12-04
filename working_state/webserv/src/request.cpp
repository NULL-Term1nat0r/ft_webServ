#include "../includes/request.hpp"

request::request(std::vector<uint8_t> &clientRequest, serverConf &serverConfig, int serverIndex) : _serverConfig(serverConfig), _serverIndex(serverIndex){

	this->_request = parsing::vectorToString(clientRequest, 2000);
	setDefaultValues();
	parseRequest();
}

request::~request(){
}

void request::setDefaultValues() {
	this->_URL = false;
	this->_validRequest = false;
	this->_httpProtocol = true;
	this->_aliveConnection = false;
	this->_closeConnection = false;
	this->_get = false;
	this->_post = false;
	this->_delete = false;
}


void request::parseRequest() {
	parseURL();
	checkMethods();
	this->_cgi = checkCgi(this->_stringURL);
	this->_stringHttpProtocol = parsing::returnValue("HTTP/", this->_request, "\r");
	if (parsing::returnValue("Connection: ", this->_request, "\r") == "keep-alive")
		this->_aliveConnection = true;
	else if (parsing::returnValue("Connection: ", this->_request, "\r") == "close")
		this->_closeConnection = true;
}

bool request::checkCgi(std::string url) {
	if (url == "/")
		return false;
	if (_get) {
		if (url.find(".php") != std::string::npos)
			return true;
		if (url.find(".py") != std::string::npos)
			return true;
	}
	return false;
}

void request::parseURL(){
	if (this->_request.find("GET") != std::string::npos)
		this->_stringURL = parsing::returnValue("GET", this->_request,  " ");
	else if (this->_request.find("POST") != std::string::npos)
		this->_stringURL = parsing::returnValue("POST", this->_request,  " ");
	else if (this->_request.find("DELETE") != std::string::npos)
		this->_stringURL = parsing::returnValue("DELETE", this->_request,  " ");
	parsing::decodeUrl(this->_stringURL);
}

std::string request::getMethodString(){
	if (getGetMethod())
		return "GET";
	else if (getPostMethod())
		return "POST";
	else if (getDeleteMethod())
		return "DELETE";
	return "";
}

void request::checkMethods(){
	if (this->_request.find("GET") != std::string::npos){
		this->_get = true;
	}
	if (this->_request.find("POST") != std::string::npos){
		this->_post = true;
	}
	if (this->_request.find("DELETE") != std::string::npos){
		this->_delete = true;
	}
}

void request::validateRequest(){
	int valid = 0;
	if (this->_get || this->_post || this->_delete)
		valid++;
	if (this->_URL)
		valid++;
	if (this->_stringHttpProtocol == "1.1" || this->_stringHttpProtocol == "1.0")
		valid++;
	if (this->_aliveConnection || this->_closeConnection)
		valid++;
	if (valid == 4)
		this->_validRequest = true;
	else
		this->_validRequest = false;
}

void request::printRequest(){
	std::cout << "alive connection : " << getAliveConnection() << std::endl;
	std::cout << "close connection : " << getCloseConnection() << std::endl;
	std::cout << "URL : " << getURL() << std::endl;
	std::cout << "valid request : " << getValidRequest() << std::endl;
}

//_serverConfig._server[_serverIndex].locations[_stringURL.substr(pos, pos2)].allowGet)

bool request::checkPageMethod(std::string method, std::string url, int _serverIndex, serverConf &_serverConfig){
	std::string page = parsing::returnPage(url);
	std::cout << "pahge string in checkPageMethod: " << page << std::endl;
	if (_serverConfig._server[_serverIndex].locations.empty()){
		std::cout << "locations empty\n";
		return true;
	}
		return true;
//	std::cout << red << "url: " << url << reset << std::endl; // "/random
	if (page != "/")
		page = "/" + page;
	if (page == "")
		return true;
	if (method == "GET" && _serverConfig._server[_serverIndex].locations[page].allowGet)
		return true;
	else if (method == "POST" && _serverConfig._server[_serverIndex].locations[page].allowPost)
		return true;
	else if (method == "DELETE" && _serverConfig._server[_serverIndex].locations[page].allowDelete)
		return true;
	else
		return false;
}



bool request::getValidRequest(){
	return this->_validRequest;
}
bool request::getAliveConnection(){
	return this->_aliveConnection;
}
bool request::getCloseConnection(){
	return this->_closeConnection;
}
bool request::getURL(){
	return this->_URL;
}
std::string request::getStringURL() {
	return this->_stringURL;
}

std::string &request::getRequestString(){
	return this->_request;
}

bool request::getGetMethod(){
	return this->_get;
}

bool request::getPostMethod(){
	return this->_post;
}

bool request::getDeleteMethod(){
	return this->_delete;
}

bool request::getCgiMethod(){
	return this->_cgi;
}

bool request::getCgi(){
	return this->_cgi;
}




//GET /favicon.ico HTTP/1.1\n
//Host: localhost:8080\n
//Connection: keep-alive\n
//sec-ch-ua: Google Chrome;v="117", Not;A=Brand;v=8, Chromium;v=117\n
//sec-ch-ua-mobile: ?0\n
//User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/117.0.0.0 Safari/537.36\n
//sec-ch-ua-platform: macOS\n
//Accept: image/avif,image/webp,image/apng,image/svg+xml,image/*,*/*;q=0.8\n
//Sec-Fetch-Site: same-origin\n
//Sec-Fetch-Mode: no-cors\n
//Sec-Fetch-Dest: image\n
//Referer: http:localhost:8080/\n
//Accept-Encoding: gzip, deflate, br\n
//Accept-Language: en-GB,en-US;q=0.9,en;q=0.8\n
