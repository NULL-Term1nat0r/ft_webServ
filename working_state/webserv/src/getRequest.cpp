/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getRequest.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: estruckm <estruckm@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 14:17:17 by estruckm          #+#    #+#             */
/*   Updated: 2023/10/24 14:17:17 by estruckm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/getRequest.hpp"

getRequest::getRequest(request *baseRequest, serverConf &serverConfig, int serverIndex) : _baseRequest(baseRequest), _serverConfig(serverConfig), serverIndex(serverIndex) {
	this->redirection = false;
	this->autoIndexListing = false;
	this->statusCode = 200;
	this->filePath = createFilePath();
}

getRequest::~getRequest() {}

//const char	*getRequest::getRequestInvalidFileException::what() const throw() {
//	return "Invalid File Exception";
//}

//std::string getRequest::createFilePath(){
//	if (_baseRequest->getStringURL() == "/") {
//		if (_serverConfig._server[serverIndex].locations[_baseRequest->getStringURL()].autoindex == "on")
//			return "./html_files/index.html";
//		return parsing::getErrorPagePath(403);
//	}				//if url is "/" return index.html
//	else if (parsing::fileExists(("./html_files" + _baseRequest->getStringURL()).c_str()))
//		return "./html_files" + _baseRequest->getStringURL();
//	else if (parsing::fileExists(("./html_files" +  _baseRequest->getStringURL() + _baseRequest->getStringURL() + ".html").c_str())){
//		if (_serverConfig._server[serverIndex].locations[_baseRequest->getStringURL()].autoindex == "on")
//			return "./html_files" + _baseRequest->getStringURL() + _baseRequest->getStringURL() + ".html";
//		return parsing::getErrorPagePath(403);
//	} // if url is a directory return file.html
//	else if (parsing::fileExists(("./html_files" +  checkRewrite(_baseRequest->getStringURL()) + ".html").c_str())) {
//		this->redirection = true;
//		return "./html_files" + checkRewrite(_baseRequest->getStringURL()) + ".html";
//	}
//	else
//		return parsing::getErrorPagePath(404); // if url is not found return 404 error page
//}

//std::string getRequest::createFilePath(){
//
//	std::cout << green << "url: " << _baseRequest->getStringURL() << reset << std::endl;
//	std::cout << green << "fileExists: " << "./html_files" + _baseRequest->getStringURL() << reset <<  std::endl;
//	std::cout << green << "boolean fileExists: " << parsing::fileExists(("./html_files" + _baseRequest->getStringURL()).c_str()) << reset <<  std::endl;
//	if (_baseRequest->getStringURL() == "/" && _serverConfig._server[serverIndex].locations[_baseRequest->getStringURL()].indexBool) {
//		std::cout << "indexBool: " << _serverConfig._server[serverIndex].locations[_baseRequest->getStringURL()].indexBool << std::endl;
//		if (_serverConfig._server[serverIndex].locations[_baseRequest->getStringURL()].autoindex == "on")
//			return "./html_files/index.html";
//		return parsing::getErrorPagePath(403);
//	}				//if url is "/" return index.html
//	else if (parsing::fileExists(("./html_files" + _baseRequest->getStringURL()).c_str())){
//		std::cout << green << "fileExists: " << "./html_files" + _baseRequest->getStringURL() << reset <<  std::endl;
//		return "./html_files" + _baseRequest->getStringURL();
//	}
//	else if (_serverConfig._server[serverIndex].locations[_baseRequest->getStringURL()].autoindex == "on" && !_serverConfig._server[serverIndex].locations[_baseRequest->getStringURL()].indexBool){
//		if (parsing::fileExists(("./html_files" +  _baseRequest->getStringURL() + _baseRequest->getStringURL() + ".html").c_str())) {
//			return "./html_files" + _baseRequest->getStringURL() + _baseRequest->getStringURL() + ".html";
//		}
//		else {
//			this->autoIndexListing = true;
//			std::cout << "autoIndexFile address: " << &this->autoIndexListing << std::endl;
//			std::cout << "noAutoIndexFile" << std::endl;
//			return "./html_files" + _baseRequest->getStringURL();
//		}
//	}
//	else if (parsing::fileExists(("./html_files" +  checkRewrite(_baseRequest->getStringURL()) + ".html").c_str())) {
//		this->redirection = true;
//		return "./html_files" + checkRewrite(_baseRequest->getStringURL()) + ".html";
//	}
//	else
//		return parsing::getErrorPagePath(404); // if url is not found return 404 error page
//}

std::string getRequest::createFilePath(){
	std::cout << "createFilePath activated\n";
	std::cout << "url in createFilePath = " << _baseRequest->getStringURL() << std::endl;
	std::string page = parsing::constructPage(_baseRequest->getStringURL());
	if (checkRewrite(page)) {
		std::cout << "check rewrite\n";
		if (parsing::folderExists(("./html_files" + page).c_str())) {
			statusCode = 301;
			page = getRewrite(page);
			std::cout << "page: " << page << std::endl;
		}
		else {
			statusCode = 404;
			return parsing::getErrorPagePath(404);
		}

	}
	bool indexBool = _serverConfig._server[serverIndex].locations[page].indexBool;

	if (parsing::folderExists("./html_files" + _baseRequest->getStringURL())) {
		std::cout << "folderpath: " << "./html_files" + page << std::endl;
		std::cout << "folder exists\n";
		std::string autoIndex = _serverConfig._server[serverIndex].locations[page].autoindex;
		if (autoIndex == "on" && indexBool){
			if (page == "/")
				return "./html_files/index.html";
			else
				return ("./html_files" + page + page + ".html");
		}
		else if (autoIndex == "on" && !indexBool) {
			statusCode = 619;
			return "./html_files" + page;
		}
		else {
			statusCode = 403;
			return parsing::getErrorPagePath(403);
		}

	}
	else if (parsing::fileExists("./html_files" + _baseRequest->getStringURL())) {
		std::cout << "filepath: " << "./html_files" + _baseRequest->getStringURL() << std::endl;
		std::cout << "file exists\n";
		std::string autoIndex = _serverConfig._server[serverIndex].locations[page].autoindex;
		if (indexBool)
			return "./html_files" + _baseRequest->getStringURL();
		else if (!indexBool && autoIndex == "on") {
			statusCode = 619;
			return "./html_files" + page;
		}
		else {
			std::cout << "made my way hier: " << _baseRequest->getStringURL() << std::endl;
			statusCode = 403;
			std::cout << "statusCode address in requestfile: " << &statusCode << std::endl;
			return parsing::getErrorPagePath(403);
		}

	}
	else {
		statusCode = 404;
		return parsing::getErrorPagePath(404);
	}

}

//is the error code for directorylisting also 200 ?
//std::string getRequest::createFilePath(){
//	std::cout << "createFilePath activated\n";
//	std::cout << "url in createFilePath = " << _baseRequest->getStringURL() << std::endl;
//	std::string page = parsing::constructPage(_baseRequest->getStringURL());
//	if (checkRewrite(page)) {
//		std::cout << "check rewrite\n";
//		if (parsing::folderExists(("./html_files" + page).c_str()))
//			page = getRewrite(page);
//		else {
//			statusCode = 404;
//			return parsing::getErrorPagePath(404);
//	}



bool getRequest::checkRewrite(std::string page){
	if (_serverConfig._server[serverIndex].locations[page].rewrite != "")
		return true;
	return false;
}

std::string getRequest::getRewrite(std::string page){
	return _serverConfig._server[serverIndex].locations[page].rewrite;
}

std::string getRequest::getFilePath(){
	return this->filePath;
}

bool getRequest::getRedirection(){
	return this->redirection;
}
