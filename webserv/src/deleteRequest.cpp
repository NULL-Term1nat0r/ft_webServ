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

deleteRequest::deleteRequest(request *baseRequest) : _baseRequest(baseRequest){
	statusCode = 204;
}

deleteRequest::~deleteRequest() {}

const char	*deleteRequest::deleteRequestInvalidFileException::what() const throw() {
	return "Couldn't remove file\n";
}

void deleteRequest::deleteFile(){

	if (remove(("html_files" + _baseRequest->getStringURL()).c_str()) != 0) {
		statusCode = 404;
		throw deleteRequestInvalidFileException();
	}

}