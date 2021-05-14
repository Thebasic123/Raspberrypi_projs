#include <iostream>
#include <string>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> 
#include <netdb.h>
#include <boost/program_options.hpp>

#define BUFFER_SIZE 1024

void connect(int portno){
    int sockfd; /* socket */
    int clientlen; /* byte size of client's address */
    struct sockaddr_in serveraddr; /* server's addr */
    struct sockaddr_in clientaddr; /* client addr */
    struct hostent *hostp; /* client host info */
    char buf[BUFFER_SIZE]; /* message buf */
    char *hostaddrp; /* dotted decimal host addr string */
    int optval; /* flag value for setsockopt */
    int n; /* message byte size */

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0){
        perror("ERROR opening socket");
        exit(EXIT_FAILURE);
    }

    /* setsockopt: Handy debugging trick that lets 
     * us rerun the server immediately after we kill it; 
     * otherwise we have to wait about 20 secs. 
     * Eliminates "ERROR on binding: Address already in use" error. 
     */
    optval = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (const void *)&optval , sizeof(int));

    /*
     * build the server's Internet address
     */
    bzero((char *) &serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons((unsigned short)portno);

    /* 
     * bind: associate the parent socket with a port 
     */
    if (bind(sockfd, (struct sockaddr *) &serveraddr, sizeof(serveraddr)) < 0){
        perror("ERROR on binding");
        exit(EXIT_FAILURE);
    }

    /* 
     * main loop: wait for a datagram, then echo it
     */
    clientlen = sizeof(clientaddr);
    while (1) {
        /*
         * recvfrom: receive a UDP datagram from a client
         */
        bzero(buf, BUFFER_SIZE);
        n = recvfrom(sockfd, buf, BUFFER_SIZE, 0, (struct sockaddr *) &clientaddr, (socklen_t *)&clientlen);
        if (n < 0){
            perror("ERROR in recvfrom");
            exit(EXIT_FAILURE);
        }

        /* 
         * gethostbyaddr: determine who sent the datagram
         */
        hostp = gethostbyaddr((const char *)&clientaddr.sin_addr.s_addr, sizeof(clientaddr.sin_addr.s_addr), AF_INET);
        if (hostp == NULL){
            perror("ERROR on gethostbyaddr");
            exit(EXIT_FAILURE);
        }
        hostaddrp = inet_ntoa(clientaddr.sin_addr);
        if (hostaddrp == NULL){
            perror("ERROR on inet_ntoa\n");
            exit(EXIT_FAILURE);
        }
        printf("server received datagram from %s (%s)\n", hostp->h_name, hostaddrp);
        printf("server received %d/%d bytes: %s\n", (int)strlen(buf), n, buf);
        
        /* 
         * sendto: echo the input back to the client 
         */
        n = sendto(sockfd, buf, strlen(buf), 0, (struct sockaddr *) &clientaddr, clientlen);
        if (n < 0){
            perror("ERROR in sendto");
            exit(EXIT_FAILURE);
        }
    }
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