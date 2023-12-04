/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   postpostRequest.hpp                                    :+:      :+:    :+:   */
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
/*   postRequest.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: estruckm <estruckm@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 14:32:29 by estruckm          #+#    #+#             */
/*   Updated: 2023/10/03 14:42:48 by estruckm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef postRequest_HPP
#define postRequest_HPP

#include "Header.h"
#include "request.hpp"


class postRequest {
private:
	request *_baseRequest;
	serverConf &_serverConfig;
	int serverIndex;

	std::string				_boundary;
	int 					_contentLength;
	long					_dataRecieved;
	std::string				_fileName;
	std::string				_fileType;
	std::string 			_filePath;
	std::string 			_fileExtension;


	std::vector<uint8_t> 	_binaryData;
	bool					_isCgiScript;
	bool					_multiFormData;
	bool					_textData;
	bool					_cgi;
	bool					_firstChunk;
	bool					_allChunksSent;
	bool					_lastChunkSent;
	bool					_firstChunkSent;

public:
	explicit postRequest(request *baseRequest, serverConf &serverConfig, int serverIndex);
	~postRequest();


	void parseBoundary(std::string &data);
	void parseContentLength(std::string &data);
	void parseDataType();
	void parseCgi(std::string &data);
	void parseFileName();
	bool checkIfCgi();
	void parseFileType();
	void parseFileExtension();
	std::string parseValue(std::string search, std::string source, std::string delimeter);

	void writeBinaryToFile(std::vector<uint8_t> &data);
	void parseFirstChunk(std::vector<uint8_t> &data);

	void	cutLastChunk(std::vector<uint8_t> &lastChunk, std::string startBoundary);

	void handleFirstChunk(std::vector<uint8_t> &data);
	void handleMiddleChunk(std::vector<uint8_t> &data);
	void handleLastChunk(std::vector<uint8_t> &data);

	std::vector<uint8_t>::iterator findEndBoundary(std::vector<uint8_t>& lastChunk, const std::vector<uint8_t>& boundary);
	void checkLastChunk(std::vector<uint8_t> &lastChunk, std::string startBoundary);



	int checkPostrequest();

	std::string getBoundary();
	std::string getFileType();
	std::string getFileExtension();
	int getContentLength();
	bool getMultiFormData();
	bool getTextData();
	bool getAllChunksSent();

	void printPostRequest();

	class postException: public std::exception
	{
	public:
		virtual const char *what() const throw();
	};
};

#endif





