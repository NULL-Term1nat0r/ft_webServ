/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbolkova <tbolkova@student.42wolfsburg.d>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/28 15:13:09 by tbolkova          #+#    #+#             */
/*   Updated: 2023/10/28 15:13:09 by tbolkova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cgiRequest.hpp"

bool cgiRequest::_errorSignal = false;


cgiRequest::cgiRequest(request *baseRequest, serverConf &serverConfig, int serverIndex) : _baseRequest(baseRequest), _serverConfig(serverConfig), serverIndex(serverIndex) {
	_cgiPath = "";
	_query = "";
	_errorCode = 200;		// set to 200 for no error. or 201
	_isError = false;
	fileIsReady = false;
	_cgiFilePath = "";
	_tempFile = "";
	_returnFilePath = "";
	_alarmSignal = false;
	_fileDescriptor = 0;
	_execPath = "";
	_execExtension = "";
	setScriptPage();
}

cgiRequest::~cgiRequest() {}

bool cgiRequest::cgiCheckLanguage() const {

	if (std::system((_execExtension +  " --version").c_str()))
		return true;
	return false;
}

bool cgiRequest::inputCheck() {
	//if (!cgiCheckLanguage())
	//	return false; // error 500
	if (access(_cgiPath.c_str(), F_OK) != 0) {
		std::cerr << "Status 2: Not Found\n" << std::endl;
		return false;
	}
	return true;
}

std::string cgiRequest::constructScriptPage(std::vector<std::string> &folderContainer) {
	std::string temp = "";
	std::cout << "containersize: " << folderContainer.size();
	for (size_t i = 0; i <= folderContainer.size() - 2; i++) {
		temp += "/" + folderContainer[i];
	}
	std::cout << "constructed cgiFolder: " << _cgiPath << std::endl;
	return temp;
}

void cgiRequest::setScriptPage() {
	if (_baseRequest->getStringURL().find(".php?") != std::string::npos) {
		std::string urlSubString = _baseRequest->getStringURL().substr(0, _baseRequest->getStringURL().find(".php?"));
		std::vector<std::string> result = parsing::split(urlSubString, '/');
		_scriptPage = constructScriptPage(result);
	}
	else if (_baseRequest->getStringURL().find(".py?") != std::string::npos){
		std::string urlSubString = _baseRequest->getStringURL().substr(0, _baseRequest->getStringURL().find(".py?"));
		std::vector<std::string> result = parsing::split(urlSubString, '/');
		_scriptPage = constructScriptPage(result);
	}
	else if (_baseRequest->getStringURL().find(".php") != std::string::npos){
		std::string urlSubString = _baseRequest->getStringURL().substr(0, _baseRequest->getStringURL().find(".php"));
		std::vector<std::string> result = parsing::split(urlSubString, '/');
		_scriptPage = constructScriptPage(result);
	}
	else if (_baseRequest->getStringURL().find(".py") != std::string::npos){
		std::string urlSubString = _baseRequest->getStringURL().substr(0, _baseRequest->getStringURL().find(".py"));
		std::vector<std::string> result = parsing::split(urlSubString, '/');
		_scriptPage = constructScriptPage(result);
	}
	std::cout << "scriptPage: " << _scriptPage << std::endl;
}

void cgiRequest::getErrorHtmlContent(int _errorCode) {
	_cgiFilePath = "./html_files/errorPages/error" + std::to_string(_errorCode) + ".html";
	_isError = true;
	}

bool cgiRequest::createTemporaryFile(){
	_fileDescriptor= open(_tempFile.c_str(), O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);
	if (_fileDescriptor != -1)
		return true;
	return false;
}

bool cgiRequest::checkExtensionServerConfig(std::string extension) {

	for (size_t i = 0; i < _serverConfig._server[serverIndex].locations[_scriptPage].cgi.size(); i++) {
		if (_serverConfig._server[serverIndex].locations[_scriptPage].cgi[i] == extension)
			return true;
	}
	return false;
}

bool cgiRequest::cgiValidExtension(std::string url) {

	size_t pos = url.find("?");
	if (pos != std::string::npos) {
		_cgiPath = "./html_files" + url.substr(0, pos);
		_query = "QUERY_STRING=" + url.substr(pos + 1);
	}
	else
		_cgiPath =  "./html_files" + url;
	std::cout << "url: " << url << std::endl;
	std::cout << "cgiPath: " << _cgiPath << std::endl;
	_tempFile = "./html_files/tmp_cgi.txt";
	if (_cgiPath.find(".php") != std::string::npos){
//		std::cout << "php boolean: " << checkExtensionServerConfig(".php") << std::endl;
//		std::cout << "cgi saved: " << _serverConfig._server[serverIndex].locations[_scriptPage].cgi[0] << std::endl;
//		std::cout << "cgi saved: " << _serverConfig._server[serverIndex].locations[_scriptPage].cgi[1] << std::endl;
		if (checkExtensionServerConfig(".php"))
			_execExtension = "php";
		else
			return false;
	}
	if (_cgiPath.find(".py") != std::string::npos) {
//		std::cout << "php boolean: " << checkExtensionServerConfig(".php") << std::endl;
//		std::cout << "cgi saved: " << _serverConfig._server[serverIndex].locations[_scriptPage].cgi[0] << std::endl;
//		std::cout << "cgi saved: " << _serverConfig._server[serverIndex].locations[_scriptPage].cgi[1] << std::endl;
		if (checkExtensionServerConfig(".py"))
			_execExtension = "python3";
		else
			return false;
	}

	_execPath = "/usr/bin/" + _execExtension;
	std::cout << "execPath: " << _execPath << std::endl;
	_skriptName = _cgiPath.substr(_cgiPath.find_last_of("/") + 1);
	_workingDirectory = _cgiPath.substr(0, _cgiPath.find(_skriptName) - 1);
	if (!createTemporaryFile()) {
		std::cout << "file didnt got created\n";
		return false;
	}
	return true;
}

bool cgiRequest::executeCgi() {
	std::cout << "cgi request incoming\n";

	if (!cgiValidExtension(_baseRequest->getStringURL())){
		_returnFilePath =  parsing::getErrorPagePath(500);
		return false;
	}

	int status;
	char *query = (char*)_query.c_str();
	char *env[] = {query, NULL};
	char *argv[] = {const_cast<char *>(_execPath.c_str()), const_cast<char *>(_skriptName.c_str()), NULL};

	// cgi timeout here
	pid_t childId = fork();
	if (childId == -1) {
		std::cout << "chile didnt ot created\n";
		getErrorHtmlContent(500);
		close(_fileDescriptor);
		_returnFilePath = parsing::getErrorPagePath(500);
		return false;
	}
	else if (childId == 0) {
		alarm(_serverConfig._scriptTimeout);
		if (dup2(_fileDescriptor, STDOUT_FILENO) == -1)
			exit(69);		//setting the timeout for the script
		if (chdir(_workingDirectory.c_str()) != 0){
			exit(69);
		}
		execve(_execPath.c_str(), argv, env);
		std::cout << "execve didnt work\n";
		exit(69);
	}
	else {
		waitpid(childId, &status, 0);
		close(_fileDescriptor);
	}
	close(_fileDescriptor);
	if (WIFSIGNALED(status))
	{
		_returnFilePath = parsing::getErrorPagePath(504);
		return false;
	}
	if (WEXITSTATUS(status) == 69) {
		_returnFilePath = parsing::getErrorPagePath(500);
		return false;
	}
	_returnFilePath = _tempFile;
	return true;
}

std::string cgiRequest::getFilePath() {
	return _returnFilePath;
}