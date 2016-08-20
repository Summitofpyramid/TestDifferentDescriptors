//
//  BGsubs.cpp
//  FosteringCode
//
//  Created by Dougalas Michael on 16/3/31.
//  Copyright © 2016年 Johnson. All rights reserved.
//

#include "BGsubs.h"
#include "string"
using namespace cv;
using namespace std;


Mat BGsubs::readimg(string path)
{
    image = imread( path);
    return image;
    
    
}


 Mat BGsubs::computeFG(const Mat &img)
{
    
    cvtColor(img, img, CV_BGR2HSV);
    Rect rectangle(x, y, img.cols,img.rows);
    // GrabCut segmentation
    cv::grabCut(img,    // input image
            result,   // segmentation result
            rectangle,// rectangle containing foreground
            bgModel,fgModel, // models
            1,        // number of iterations
            cv::GC_INIT_WITH_RECT); // use rectangle
    
    compare(result,cv::GC_PR_FGD,result,cv::CMP_EQ);
    // Generate output image
    cv::Mat foreground(img.size(),CV_8UC3,cv::Scalar(255,255,255));
    
    
    img.copyTo(foreground,result);
    // draw rectangle on original image
   // cv::rectangle(image, rectangle, cv::Scalar(255,255,255),1);
    
    return foreground;
    
    /*
    cv::namedWindow("Image");
    cv::imshow("Image",image);
    
    // display result
    cv::namedWindow("Segmented Image");
    cv::imshow("Segmented Image",foreground);
    
    
    
    waitKey();
     */

    
    
}


