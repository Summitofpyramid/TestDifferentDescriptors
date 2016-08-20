#ifndef _HISTDESCRIPTOR_H
#define _HISTDESCRIPTOR_H

#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/core/core.hpp>
#include<iostream>
#include<algorithm>
#include<math.h>
#include <iostream>
#include "descriptor.h"
#include "watershed.h"
#include "gauss.h"
//#include "BGsubs.h"

using namespace cv;
using namespace std;

#define E  2.71828
#define PI 3.1415926

class DescriptorY : public Descriptor {
    
public:
    //-------this part is for the segmentation
    int x=1,y=1; // the two points of the rectangle
    Mat result; // segmentation result (4 possible values)
    Mat bgModel,fgModel; // the models (internally used)
    // -----------------------------------------
   
    Mat src, combo1,combo2,HistCombo,b_hist, g_hist, r_hist;
    string probeID, testID; //testID is the certain one image's ID
    double a, b, c;
    float SimScore;
    
    int width, height, binWidth, binHeight,binLocx,binLocy;
    
        
    vector<string> file_list;
    vector<Mat> bgr_planes;
    vector <Mat> descriptor; // a vector of computed image hist
    Mat histImage;
    
    
    Mat ImageHistCompute(const Mat &image);
    Mat HSVVector(Mat img);
    Mat getWhsvFeature(Mat img);
    Mat Segment(Mat image);
//

    virtual double distance(const Mat &probeImage, const Mat &galleryImage)
    {
        Mat desc1, desc2;
        desc1 = ImageHistCompute(probeImage);
        desc2 = ImageHistCompute(galleryImage);
        return bhattacharyya(desc1, desc2);
    }
};




#endif