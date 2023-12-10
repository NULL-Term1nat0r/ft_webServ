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
//	std::cout << yellow << _baseRequest->getRequestString() << reset << std::endl;
	this->redirection = false;
	this->autoIndexListing = false;
	this->filePath = "";
	this->statusCode = 200;
	this->autoIndexActivated = true;
	this->indexPageExists = true;
	this->url = _baseRequest->getStringURL();
	checkRewrite();
	this->page = parsing::constructPage(url);
	this->isFile = parsing::fileExists(("./html_files" + url).c_str());
	this->isFolder = parsing::folderExists(("./html_files" + url).c_str());
	if (_baseRequest->isPageConfigured){
		this->indexPageExists = _serverConfig._server[serverIndex].locations[page].indexBool;
		this->autoIndexActivated = _serverConfig._server[serverIndex].locations[page].autoindex == "on";
	}
	this->filePath = createFilePath();
}

getRequest::~getRequest() {}

std::string getRequest::createFilePath() {

	if (!_baseRequest->isPageConfigured)
		return NonConfiguredPage();

	if (isFile){
		return constructFilePath();
	}
	if (isFolder){
		if (autoIndexActivated && indexPageExists){
				return constructFolderPath();
		}
		else if (autoIndexActivated && !indexPageExists) {
			statusCode = 619;
			return constructFolderPath();
		}
		else {
			statusCode = 403;
			return _serverConfig.getErrorPage(serverIndex, 403);
		}
	}
	statusCode = 404;
	return _serverConfig.getErrorPage(serverIndex, 404);
}

std::string getRequest::NonConfiguredPage() {
	if (isFile)
		return constructFilePath();
	if (isFolder) {
		if (parsing::fileExists("html_files" + url + "/index.html"))
			return "html_files" + url + "/index.html";
		else {
			statusCode = 619;
			return "html_files" + url;
		}
	}
	statusCode = 404;
	return _serverConfig.getErrorPage(serverIndex, 404);
}

std::string getRequest::constructFolderPath() {
	if (url == "/")
		return "html_files/index.html";
	return "html_files" + url + "/" + _serverConfig._server[serverIndex].locations[page].index;
}

std::string getRequest::constructFilePath() {
	return "html_files" + url;
}

void getRequest::checkRewrite() {
	while (_serverConfig._server[serverIndex].locations[page].rewrite != "") {
		url = getRewrite(page);
		page = parsing::constructPage(url);
	}
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
