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
	_extension = parseExtension();
	std::cout << "extension: " << _extension << std::endl;
	_execExtension = setExecExtension();
	std::cout << "execExtension: " << _execExtension << std::endl;
	_hasQueryString = checkQueryString();
	std::cout << "hasQueryString: " << _hasQueryString << std::endl;
	_scriptPage = setScriptPage();
	std::cout << "scriptPage: " << _scriptPage << std::endl;
	_cgiFilePath = createCgiFilePath();
	std::cout << "cgiFilePath: " << _cgiFilePath << std::endl;
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

std::string cgiRequest::createCgiFilePath(){
	return "./html_files" + _scriptPage + "/" + _baseRequest->fileName;
}

bool cgiRequest::cgiCheckLanguage() const {

	if (std::system((_execExtension +  " --version").c_str()))
		return true;
	return false;
}

bool cgiRequest::checkFilePermissions() {
	if (access(_cgiFilePath.c_str(), F_OK) != 0) {
		std::cerr << "Status 2: Not Found\n" << std::endl;
		return false;
	}
	return true;
}

std::string cgiRequest::constructScriptPage(std::vector<std::string> &folderContainer) {
	std::string temp = "";
	for (size_t i = 0; i <= folderContainer.size() - 2; i++) {
		temp += "/" + folderContainer[i];
	}
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
	if (!checkFilePermissions()) {
		statusCode = 500;
		std::cout << "file not found\n";
		return false;
	}
	return true;
}

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
	std::cout << "cgiPath: " << _cgiFilePath << std::endl;
	std::cout << "skriptName: " << _baseRequest->fileName << std::endl;
	std::cout << "tempFile: " << _tempFile << std::endl;
	std::cout << "workingDirectory: " << _workingDirectory << std::endl;
	std::cout << "fileDescriptor: " << _fileDescriptor << std::endl;
	std::cout << "queryString: " << _queryString << std::endl;
	std::cout << "extension: " << _extension << std::endl;
	std::cout << "query: " << _hasQueryString << std::endl;
	std::cout << "execExtension: " << _execExtension << std::endl;
	std::cout << "execPath: " << _execPath << std::endl;
	std::cout << "cgiFilePath: " << _cgiFilePath << std::endl;
	std::cout << "returnFilePath: " << _returnFilePath << std::endl;
	std::cout << "hasQueryString: " << _hasQueryString << std::endl;
	std::cout << "fileIsReady: " << fileIsReady << std::endl;
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
