//
//  gauss.cpp
//  Configuration
//
//  Created by Dougalas Michael on 16/4/23.
//  Copyright © 2016年 Johnson. All rights reserved.
//

#include "gauss.h"

#include "iostream"



Mat weightImg::HSVVector(Mat img) {
    cv::Mat img_hsv, hist, hist_h, hist_s, hist_v;
    cvtColor(img, img_hsv, CV_BGR2HSV);
    // Normalisation ?
    vector<cv::Mat> temp;
    split(img_hsv, temp);
    
    temp[0] = temp[0].reshape(0, 1);
    temp[1] = temp[1].reshape(0, 1);
    temp[2] = temp[2].reshape(0, 1);
    
    // Histogram computation
    float h_ranges[] = { 0, 180 };
    float s_ranges[] = { 0, 256 };
    float v_ranges[] = { 0, 256 };
    
    int histSize_h[] = { 180 };
    int histSize_s[] = { 256 };
    int histSize_v[] = { 256 };
    
    const float * ranges_h[] = { h_ranges };
    const float * ranges_s[] = { s_ranges };
    const float * ranges_v[] = { v_ranges };
    
    int channels[] = { 0 };
    
    calcHist(&temp[0], 1, channels, Mat(), hist_h, 1, histSize_h, ranges_h);
    normalize(hist_h, hist_h, 0, 1, NORM_MINMAX, -1, Mat());
    calcHist(&temp[1], 1, channels, Mat(), hist_s, 1, histSize_s, ranges_s);
    normalize(hist_s, hist_s, 0, 1, NORM_MINMAX, -1, Mat());
    calcHist(&temp[2], 1, channels, Mat(), hist_v, 1, histSize_v, ranges_v);
    normalize(hist_v, hist_v, 0, 1, NORM_MINMAX, -1, Mat());
    
    vconcat(hist_h, hist_s, hist);
    vconcat(hist, hist_v, hist);
    
    return hist;
    
}

cv::Mat weightImg::getWhsvFeature(cv::Mat img)
{
    //-------------------
    int x =HorizonAxis(img);
    int y =VerticalAxis(img);
    img = GaussianBlur(img, y);
    //-------------------
    int offset = img.rows / 5;
    vector<cv::Mat> sub(5);
    
    // Divide the image into 5x1 cells
    for(int i = 0 ; i < 4 ; i++) {
        sub[i] = img.rowRange(offset * i, offset * (i + 1));
    }
    sub[4] = img.rowRange(offset * 4, img.rows);
    // Debug this
    
    cv::Mat conc;
    cv::Mat temp;
    for(int i = 0 ; i < 5 ; i++) {
        cv::Mat HSV = HSVVector(sub[i]);
        if(i == 0) {
            conc = HSV;
        } else {
            vconcat(conc, HSV, conc);
        }
    }
    
    return conc;
}


// compute the horizontal axis
int weightImg::HorizonAxis(const  Mat &img){
    cvtColor(img, img, CV_BGR2HSV);
    vector<double> error;
    int nonzero1=0;
    int nonzero2 = 0;
    int maskHeight = floor(img.rows/4);
    int width = img.cols;
    Mat mask = Mat(maskHeight,width,CV_8UC3);
    for (int row = maskHeight+1;row<img.rows-maskHeight;++row)
    {
        Mat mask1 = img(Range(row-maskHeight+1,row+1),Range::all());
        Mat mask2 = img(Range(row,row+maskHeight),Range::all());
        Mat mask_t = mask2;
        flip(mask_t, mask_t, 0);
        Mat mask3 =  mask1 - mask_t;
        Mat mask4 = mask3.mul(mask3);
        cvtColor(mask1, mask1, CV_BGR2GRAY);
       cvtColor(mask_t, mask_t, CV_BGR2GRAY);
        nonzero1 = countNonZero(mask1);
        nonzero2= countNonZero(mask_t);
        double S_operator = (abs(nonzero1-nonzero2))/(width*maskHeight);
        error.push_back(sum(mask4)[0]);
        
    }
    double max = error[0];
    int index= 0;// The index of  minimum of error
    for(int j = 0;j!=error.size()-1;++j)
    {
        if (error[j]>max)
        {
            max = error[j];
            index = j;
        }
    }
    axis_H = index+maskHeight;
    return axis_H;
    
    }
int weightImg::VerticalAxis(const Mat &image){
    Mat img_;
    int height = image.rows;
    vector<double> error2;
    cvtColor(image, img_, CV_BGR2HSV);
    int maskWidth = floor(image.cols/4);
    for(int k = maskWidth+1;k<image.cols-maskWidth;++k){
    
        Mat mask3 = img_(Range(0,axis_H),Range(k-maskWidth+1,k+1));
        Mat mask4 = img_(Range(0,axis_H),Range(k,k+maskWidth));
        flip(mask4, mask4, 1);
       Mat mask_ = mask3 -mask4;
    
        mask_ = mask_.mul(mask_);
        
        cvtColor(mask3, mask3, CV_BGR2GRAY);
        cvtColor(mask4, mask4, CV_BGR2GRAY);
        int nonZero1 = countNonZero(mask3);
        int nonZero2 = countNonZero(mask4);
        
        double S_operator_ = (abs(nonZero1-nonZero2))/(height*maskWidth);
        error2.push_back(sum(mask_)[0]);
        
    }
    double min = error2[0];
    int index= 0;// The index of  minimum of error
    for(int j = 0;j!=error2.size()-1;++j)
    {
        if (error2[j]<min)
        {
            min = error2[j];
            index = j;
        }
    }
    axis_V = index+maskWidth;
    
    return axis_V;
}


//draw the axis in a specified image
void weightImg::DrawAxis(const Mat &img){
    MyLine(img,Point(0,axis_H),Point(img.cols,axis_H));
    MyLine(img, Point(axis_V,0), Point(axis_V,axis_H));
    imshow("Axis_V Annotated", img);
    waitKey(2000);
    
}

// operate the gaussian weights to the n slides of the image to decrease the backgroud effects
Mat weightImg::GaussianBlur(Mat img,int axis)
{
    vector<double> gauss;
    int  W = floor(img.cols/4); // the standard deviation
    for (int i =0;i!=img.cols;++i)
    {
     //   double K = 1/(W*sqrt(2*PI));
        double exponent =-pow((i-axis),2)/(2*(pow(W,2)));
       
        gauss.push_back(pow(E,exponent));
        
    }
    
    for(int i = 0;i<img.cols;++i){
        img(Range::all(),Range(i,i+1)) = img(Range::all(),Range(i,i+1))* gauss[i];
        
    }
    
    return img;
    
}

float weightImg::bhattacharyya(Mat k, Mat q)
{
    cv::normalize(k, k, 1, 0, cv::NORM_L1);
    cv::normalize(q, q, 1, 0, cv::NORM_L1);
    
    //show the histograms
    //drawHist("hist1", k);
    //drawHist("hist2", q);
    
    cv::Mat temp = k.mul(q);
    sqrt(temp, temp);
    
    return (float)sqrt(1 - cv::sum(temp)[0]); // sqrt(1-sum(sqrt(k.*q)))
}


