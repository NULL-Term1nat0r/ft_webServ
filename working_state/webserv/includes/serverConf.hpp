#ifndef serverConf_HPP
#define serverConf_HPP

//#include "Header.h"
#include "config.hpp"
#include "parsing.hpp"
// #include "request.hpp"
// #include "postRequest.hpp"
// #include "deleteRequest.hpp"
// #include "getRequest.hpp"
// #include "response.hpp"

typedef struct LocationStruc
{
	LocationStruc();
	~LocationStruc();
	bool						allowGet;
	bool						allowPost;
	bool						allowDelete;
	std::string					rewrite;
	std::string					autoindex;
	std::string					index;
	std::vector<std::string>	cgi;
	bool						indexBool;
} LocationStruc;

typedef struct serverSettings
{
	serverSettings();
	~serverSettings();
	std::map<std::string, LocationStruc>	locations;
	short unsigned							port;
	std::string								serverName;
	int										bodySize;
	std::map<int, std::string>				errorPages;
} serverSettings;

class serverConf
{
public:
	serverConf();
	serverConf(Config conf);
	~serverConf();

	void	getServerConf(Config conf);
	std::string getFileType(std::string filePath);
	bool checkFileType(std::string fileExtension);
	void constructFileTypeContainer();

	class WrongPort : public std::exception
	{
		public:
			virtual const char	*what() const throw();
	};
	class WrongAmount : public std::exception
	{
		public:
			virtual const char	*what() const throw();
	};
	class PortAlreadyInUse : public std::exception
	{
		public:
			virtual const char	*what() const throw();
	};
	class WrongCgiExtension : public std::exception
	{
		public:
			virtual const char	*what() const throw();
	};
	class PathToErrorPageWrong : public std::exception
	{
		public:
			virtual const char	*what() const throw();
	};


	void	iterate();
	std::vector<std::string>		_validExtensions;
	void							_setValidExtensions(std::map<std::string, std::vector<std::string> > globalContext);

	int		  							_workerProcesses;
	int									_workerConnections;
	int									_scriptTimeout;
	int									_clientTimeout;
	int									_buffSize;
	int									_backlog;
	std::vector<serverSettings>			_server;
	std::map<std::string, std::string>	fileTypeContainer;
private:
	//*******************//
	//*** server.cpp ***//
	//*******************//
	void						_setCgi(std::map<std::string, std::vector<std::string> > location, std::string locationName, serverSettings &conf);
	void						_setRewrite(std::map<std::string, std::vector<std::string> > location, std::string locationName, serverSettings &conf);
	void						_setAutoIndex(std::map<std::string, std::vector<std::string> > location, std::string locationName, serverSettings &conf);
	void						_setIndex(std::map<std::string, std::vector<std::string> > location, std::string locationName, serverSettings &conf);
	void						_setAllowMethods(std::map<std::string, std::vector<std::string> > location, std::string locationName, serverSettings &conf);
	void						_setLocationServerValues(std::map<std::string, std::vector<std::string> > location, std::string locationName, serverSettings &conf, size_t i);
	void						_setServerValues(std::map<std::string, std::map<std::string, std::vector<std::string> > > server, std::vector<std::string> locations);
	void						_serverValues(Config conf);
	void						_indexFileNotExisting(Config conf);

	void						initErrorPages(serverSettings &conf);
	//*******************//
	//*** server2.cpp ***//
	//*******************//
	void						_setErrorPage400(std::map<std::string, std::vector<std::string> > location, serverSettings &conf);
	void						_setErrorPage403(std::map<std::string, std::vector<std::string> > location, serverSettings &conf);
	void						_setErrorPage404(std::map<std::string, std::vector<std::string> > location, serverSettings &conf);
	void						_setErrorPage405(std::map<std::string, std::vector<std::string> > location, serverSettings &conf);
	void						_setErrorPage413(std::map<std::string, std::vector<std::string> > location, serverSettings &conf);
	void						_setErrorPage415(std::map<std::string, std::vector<std::string> > location, serverSettings &conf);
	void						_setErrorPage500(std::map<std::string, std::vector<std::string> > location, serverSettings &conf);
	void						_setErrorPage504(std::map<std::string, std::vector<std::string> > location, serverSettings &conf);
	void						_setErrorPages(std::map<std::string, std::vector<std::string> > location, serverSettings &conf);
	void						_setServerName(std::map<std::string, std::vector<std::string> > location, serverSettings &conf);
	void						_setPort(std::map<std::string, std::vector<std::string> > location, serverSettings &conf);
	void						_setBodySize(std::map<std::string, std::vector<std::string> > location, serverSettings &conf);
	void						_setGlobalServerValues(std::map<std::string, std::vector<std::string> > location, serverSettings &conf);
	void						_setWorkerProcesses(std::map<std::string, std::vector<std::string> > globalContext);
	void						_setWorkerConnections(std::map<std::string, std::vector<std::string> > globalContext);
	void						_setScriptTimeouts(std::map<std::string, std::vector<std::string> > globalContext);
	void						_setClientTimeout(std::map<std::string, std::vector<std::string> > globalContext);
	void						_setBuffSize(std::map<std::string, std::vector<std::string> > globalContext);
	void						_setBackLog(std::map<std::string, std::vector<std::string> > globalContext);
	void						_globalValues(Config conf);
	void						_checkDuplicatePorts();
};

#endif
