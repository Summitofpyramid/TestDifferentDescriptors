//
//  gauss.hpp
//  Configuration
//
//  Created by Dougalas Michael on 16/4/23.
//  Copyright © 2016年 Johnson. All rights reserved.
//THis code is to test the gauss weight on the image based on the 5-slide mode HSV feature mode(the one wrote by Mehdi)

#ifndef _GAUSS_H_
#define _GAUSS_H_

#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/core/core.hpp>
#include<iostream>
#include<algorithm>
#include<math.h>
#include <iostream>
#include "descriptor.h"

using namespace cv;
using namespace std;

#define E  2.71828
#define PI 3.1415926

class weightImg:public Descriptor{
    private:
   
    
    void MyLine( Mat img, Point start, Point end )
    {
        int thickness = 2;
        int lineType = 8;
        line( img,
             start,
             end,
             Scalar( 0, 0, 255 ),
             thickness,
             lineType );
    }

    
    public:
    int axis_H,axis_V;
    Mat HSVVector(Mat);
    Mat getWhsvFeature(Mat);
    int HorizonAxis( const Mat &);
    int VerticalAxis(const Mat &);
    void DrawAxis(const Mat &img);
    Mat GaussianBlur(Mat img,int axis);
    float bhattacharyya(Mat,Mat);
   

    double distance(const Mat &probeImage, const Mat &galleryImage){
        
        Mat m1 = getWhsvFeature(probeImage);
        Mat m2 = getWhsvFeature(galleryImage);
        
        return bhattacharyya(m1, m2);
  
    }
};


#endif /* gauss_h */
