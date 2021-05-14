#include <iostream>
#include <string>
#include <sys/socket.h> 
#include <sys/types.h>
#include <stdlib.h> 
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <boost/program_options.hpp>

#define BUFFER_SIZE 1024

void connect(std::string addr, int portno){
    int sockfd, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[BUFFER_SIZE];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0){
        perror("ERROR opening socket");
        exit(EXIT_FAILURE);
    }
    server = gethostbyname(addr.c_str());
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0){
        perror("ERROR connecting");
        exit(EXIT_FAILURE);
    }
    printf("Please enter the message: ");
    bzero(buffer,BUFFER_SIZE);
    fgets(buffer,BUFFER_SIZE,stdin);
    n = write(sockfd,buffer,strlen(buffer));
    if (n < 0){
        perror("ERROR writing to socket");
        exit(EXIT_FAILURE);
    }
    bzero(buffer,BUFFER_SIZE);
    n = read(sockfd,buffer,BUFFER_SIZE);
    if (n < 0) {
         perror("ERROR reading from socket");
         exit(EXIT_FAILURE);
    }
    printf("%s\n",buffer);
    close(sockfd);
}

int main(int argc, char *argv[]){
    try{
        std::string address;
        int port;

        namespace po = boost::program_options;
        po::options_description description("Display discription");

        description.add_options()
            ("address,a", po::value<std::string>(&address)->required(), "IP Address")
            ("port,p",po::value<int>(&port)->required(), "Port Number")
            ("help,h", "Display this help message");
        
        po::variables_map vm;
        po::store(po::command_line_parser(argc, argv).options(description).run(), vm);
        po::notify(vm);


        if(vm.count("help")){
            std::cout << description;
        }

        if (vm.count("address") && vm.count("port")) {
            std::cout << address << ":" << port << "\n";

            connect(address, port);
        } 

        
        return EXIT_SUCCESS;
    }catch(std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
}