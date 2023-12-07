/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: estruckm <estruckm@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 14:32:29 by estruckm          #+#    #+#             */
/*   Updated: 2023/10/03 14:42:48 by estruckm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENTREQUEST_HPP
#define CLIENTREQUEST_HPP

#include "Header.h"


class request {
private:


public:
	explicit request(std::vector<uint8_t> &clientRequest, serverConf &serverConfig, int serverIndex);
	virtual ~request();

	bool isPageConfigured;
	bool isPageValid;
	bool _get;
	bool _post;
	bool _delete;
	bool methodIsValid;
	bool _cgi;

	bool _aliveConnection;
	bool _closeConnection;

	serverConf &_serverConfig;
	int _serverIndex;
	std::string fileName;
	std::string methodString;
	std::string page;
	std::string url;
	std::string _request;
	std::string filePath;

	std::string parseFileName();
	std::string checkRewrite();
	std::string getRewrite(std::string page);
	bool isCgi();
	bool isAliveConnection();
	bool isCloseConnection();
	bool isMethodConfigured();



	bool isGetMethod();
	bool isPostMethod();
	bool isDeleteMethod();

	bool getGetMethod();
	bool getPostMethod();
	bool getDeleteMethod();
	bool getCgi();
	bool getURL();

	bool getAliveConnection();
	bool getCloseConnection();

	std::string getStringURL();
	std::string getReferer();
	std::string &getRequestString();
	bool getCgiMethod();
	void checkMethods();
	std::string getMethodString();
	std::string parseUrl();

	bool checkCgi(std::string url);

	void setDefaultValues();
	void parseRequest();

	void validateRequest();
	void printRequest();
};

#endif



/*
steps for response class:
		what do I dont know ?
			- how much is the contenlength, all data or just file data
		- inherit from request class to get the url string
		-*/
