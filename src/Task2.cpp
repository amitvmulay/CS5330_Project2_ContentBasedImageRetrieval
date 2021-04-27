//
//  Task2.cpp
//  
//
//  Created by Amit Mulay on 2/23/21.
//

#include "Task2.hpp"

using namespace std;
using namespace cv;

Task2::Task2()
{
    
    std::string image_path = samples::findFile("../media/olympus/pic.0164.jpg");
    src = imread(image_path, IMREAD_COLOR);
    image_path = samples::findFile("../media/olympus/pic.0135.jpg");
    src_t3 = imread(image_path, IMREAD_COLOR);
    cv::resize(src, src, cv::Size(), 0.75, 0.75);
    cv::resize(src_t3, src_t3, cv::Size(), 0.75, 0.75);
    if(src.empty())
    {
        std::cout << "Could not read the image: " << image_path << std::endl;
    }
}

Task2::~Task2(){}

int Task2::setDirName(char *dn)
{
    dirname = dn;
    return 0;
}

char * Task2::getDirName()
{
    return dirname;
}

int Task2::setDir(char *dirname)
{
    dirp = opendir( dirname );
    if( dirp == NULL) {
      printf("Cannot open directory %s\n", dirname);
      exit(-1);
    }
    return 0;
}

DIR * Task2::getDir()
{
    return dirp;
}

int Task2::getNormalisedHist(cv::Mat &hist, cv::Mat &normHist)
{
    long totalSum = 0;
    float totalSumFloat = 0;
    for(int y = 0; y < hist.rows; y++)
    {
      for(int x = 0; x < hist.cols; x++)
      {
          int pix = hist.at<int>(y,x);
          //std::cout << "x:" << x << "y:" << y << "pix:" << pix << std::endl;
          totalSum += pix;
          //std::cout << "total sum:" << totalSum << std::endl;
      }
    }
    if (totalSum == 0)
    {
        //std::cout << "total sum:" << totalSum << std::endl;
        return 0;
    }
        
    for(int y = 0; y < hist.rows; y++)
    {
      for(int x = 0; x < hist.cols; x++)
      {
          int temp1 = hist.at<int>(y,x);
          float temp2 = (float)temp1;
          float temp =  temp2/ totalSum;
          //std::cout << "temp1:" << temp1 << std::endl;
          //std::cout << "temp2:" << temp2 << std::endl;
          //std::cout << "temp:" << temp << std::endl;
          normHist.at<float>(y,x) = temp;
      }
    }
    /*
    //Print total
    for(int y = 0; y < normHist.rows; y++)
    {
      for(int x = 0; x < normHist.cols; x++)
      {
          float pix = normHist.at<float>(y,x);
          totalSumFloat += pix;
          std::cout << "total of Hist:" << totalSumFloat << std::endl;
      }
    }
    std::cout << "total of Hist:" << totalSumFloat << std::endl;
     */
    return 0;

}

int Task2::buildImageHistogram(cv::Mat &src, cv::Mat &hist, bool centerHistogram)
{
    //Initialise for whole image histogram
    int startY = 0;
    int startX = 0;
    int lastRowPix = src.rows;
    int lastColPix = src.cols;
    
    if(centerHistogram) //Consider only center 100x100 image patch
    {
        startY = (src.rows/2) - 50;
        startX = (src.cols/2) - 50;
        lastRowPix = (src.rows/2) + 50;
        lastColPix = (src.cols/2) + 50;
    }
    //std::cout << "Here" <<  std::endl;
    for(int y = startY; y < lastRowPix; y++)
    {
      for(int x = startX; x < lastColPix; x++)
      {
          //std::cout << "x:" << x << " y:" << y <<  std::endl;
          cv::Vec3b tempPix = src.at<Vec3b>(y, x);
          float R_p = tempPix.val[2];
          float G_p = tempPix.val[1];
          float B_p = tempPix.val[0];
          //std::cout << "R_p:" << R_p << " G_p:" << G_p << " B_p:" << B_p << std::endl;
          float RGBSum = R_p + G_p + B_p;
          //std::cout << "RGBSum:" << RGBSum << std::endl;
          int R_ind,G_ind;
          float Rtemp,Gtemp;
         // if(RGBSum != 0)
          if(R_p > 0 || G_p > 0 || B_p >0)
          {
              float R = R_p / RGBSum;
              if(R == 1)
                  R_ind = BEAN_SIZE - 1;
              else
              {
                  Rtemp = R * BEAN_SIZE;
                  R_ind = Rtemp;
              }
              
              float G = G_p / (RGBSum);
              if(R == 1)
                  G_ind = BEAN_SIZE - 1;
              else
              {
                  Gtemp = G * BEAN_SIZE;
                  G_ind = Gtemp;
              }
              
              hist.at<int>(R_ind,G_ind)++;
          }
          
          //std::cout << "R_ind" << R_ind << " G_ind" << G_ind << std::endl;
          //unsigned short histBinIndex = hist.at<unsigned short>(R_ind,G_ind);
          //hist.at<int>(R_ind,G_ind)++;
          //std::cout << "R_ind" << R_ind << " G_ind" << G_ind << " Val:" << hist.at<int>(R_ind,G_ind) << std::endl;
    
      }
        
    }
    return 0;

}

