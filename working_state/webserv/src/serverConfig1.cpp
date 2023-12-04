
#include "../includes/serverConf.hpp"

LocationStruc::LocationStruc() : allowGet(false), allowPost(false), allowDelete(false), rewrite(""), autoindex(""), index(""), cgi(), indexBool(true) {}

LocationStruc::~LocationStruc(){}

serverSettings::serverSettings() : locations(), port(0), serverName(""), errorPages() {}


serverSettings::~serverSettings() {}

void serverConf::constructFileTypeContainer(){
	fileTypeContainer["html"] = "text/html";
	fileTypeContainer["css"] = "text/css";
	fileTypeContainer["py"] = "text/python";
	fileTypeContainer["php"] = "text/php";
	fileTypeContainer["jpg"] = "image/jpeg";
	fileTypeContainer["jpeg"] = "image/jpeg";
	fileTypeContainer["png"] = "image/png";
	fileTypeContainer["gif"] = "image/gif";
	fileTypeContainer["svg"] = "image/svg+xml";
	fileTypeContainer["ico"] = "image/x-icon";
	fileTypeContainer["mp3"] = "audio/mpeg";
	fileTypeContainer["woff"] = "font/woff";
	fileTypeContainer["woff2"] = "font/woff2";
	fileTypeContainer["ttf"] = "font/ttf";
	fileTypeContainer["otf"] = "font/otf";
	fileTypeContainer["txt"] = "text/plain";
	fileTypeContainer["pdf"] = "application/pdf";
	fileTypeContainer["xml"] = "application/xml";
	fileTypeContainer["tar"] = "application/x-tar";
	fileTypeContainer["xpm"] = "image/xpm";
}

bool serverConf::checkFileType(std::string fileExtension) {
	if (fileTypeContainer.find(fileExtension) == fileTypeContainer.end())
		return false;
	return true;
}

std::string serverConf::getFileType(std::string filePath){
	std::string extension = parsing::getFileExtension(filePath);
	std::map<std::string, std::string>::iterator fileType = fileTypeContainer.find(extension);
	if (fileType == fileTypeContainer.end())
		return "text/plain";
	return fileType->second;
}

void	serverConf::_setCgi(std::map<std::string, std::vector<std::string> > location, std::string locationName,serverSettings &conf) {
	if (location.find("cgi") != location.end() && location["cgi"].size() > 0) {
		for (size_t i = 0; i < location["cgi"].size(); i++) {
			if (location["cgi"][i] != ".py" && location["cgi"][i] != ".php")
				throw WrongCgiExtension();
		}
		conf.locations[locationName].cgi = location["cgi"];
	}
}

void	serverConf::_setRewrite(std::map<std::string, std::vector<std::string> > location, std::string locationName, serverSettings &conf) {
	if (location.find("rewrite") != location.end() && location["rewrite"][0].size() > 0)
		conf.locations[locationName].rewrite = location["rewrite"][0];
}

void	serverConf::_setAllowMethods(std::map<std::string, std::vector<std::string> > location, std::string locationName, serverSettings &conf) {
	if (location.find("allow_methods") != location.end()) {
		for (size_t j = 0; j < location["allow_methods"].size(); j++) {
			if (location["allow_methods"][j] == "GET")
				conf.locations[locationName].allowGet = true;
			else if (location["allow_methods"][j] == "POST")
				conf.locations[locationName].allowPost = true;
			else if (location["allow_methods"][j] == "DELETE")
				conf.locations[locationName].allowDelete = true;
		}
	}
	else {
		conf.locations[locationName].allowDelete = true;
		conf.locations[locationName].allowGet = true;
		conf.locations[locationName].allowPost = true;
	}
}

void	serverConf::_setAutoIndex(std::map<std::string, std::vector<std::string> > location, std::string locationName, serverSettings &conf) {
	if (location.find("autoindex") != location.end() && location["autoindex"].size() > 0)
		conf.locations[locationName].autoindex = location["autoindex"][0];
}


