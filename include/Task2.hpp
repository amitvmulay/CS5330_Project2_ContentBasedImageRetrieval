//
//  Task2.hpp
//  
//
//  Created by Amit Mulay on 2/23/21.
//

#ifndef Task2_hpp
#define Task2_hpp

#include <stdio.h>
#include <opencv2/core.hpp>
#include <dirent.h>
#include <utility>
#include <queue>
#include <stdio.h>
#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <algorithm>    // std::min

#define BEAN_SIZE 16

class Task2
{
private:
    int hid;
    DIR *dirp = NULL;
    char *dirname = NULL;
public:
    cv::Mat src;
    cv::Mat src_t3;
    Task2();
    ~Task2();
    int getNormalisedHist(cv::Mat &hist, cv::Mat &normHist);
    float getHistIntersectionDist(cv::Mat &normSrcHist,cv::Mat &dstHist);
    int buildImageHistogram(cv::Mat &src, cv::Mat &hist, bool centerHistogram);
    int readDBandCompare(std::priority_queue <std::pair<int,std::string>> &pq,bool centerHistogram, int matchCount);
    int setDirName(char *);
    char *getDirName();
    int setDir(char *dn);
    DIR * getDir();
    
};

#endif /* Task2_hpp */
