#pragma once
#ifndef __PNP_
#define __PNP_
#include <opencv2/opencv.hpp>
#include "ovinference.h"
#include "inference.h"
class PNPSolver{
public:

    explicit PNPSolver(const std::string& file_path):file_path_(file_path){

        cv::FileStorage fs(file_path_, cv::FileStorage::READ);
        if(!fs.isOpened()){
            std::cout<<"[ERROR ] open camera params file failed! "<<std::endl;
            exit(-1);
        }

        fs["camera_matrix"]>>K_;
        fs["distortion_coefficients"]>>D_;
        fs.release();
        std::cout<<"k "<<K_<<std::endl;
        std::cout<<"d "<<D_<<std::endl;
        cx = K_.at<double>(0, 2);
        cy = K_.at<double>(1, 2);
        fx = K_.at<double>(0, 0);
        fy = K_.at<double>(1, 1);
    }
    double inline get_distance(const cv::Point2f& p1, const cv::Point2f& p2){
        return sqrt(pow((p1.x-p2.x),2)+pow((p1.y-p2.y),2));
    }



    inline cv::Point3f get_cam_point(const ArmorObject& obj){
        if(obj.cls<=0)
            return cv::Point3f (0,0,0);
        object_corners.clear();
        image_points.clear();
        cv::Point3f tmp_point;
        double w = get_distance(obj.apex[0],obj.apex[3]);
        double h = get_distance(obj.apex[0],obj.apex[1]);
        double wh_rate = w/h;
//        std::cout<<"wh rate "<<wh_rate<<"\n";

        // big armor
        //todo infantry3
        if(obj.cls==1 || wh_rate>3.3){
            std::cout<<"big armor!! \n";
            for (int i = 0; i < 4; i++){
                tmp_point = {armor_big_pt[i][0],armor_big_pt[i][1],0};
                object_corners.push_back(tmp_point);
            }
        }else{
            for(int i=0; i<4; i++){
                tmp_point = {armor_small_pt[i][0],armor_small_pt[i][1], 0};
                object_corners.push_back(tmp_point);
            }
        }
        image_points = {obj.apex[0],obj.apex[1],obj.apex[2],obj.apex[3]};

        cv::Mat rvec, tvec;
        cv::solvePnP(cv::Mat(object_corners), cv::Mat(image_points), K_, D_, rvec, tvec, false);
        double X = tvec.at<double>(2, 0);
        double Y = -tvec.at<double>(0, 0);
        double Z = -tvec.at<double>(1, 0);
        return cv::Point3f(X, Y, Z);

    }

    cv::Point2f cam2pixel(cv::Point3f camPoint) {
        float pixel_x = (-camPoint.y * fx / camPoint.x + cx);
        float pixel_y = (-camPoint.z * fy / camPoint.x + cy);
        return cv::Point2f(pixel_x, pixel_y);
    }



private:

    cv::Mat K_;
    cv::Mat D_;
    const std::string& file_path_;
    double fx,fy,cx,cy;
    std::vector<cv::Point3f> object_corners;
    std::vector<cv::Point2f> image_points;
    const float armor_small_pt[4][2] = {
        -0.0675, 0.0275,
        -0.0675, -0.0275,
        0.0675, -0.0275,
        0.0675, 0.0275,
    };

    const float armor_big_pt[4][2] = {
        -0.1125, 0.0275,    //lu
        -0.1125, -0.0275,    //上
        0.1125, -0.0275,   //左
        0.1125, 0.0275,   //下
    };

    

};





#endif
