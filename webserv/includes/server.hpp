/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server1.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: estruckm <estruckm@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 10:02:17 by estruckm          #+#    #+#             */
/*   Updated: 2023/11/13 17:25:27 by estruckm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "Header.h"
//#include "config.hpp"
#include "request.hpp"
#include "postRequest.hpp"
#include "deleteRequest.hpp"
#include "getRequest.hpp"
#include "cgiRequest.hpp"
#include "response.hpp"

class response;

class server {

	private:

	public:
		static bool exitServer;


		server(serverConf &serverConf, int serverIndex);
		~server();

		class client {
			public:

				client(int clientSocket, serverConf &serverConfig, int serverIndex);
				~client();

				void executeClientRequest();
				void executeClientResponse();

				void createNewRequest(std::vector<uint8_t> _request);
				bool checkPostRequest(std::vector<uint8_t> _request);
				bool checkGetRequest();
				bool checkDeleteRequest();
				bool checkCgiRequest();

				void createNewGetRequest(std::vector<uint8_t> _request, request &newRequest);
				void createNewPostRequest(std::vector<uint8_t> _request, request &newRequest);
				void createNewDeleteRequest(std::vector<uint8_t> _request, request &newRequest);
				void createNewCgiRequest(std::vector<uint8_t> _request, request &newRequest);

				void setUpResponse(std::string filePath, int statusCode, request *request);


				int clientSocket;
				serverConf &serverConfig;
				int serverIndex;
				time_t lastActivity;
				getRequest *clientGetRequest;
				postRequest *clientPostRequest;
				deleteRequest *clientDeleteRequest;
				cgiRequest *clientCgiRequest;
				response *clientResponse;
		};

		class pollNotWorking : public std::exception
		{
			public:
				virtual const char	*what() const throw();
		};

		serverConf &serverConfig;
		int serverSocket;
		int serverIndex;
		std::vector<struct pollfd> pollEvents;
		std::vector<client> clients;

		std::vector<long long> clientTimeouts;
		std::map<int, request*> clientRequests;
		std::map<int, response*> clientResponses;

		request *returnRequestClass(int clientSocket);
		response *returnResponseClass(int clientSocket);

		void updateClientRequests(int clientSocket, request *newRequest);
		void updateClientResponses(int clientSocket, response *newResponse);

		void handleNewConnection();

		void addSocket(int clientSocket);
		void removeSocket(int clientSocket);

		void handleClientRequest(int clientSocket);
		void handleClientResponse(int clientSocket);

		void createServerSocket();

		void serverRoutine();

		static void runAllServers(char * configFilePath);

		serverConf& getServerConfig();
		int getServerIndex();
};


#endif