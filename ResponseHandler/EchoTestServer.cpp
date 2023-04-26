#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sstream>
#include "Response.hpp"
#include "ResponseHandler.hpp"

using namespace std;

const int PORT = 1815;
const int BUFFER_SIZE = 1024;

void responseTestSetting(Response &response)
{
	response.setStatusCode("201");
	response.setBody("this is test Body message.");
	response.setLocation("/testLocation"); //for redirection
	// response.setCookie("session_id", "johnson");
	response.setCookie("id", "johnson");
	response.setCookie("pw", "abcd1234");
}

int main() {
    Response response;
    responseTestSetting(response);
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};

    // Create socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Set socket options
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    // Bind socket to port
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for connections
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    while(1)
    {
        // Accept incoming connections
        if ((new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        // Receive data from client
        valread = read(new_socket, buffer, BUFFER_SIZE);
        printf("request message===============================\n");
        printf("%s\n", buffer);

        std::string responseMessage = ResponseHandler::createResponseMessage(response);

        send(new_socket, responseMessage.c_str(), strlen(responseMessage.c_str()), 0);
        std::cout << "responseMessage===============================" << std::endl;
        std::cout << responseMessage << std::endl;
        
        printf("message sent==========================\n");
    }
    // close(new_socket);
    return 0;
}

//c++ -Wall -Wextra -Werror -std=c++98 EchoTestServer.cpp Response.cpp ResponseHandler.cpp StatusText.cpp -o echoTestServer