#ifndef SRC_VIDEO_VIDEO_FRAME_HPP_
#define SRC_VIDEO_VIDEO_FRAME_HPP_

#include <vector>

#include <opencv2/opencv.hpp>

namespace video_streaming {

class VideoFrame{
public:
    VideoFrame() {}

    explicit VideoFrame(const cv::Mat& image) : frame_image_(image) {}

    explicit VideoFrame(const std::vector<unsigned char> frame_bytes);

    void Display() const;
    
    std::vector<unsigned char> GetJPEG() const;

private:
    cv::Mat frame_image_;

};

} // namespace video_streaming

#endif
