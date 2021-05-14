#include "protocols/basic_protocol.hpp"

#include <vector>

namespace video_streaming {

std::vector<unsigned char> BasicProtocolData::PackageData() const {
    return video_frame_.GetJPEG();
}

void BasicProtocolData::UnpackData(const std::vector<unsigned char>& raw_bytes) {
    video_frame_ = VideoFrame(raw_bytes);
}

}  // namespace video_streaming
