/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbolkova <tbolkova@student.42wolfsburg.d>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/28 15:13:09 by tbolkova          #+#    #+#             */
/*   Updated: 2023/10/28 15:13:09 by tbolkova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_HPP
# define CGI_HPP

#include "Header.h"
#include "request.hpp"

class cgiRequest {
private:
	request *_baseRequest;
	serverConf &_serverConfig;
	int serverIndex;
	std::string _scriptPage;
	std::string _skriptName;
	std::string _tempFile;
	std::string _workingDirectory;
	int _fileDescriptor;
	std::string _queryString;
	std::string _extension;
	std::string _execExtension;
	std::string _execPath;
	std::string _cgiFilePath;
	std::string _returnFilePath;
	bool _hasQueryString;
	bool fileIsReady;
	bool _alarmSignal;	// boolean to check if alarm was activated.

public:
	explicit cgiRequest(request *baseRequest, serverConf &serverConfig, int serverIndex);
	~cgiRequest();
	static bool _errorSignal;
	int statusCode;
	bool cgiCheckLanguage() const;
	bool checkExtensionServerConfig();
	bool cgiValidExtension(std::string url);
	bool createTemporaryFile();
	void executeCgi();


	std::string parseExtension();
	std::string setExecExtension();
	std::string createWorkingDirectoryPath();
	std::string createExecPath();
	std::string createTempPath();
	std::string createQueryString();
	std::string parseGetRequestPage();
	std::string parsePostRequestPage();
	bool checkQueryString();
	bool prepareExecution();
	void printCgiRequest();

	std::string constructScriptPage(std::vector<std::string> &folderContainer);
	std::string setScriptPage();

	bool inputCheck();
	static void handleAlarmSignal(int signal);

	void getErrorHtmlContent(int _errorCode);

	std::string getFilePath();
};

#endif