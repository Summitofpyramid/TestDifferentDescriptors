#include "histdescriptor.h"


using namespace cv;
using namespace std;
// COMPUTE THE Img descriptor
Mat DescriptorY::ImageHistCompute(const Mat &image1){
    
    weightImg axisDraw;
    axisDraw.HorizonAxis(image1);
    int axis_posi = axisDraw.VerticalAxis(image1);
    axisDraw.GaussianBlur(image1, axis_posi);
    //axisDraw.DrawAxis(image1);
    //   Mat image1 = computeFG(image);
    Mat binMask, binHist;
    /// Set the ranges ( for B,G,R) )
    float range1[2]= {0,180};
    
    float range[2] = {0, 256};
    const float* histRange_h = {range1};
    const float* histRange = { range };
    /// Establish the number of bins
    int histSize = 256;

    int hist_w = 512; int hist_h = 400;
    int bin_w = cvRound((double)hist_w / histSize);
    int width, height;
    vector<Mat>Hists;
    width = image1.cols; //get image width
    height = image1.rows;
    
    int binCountX = 1; //# of small bins horizontally of image
    int binCountY = 5; //# of small bins vertically of image

    
    binWidth = width / binCountX; // calculate bin width
    binHeight = height / binCountY;
    
    for (int i = 0; i<(binCountX*binCountY); i++)
    {
        binLocx = i%binCountX; //the x coordinate of a certain bin with order i
        binLocy = floor(i / binCountX);// the y coordinate of a certain bin
        binMask = image1(Range(binHeight*binLocy, binHeight*(binLocy + 1)),
                        Range(binWidth*binLocx, binWidth*(binLocx + 1)));
      //---------------------------------
        cvtColor(binMask, binMask, CV_BGR2HSV); //change color space
        split(binMask, bgr_planes);
        bgr_planes[0]=bgr_planes[0].reshape(0,1);
        bgr_planes[1]=bgr_planes[1].reshape(0,1);
        bgr_planes[2]=bgr_planes[2].reshape(0,1);
        
        bool accumulate = false;
        bool uniform = true;
        // Compute the histograms:
        calcHist(&bgr_planes[0], 1, 0, Mat(), b_hist, 1, &histSize, &histRange_h);
        calcHist(&bgr_planes[1], 1, 0, Mat(), g_hist, 1, &histSize, &histRange);
        calcHist(&bgr_planes[2], 1, 0, Mat(), r_hist, 1, &histSize, &histRange);
        
        /// Normalize the result to [ 0, histImage.rows ]
        normalize(b_hist, b_hist, 0, 1, NORM_MINMAX, -1, Mat());
        normalize(g_hist, g_hist, 0, 1, NORM_MINMAX, -1, Mat());
        normalize(r_hist, r_hist, 0, 1, NORM_MINMAX, -1, Mat());
        
        
        /*a = sqrt(norm(b_hist));
         b = sqrt(norm(g_hist));
         c = sqrt(norm(r_hist));
         b_hist = b_hist / a;
         g_hist = g_hist / b;
         r_hist = r_hist / c;
         */
        
        vconcat(b_hist, g_hist, combo1);
        vconcat(combo1, r_hist, binHist);
      //  binHist = binHist.t(); //transpose the matrix
        Hists.push_back(binHist);
    }
    
    combo2 = Hists[0];
    for (vector<Mat>::size_type i = 1; i < binCountX*binCountY; i++)
        vconcat(combo2, Hists[i], combo2);
    return combo2;
    //---------------------------------
    
}


Mat DescriptorY::HSVVector(Mat img) {
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

Mat DescriptorY::getWhsvFeature(cv::Mat img)
{
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

//////Segment
Mat DescriptorY::Segment(Mat image){

    cv::Mat blank(image.size(),CV_8U,cv::Scalar(0xFF));
    cv::Mat dest;
    
    // Create markers image
    cv::Mat markers(image.size(),CV_8U,cv::Scalar(-1));
    //Rect(topleftcornerX, topleftcornerY, width, height);
    //top rectangle
    markers(Rect(0,0,image.cols, 5)) = Scalar::all(1);
    
    //bottom rectangle
    markers(Rect(0,image.rows-5,image.cols, 5)) = Scalar::all(1);
    //left rectangle
    markers(Rect(0,0,5,image.rows)) = Scalar::all(1);
    //right rectangle
    markers(Rect(image.cols-5,0,5,image.rows)) = Scalar::all(1);
    //centre rectangle
    
    int centreW = image.cols/4;
    int centreH = image.rows/4;
    markers(Rect((image.cols/2)-(centreW/2),(image.rows/2)-(centreH/2), centreW, centreH)) = Scalar::all(2);
    markers.convertTo(markers,CV_BGR2GRAY);
    //imshow("markers", markers);
    
    //Create watershed segmentation object
    WatershedSegmenter segmenter;
    segmenter.setMarkers(markers);
    Mat wshedMask = segmenter.process(image);
    Mat mask;
    convertScaleAbs(wshedMask, mask, 1, 0);
    double thresh = threshold(mask, mask, 1, 255, THRESH_BINARY);
    bitwise_and(image, image, dest, mask);
    dest.convertTo(dest,CV_8U);
    return dest;

}
