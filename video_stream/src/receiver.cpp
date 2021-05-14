// This program opens a socket that listens to incoming UDP packets. When a
// video frame packet is received, it will be decoded and displayed in a GUI
// window.

#include <iostream>
#include <string>
#include <vector>
#include <boost/program_options.hpp>

#include "protocols/basic_protocol.hpp"
#include "receiver/receiver_socket.hpp"

using video_streaming::BasicProtocolData;
using video_streaming::ReceiverSocket;

int main(int argc, char** argv) {
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
            const ReceiverSocket socket(port);
            if (!socket.BindSocketToListen()) {
                std::cerr << "Could not bind socket.\n";
                exit(EXIT_FAILURE);
            }
            std::cout << "Listening on port " << port << ".\n";
            
            BasicProtocolData protocol_data;
            while (true) {  // TODO: break out cleanly when done.
                protocol_data.UnpackData(socket.GetPacket());
                protocol_data.GetImage().Display();
            }
        }

        return EXIT_SUCCESS;
    }catch(std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

}






