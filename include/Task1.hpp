//
//  Task1.hpp
//  
//
//  Created by Amit Mulay on 2/20/21.
//

#ifndef Task1_hpp
#define Task1_hpp

#include <opencv2/core.hpp>
#include <dirent.h>
#include <utility>
#include <queue>
#include <stdio.h>
#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

class Task1
{
private:
    cv::Mat src;
    int ssd;
    DIR *dirp = NULL;
    char *dirname = NULL;
public:
    Task1();
    ~Task1();
    int getSSD(cv::Mat &dst);
    int readDBandCompare(std::priority_queue <std::pair<int,std::string>> &pq,int matchCount);
    int setDirName(char *);
    char *getDirName();
    int setDir(char *dn);
    DIR * getDir();
    
};

#endif /* Task1_hpp */
