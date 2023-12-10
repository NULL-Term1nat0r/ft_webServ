/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: estruckm <estruckm@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/10 20:29:28 by estruckm          #+#    #+#             */
/*   Updated: 2023/12/10 20:29:36 by estruckm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <string>
#include <limits>
#include <climits>
#include <cfloat>
#include <cstdlib>
#include <cmath>
#include <sstream>


class ft_std
{
public:
	class invalid_Range : public std::exception {
		virtual const char *what() const throw();
	};
//	static double	stod(std::string);
//	static int		stoi(std::string);
//	static float 	stof(std::string);
	static long stringToLong(const std::string& str);
	static std::string to_string(long value);
	static std::string to_string(int value);
};