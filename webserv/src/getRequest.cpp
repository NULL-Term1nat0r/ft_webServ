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

getRequest::getRequest(std::vector<uint8_t> &getRequest, serverConf &serverConfig, int serverIndex) : request(getRequest, serverConfig, serverIndex), _serverConfig(serverConfig), serverIndex(serverIndex) {
	this->redirection = false;
	this->autoIndexListing = false;
	this->filePath = createFilePath();
}

getRequest::~getRequest() {}

//const char	*getRequest::getRequestInvalidFileException::what() const throw() {
//	return "Invalid File Exception";
//}

//std::string getRequest::createFilePath(){
//	if (getStringURL() == "/") {
//		if (_serverConfig._server[serverIndex].locations[getStringURL()].autoindex == "on")
//			return "./html_files/index.html";
//		return parsing::getErrorPagePath(403);
//	}				//if url is "/" return index.html
//	else if (parsing::fileExists(("./html_files" + getStringURL()).c_str()))
//		return "./html_files" + getStringURL();
//	else if (parsing::fileExists(("./html_files" +  getStringURL() + getStringURL() + ".html").c_str())){
//		if (_serverConfig._server[serverIndex].locations[getStringURL()].autoindex == "on")
//			return "./html_files" + getStringURL() + getStringURL() + ".html";
//		return parsing::getErrorPagePath(403);
//	} // if url is a directory return file.html
//	else if (parsing::fileExists(("./html_files" +  checkRewrite(getStringURL()) + ".html").c_str())) {
//		this->redirection = true;
//		return "./html_files" + checkRewrite(getStringURL()) + ".html";
//	}
//	else
//		return parsing::getErrorPagePath(404); // if url is not found return 404 error page
//}

std::string getRequest::createFilePath(){
	if (getStringURL() == "/") {
		if (_serverConfig._server[serverIndex].locations[getStringURL()].autoindex == "on")
			return "./html_files/index.html";
		return parsing::getErrorPagePath(403);
	}				//if url is "/" return index.html
	else if (parsing::fileExists(("./html_files" + getStringURL()).c_str()))
		return "./html_files" + getStringURL();


	else if (_serverConfig._server[serverIndex].locations[getStringURL()].autoindex == "on"){
		if (parsing::fileExists(("./html_files" +  getStringURL() + getStringURL() + ".html").c_str()))
			return "./html_files" + getStringURL() + getStringURL() + ".html";
		else {
			this->autoIndexListing = true;
			std::cout << "autoIndexFile address: " << &this->autoIndexListing << std::endl;
			std::cout << "noAutoIndexFile" << std::endl;
			return "./html_files" + getStringURL();
		}
	}
	else if (parsing::fileExists(("./html_files" +  checkRewrite(getStringURL()) + ".html").c_str())) {
		this->redirection = true;
		return "./html_files" + checkRewrite(getStringURL()) + ".html";
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
