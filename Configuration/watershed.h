//
//  watershed.hpp
//  MultiShotsRepeat
//
//  Created by Dougalas Michael on 16/4/29.
//  Copyright © 2016年 Johnson. All rights reserved.
//

#ifndef _WATERSHED_H
#define _WATERSHED_H

#include <iostream>
#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

class WatershedSegmenter{
private:
    cv::Mat markers;
public:
    void setMarkers(cv::Mat& markerImage)
    {
        markerImage.convertTo(markers, CV_32S);
    }
    
    cv::Mat process(cv::Mat &image)
    {
        cv::watershed(image, markers);
        markers.convertTo(markers,CV_8U);
        return markers;
    }
};

#endif /* watershed_hpp */
