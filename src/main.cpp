/*
Project 2:Content based image retrieval
 
Author:Amit Mulay
*/

#include <opencv2/opencv.hpp>
#include <cstdio>
#include <string>
#include <utility>
#include <iostream>
#include <cstring>
#include <cstdlib>

#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <queue>
#include "Task1.hpp"
#include "Task2.hpp"

// One (and only one) of your C++ files must define CVUI_IMPLEMENTATION
// before the inclusion of cvui.h to ensure its implementaiton is compiled.
#define CVUI_IMPLEMENTATION
#include "cvui.h"

#define WINDOW_NAME "CBIR"

enum taskMode{
    task1 = 1,
    task2 = 2,
    task3 = 3,
    task4 = 4,
    task5 = 5
}tMode;

int displayPList(std::priority_queue <std::pair<int,std::string>> &pq, std::vector <std::pair<int,std::string>> &vec)
{
    while(!pq.empty())
    {
        vec.insert(vec.begin(),pq.top());
        pq.pop();
    }
    
    for(int i=0; i<vec.size(); i++)
    {
        std::cout<< "File Name:" << vec[i].second << std::endl;
    }
    return 0;
}

int main(int argc, const char *argv[])
{
    
    //Read database path
    char dirname[256];
    char buffer[256];
    char similarImage[256];
    FILE *fp;
    struct dirent *dp;
    int i;
    std::priority_queue <std::pair<int,std::string>> pq;
    std::vector <std::pair<int,std::string>> vect1;
    int vec1count = 0;

    // check for sufficient arguments
    if( argc < 2) {
      printf("usage: %s <directory path>\n", argv[0]);
      exit(-1);
    }

    // get the directory path
    strcpy(dirname, argv[1]);
    printf("Processing directory %s\n", dirname );
    
	cv::Mat frame = cv::Mat(800, 1000, CV_8UC3);

	// Init cvui and tell it to create a OpenCV window, i.e. cv::namedWindow(WINDOW_NAME).
	cvui::init(WINDOW_NAME);
    cv::Mat img;
    //std::string image_path = cv::samples::findFile("/Users/amitmulay/Desktop/Computer Vision/Project2/media/olympus/pic.1016.jpg");
    //img = imread(image_path, cv::IMREAD_COLOR);
    // Fill the frame with a nice color
    int matchCount = 3;
    frame = cv::Scalar(49, 52, 49);
	while (true) {
        bool centerHistogram = false;
		// Buttons will return true if they were clicked, which makes
		// handling clicks a breeze.
        cvui::trackbar(frame, 10, 350, 200, &matchCount, (int)3, (int)15);
        cvui::printf(frame, 10, 330, 0.5, 0xff0000, "Number of matches");
        if (cvui::button(frame, 10, 200,200,30, "Display Task1 Target Image"))
        {
            std::string image_path = cv::samples::findFile("../media/olympus/pic.1016.jpg");
            img = imread(image_path, cv::IMREAD_COLOR);
            cvui::image(frame, 210, 10, img);
        }
        if (cvui::button(frame, 10, 230,200,30, "Display Task2 Target Image"))
        {
            std::string image_path = cv::samples::findFile("../media/olympus/pic.0164.jpg");
            img = imread(image_path, cv::IMREAD_COLOR);
            cvui::image(frame, 210, 10, img);
        }
        if (cvui::button(frame, 10, 260,200,30, "Display Task3 Target Image"))
        {
            std::string image_path = cv::samples::findFile("../media/olympus/pic.0135.jpg");
            img = imread(image_path, cv::IMREAD_COLOR);
            cvui::image(frame, 210, 10, img);
        }
		if (cvui::button(frame, 10, 10,100,30, "Task1"))
        {
            vect1.clear();
			// The button was clicked, current task set to this.
            
			tMode = task1;
            Task1 t1;
            t1.setDirName(dirname);
            t1.setDir(t1.getDirName());
            t1.readDBandCompare(pq,matchCount);
            std::cout << "Top " << matchCount << " matches for task 1:" << std::endl;
            displayPList(pq,vect1);
            img = imread(vect1[0].second, cv::IMREAD_COLOR);
            cvui::image(frame, 210, 10, img);
            //cvui::printf(frame, 350, 550, 0.5, 0xff0000, "match 1 %s", vect1[vec1count].second.c_str());
            cvui::update();
            
		}
        if (cvui::button(frame, 10, 40,100,30, "Task2"))
        {
            // The button was clicked, current task set to this.
            tMode = task2;
            vect1.clear();
            Task2 t2;
            t2.setDirName(dirname);
            t2.setDir(t2.getDirName());
            t2.readDBandCompare(pq,centerHistogram,matchCount);
            std::cout << "Top " << matchCount << " matches for task 2:" << std::endl;
            displayPList(pq,vect1);
            img = imread(vect1[0].second, cv::IMREAD_COLOR);
            cvui::image(frame, 210, 10, img);
            
        }
        if (cvui::button(frame, 10, 70,100,30, "Task3"))
        {
            // The button was clicked, current task set to this.
            tMode = task3;
            centerHistogram = true;
            vect1.clear();
            Task2 t2;
            t2.setDirName(dirname);
            t2.setDir(t2.getDirName());
            t2.readDBandCompare(pq,centerHistogram,matchCount);
            std::cout << "Top " << matchCount << " matches for task 3:" << std::endl;
            displayPList(pq,vect1);
            img = imread(vect1[0].second, cv::IMREAD_COLOR);
            cvui::image(frame, 210, 10, img);
        }
        /*
        if (cvui::button(frame, 10, 100,100,30, "Task4"))
        {
            // The button was clicked, current task set to this.
            tMode = task4;
        }
        if (cvui::button(frame, 10, 130,100,30, "Task5"))
        {
            // The button was clicked, current task set to this.
            tMode = task5;
        } */
        if (cvui::button(frame, 550, 550,100,30, "Next"))
        {
            if(!vect1.empty())
            {
                if(vec1count == vect1.size() - 1)
                    vec1count = -1;
                img = imread(vect1[++vec1count].second, cv::IMREAD_COLOR);
                cvui::image(frame, 210, 10, img);
            }
        }
        if (cvui::button(frame, 400, 550,100,30, "Previous"))
        {
            if(!vect1.empty())
            {
                if(vec1count <= 0)
                    vec1count = vect1.size();
                img = imread(vect1[--vec1count].second, cv::IMREAD_COLOR);
                cvui::image(frame, 210, 10, img);
            }
        }
		//cvui::printf(frame, 10, 700, 0.5, 0xff0000, "Current task selected %d", tMode);
        cv::String input1_value = "text1";
        cvui::input(frame, 10, 700, 100, "input1", input1_value);
        //cvui::image(frame, 210, 10, img);
        cvui::update();
        
		// Update cvui stuff and show everything on the screen
        cvui::imshow(WINDOW_NAME, frame );
        
		// Check if ESC key was pressed
		if (cv::waitKey(20) == 27) {
			break;
		}
	}

	return 0;
}

