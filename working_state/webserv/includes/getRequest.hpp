/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getgetRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: estruckm <estruckm@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 14:16:11 by estruckm          #+#    #+#             */
/*   Updated: 2023/10/24 14:16:11 by estruckm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getRequest.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: estruckm <estruckm@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 14:32:29 by estruckm          #+#    #+#             */
/*   Updated: 2023/10/03 14:42:48 by estruckm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef getRequest_HPP
#define getRequest_HPP

#include "Header.h"
#include "request.hpp"


class getRequest {
private:

public:
	explicit getRequest(request *baseRequest, serverConf &serverConfig, int serverIndex);
	~getRequest();

	class getRequestInvalidFileException : public std::exception
	{
		virtual const char *what() const throw();
	};

	std::string filePath;
	int statusCode;
	bool isFile;
	bool isFolder;
	bool indexPageExists;
	bool autoIndexActivated;


	request *_baseRequest;
	serverConf &_serverConfig;
	int serverIndex;
	bool redirection;
	bool autoIndexListing;
	std::string page;
	std::string url;

	void  checkRewrite();
	std::string checkFolderPath(std::string url);
	std::string checkFilePath(std::string url);
	std::string getRewrite(std::string url);
	static bool fileExists(const char* filePath);
	std::string createFilePath();
	std::string constructFilePath();
	std::string constructFolderPath();
	std::string NonConfiguredPage();

	std::string getFilePath();
	bool getRedirection();

};

#endif


