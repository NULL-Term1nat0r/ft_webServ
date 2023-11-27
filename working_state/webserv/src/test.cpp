#include "../includes/Header.h"


std::string Function(std::string &str){
	std::string result;
	std::string::iterator it = str.begin();
	while (it != str.end()){
		if (*it == '.'){
			result = std::string(it, str.end());
			break;
		}
		it++;
	}
	return result;
}




//int main()
//{
//	std::string s1 = "/upload/haha/picture.jpeg";
//	std::string s2 = "/upload/haha";
//	std::string s3 = "/upload";
//	std::string s4 = "/";
//
//	std::string &s1Ref = s1;
//
//	std::string result = Function(s1Ref);
//	std::cout << "result : " << result << std::endl;
//
//	return 0;
//}