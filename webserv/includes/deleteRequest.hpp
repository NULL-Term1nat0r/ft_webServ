/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deletedeleteRequest.hpp                                    :+:      :+:    :+:   */
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
/*   deleteRequest.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: estruckm <estruckm@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 14:32:29 by estruckm          #+#    #+#             */
/*   Updated: 2023/10/03 14:42:48 by estruckm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef deleteRequest_HPP
#define deleteRequest_HPP

#include "Header.h"
#include "request.hpp"


class deleteRequest {
private:
	request *_baseRequest;

public:
	explicit deleteRequest(request *baseRequest);
	~deleteRequest();

	class deleteRequestInvalidFileException : public std::exception
	{
		public:
			virtual const char	*what() const throw();
	};
	int statusCode;

	void deleteFile();

};

#endif


