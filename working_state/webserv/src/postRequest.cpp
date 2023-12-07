/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   postRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: estruckm <estruckm@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 14:17:08 by estruckm          #+#    #+#             */
/*   Updated: 2023/10/24 19:16:35 by estruckm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/postRequest.hpp"

postRequest::postRequest(request *baseRequest, serverConf &serverConfig, int serverIndex) : _baseRequest(baseRequest), _serverConfig(serverConfig), serverIndex(serverIndex) {
	parseFileName();
	this->_isCgiScript = checkIfCgi();
	parseFileExtension();
	this->_firstChunkSent = false;
	this->_allChunksSent = false;
	this->_boundary = parseValue("boundary=", _baseRequest->getRequestString(), "\r");
	std::cout << "boundary: " << this->_boundary << std::endl;
	this->_contentLength = std::stoi(parsing::returnValue("Content-Length:", _baseRequest->getRequestString(), "\r"));
	std::cout << "contentLength: " << this->_contentLength << std::endl;
	this->_filePath = "./html_files/upload/" + this->_fileName;
	this->_fileType = _serverConfig.fileTypeContainer[_fileExtension];
	this->_dataRecieved = 0;
}

postRequest::~postRequest() {}

void postRequest::printPostRequest() {
	std::cout << "allChunksSent: " << this->_allChunksSent << std::endl;
	std::cout << "dataRecieved: " << this->_dataRecieved << std::endl;
}

void postRequest::parseFileExtension() {
	size_t start = _fileName.find_last_of('.');
	if (start == std::string::npos)
		this->_fileExtension = "not supported";
	else
		this->_fileExtension = _fileName.substr(start + 1);
}

void postRequest::writeBinaryToFile(std::vector<uint8_t> &data){

	if (!_firstChunkSent) {
		std::cout << yellow << _baseRequest->getRequestString() << reset << std::endl;
		handleFirstChunk(data);
	}
	if (_firstChunkSent && static_cast<size_t>(_contentLength - _dataRecieved) > data.size()) { //chunks after first chunk
		handleMiddleChunk(data);
	}
	if (_firstChunkSent && static_cast<size_t>(_contentLength - _dataRecieved) <= data.size() && !_allChunksSent){ //last chunk recieved
		handleLastChunk(data);
	}
	_firstChunkSent = true;
	_dataRecieved += data.size();
}

void postRequest::handleFirstChunk(std::vector<uint8_t> &data){
	size_t boundaryPos = _baseRequest->getRequestString().find("Content-Type: " + _fileType + "\r\n\r\n");
	std::cout << "size of data: " << data.size() << std::endl;

	if (boundaryPos != std::string::npos) {
		boundaryPos += ("Content-Type: " + _fileType + "\r\n\r\n").length();
	} else {
		std::cout << "boundary not found" << std::endl;
		throw postException();
	}
	std::ofstream file(this->_filePath, std::ios::out | std::ios::binary);

	if (file.is_open()) {
		std::vector<uint8_t> binaryData(data.begin() + boundaryPos, data.end());
		if (static_cast<size_t>(this->_contentLength) <= data.size()){
			checkLastChunk(binaryData, this->_boundary);
			std::cout << "size of data: " << data.size() << std::endl;
			std::cout << " everything is in one chunk\n";
			this->_allChunksSent = true;
		}
		file.write(reinterpret_cast<char *>(&binaryData[0]), binaryData.size());
		file.close();
	}
	else {
		throw postException();
	}
}


void postRequest::handleMiddleChunk(std::vector<uint8_t> &data){
	std::ofstream file(this->_filePath, std::ios::app | std::ios::binary);
	if (file.is_open()){
		file.write(reinterpret_cast<char *>(&data[0]), data.size());
		file.close();
	}
	else {
		std::cout << "File not open" << std::endl;
		throw postException();
	}
}

void postRequest::handleLastChunk(std::vector<uint8_t> &data){
	this->_allChunksSent = true;
	checkLastChunk(data, this->_boundary);
	std::ofstream file(this->_filePath, std::ios::app | std::ios::binary);
	if (file.is_open()){
		file.write(reinterpret_cast<char *>(&data[0]), data.size());
		file.close();
	}
	else {
		std::cout << "File not open" << std::endl;
		throw postException();
	}
}

std::vector<uint8_t>::iterator postRequest::findEndBoundary(std::vector<uint8_t>& lastChunk, const std::vector<uint8_t>& boundary) {
	for (std::vector<uint8_t>::iterator it = lastChunk.begin(); it != lastChunk.end(); ++it) {
		if (std::equal(boundary.begin(), boundary.end(), it))
			return it;
	}
	return lastChunk.end();
}

void	postRequest::checkLastChunk(std::vector<uint8_t> &lastChunk, std::string startBoundary) {
	std::string endBoundary = "\r\n--" + startBoundary;
	std::vector<uint8_t> boundary = std::vector<uint8_t>(endBoundary.begin(), endBoundary.end());
	std::vector<uint8_t>::iterator boundaryPos = findEndBoundary(lastChunk, boundary);
	if (boundaryPos != lastChunk.end())
		lastChunk.erase(boundaryPos, lastChunk.end());
}

std::string postRequest::parseValue(std::string search, std::string source, std::string delimeter){
	int start = source.find(search) + search.length();
	int end = source.find(delimeter, start);
	return source.substr(start, end - start);
}

void postRequest::parseFileName(){
	int start = _baseRequest->getRequestString().find("filename=\"") + 10;
	int end = _baseRequest->getRequestString().find("\"", start);
	this->_fileName = _baseRequest->getRequestString().substr(start, end - start);
	std::cout << "fileName: " << _fileName << std::endl;
}

bool postRequest::checkIfCgi(){
	if (_fileName.find(".php") != std::string::npos || _fileName.find(".py") != std::string::npos)
		return true;
}

std::string postRequest::getBoundary() {
	return this->_boundary;
}

int postRequest::getContentLength() {
	return this->_contentLength;
}

bool postRequest::getMultiFormData() {
	return this->_multiFormData;
}

bool postRequest::getTextData() {
	return this->_textData;
}

bool postRequest::getAllChunksSent() {
	return this->_allChunksSent;
}

std::string postRequest::getFileType() {
	return this->_fileType;
}

const char	*postRequest::postException::what() const throw() {
	return "postException has accured\n";
}

std::string postRequest::getFileExtension() {
	return this->_fileExtension;
}

