/*
 *  Classroom Monitor - Classroom Response System & Classroom Use Monitor
 *  Copyright (C) 2014  Joel M. Smith
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  I can be contacted at joel.smith3@uqconnect.edu.au
 *
 */

#ifndef CLASSROOM_H
#define CLASSROOM_H

// OpenCV libraries
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>

// Standard libraries
#include <stdio.h>
#include <stdlib.h>
#include <ctime>
#include <time.h>       /* time_t, struct tm, time, localtime */

#include <iostream>
#include <fstream>

#include "response.h"
#include "monitor.h"

#define MOVEMENT_SAMPLES 127
#define VIDEO_SAMPLE_RATE 0.5

using namespace std;
using namespace cv;

class Classroom {
private:
    VideoCapture cap;
    CascadeClassifier hand_cascade;
    Mat frame, motion, cropped;
    Monitor *monitor;
    Question *question;
    MainWindow *window;
    ofstream data;
    time_t rawtime;
    struct tm * timeinfo;
    int hands = 0, sampleIndex = 0;
    double difference = 0, average = 0, samples[MOVEMENT_SAMPLES] = {};
    bool record = false, video = false;
    float video_time = 0;

    int openMonitor();
    int getFrame();
    void vote();
    void activity();
    void recordData();
    char* myAsctime(const struct tm *timeptr);

public:
    Classroom() {}
    Classroom(MainWindow *window);
    ~Classroom();
    int loadHandCascade(const string& filename);
    void loadDecisionTree(const string& filename);
    int openCamera(int id);
    int openVideo(const string& filename);
    int openDataFile(const string& filename);
    void update();
    void setRecord(bool record);
    void closeDataFile();
    int countCameras();


    //std::clock_t start;
    double duration;
    struct timespec start, finish;
    double elapsed;
};


#endif // CLASSROOM_H
