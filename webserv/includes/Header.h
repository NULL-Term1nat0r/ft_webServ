/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Header.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: estruckm <estruckm@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 15:16:52 by estruckm          #+#    #+#             */
/*   Updated: 2023/10/03 15:16:52 by estruckm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEADER_HPP
#define HEADER_HPP

#include <string>
#include <iostream>
#include <cstring>
#include <vector>
#include <map>
#include <sstream>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <limits>
#include <cfloat>
#include <csignal>
#include <cstdlib>

#include <sys/types.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <poll.h>
#include <dirent.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>


#include "ft_utils.hpp"


#define SERVER_PORT 8080
#define MAX_CLIENTS 10

#define red "\033[1;31m"
#define green "\033[1;32m"
#define yellow "\033[1;33m"
#define blue "\033[1;34m"
#define reset "\033[0m"





#include "parsing.hpp"
#include "serverConf.hpp"

#endif