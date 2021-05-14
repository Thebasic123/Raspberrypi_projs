#include <iostream>
#include <string>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <boost/program_options.hpp>

#define BUFFER_SIZE 1024

void connect(int portno){
    int sockfd, newsockfd;
    socklen_t clilen;
    char buffer[BUFFER_SIZE];
    struct sockaddr_in serv_addr, cli_addr;
    int n;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0) {
        perror("ERROR opening socket");
        exit(EXIT_FAILURE);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        perror("ERROR on binding");
        exit(EXIT_FAILURE);
    }
    listen(sockfd,5);
    clilen = sizeof(cli_addr);
    newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);

    if (newsockfd < 0) {
        perror("ERROR on accept");
        exit(EXIT_FAILURE);   
    }

    bzero(buffer,BUFFER_SIZE);
    n = read(newsockfd, buffer, BUFFER_SIZE);
    if (n < 0){
        perror("ERROR reading from socket");
        exit(EXIT_FAILURE);  
    } 
    printf("Here is the message: %s\n",buffer);
    n = write(newsockfd,"I got your message",18);
    
    if (n < 0) {
        perror("ERROR writing to socket");
        exit(EXIT_FAILURE);  
    }
    close(newsockfd);
    close(sockfd);
}


int main(int argc, char *argv[]){
    try{
        int port;
        namespace po = boost::program_options;

        po::options_description description("Display discription");

        description.add_options()
            ("port,p",po::value<int>(&port)->required(), "Port Number")
            ("help,h", "Display this help message");
        
        po::variables_map vm;
        po::store(po::command_line_parser(argc, argv).options(description).run(), vm);
        po::notify(vm);


        if(vm.count("help")){
            std::cout << description;
        }

        if (vm.count("port")) {
            std::cout << port << "\n";
            connect(port);
        } 

        
        return EXIT_SUCCESS;
    }catch(std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
}