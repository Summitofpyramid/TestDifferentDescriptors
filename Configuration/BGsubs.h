//
//  BGsubs.h
//  FosteringCode
//
//  Created by Dougalas Michael on 16/3/31.
//  Copyright © 2016年 Johnson. All rights reserved.
//This code is to use grabCut function to segment the foreground



#ifndef BGSUBS_H
#define BGSUBS_H

#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/core/core.hpp>
#include<iostream>
#include<algorithm>
#include<math.h>
#include<limits>

using namespace std;
using namespace cv;

class BGsubs
{
private:
    
    Mat image;
    int x=1,y=1; // the two points of the rectangle
    Mat result; // segmentation result (4 possible values)
    Mat bgModel,fgModel; // the models (internally used)
public:
    Mat readimg(string);
    Mat computeFG(const Mat &img);
   

};



#endif /* BGsubs_hpp */
