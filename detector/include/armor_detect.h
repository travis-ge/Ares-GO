//
// Created by quonone on 23-1-31.
//

#ifndef ARES_CV_ARMOR_DETECT_H
#define ARES_CV_ARMOR_DETECT_H

#include <opencv2/opencv.hpp>
#include "PNPSolver.hpp"
#include "Tictoc.hpp"
#include "../include/ovinference.h"
#include "../../predictor/include/EKFpredictor.h"
#include "../../predictor/include/predictor.h"
#include "../../serial/serial.h"
#include "anglesolver.hpp"
#include "Message.hpp"
#include "classifier.hpp"
#include "inference.h"

typedef std::pair<double, cv::Mat> Camdata;
extern Publisher<Camdata> cam_publisher;
//extern Publisher<RobotInfo> serial_publisher;
extern std::shared_ptr<SerialPort> serial;
//use for traditional image process
struct LightBar{
    float width;
    float height;
    float angle;
    cv::Point p1;
    cv::Point p2;
};
struct Armor_t{
    cv::Point2f p1;
    cv::Point2f p2;
    cv::Point2f p3;
    cv::Point2f p4;
    cv::Point2f center;
    int id;
};

class ArmorDetect{
public:
    explicit ArmorDetect();
    ~ArmorDetect()=default;

    void run();
private:
    int img_send_cnt = 0;
    std::shared_ptr<PNPSolver> pnpsolver;
    std::unique_ptr<Tictok> tic;
//    std::shared_ptr<OvInference> ovinfer;
    std::shared_ptr<Inference> ovinfer;
//    std::shared_ptr<EKFPredictor> ekfpredictor;
    std::shared_ptr<Predictor> kfpredictor;
    std::shared_ptr<AngleSolver> as;

    cv::Rect tracking_roi;
    int locked_id = 0;
    int lose_cnt = 0;
    int lock_cnt = 0;
    int autoaim_fps;

    double pitch_last,yaw_last,dis_last,id_last;

    void color_check(const char color, std::vector<ArmorObject>& results);
    void armor_sort(ArmorObject& final_obj, std::vector<ArmorObject>& results, cv::Mat& src);
    void draw_target(const ArmorObject& obj, cv::Mat& src);
    bool if_shoot(const cv::Point3f& cam_);

    //tradition
    void detect(cv::Mat& src, cv::Mat& dst, const int team, std::vector<OvInference::Detection>& objs);
    std::shared_ptr<Classifier> num_c;

    int image_empty_cnt;

};
#endif //ARES_CV_ARMOR_DETECT_H
