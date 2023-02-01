//
// Created by quonone on 23-1-31.
//

#ifndef ARES_CV_CAMERA_H
#define ARES_CV_CAMERA_H

#include <iostream>
#include <memory>
#include <opencv2/opencv.hpp>
#include "Tictoc.hpp"
#include "../include/camera_api.h"
class Camera{
public:
    explicit Camera(const char* SN, const int width, const int height);
    ~Camera();
    std::deque<std::pair<double, cv::Mat>> raw_image_pub;
    void camera_stream_thread();

private:
    const char *cameraSN_;
    const int image_width, image_height;
    int fps, fps_count;
    bool if_time_set = false;
    std::mutex cam_lock;
    camera_config cam0_info;
    std::chrono::time_point<std::chrono::steady_clock> start_time;
    char *origin_buff ;
    std::unique_ptr<MercureDriver> cam0;
    PGX_FRAME_BUFFER pFrameBuffer;
    std::shared_ptr<Tictok> tic;
};
#endif //ARES_CV_CAMERA_H