float Task2::getHistIntersectionDist(cv::Mat &normSrcHist,cv::Mat &dstHist)
{
    cv::Mat normDstHist(BEAN_SIZE,BEAN_SIZE,CV_32F);
    normDstHist = cv::Mat::zeros(cv::Size(normDstHist.rows,normDstHist.cols),CV_32F);
    getNormalisedHist(dstHist, normDstHist);
    
    double intersectionOfHist = 0;
    for(int y = 0; y < normDstHist.rows; y++)
    {
      for(int x = 0; x < normDstHist.cols; x++)
      {
          float tempDstVal = normDstHist.at<float>(y,x);
          float tempSrcVal = normSrcHist.at<float>(y,x);
          //std::cout << "tempDstVal:" << tempDstVal << std::endl;
          //std::cout << "tempSrcVal:" << tempSrcVal << std::endl;
          intersectionOfHist += std::min(tempSrcVal,tempDstVal);
          
      }
    }
    //std::cout << "intersectionOfHist:" << intersectionOfHist;
    return 1 - intersectionOfHist;
}

int Task2::readDBandCompare(std::priority_queue <std::pair<int,std::string>> &pq, bool centerHistogram, int matchCount)
{
    struct dirent *dp;
    char buffer[256];
    char similarImage[256];
    
    cv::Mat dstHist(BEAN_SIZE,BEAN_SIZE,CV_32SC1,Scalar(0));
    cv::Mat dstCenterHist(BEAN_SIZE,BEAN_SIZE,CV_32SC1,Scalar(0));
    
    cv::Mat srcHist(BEAN_SIZE,BEAN_SIZE,CV_32SC1,Scalar(0));
    cv::Mat srcCenterHist(BEAN_SIZE,BEAN_SIZE,CV_32SC1,Scalar(0));
    cv::Mat normSrcHist(BEAN_SIZE,BEAN_SIZE,CV_32F,Scalar(0));
    cv::Mat normSrcCenterHist(BEAN_SIZE,BEAN_SIZE,CV_32F,Scalar(0));
    if(centerHistogram)
    {
        buildImageHistogram(src_t3, srcCenterHist, true);  //Build center 100x100 histogram for task 3
        buildImageHistogram(src_t3, srcHist, false);       //Build whole image histogram
        getNormalisedHist(srcCenterHist, normSrcCenterHist); //get normalized histogram for center histogram
    }
    else
        buildImageHistogram(src, srcHist, false);       //Build whole image histogram

    getNormalisedHist(srcHist, normSrcHist);            //get normalized histogram for whole image histogram
    
    while( (dp = readdir(dirp)) != NULL )
    {
        //std::cout << " File name:" << dp->d_name ;
        if(!centerHistogram)
        {
            if(strstr(dp->d_name,"pic.0164.jpg"))
                continue;
        }
        else
        {
            if(strstr(dp->d_name,"pic.0135.jpg"))
                continue;
        }
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
            cv::resize(cimg, cimg, cv::Size(), 0.75, 0.75);
            dstHist = cv::Mat::zeros(cv::Size(dstHist.rows,dstHist.cols),CV_32SC1);
            dstCenterHist = cv::Mat::zeros(cv::Size(dstCenterHist.rows,dstCenterHist.cols),CV_32SC1);
            if(centerHistogram)
                buildImageHistogram(cimg,dstCenterHist,true);   //Build center 100x100 histogram for task 3
            
            buildImageHistogram(cimg,dstHist,false);            //Build whole image histogram
            
            float diff = getHistIntersectionDist(normSrcHist,dstHist);
            float diffCenter = 0;
            if(centerHistogram)
            {
                diffCenter = getHistIntersectionDist(normSrcCenterHist,dstCenterHist);
                diff = (diff + diffCenter) / 2;
            }
            int pr_diff = (int)(diff * 100000000);

            if(pq.size() < matchCount)
            {
                pq.push(make_pair(pr_diff,cimage_path));
            }
            else if(pr_diff < pq.top().first)
            {
                pq.pop();
                pq.push(make_pair(pr_diff,cimage_path));
            }
        }
    }
    return 0;
}
