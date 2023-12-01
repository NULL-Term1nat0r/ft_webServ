
#include "../includes/response.hpp"

response::response(std::string responseFilePath, int statusCode, serverConf &serverConfig) : statusCode(statusCode), serverConfig(serverConfig), startPosition(0), _dataSend(0), firstChunkSent(false), _allChunkSent(false) {
	filePath = responseFilePath;
	if (statusCode == 619)
		this->bodySize = 0;
	else
	{
		// std::cout << green << "bodySize-> Counting file size" << filePath << reset << std::endl;
		this->bodySize = countFileSize(this->filePath);
	}
//	this->serverClass = serverClass;
}

response::~response(){}

response &response::operator=(const response &other)
{
	if (this == &other)
		return *this;
	return *this;
}

const char	*response::responseInvalidFileException::what() const throw() {
	return "Invalid File Exception in response";
}

std::string response::getChunk(int chunkSize){
	// std::cout << "send chunk\n";
	if (statusCode == 619){
		this->_allChunkSent = true;
		return createDirectoryListingHtml();
	}
	else if (!this->firstChunkSent){
		this->firstChunkSent = true;
		return (createFirstChunk(chunkSize));
	}
	else {
		return (readFileContent(chunkSize));
	}
}

std::string response::createDirectoryListingHtml(){
	std::string header;
	std::string body = generateDirectoryListing(this->filePath);
	std::stringstream ss;
	ss << body.size();
	std::string _bodySize = ss.str();
	header += "HTTP/1.1 200 OK\r\n";
	header += "Content-Type: text/html\r\n";
	header += "Content-Length: " + _bodySize + "\r\n";
	header += "\r\n";
	std::cout  << header + body << std::endl;
	return header + body;
}

std::string response::createFirstChunk(int chunkSize){
	std::ifstream file(this->filePath.c_str(), std::ios::binary);

//	std::cout << "filePath in createFirstChunk: " << this->filePath << std::endl;
	
	if (!file.is_open())
	{
		std::cout << "createFirstChunk: file not open\n";
		throw responseInvalidFileException();
	}
	std::string header;
	std::string body;
	std::string responseChunk;

	std::stringstream ss;
	ss << statusCode;
	std::string _statusCode = ss.str();

	header += "HTTP/1.1 " + _statusCode + " OK\r\n";
	header += "Content-Type: " + serverConfig.getFileType(this->filePath) + "\r\n";
	// std::cout << blue << "ContentType: " << serverConfig.getFileType(this->filePath) << reset << std::endl;
	header += "Content-Length: " + std::to_string(countFileSize(this->filePath)) + "\r\n";
	// std::cout << blue << "ContentLength: " << countFileSize(this->filePath) << reset << std::endl;
//	header += "Cache-Control: public, max-age=31536000\r\n";
//	header += "ETag: unique-identifier\r\n";
	header += "Connection: keep-alive\r\n";
	header += "\r\n";
	body = readFileContent(chunkSize - header.length());
	_dataSend = body.length();
	this->startPosition = body.length();
	return responseChunk = header + body;
}

std::string response::readFileContent(int chunkSize){
	std::ifstream file(this->filePath.c_str(), std::ios::binary);
	if (!file.is_open()){
		std::cout << "readFileContent: file not open\n";
		throw responseInvalidFileException();
	}
	file.seekg(this->startPosition);
	this->startPosition += chunkSize;
	char *buffer = new char[chunkSize];
	file.read(buffer, chunkSize);
	std::string result(buffer, file.gcount());
	delete[] buffer;
	file.close();
	_dataSend += result.length();
	// std::cout << "dataSend: " << _dataSend <<  "bodySize: " << bodySize << std::endl;
	if (_dataSend >= bodySize){
		std::cout << "all chunks in response send\n";
		_allChunkSent = true;
	}
	return result;
}

long response::countFileSize(std::string filePath){
	std::ifstream file(filePath.c_str(), std::ios::binary);
	if (!file.is_open())
	{
		std::cout << "countFileSize: file not open\n";
		throw responseInvalidFileException();
	}
	file.seekg(0, std::ios::end);
	long fileSize = file.tellg();
	file.close();

	return fileSize;
}

bool response::removeFile(const char* filepath) {
	if (std::remove(filepath) == 0)
		return false;
	return true;
}

std::string response::generateDirectoryListing(std::string path) {
//	std::cout << "filePath in generate: " << path << std::endl;
	DIR* dir = opendir(path.c_str());
	std::string dirList;
	if (dir != nullptr) {
		dirList = "<html><body><h1>Directory Listing</h1><ul>";

		struct dirent* entry;
		while ((entry = readdir(dir)) != nullptr) {
			std::string entryName = entry->d_name;
			if (entry->d_type == DT_DIR)
				dirList += "<li><b>/</b> " + entryName + "</li>";
			else
				dirList += "<li>" + entryName + "</li>";
		}
		dirList += "</ul></body></html>";
		closedir(dir);
	}
	else
		std::cerr << "Error opening directory." << std::endl;
	return dirList;
}
