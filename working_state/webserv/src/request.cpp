#include "../includes/request.hpp"

request::request(std::vector<uint8_t> &clientRequest, serverConf &serverConfig, int serverIndex) : _serverConfig(serverConfig), _serverIndex(serverIndex){

	this->_request = parsing::vectorToString(clientRequest, 2000);
	std::cout << yellow <<  _request << reset << std::endl;
	this->_get = isGetMethod();
	this->_post = isPostMethod();
	this->_delete = isDeleteMethod();
	this->methodString = getMethodString();
	this->url = parseUrl();
	parsing::decodeUrl(url);
	this->fileName = parseFileName();
	this->_cgi = isCgi();
	this->_aliveConnection = isAliveConnection();
	this->_closeConnection = isCloseConnection();
	this->page = parsing::constructPage(url);
	std::cout << blue << "page: " << page << reset << std::endl;
	this->page = checkRewrite();
	this->isPageConfigured = parsing::checkIfPageConfigured(_serverConfig._server[serverIndex].locations, page);
	this->methodIsValid = isMethodConfigured();
	printRequest();
}

request::~request(){
}

bool request::isGetMethod() {
	return _request.find("GET") != std::string::npos;
}

bool request::isPostMethod() {
	return _request.find("POST") != std::string::npos;
}

bool request::isDeleteMethod() {
	return _request.find("DELETE") != std::string::npos;
}

bool request::isAliveConnection() {
	return _request.find("Connection: keep-alive") != std::string::npos;
}

bool request::isCloseConnection() {
	return _request.find("Connection: close") != std::string::npos;
}

std::string request::parseFileName(){
	if (_get){
		if (url.find('?') != std::string::npos)
			return url.substr(url.find_last_of('/') + 1, url.find('?') - url.find_last_of('/') - 1);
		return url.substr(url.find_last_of('/'));
	}
	if (_post)
		return parsing::returnValue("filename=", _request, "\r");
	return "";
}

std::string request::getMethodString() {
	if (_get)
		return "GET";
	else if (_post)
		return "POST";
	else if (_delete)
		return "DELETE";
	return "";
}

bool request::isCgi(){
	if (_get)
		return url.find(".php") != std::string::npos || url.find(".py") != std::string::npos;
	if (_post)
		return fileName.find(".php") != std::string::npos || fileName.find(".py") != std::string::npos;
}

std::string request::getRewrite(std::string page){
	return _serverConfig._server[_serverIndex].locations[page].rewrite;
}

std::string request::checkRewrite() {
	std::string tempPage = page;
	while (_serverConfig._server[_serverIndex].locations[tempPage].rewrite != "") {
		url = getRewrite(tempPage);
		tempPage = parsing::constructPage(url);
	}
	return tempPage;
}

std::string request::parseUrl() {
	std::string method = getMethodString();
	if (this->_request.find(method) != std::string::npos)
		return parsing::returnValue(method, this->_request, " ");
	return "";
}

void request::printRequest(){
	std::cout << "alive connection : " << _aliveConnection << std::endl;
	std::cout << "close connection : " << _closeConnection << std::endl;
	std::cout << "method string : " << methodString << std::endl;
	std::cout << "method is valid : " << methodIsValid << std::endl;
	std::cout << "page : " << page << std::endl;
	std::cout << "is page configured : " << isPageConfigured << std::endl;
	std::cout << "is cgi : " << _cgi << std::endl;
	std::cout << "is get : " << _get << std::endl;
	std::cout << "is post : " << _post << std::endl;
	std::cout << "is delete : " << _delete << std::endl;
	std::cout << "url : " << url << std::endl;
	std::cout << "fileName: " << fileName << std::endl;
}

bool request::isMethodConfigured(){
	if (!isPageConfigured)
		return true;
	if (_serverConfig._server[_serverIndex].locations.empty())
		return true;
	if (_get)
		return _serverConfig._server[_serverIndex].locations[page].allowGet;
	if (_post)
		return _serverConfig._server[_serverIndex].locations[page].allowPost;
	if (_delete)
		return _serverConfig._server[_serverIndex].locations[page].allowDelete;
	return false;
}
//_serverConfig._server[_serverIndex].locations[_stringURL.substr(pos, pos2)].allowGet)

//bool request::checkPageMethod(std::string method, std::string url, int _serverIndex, serverConf &_serverConfig){
//	std::string page = parsing::returnPage(url);
//	std::cout << "pahge string in checkPageMethod: " << page << std::endl;
//	if (_serverConfig._server[_serverIndex].locations.empty()){
//		std::cout << "locations empty\n";
//		return true;
//	}
////	std::cout << red << "url: " << url << reset << std::endl; // "/random
//	if (page != "/")
//		page = "/" + page;
//	if (page == "")
//		return true;
//	if (method == "GET" && _serverConfig._server[_serverIndex].locations[page].allowGet)
//		return true;
//	else if (method == "POST" && _serverConfig._server[_serverIndex].locations[page].allowPost)
//		return true;
//	else if (method == "DELETE" && _serverConfig._server[_serverIndex].locations[page].allowDelete)
//		return true;
//	else
//		return false;
//}


bool request::getAliveConnection(){
	return this->_aliveConnection;
}
bool request::getCloseConnection(){
	return this->_closeConnection;
}

std::string request::getStringURL() {
	return this->url;
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
