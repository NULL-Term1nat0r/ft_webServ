
#include "../includes/parsing.hpp"

// Implement class methods here

parsing::parsing()
{
	std::cout << "parsing default constructor called" << std::endl;
}

parsing::~parsing()
{
	std::cout << "parsing destructor called" << std::endl;
}

std::string parsing::vectorToString(const std::vector<uint8_t>& inputVector, int size) {
	std::string result;
	for (size_t i = 0; i < size; ++i) {
		result += static_cast<char>(inputVector[i]);
	}
	return result;
}

std::string parsing::returnValue(std::string search, std::string source, std::string delimeter){
	int start = source.find(search) + search.length() + delimeter.length();
	int end = source.find(delimeter, start);
	return source.substr(start, end - start);
}

std::vector<uint8_t> parsing::unsignedCharToVector(unsigned char *data, size_t size){
	std::vector<uint8_t> result;
	result.reserve(size); // Reserve space for the expected number of elements to avoid reallocation

	for (size_t i = 0; i < size; ++i) {
		result.push_back(data[i]);
	}

	return result;
}

std::string parsing::getErrorFilePath(int errorCode){
	std::string filePath = "./html_files/errorPages/error" + std::to_string(errorCode) + ".html";
	return filePath;
}

std::string parsing::getFileExtension(std::string filePath){
	size_t start = filePath.find_last_of('.');
	if (start == std::string::npos)
		return "";
	else{
		return filePath.substr(start + 1);
	}

}

bool parsing::folderExists(const std::string& path) {
	struct stat st;
	return (stat(path.c_str(), &st) == 0 && S_ISDIR(st.st_mode));
}

bool parsing::fileExists(const std::string&path) {
	struct stat st;
	return (stat(path.c_str(), &st) == 0 && S_ISREG(st.st_mode));
}

std::vector<std::string> parsing::split(const std::string s, char delim) {
	std::vector<std::string> result;
	std::istringstream iss(s);
	std::string token;
	std::getline(iss, token, delim);
	while (std::getline(iss, token, delim)) {
		result.push_back(token);
	}
	return result;
}

std::string parsing::returnPage(const std::string &url) {
	if (url == ""){
		return "";
	}
	if (url == "/")	{
		return "/";
	}			//if url is "/" return index.html
	std::vector<std::string> result = split(url, '/');
	std::string path = "./html_files" + url;
	if (folderExists(path)) {
		return (result[result.size() - 1]);
	}
	if (fileExists(path)){
		if (result.size() == 1){
			return ("/");
		}
		return (result[result.size() - 2]);
	}
	return "";
}

char parsing::decodeHex(char c) {
	if (c >= '0' && c <= '9')
		return c - '0';
	else if (c >= 'A' && c <= 'F')
		return c - 'A' + 10;
	else if (c >= 'a' && c <= 'f')
		return c - 'a' + 10;
	else
		return 0;
}

char parsing::decodePercentEncoding(const char* encoded) {
	if (encoded[0] != '%' || !isxdigit(encoded[1]) || !isxdigit(encoded[2]))
		return 0;
	return (decodeHex(encoded[1]) << 4) + decodeHex(encoded[2]);
}

void parsing::decodeUrl(std::string &url) {
	size_t pos = 0;
	while ((pos = url.find('%', pos)) != std::string::npos) {
		if (pos + 2 < url.length()) {
			url = url.substr(0, pos) + decodePercentEncoding(url.substr(pos, 3).c_str()) + url.substr(pos + 3);
			pos += 3;  // Move past the decoded part
		} else {
			// Handle the case where '%' is at the end of the string without enough characters to decode
			break;
		}
	}
}

std::string parsing::getErrorPagePath(int errorCode){
	std::string errorFilePath;
	switch (errorCode) {
		case 400:
			errorFilePath = "./html_files/errorPages/error400.html";
			break;
		case 403:
			errorFilePath = "./html_files/errorPages/error403.html";
			break;
		case 404:
			errorFilePath = "./html_files/errorPages/error404.html";;
			break;
		case 405:
			errorFilePath = "./html_files/errorPages/error405.html";;
			break;
		case 408:
			errorFilePath = "./html_files/errorPages/error408.html";;
			break;
		case 413:
			errorFilePath = "./html_files/errorPages/error413.html";;
			break;
		case 414:
			errorFilePath = "./html_files/errorPages/error414.html";;
			break;
		case 415:
			errorFilePath = "./html_files/errorPages/error415.html";;
			break;
		case 500:
			errorFilePath = "./html_files/errorPages/error500.html";;
			break;
		case 501:
			errorFilePath = "./html_files/errorPages/error501.html";;
			break;
		case 504:
			errorFilePath = "./html_files/errorPages/error504.html";;
			break;
		case 505:
			errorFilePath = "./html_files/errorPages/error505.html";;
			break;
		default:
			errorFilePath = "./html_files/errorPages/error404.html";
	}
	return errorFilePath;
}

