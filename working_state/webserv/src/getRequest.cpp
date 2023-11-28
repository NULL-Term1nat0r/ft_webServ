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

std::string getRequest::createFilePath(){

	std::cout << green << "url: " << _baseRequest->getStringURL() << reset << std::endl;
	std::cout << green << "fileExists: " << "./html_files" + _baseRequest->getStringURL() << reset <<  std::endl;
	std::cout << green << "boolean fileExists: " << parsing::fileExists(("./html_files" + _baseRequest->getStringURL()).c_str()) << reset <<  std::endl;
	if (_baseRequest->getStringURL() == "/" && _serverConfig._server[serverIndex].locations[_baseRequest->getStringURL()].indexBool) {
		std::cout << "indexBool: " << _serverConfig._server[serverIndex].locations[_baseRequest->getStringURL()].indexBool << std::endl;
		if (_serverConfig._server[serverIndex].locations[_baseRequest->getStringURL()].autoindex == "on")
			return "./html_files/index.html";
		return parsing::getErrorPagePath(403);
	}				//if url is "/" return index.html
	else if (parsing::fileExists(("./html_files" + _baseRequest->getStringURL()).c_str())){
		std::cout << green << "fileExists: " << "./html_files" + _baseRequest->getStringURL() << reset <<  std::endl;
		return "./html_files" + _baseRequest->getStringURL();
	}
	else if (_serverConfig._server[serverIndex].locations[_baseRequest->getStringURL()].autoindex == "on" && _serverConfig._server[serverIndex].locations[_baseRequest->getStringURL()].indexBool){
		if (parsing::fileExists(("./html_files" +  _baseRequest->getStringURL() + _baseRequest->getStringURL() + ".html").c_str())) {
			return "./html_files" + _baseRequest->getStringURL() + _baseRequest->getStringURL() + ".html";
		}
		else {
			this->autoIndexListing = true;
			std::cout << "autoIndexFile address: " << &this->autoIndexListing << std::endl;
			std::cout << "noAutoIndexFile" << std::endl;
			return "./html_files" + _baseRequest->getStringURL();
		}
	}
	else if (parsing::fileExists(("./html_files" +  checkRewrite(_baseRequest->getStringURL()) + ".html").c_str())) {
		this->redirection = true;
		return "./html_files" + checkRewrite(_baseRequest->getStringURL()) + ".html";
	}
	else
		return parsing::getErrorPagePath(404); // if url is not found return 404 error page
}




std::string getRequest::checkRewrite(std::string url){
	std::string page = url.substr(url.find_last_of('/'));
//	std::cout << "page: " << page << std::endl;
	return (_serverConfig._server[serverIndex].locations[page].rewrite);
}

std::string getRequest::getFilePath(){
	return this->filePath;
}

bool getRequest::getRedirection(){
	return this->redirection;
}
