// The basic protocol transmits raw, uncompressed video frames only. This is
// the most basic video protocol.

#ifndef SRC_PROTOCOLS_BASIC_PROTOCOL_HPP_
#define SRC_PROTOCOLS_BASIC_PROTOCOL_HPP_

#include <vector>

#include "protocols/protocol.hpp"
#include "video/video_frame.hpp"

namespace video_streaming {

class BasicProtocolData : public ProtocolData {
public:
    std::vector<unsigned char> PackageData() const override;

    void UnpackData(const std::vector<unsigned char>& raw_bytes) override;

    // Sets the next video frame.
    void SetImage(const VideoFrame& image) {
        video_frame_ = image;
    }

    // Returns the video frame image.
    VideoFrame GetImage() const {
        return video_frame_;
    }

private:
    // The video frame received from the packet is stored here.
    VideoFrame video_frame_;
};

} // namespace video_streaming

#endif // SRC_PROTOCOLS_BASIC_PROTOCOL_HPP_