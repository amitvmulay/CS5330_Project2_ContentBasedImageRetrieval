//
//  Task1.cpp
//  
//
//  Created by Amit Mulay on 2/20/21.
//

#include "Task1.hpp"

using namespace std;
using namespace cv;

Task1::Task1()
{
    std::string image_path = samples::findFile("/Users/amitmulay/Desktop/Computer Vision/Project2/media/olympus/pic.1016.jpg");
    src = imread(image_path, IMREAD_COLOR);

    if(src.empty())
    {
        std::cout << "Could not read the image: " << image_path << std::endl;
    }
}

Task1::~Task1(){}

int Task1::setDirName(char *dn)
{
    dirname = dn;
    return 0;
}

char * Task1::getDirName()
{
    return dirname;
}

int Task1::setDir(char *dirname)
{
    dirp = opendir( dirname );
    if( dirp == NULL) {
      printf("Cannot open directory %s\n", dirname);
      exit(-1);
    }
    return 0;
}

DIR * Task1::getDir()
{
    return dirp;
}

int Task1::getSSD(cv::Mat &dst)
{
    int sum[3] = {0,0,0};
    for(int y = (src.rows/2) - 4; y <= (src.rows/2) + 4; y++)
    {
        for(int x = (src.cols/2) - 4; x
            <= (src.cols/2) + 4; x++)
        {
            cv::Vec3b src_pix = src.at<Vec3b>(y, x);
            cv::Vec3b dst_pix = dst.at<Vec3b>(y, x);
            for(int i = 0; i < 3; i++)
            sum[i] += (src_pix.val[i] - dst_pix.val[i]) * (src_pix.val[i] - dst_pix.val[i]);
        }
    }
    
    return sum[0] + sum[1] + sum[2];
}

int Task1::readDBandCompare(std::priority_queue <std::pair<int,std::string>> &pq, int matchCount)
{
    struct dirent *dp;
    char buffer[256];
    char similarImage[256];
    while( (dp = readdir(dirp)) != NULL )
    {
       
        if(strstr(dp->d_name,"pic.1016.jpg"))
            continue;
        
        // check if the file is an image
        if( strstr(dp->d_name, ".jpg") ||
        strstr(dp->d_name, ".png") ||
        strstr(dp->d_name, ".ppm") ||
        strstr(dp->d_name, ".tif") )
        {
            strcpy(buffer, dirname);
            strcat(buffer, "/");
            strcat(buffer, dp->d_name);
            
            std::string cimage_path = samples::findFile(buffer);
            cv::Mat cimg = cv::imread(cimage_path,IMREAD_COLOR);
            //! [empty]
            if(cimg.empty())
            {
                std::cout << "Could not read the image from database: " << cimage_path << std::endl;
                return 1;
            }
            //! [empty]
            int diff = getSSD(cimg);
            
            if(pq.size() < matchCount)
            {
                pq.push(make_pair(diff,cimage_path));
            }
            else if(diff < pq.top().first)
            {
                pq.pop();
                pq.push(make_pair(diff,cimage_path));
            }
        }
    }
    return 0;
}
