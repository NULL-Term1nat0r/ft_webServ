#include "../includes/Header.h"





//int main() {
//	std::string str = "23";
//	bool activated = str == "25";
//	std::cout << activated << std::endl;
//}

bool checkString(std::string string){
	return string.find("test") != std::string::npos;
}


//int main()
//{
//	std::cout << "bool: " << checkString("arfgafasfa test eggafafasgadrgag") << std::endl;
//	return 0;
//}