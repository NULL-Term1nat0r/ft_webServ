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


cgiRequest::cgiRequest(request *baseRequest, serverConf &serverConfig, int serverIndex) : _baseRequest(baseRequest), _serverConfig(serverConfig), serverIndex(serverIndex) {// set to 200 for no error. or 201
	fileIsReady = false;
	_cgiFilePath = "";
	_returnFilePath = "";
	_alarmSignal = false;
	_fileDescriptor = 0;
	_extension = parseExtension();
	std::cout << "extension: " << _extension << std::endl;
	_execExtension = setExecExtension();
	std::cout << "execExtension: " << _execExtension << std::endl;
	_hasQueryString = checkQueryString();
	std::cout << "hasQueryString: " << _hasQueryString << std::endl;
	_scriptPage = setScriptPage();
	std::cout << "scriptPage: " << _scriptPage << std::endl;
	_workingDirectory = createWorkingDirectoryPath();
	std::cout << "workingDirectory: " << _workingDirectory << std::endl;
	_execPath = createExecPath();
	std::cout << "execPath: " << _execPath << std::endl;
	_tempFile = createTempPath();
	std::cout << "tempFile: " << _tempFile << std::endl;
	_queryString = createQueryString();
	std::cout << "queryString: " << _queryString << std::endl;
	statusCode = 200;
	printCgiRequest();
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

std::string cgiRequest::setScriptPage() {
	if (_baseRequest->_get)
		return parseGetRequestPage();
	else if (_baseRequest->_post)
		return parsePostRequestPage();
	else
		return "";
}

bool cgiRequest::checkQueryString() {
	if (_baseRequest->url.find("?") != std::string::npos)
		return true;
	return false;
}

std::string cgiRequest::parseGetRequestPage() {

	if (_hasQueryString) {
		std::string urlSubString = _baseRequest->url.substr(0, _baseRequest->url.find('?'));
		std::vector <std::string> result = parsing::split(urlSubString, '/');
		return constructScriptPage(result);
	}
	return parsing::returnPage(_baseRequest->url);
}

std::string cgiRequest::parsePostRequestPage() {
	if (_hasQueryString) {
		std::string urlSubString = _baseRequest->url.substr(0, _baseRequest->url.find('?'));
		std::vector<std::string> result = parsing::split(urlSubString, '/');
		return constructScriptPage(result);
	}
	return parsing::returnPage(_baseRequest->url);
}

void cgiRequest::getErrorHtmlContent(int _errorCode) {
	_cgiFilePath = "./html_files/errorPages/error" + std::to_string(_errorCode) + ".html";
	_isError = true;
	}

bool cgiRequest::createTemporaryFile(){
	_fileDescriptor= open(_tempFile.c_str(), O_TRUNC| O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);
	if (_fileDescriptor != -1)
		return true;
	return false;
}

bool cgiRequest::checkExtensionServerConfig() {

	for (size_t i = 0; i < _serverConfig._server[serverIndex].locations[_scriptPage].cgi.size(); i++) {
		if (_serverConfig._server[serverIndex].locations[_scriptPage].cgi[i] == _extension)
			return true;
	}
	return false;
}

std::string cgiRequest::parseExtension(){
	if (_baseRequest->_get) {
		if (_baseRequest->url.find(".php") != std::string::npos)
			return ".php";
		else if (_baseRequest->url.find(".py") != std::string::npos)
			return ".py";
	}
	else if (_baseRequest->_post) {
		if (_baseRequest->fileName.find(".php") != std::string::npos)
			return ".php";
		else if (_baseRequest->fileName.find(".py") != std::string::npos)
			return ".py";
	}
	return "";
}

std::string cgiRequest::setExecExtension(){
	if (_extension == ".php")
		return "php";
	else if (_extension == ".py")
		return "python3";
	else
		return "";
}

std::string cgiRequest::createWorkingDirectoryPath(){
		return "./html_files" + _scriptPage;
}

std::string cgiRequest::createExecPath(){
	return "/usr/bin/" + _execExtension;
}

std::string cgiRequest::createTempPath(){
	return "./html_files/tmp_cgi.txt";
}

std::string cgiRequest::createQueryString(){
	if (_hasQueryString) {
		size_t pos = _baseRequest->url.find('?');
		return "QUERY_STRING=" + _baseRequest->url.substr(pos + 1);
	}
	return "";
}

bool cgiRequest::prepareExecution(){
	if (!checkExtensionServerConfig()){
		statusCode = 500;
		std::cout << "invalid file extension/n";
		return false;
	}
	if (!createTemporaryFile()) {
		statusCode = 500;
		std::cout << "file couldn't get created\n";
		return false;
	}
	return true;
}



//bool cgiRequest::cgiValidExtension(std::string url) {
//
//	size_t pos = url.find("?");
//	if (_hasQueryString) {
//		_cgiPath = "./html_files" + url.substr(url.find("?"), pos);
//		_query = "QUERY_STRING=" + url.substr(pos + 1);
//	}
//	else
//		_cgiPath =  "./html_files" + url;
//	std::cout << "url: " << url << std::endl;
//	std::cout << "cgiPath: " << _cgiPath << std::endl;
//	_tempFile = "./html_files/tmp_cgi.txt";
//	if (_cgiPath.find(".php") != std::string::npos){
////		std::cout << "php boolean: " << checkExtensionServerConfig(".php") << std::endl;
////		std::cout << "cgi saved: " << _serverConfig._server[serverIndex].locations[_scriptPage].cgi[0] << std::endl;
////		std::cout << "cgi saved: " << _serverConfig._server[serverIndex].locations[_scriptPage].cgi[1] << std::endl;
//		if (checkExtensionServerConfig(".php"))
//			_execExtension = "php";
//		else
//			return false;
//	}
//	if (_cgiPath.find(".py") != std::string::npos) {
////		std::cout << "php boolean: " << checkExtensionServerConfig(".php") << std::endl;
////		std::cout << "cgi saved: " << _serverConfig._server[serverIndex].locations[_scriptPage].cgi[0] << std::endl;
////		std::cout << "cgi saved: " << _serverConfig._server[serverIndex].locations[_scriptPage].cgi[1] << std::endl;
//		if (checkExtensionServerConfig(".py"))
//			_execExtension = "python3";
//		else
//			return false;
//	}
//
//	_execPath = "/usr/bin/" + _execExtension;
//	std::cout << "execPath: " << _execPath << std::endl;
//	_skriptName = _cgiPath.substr(_cgiPath.find_last_of("/") + 1);
//	_workingDirectory = _cgiPath.substr(0, _cgiPath.find(_skriptName) - 1);
//	if (!createTemporaryFile()) {
//		std::cout << "file didnt got created\n";
//		return false;
//	}
//	return true;
//}

void cgiRequest::executeCgi() {
	if(!prepareExecution())
		_returnFilePath = _serverConfig.getErrorPage(serverIndex, 500);

	int status;
	char *query = (char*)_queryString.c_str();
	char *env[] = {query, NULL};
	char *argv[] = {const_cast<char *>(_execPath.c_str()), const_cast<char *>(_baseRequest->fileName.c_str()), NULL};

	pid_t childId = fork();
	if (childId == -1) {
		std::cout << "chile didnt ot created\n";
		close(_fileDescriptor);
		_returnFilePath = _serverConfig.getErrorPage(serverIndex, 500);
		statusCode = 500;
		return;
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
		_returnFilePath = _serverConfig.getErrorPage(serverIndex, 504);
		status = 504;
		return;
	}
	if (WEXITSTATUS(status) == 69) {
		_returnFilePath = _serverConfig.getErrorPage(serverIndex, 500);
		statusCode = 500;
		return;
	}
	_returnFilePath = _tempFile;
}

void cgiRequest::printCgiRequest(){
	std::cout << yellow << "-------------cgi Values-------------" << reset << std::endl;
	std::cout << "scriptPage: " << _scriptPage << std::endl;
	std::cout << "cgiPath: " << _cgiPath << std::endl;
	std::cout << "skriptName: " << _baseRequest->fileName << std::endl;
	std::cout << "tempFile: " << _tempFile << std::endl;
	std::cout << "workingDirectory: " << _workingDirectory << std::endl;
	std::cout << "fileDescriptor: " << _fileDescriptor << std::endl;
	std::cout << "queryString: " << _queryString << std::endl;
	std::cout << "extension: " << _extension << std::endl;
	std::cout << "query: " << _query << std::endl;
	std::cout << "execExtension: " << _execExtension << std::endl;
	std::cout << "execPath: " << _execPath << std::endl;
	std::cout << "cgiFilePath: " << _cgiFilePath << std::endl;
	std::cout << "returnFilePath: " << _returnFilePath << std::endl;
	std::cout << "hasQueryString: " << _hasQueryString << std::endl;
	std::cout << "errorCode: " << _errorCode << std::endl;
	std::cout << "fileIsReady: " << fileIsReady << std::endl;
	std::cout << "isError: " << _isError << std::endl;
	std::cout << "alarmSignal: " << _alarmSignal << std::endl;
	std::cout << yellow << "------------------------------------" << reset << std::endl;
}
//bool cgiRequest::executeCgi() {
//	std::cout << "cgi request incoming\n";
//
//	if(!prepareExecution())
//		return;
//
//	int status;
//	char *query = (char*)_query.c_str();
//	char *env[] = {_queryString, NULL};
//	char *argv[] = {const_cast<char *>(_execPath.c_str()), const_cast<char *>(_skriptName.c_str()), NULL};
//
//	// cgi timeout here
//	pid_t childId = fork();
//	if (childId == -1) {
//		std::cout << "chile didnt ot created\n";
//		getErrorHtmlContent(500);
//		close(_fileDescriptor);
//		_returnFilePath = parsing::getErrorPagePath(500);
//		statusCode = 500;
//		return false;
//	}
//	else if (childId == 0) {
//		alarm(_serverConfig._scriptTimeout);
//		if (dup2(_fileDescriptor, STDOUT_FILENO) == -1)
//			exit(69);		//setting the timeout for the script
//		if (chdir(_workingDirectory.c_str()) != 0){
//			exit(69);
//		}
//		execve(_execPath.c_str(), argv, env);
//		std::cout << "execve didnt work\n";
//		exit(69);
//	}
//	else {
//		waitpid(childId, &status, 0);
//		close(_fileDescriptor);
//	}
//	close(_fileDescriptor);
//	if (WIFSIGNALED(status))
//	{
//		_returnFilePath = parsing::getErrorPagePath(500);
//		status = 500;
//		return false;
//	}
//	if (WEXITSTATUS(status) == 69) {
//		_returnFilePath = parsing::getErrorPagePath(504);
//		statusCode = 504;
//		return false;
//	}
//	_returnFilePath = _tempFile;
//	return true;
//}

std::string cgiRequest::getFilePath() {
	return _returnFilePath;
}
