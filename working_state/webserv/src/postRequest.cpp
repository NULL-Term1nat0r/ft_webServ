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
	parseFileExtension();
//	parseDataType();
//	parseFileType();
	std::cout << "fileNamepost: " << _fileName << std::endl;
	std::cout << "fileTypepost: " << _fileType << std::endl;
	this->_firstChunkSent = false;
	this->_allChunksSent = false;
	this->_boundary = parsing::returnValue("boundary=", _baseRequest->getRequestString(), "\r");
	this->_contentLength = std::stoi(parsing::returnValue("Content-Length:", _baseRequest->getRequestString(), "\r"));
	this->_filePath = "./html_files/upload/" + this->_fileName;
	this->_dataRecieved = 0;
}

postRequest::~postRequest() {}

void postRequest::printPostRequest() {
	std::cout << "allChunksSent: " << this->_allChunksSent << std::endl;
	std::cout << "dataRecieved: " << this->_dataRecieved << std::endl;
}

//void postRequest::parseDataType() {
//
//	if (_baseRequest->getRequestString().find("Content-Type: multipart/form-data") != std::string::npos)
//		this->_multiFormData = true;
//	else if (_baseRequest->getRequestString().find("Content-Type: text/plain") != std::string::npos)
//		this->_textData = true;
//}

//void postRequest::parseFileType() {
//	size_t start = _baseRequest->getRequestString().find("Content-Type: ");
//	start = _baseRequest->getRequestString().find("Content-Type: ", start + 1);
//	size_t end = _baseRequest->getRequestString().find("\r", start + 14);
//	this->_fileType = _baseRequest->getRequestString().substr(start + 14, end - start - 14);
//}

void postRequest::parseFileExtension() {
	size_t start = _fileName.find_last_of('.');
	if (start == std::string::npos)
		this->_fileExtension = "not supported";
	else
		this->_fileExtension = _fileName.substr(start + 1);
}

void postRequest::writeBinaryToFile(std::vector<uint8_t> &data){

	if (!_firstChunkSent) { //first chunk is revieced
		handleFirstChunk(data);
	}
	if (_firstChunkSent && static_cast<size_t>(_contentLength - _dataRecieved) > data.size()) { //chunks after first chunk
		handleMiddleChunk(data);
	}
	if (_firstChunkSent && static_cast<size_t>(_contentLength - _dataRecieved) <= data.size() && !_allChunksSent){ //last chunk recieved
		handleLastChunk(data);
	}
	std::cout << "chunk sent\n";
	_firstChunkSent = true;
	_dataRecieved += data.size();
}

void postRequest::handleFirstChunk(std::vector<uint8_t> &data){
	std::cout << "-------content length: " << _contentLength << std::endl;
	size_t boundaryPos = _baseRequest->getRequestString().find(this->_fileType + "\r\n\r\n");
	if (boundaryPos != std::string::npos) {
		boundaryPos += this->_fileType.length() + 4;
	} else {
		std::cout << "boundary not found" << std::endl;
		throw postException();
	}
	std::ofstream file(this->_filePath, std::ios::out | std::ios::binary);

	if (file.is_open()) {
		std::vector<uint8_t> binaryData(data.begin() + boundaryPos, data.end());
		file.write(reinterpret_cast<char *>(&binaryData[0]), binaryData.size());
		file.close();
	}
	else {
		throw postException();
	}
	if (static_cast<size_t>(this->_contentLength) <= data.size()){
		checkLastChunk(data, this->_boundary);
		std::cout << " everything is in one chunk\n";
		this->_allChunksSent = true;
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
	std::cout << "last vhunk has been sent-------" << std::endl;
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

void postRequest::parseFileName(){
	int start = _baseRequest->getRequestString().find("filename=\"") + 10;
	int end = _baseRequest->getRequestString().find("\"", start);
	this->_fileName = _baseRequest->getRequestString().substr(start, end - start);
	std::cout << "fileName: " << _fileName << std::endl;
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

