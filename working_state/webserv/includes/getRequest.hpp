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
	std::string filePath;
public:
	explicit getRequest(request *baseRequest, serverConf &serverConfig, int serverIndex);
	~getRequest();

	class getRequestInvalidFileException : public std::exception
	{
		virtual const char *what() const throw();
	};
	request *_baseRequest;
	serverConf &_serverConfig;
	int serverIndex;
	bool redirection;
	bool autoIndexListing;

	std::string checkRewrite(std::string url);
	static bool fileExists(const char* filePath);
	std::string createFilePath();

	std::string getFilePath();
	bool getRedirection();

};

#endif

