/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkroger <jkroger@student.42wolfsburg.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 15:13:02 by estruckm          #+#    #+#             */
/*   Updated: 2023/12/11 00:17:29 by jkroger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/server.hpp"

server::server(serverConf &serverConf, int serverIndex) : serverConfig(serverConf), serverIndex(serverIndex) {
	std::cout << "server " << serverIndex << " constructor for port: " << serverConfig._server[serverIndex].port << " got called\n" << std::endl;
	try {
		createServerSocket();
	}
	catch (std::exception &e){
		std::cout << "caught exception of server" << std::endl;
	}
}

server::~server() {
	for (size_t i = 0; i < pollEvents.size(); i++) {
		close(pollEvents[i].fd);
	}
	for (size_t i = 0; i < clients.size(); i++) {
		clients[i]->~client();
		delete clients[i];
	}

	//close(serverSocket);
}

void server::createServerSocket() {
	struct sockaddr_in serverAddress;
	memset(&serverAddress, 0, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(serverConfig._server[serverIndex].port);
	serverAddress.sin_addr.s_addr = INADDR_ANY;

	this->serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (this->serverSocket < 0)
		throw std::runtime_error("Socket creation failed");

	int opt = 1;
	if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
	{
		close(this->serverSocket);
		throw std::runtime_error("setsockopt");
	}
	if (bind(this->serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
		throw std::runtime_error("Socket binding failed");

	if (listen(this->serverSocket, 5) < 0)
		throw std::runtime_error("Socket listening failed");
	addSocket(this->serverSocket);
}

void server::addSocket(int clientSocket) {
	struct pollfd _pollfd;
	_pollfd.fd = clientSocket;
	_pollfd.events = POLLIN;
	fcntl(_pollfd.fd, F_SETFL, O_NONBLOCK, FD_CLOEXEC);
	pollEvents.push_back(_pollfd);
	client *newClient = new client(clientSocket, serverConfig, serverIndex);
	clients.push_back(newClient);
	clientTimeouts.push_back(time(NULL));
}

void server::removeSocket(int index) {
	close(pollEvents[index].fd);
	pollEvents.erase(pollEvents.begin() + index);
	delete(clients[index]);
	clients.erase(clients.begin() + index);
}

void server::updateClientRequests(int clientSocket, request *newRequest) {
	clientRequests[clientSocket] = newRequest;
}

void server::updateClientResponses(int clientSocket, response *newResponse) {
	clientResponses[clientSocket] = newResponse;
}

request *server::returnRequestClass(int clientSocket) {
	if (clientRequests.find(clientSocket) != clientRequests.end())
		return clientRequests[clientSocket];
	return NULL;
}

response *server::returnResponseClass(int clientSocket) {
	if (clientResponses.find(clientSocket) != clientResponses.end())
		return clientResponses[clientSocket];
	return NULL;
}

void server::handleNewConnection() {
	int clientSocket;
	struct sockaddr_in clientAddress;
	socklen_t clientAddressSize = sizeof(clientAddress);

	clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddress, &clientAddressSize);
	if (clientSocket < 0) {
		throw std::runtime_error("Socket accept failed");
	}

//	fcntl(clientSocket, F_SETFL, O_NONBLOCK);
	addSocket(clientSocket);
}

void server::serverRoutine() {

//	poll(_pollFds, POLLFD_LIMIT, POLL_TIMEOUT) < 0

	int num_ready = poll(&pollEvents[0], pollEvents.size(), 0);
	if (num_ready < 0) {
		std::cout << "num_ready smaller 0 " << std::endl;
		//throw pollNotWorking();
	}
	for (size_t i = 0; i < pollEvents.size(); ++i) {
		try{
			if (pollEvents[i].revents == POLLIN) {
				if (pollEvents[i].fd == serverSocket) {
					handleNewConnection();
				} else {
					clients[i]->executeClientRequest();
				}
			}
			if (pollEvents[i].revents == 0 && clients[i]->clientResponse != NULL) {
				try {
					clients[i]->executeClientResponse();
					if (clients[i]->clientResponse->_allChunkSent) {
						removeSocket(static_cast<int>(i));
					}
				}//pollEvents[i].revents == 0 &&
				catch (std::exception &e){
					std::cout << "caught exception of server" << e.what() <<  std::endl;
				}
				// if (clients[i].clientResponse->_allChunkSent) {
				// 	removeSocket(static_cast<int>(i));
				// }
			}
		}
		catch (std::exception &e){
			// clients[i]->executeClientResponse();
			std::cout << "caught exception of server" << e.what() <<  std::endl;
		}
//		std::cout << "if (time: " << time(NULL) << "- socketTimeouts[" << i << "]: " << socketTimeouts[i] << " > serv._clienttimeout: " << serv._clientTimeout << std::endl;
// 		if (time(NULL) - clients[i].lastActivity > serverConfig._clientTimeout) {
// 			if (clients[i].clientSocket != serverSocket) {
// //				std::cout << "actual time: " << time(NULL) << " - lastActivity: " << clients[i].lastActivity << " > serverConfig._clientTimeout: " << serverConfig._clientTimeout << std::endl;
// //				std::cout << "client: " << i << " timed out" << std::endl;
// 				removeSocket(static_cast<int>(i));
// 			}

// 		}
	}
}

bool server::exitServer = false;

void sigintHandler(int sigNum)
{
	if (sigNum == SIGINT) {
		std::cout << "webserver closed" << std::endl;
		server::exitServer = true;
	}
}

void	getSignals() {
	signal(SIGINT, sigintHandler);
}


void server::runAllServers(char *configFilePath) {
	Config conf = Config();

	conf.parseConfFile(configFilePath);
	serverConf serverConfigs(conf);
	std::vector<server *> servers;
	for (int i = 0; i < (int)serverConfigs._server.size(); i++) {
		try{
			server *serverClass = new server(serverConfigs, i);
//			for(std::map<std::string, LocationStruc>::iterator it = serverClass->serverConfig._server[0].locations.begin(); it != serverClass->serverConfig._server[0].locations.end(); it++){
//				std::cout << "location = " << it->first << std::endl;
//			}
			servers.push_back(serverClass);
		}
		catch (std::exception &e){
			std::cout << "caught exception of server" << e.what() <<  std::endl;
		}
	}

	getSignals();
	while (!exitServer) {
		for (int i = 0; static_cast<size_t>(i) < servers.size(); i++){
			servers[i]->serverRoutine();
		}
	}
	sleep(1);
	for (size_t i = 0; i < servers.size(); i++) {
		delete servers[i];
	}
}




serverConf& server::getServerConfig(){
	return this->serverConfig;
}

int server::getServerIndex(){
	return this->serverIndex;
}


