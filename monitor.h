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

#ifndef MONITOR_H_
#define MONITOR_H_

#include <stdio.h>
#include <thread>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <portaudio.h>

class MainWindow;


#define SAMPLE_RATE		(44100)
#define FRAMES_PER_BUFFER	(1024)
#define NUM_SECONDS 	(1)
#define NUM_CHANNELS	(2)
#define DITHER_FLAG    (0)
#define AVERAGE			(1)

/* Select sample format. */
#if 1
#define PA_SAMPLE_TYPE  paFloat32
typedef float SAMPLE;
#define SAMPLE_SILENCE  (0.0f)
#define PRINTF_S_FORMAT "%.8f"
#elif 1
#define PA_SAMPLE_TYPE  paInt16
typedef short SAMPLE;
#define SAMPLE_SILENCE  (0)
#define PRINTF_S_FORMAT "%d"
#elif 0
#define PA_SAMPLE_TYPE  paInt8
typedef char SAMPLE;
#define SAMPLE_SILENCE  (0)
#define PRINTF_S_FORMAT "%d"
#else
#define PA_SAMPLE_TYPE  paUInt8
typedef unsigned char SAMPLE;
#define SAMPLE_SILENCE  (128)
#define PRINTF_S_FORMAT "%d"
#endif

using namespace std;
using namespace cv;

class Monitor {
    private:
        Mat imggray1, imggray2, imggray3;
        CvScalar average;
        CvDTree* dtree;
        MainWindow *w;
        thread aud;
        bool running;
        int audio();
        Mat kernel_ero;
    public:
        Monitor() {}
        Monitor(Mat img, MainWindow *w);
        ~Monitor();
        double difference(Mat *frame, Mat &motion, bool show);
        void detectMotion(Mat &motion, Mat &frame, Mat &cropped);
        void startAudio();
        void stopAudio();
        void loadDecisionTree(const char* filename);
        int getActivity(float motion, float noise);
};


#endif /* MONITOR_H_ */