void	serverConf::_setIndex(std::map<std::string, std::vector<std::string> > location, std::string locationName, serverSettings &conf) {
	if (location.find("index") != location.end() && location["index"].size() > 0)
		conf.locations[locationName].index = location["index"][0];
}

void	serverConf::_setLocationServerValues(std::map<std::string, std::vector<std::string> > location, std::string locationName, serverSettings &conf, size_t i) {
	if (i == 0)
		conf.locations = std::map<std::string, LocationStruc>();
	locationName = locationName.substr(8, locationName.size() - 8);
	conf.locations[locationName] = LocationStruc();
	void (serverConf::*locationFunc[]) (std::map<std::string, std::vector<std::string> > location, std::string locationName, serverSettings &conf)
	= {&serverConf::_setCgi, &serverConf::_setRewrite, &serverConf::_setAutoIndex, &serverConf::_setIndex, &serverConf::_setAllowMethods};
	for (size_t j = 0; j < 5; j++)
		(this->*locationFunc[j])(location, locationName, conf);
}

void	serverConf::_setServerValues(std::map<std::string, std::map<std::string, std::vector<std::string> > > server, std::vector<std::string> locations) {
	serverSettings	conf;
	for (size_t i = 0, j = 0; j < locations.size(); j++) {
		if (locations[j] == "noLocation")
			_setGlobalServerValues(server[locations[j]], conf);
		else
			_setLocationServerValues(server[locations[j]], locations[j], conf, i++);
	}
	_server.push_back(conf);
}

void	serverConf::_serverValues(Config conf) {
	std::vector<std::map<std::string, std::map<std::string, std::vector<std::string> > > >	serverContext = conf.getConfFile();
	std::vector<std::vector<std::string> >	locations = conf.getLocations();
	for (size_t i = 0; i < serverContext.size(); i++)
		_setServerValues(serverContext[i], locations[i]);
}

void	serverConf::_checkDuplicatePorts() {
	for (size_t i = 0; i < _server.size() - 1; i++) {
		for (size_t j = i + 1 ; j < _server.size(); j++) {
			if (_server[i].port == _server[j].port)
				throw PortAlreadyInUse();
		}
	}
}

void	serverConf::_indexFileNotExisting(Config conf) {
	std::string file;
	int			fileDescriptor;
	for (int i = 0; i < _server.size(); i++) {
		for (std::map<std::string, LocationStruc>::iterator it = _server[i].locations.begin(); it != _server[i].locations.end(); it++) {
			if (it->first != "/")
				file = "html_files" + it->first + "/" + it->second.index;
			else
				file = "html_files" + it->first + it->second.index;
			fileDescriptor = open(file.c_str(), O_RDONLY);
			if (fileDescriptor == -1 || it->second.index == "")
				it->second.indexBool = false;
			if (fileDescriptor != -1)
				close(fileDescriptor);
		}
	}
}

std::string    serverConf::getErrorPage(int serverIndex, int errorCode) {
	return _server[serverIndex].errorPages[errorCode];
}

void	serverConf::initErrorPages(serverSettings &conf) {
	conf.errorPages[400] = "./html_files/errorPages/error400.html";
	conf.errorPages[403] = "./html_files/errorPages/error403.html";
	conf.errorPages[404] = "./html_files/errorPages/error404.html";
	conf.errorPages[405] = "./html_files/errorPages/error405.html";
	conf.errorPages[413] = "./html_files/errorPages/error413.html";
	conf.errorPages[415] = "./html_files/errorPages/error415.html";
	conf.errorPages[500] = "./html_files/errorPages/error500.html";
	conf.errorPages[504] = "./html_files/errorPages/error504.html";
}

void	serverConf::getServerConf(Config conf) {
	_globalValues(conf);
	_serverValues(conf);
	_checkDuplicatePorts();
	if (_server.empty())
		throw WrongAmount();
	_indexFileNotExisting(conf);

	for (size_t i = 0; i < _server.size(); i++) {
		for (std::map<int, std::string>::iterator it = _server[i].errorPages.begin(); it != _server[i].errorPages.end(); it++) {
			std::cout << it->first << " " << it->second << std::endl;
		}
		std::cout << std::endl;
	}
}
