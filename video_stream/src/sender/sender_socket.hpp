// The SenderSocket class

#ifndef SRC_SENDER_SENDER_SOCKET_HPP_
#define SRC_SENDER_SENDER_SOCKET_HPP_

#include <arpa/inet.h>

#include <string>
#include <vector>

namespace video_streaming {

class SenderSocket {
public:
    SenderSocket(const std::string &receiver_ip, const int receiver_port);

    // TODO: add destructor to clear the socket
    // close(fd);

    void SendPacket(const std::vector<unsigned char> &data) const;

private:
    // The socket identifier (handle).
    int socket_handle_;

    // The struct that contains the receiver's address and port. This is set up
    // in the constructor.
    sockaddr_in receiver_addr_;
};  // SenderSocket

} // namespace video_streaming

#endif // SRC_SENDER_SENDER_SOCKET_HPP_
