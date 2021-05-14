#include <iostream>
#include <string>
#include <vector>
#include <boost/program_options.hpp>
#include <opencv2/opencv.hpp>

#include "protocols/basic_protocol.hpp"
#include "sender/sender_socket.hpp"
#include "video/video_capture.hpp"

using video_streaming::BasicProtocolData;
using video_streaming::SenderSocket;
using video_streaming::VideoCapture;

int main(int argc, char** argv) {
    try{
        std::string address;
        int port;
        std::string filename;
        float scale;

        namespace po = boost::program_options;
        po::options_description description("Display discription");

        description.add_options()
            ("address,a", po::value<std::string>(&address)->default_value("127.0.0.1"), "IP Address")
            ("port,p",po::value<int>(&port)->required(), "Port Number")
            ("file,f",po::value<std::string>(&filename)->default_value(""), "Video for steaming")
            ("scale,s",po::value<float>(&scale)->default_value(1.0), "Scale down the original video")
            ("help,h", "Display this help message");
        
        po::variables_map vm;
        po::store(po::command_line_parser(argc, argv).options(description).run(), vm);
        po::notify(vm);

        if(vm.count("help")){
            std::cout << description;
        }

        if (vm.count("address") && vm.count("port")) {
            std::cout << "Sending to " << address << ":" << port << ".\n";

            const SenderSocket socket(address, port);

            VideoCapture video_capture(false, scale, filename);
            BasicProtocolData protocol_data;
            while (true) {  // TODO: break out cleanly when done.
                protocol_data.SetImage(video_capture.GetFrameFromCamera());
                socket.SendPacket(protocol_data.PackageData());
            }
        }

        return EXIT_SUCCESS;
    }catch(std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
}


