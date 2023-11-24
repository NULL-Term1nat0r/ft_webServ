/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deleteRequest.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: estruckm <estruckm@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 14:17:32 by estruckm          #+#    #+#             */
/*   Updated: 2023/10/24 20:25:26 by estruckm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/deleteRequest.hpp"

deleteRequest::deleteRequest(std::vector<uint8_t> &requestContainer, serverConf &serverConfig, int serverIndex) : request(requestContainer, serverConfig, serverIndex), _serverConfig(serverConfig), serverIndex(serverIndex){
}

deleteRequest::~deleteRequest() {}

const char	*deleteRequest::deleteRequestInvalidFileException::what() const throw() {
	return "Couldn't remove file\n";
}

void deleteRequest::deleteFile(){

	std::string deletePath = "html_files" + getStringURL();
	std::cout << "hopped into delete request\n";
	if (remove(deletePath.c_str()) != 0)
		throw deleteRequestInvalidFileException();
}