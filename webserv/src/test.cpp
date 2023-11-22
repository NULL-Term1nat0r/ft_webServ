#include "../includes/Header.h"


bool folderExists(const std::string path) {
	struct stat st;
	return (stat(path.c_str(), &st) == 0 && S_ISDIR(st.st_mode));
}

bool fileExists(const std::string path) {
	struct stat st;
	return (stat(path.c_str(), &st) == 0 && S_ISREG(st.st_mode));
}

std::vector<std::string> split(std::string s, char delim) {
	std::vector<std::string> result;
	std::istringstream iss(s);
	std::string token;
	std::getline(iss, token, delim);
	while (std::getline(iss, token, delim)) {
//		std::cout << red << "token: " << token << reset << std::endl;
		result.push_back(token);
	}
	return result;
}

void printStringVector(const std::vector<std::string>& strVector) {
	for (size_t i = 0; i < strVector.size(); ++i) {
		std::cout << strVector[i] << std::endl;
	}
}

std::string returnPage2(std::string url) {
	if (url == "")
		return "";
	if (url == "/")				//if url is "/" return index.html
		return "/";
	std::vector<std::string> result = split(url, '/');
	printStringVector(result);
	std::string path = "./html_files" + url;
	if (folderExists(path)) {
		return (result[result.size() - 1]);
	}
	if (fileExists(path)){
		if (result.size() == 1){
			std::cout << "activated" << std::endl;
			return ("/");
		}
		return (result[result.size() - 2]);
	}
	return "";
}

//int main()
//{
//	std::string s1 = "/upload/haha/picture.jpeg";
//	std::string s2 = "/upload/haha";
//	std::string s3 = "/upload";
//	std::string s4 = "/";
//
//
//
//
////	std::cout << "page: " << returnPage2("../html_files/index.html") << std::endl;
//	std::cout << "page: " << returnPage2("/test.cpp") << std::endl;
//
//	return 0;
//}