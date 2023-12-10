/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd_utils.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: estruckm <estruckm@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/10 20:28:16 by estruckm          #+#    #+#             */
/*   Updated: 2023/12/10 20:28:21 by estruckm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_utils.hpp"

const char *ft_std::invalid_Range::what() const throw(){
	return "invalid argument or range";
}

//double	ft_std::stod(std::string str){
//	const char* cstr = str.c_str();
//	char *endptr;
//	if (!isdigit(cstr[0]) && cstr[0] != '+' && cstr[0] != '-')
//		throw invalid_Range();
//	if ((cstr[0] == '+' || cstr[0] == '-') && !isdigit(cstr[1]))
//		throw invalid_Range();
//	double result = strtod(cstr, &endptr);
//	if (result == HUGE_VAL || result == -HUGE_VAL)
//		throw invalid_Range();
//	return result;
//}
//
//int		ft_std::stoi(std::string str){
//	const char *cstr = str.c_str();
//	double result;
//	if (!isdigit(cstr[0]) && cstr[0] != '+' && cstr[0] != '-')
//		throw invalid_Range();
//	if ((cstr[0] == '+' || cstr[0] == '-') && !isdigit(cstr[1]))
//		throw invalid_Range();
//	result = ft_std::stod(str);
//	if ((result > INT_MAX || result < INT_MIN))
//		throw invalid_Range();
//	return static_cast<int>(result);
//}
//float 	ft_std::stof(std::string str) {
//	const char *cstr = str.c_str();
//	char *endptr;
//	if (!isdigit(cstr[0]) && cstr[0] != '+' && cstr[0] != '-')
//		throw invalid_Range();
//	if ((cstr[0] == '+' || cstr[0] == '-') && !isdigit(cstr[1]))
//		throw invalid_Range();
//	float result = strtof(cstr, &endptr);
//	if (result == HUGE_VAL || result == -HUGE_VAL)
//		throw invalid_Range();
//	return result;
//}

long ft_std::stringToLong(const std::string& str) {
	std::istringstream iss(str);
	long result;
	iss >> result;
	return result;
}


std::string ft_std::to_string(long value) {
	std::stringstream ss;
	ss << value;
	return ss.str();
}

std::string ft_std::to_string(int value) {
	std::stringstream ss;
	ss << value;
	return ss.str();
}