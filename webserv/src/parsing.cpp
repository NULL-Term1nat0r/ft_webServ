
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

std::string parsing::vectorToString(const std::vector<uint8_t>& inputVector) {
	std::string result;
	for (size_t i = 0; i < inputVector.size(); ++i) {
		result += static_cast<char>(inputVector[i]);
	}
	return result;
}

std::string parsing::returnValue(std::string search, std::string source, std::string delimeter){
	int start = source.find(search) + search.length() + delimeter.length();
	int end = source.find(delimeter, start);
	return source.substr(start, end - start);
}

//std::string parsing::returnValue(std::string key, std::string source, std::string limiter){
//	size_t start = source.find(key);
//	size_t end = source.find(limiter, start + key.length());
//	if (key == "filename=\""){
//		if (source.find(key) == std::string::npos)
//			std::cout << "key not found" << std::endl;
//	}
//	return source.substr(start + key.length(), end - start - key.length());
//}

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
	std::cout << "jump into returnPage" << std::endl;
	if (url == ""){
		std::cout << "url is empty" << std::endl;
		return "";
	}
	if (url == "/")	{
		std::cout << "url is /" << std::endl;
		return "/";
	}			//if url is "/" return index.html
	std::vector<std::string> result = split(url, '/');
	std::string path = "./html_files" + url;
	if (folderExists(path)) {
		std::cout << "the url is a folderPath and not a filePath" << std::endl;
		return (result[result.size() - 1]);
	}
	if (fileExists(path)){
		std::cout << "the url is a filePath and not a folderPath" << std::endl;
		if (result.size() == 1){
			return ("/");
		}
		return (result[result.size() - 2]);
	}
	std::cout << "the url is neither a filePath nor a folderPath" << std::endl;
	std::cout << "urlstring: " << path << std::endl;
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

void	parsing::decodeUrl(std::string &url) {
	for (size_t pos = 0; pos != std::string::npos; pos = url.find('%', pos + 1)) {
		pos = url.find('%', pos);
		if (pos != std::string::npos)
			url = url.substr(0, pos) + decodePercentEncoding(url.substr(pos, 3).c_str()) + url.substr(pos + 3);
	}
}



std::string parsing::getFileType(std::string filePath){
	std::string extension = getFileExtension(filePath);
	if (extension == "")
		return "text/plain";
	if (extension == "html")
		return "text/html";
	if (extension == "css")
		return "text/css";
	if (extension == "py")
		return "text/html";
	if (extension == "php")
		return "text/html";
	if (extension == "jpg")
		return "image/jpeg";
	if (extension == "jpeg")
		return "image/jpeg";
	if (extension == "png")
		return "image/png";
	if (extension == "gif")
		return "image/gif";
	if (extension == "svg")
		return "image/svg+xml";
	if (extension == "ico")
		return "image/x-icon";
	if (extension == "mp3")
		return "audio/mpeg";
	if (extension == "mp4")
		return "video/mp4";
	if (extension == "woff")
		return "font/woff";
	if (extension == "woff2")
		return "font/woff2";
	if (extension == "ttf")
		return "font/ttf";
    if (extension == "otf")
		return "font/otf";
	if (extension == "txt")
		return "text/plain";
	if (extension == "pdf")
		return "application/pdf";
	if (extension == "json")
		return "application/json";
	if (extension == "xml")
		return "application/xml";
	if (extension == "zip")
		return "application/zip";
	if (extension == "tar")
		return "application/x-tar";
	return "";
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

