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
    int serverlen;
    struct sockaddr_in serveraddr;
    struct hostent *server;
    char buf[BUFFER_SIZE];

    /* socket: create the socket */
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("ERROR opening socket");
        exit(EXIT_FAILURE);
    }

    /* gethostbyname: get the server's DNS entry */
    server = gethostbyname(addr.c_str());
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host as %s\n", addr.c_str());
        exit(0);
    }

    /* build the server's Internet address */
    bzero((char *) &serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serveraddr.sin_addr.s_addr, server->h_length);
    serveraddr.sin_port = htons(portno);

    /* get a message from the user */
    bzero(buf, BUFFER_SIZE);
    printf("Please enter msg: ");
    fgets(buf, BUFFER_SIZE, stdin);

    /* send the message to the server */
    serverlen = sizeof(serveraddr);
    n = sendto(sockfd, buf, strlen(buf), 0, (const struct sockaddr *) &serveraddr, serverlen);
    if (n < 0) {
        perror("ERROR in sendto");
        exit(EXIT_FAILURE);
    }
    
    /* print the server's reply */
    n = recvfrom(sockfd, buf, strlen(buf), 0, (struct sockaddr *) &serveraddr, (socklen_t *)&serverlen);
    if (n < 0) {
        perror("ERROR in recvfrom");
        exit(EXIT_FAILURE);
    }
    printf("Echo from server: %s", buf);

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