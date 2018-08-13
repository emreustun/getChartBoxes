/*
* create_mask.cpp
*
* Author:
* Siddharth Kherada <siddharthkherada27[at]gmail[dot]com>
*
* This tutorial demonstrates how to make mask image (black and white).
* The program takes as input a source image and outputs its corresponding
* mask image.
*/

#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/core.hpp"
#include <iostream>
#include <stdlib.h>
#include <typeinfo>

cv::Mat img0, img1, res1, final, tmp, dst;
cv::Point point;
int drag = 0;

int numpts = 4;
cv::Point* pts = new cv::Point[100];

int var = 0;
int flag = 0;
int flag1 = 0;

int minx,miny,maxx,maxy,lenx,leny;

void mouseHandler(int, int, int, int, void*);

void mouseHandler(int event, int x, int y, int, void*) // (x,y) is where the mouse touches
{
    if (event == cv::EVENT_LBUTTONDOWN && !drag)
    {
        if(flag1 == 0)
        {
            if(var==0)
                img1 = img0.clone();
            point = cv::Point(x, y); //
            cv::circle(img1,point,2,cv::Scalar(0, 0, 255),-1, 8, 0);
//            printf("BUTTON DOWN!! x = %d, y = %d\n", x, y);
            pts[var] = point;
            var++;
            drag  = 1;
            if(var>1)
            imshow("Source", img1);
        }
    }

    if (event == cv::EVENT_LBUTTONUP && drag)
    {
        imshow("Source", img1);
        drag = 0;
    }
    if (event == cv::EVENT_RBUTTONDOWN)
    {
        flag1 = 1;
        img1 = img0.clone();
        for(int i = var; i < numpts ; i++)
            pts[i] = point;

        if(var!=0)
        {
            cv::rectangle(img1, pts[var-2], pts[var-1], cv::Scalar(0, 255, 0), 2, 8, 0);
        }

        for(int i=var-2;i<var;i++)
        {
            minx = cv::min(minx,pts[i].x);
            maxx = cv::max(maxx,pts[i].x);
            miny = cv::min(miny,pts[i].y);
            maxy = cv::max(maxy,pts[i].y);
        }

        imshow("Source", img1);
    }

    if (event == cv::EVENT_RBUTTONUP)
    {
        flag = var;

        final = cv::Mat::zeros(img0.size(),CV_8UC3);
        res1 = cv::Mat::zeros(img0.size(),CV_8UC1);
        pts[0] = cv::Point(minx, miny);
        pts[1] = cv::Point(minx, maxy);
        pts[2] = cv::Point(maxx, maxy);
        pts[3] = cv::Point(maxx, miny);
        const cv::Point* pts4[1] = {&pts[0]};
        printf("Corner 1 ==> %d, %d\n", pts4[0]->x, pts4[0]->y);
        printf("Corner 2 ==> %d, %d\n", (pts4[0]+1)->x, (pts4[0]+1)->y);
        printf("Corner 3 ==> %d, %d\n", (pts4[0]+2)->x, (pts4[0]+2)->y);
        printf("Corner 4==> %d, %d\n", (pts4[0]+3)->x, (pts4[0]+3)->y);

        cv::fillPoly(res1, pts4,&numpts, 1, cv::Scalar(177), 8, 0);
        bitwise_and(img0, img0, final,res1); // img0 & img0 = final, masked by res1
        imshow("mask",final);
        imwrite("mask.png",res1);

        imshow("Source", img1);

    }
    if (event == cv::EVENT_MBUTTONDOWN)
    {
        for(int i = 0; i < numpts ; i++)
        {
            pts[i].x=0;
            pts[i].y=0;
        }
        var = 0;
        flag1 = 0;
        minx = INT_MAX; miny = INT_MAX; maxx = INT_MIN; maxy = INT_MIN;
        imshow("Source", img0);
        drag = 0;
    }

    if (event == cv::EVENT_MOUSEWHEEL)
    {
        if (event > 0)
        {
//            printf("scrolling Up!!!!\n");
        }
    }

    if (event == cv::EVENT_LBUTTONDBLCLK)
    {
//        printf("scrolling Down!!!!\n");
    }
}

void help()
{
    std::cout << "Instructions:" << std::endl;
    std::cout << "Left button selects the corners of the rectangle"  << std::endl;
    std::cout << "Middle button resets the process" << std::endl;
    std::cout << "Right button extracts the mask and saves it" << std::endl << std::endl;
}

int main(int argc, char **argv)
{
    help();

    //Insert image path
    cv::Mat src = cv::imread("tempImage.jpg");
    minx = INT_MAX; miny = INT_MAX; maxx = INT_MIN; maxy = INT_MIN;
    img0 = src;

    res1 = cv::Mat::zeros(img0.size(),CV_8UC1);
    final = cv::Mat::zeros(img0.size(),CV_8UC3);

    cv::namedWindow("Source", 1);
    cv::setMouseCallback("Source", mouseHandler, NULL);

    imshow("Source", img0);
    cv::waitKey(0);

    printf("Process has finished successfully");

    return 0;
}
