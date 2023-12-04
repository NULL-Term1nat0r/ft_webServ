
#ifndef parsing_HPP
#define parsing_HPP

#include "Header.h"

class parsing
{
private:
	// Class members

public:
	parsing();
	~parsing();

//	static int checkIfFound(std::string array[], std::string str);
//	static std::string getValue(std::string str, std::string key, int number);
	static std::string vectorToString(const std::vector<uint8_t>& inputVector, int size);
	static std::string vectorToLimitedString(const std::vector<uint8_t>& data, size_t maxBytes);
//	static std::string getLineOfKey(std::string str, std::string key);


	static std::string returnValue(std::string search, std::string source, std::string delimeter);
	static std::vector<uint8_t> unsignedCharToVector(unsigned char *data, size_t size);
	static std::string getFileExtension(std::string filePath);
	static std::string getErrorFilePath(int errorCode);

	static bool folderExists(const std::string& path);
	static bool fileExists(const std::string&path);
	static std::vector<std::string> split(const std::string s, char delim);
	static std::string returnPage(const std::string &url);
	static std::string getErrorPagePath(int errorCode);

	static char decodeHex(char c);
	static char decodePercentEncoding(const char *encoded);
	static void decodeUrl(std::string &url);
	static std::string getFileName(std::string filePath);
	static std::string constructPage(std::string url);
	static bool checkIfPageConfigured(std::map<std::string, struct LocationStruc> locations, std::string page);

};

#endif
