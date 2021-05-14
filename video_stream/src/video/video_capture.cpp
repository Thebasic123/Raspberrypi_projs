#include "video/video_capture.hpp"

#include <iostream>
#include <vector>

#include <opencv2/opencv.hpp>

namespace video_streaming {

VideoCapture::VideoCapture(const bool show_video, const float scale, const std::string filename)
    : show_video_(show_video)
    , scale_(scale)
    , capture_(cv::VideoCapture(filename))
{
}




VideoFrame VideoCapture::GetFrameFromCamera() {
    if (!capture_.isOpened()) {
        std::cerr << "Could not get frame. Camera or video not available." << std::endl;
        exit(EXIT_FAILURE);
    }
    cv::Mat image;
    capture_ >> image;
    // If the image is being downsampled, resize it first.
    if (scale_ < 1.0) {
        cv::resize(image, image, cv::Size(0, 0), scale_, scale_);
    }
    VideoFrame video_frame(image);
    if (show_video_) {
        video_frame.Display();
    }
    return video_frame;
}

}  // namespace video_streaming