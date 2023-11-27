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
	std::string _cgiPath;
	std::string _skriptName;
	std::string _tempFile;
	std::string _workingDirectory;
	int _fileDescriptor;
	std::string _query;
	std::string _execExtension;
	std::string _execPath;
	std::string _cgiFilePath;
	std::string _returnFilePath;
	int	_errorCode;
	bool fileIsReady;
	bool _isError;
	bool _alarmSignal;	// boolean to check if alarm was activated.

public:
	explicit cgiRequest(request *baseRequest, serverConf &serverConfig, int serverIndex);
	~cgiRequest();
	static bool _errorSignal;
	bool cgiCheckLanguage() const;
	bool checkExtensionServerConfig(std::string extension);
	bool cgiValidExtension(std::string url);
	bool createTemporaryFile();
	bool executeCgi();

	std::string constructScriptPage(std::vector<std::string> &folderContainer);
	void setScriptPage();

	bool inputCheck();
	static void handleAlarmSignal(int signal);

	void getErrorHtmlContent(int _errorCode);

	std::string getFilePath();
};

#endif