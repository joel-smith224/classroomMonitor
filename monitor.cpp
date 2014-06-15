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

#include "monitor.h"
#include "mainwindow.h"


Monitor::Monitor(Mat img, MainWindow *w) {
    this->w = w;
    dtree = new CvDTree;
    cvtColor(img, imggray1, CV_BGR2GRAY);
    kernel_ero = getStructuringElement(MORPH_RECT, Size(2,2));
}

Monitor::~Monitor () {
    imggray1.release();
    imggray2.release();
    imggray3.release();
    kernel_ero.release();
    dtree->~CvDTree();
    if(running)
        aud.join();
}

/* Estimates the motion between frame and the previous frame.
 */
double Monitor::difference(Mat *frame, Mat &motion, bool show) {

    cvtColor(*frame, imggray2, CV_BGR2GRAY);
    absdiff(imggray1, imggray2, imggray3);

    imggray2.copyTo(imggray1);
    average = mean(imggray3);

    if(show)
        *frame = imggray3;
    else {
        threshold(imggray3, motion, 25, 255, CV_THRESH_BINARY);
        erode(motion, motion, kernel_ero);
    }

    return average.val[0];
}

/* Crops frame to where motion occured.
 */
void Monitor::detectMotion(Mat &motion, Mat &frame, Mat &cropped) {
    int min_x = motion.cols, max_x = 0;
    int min_y = motion.rows, max_y = 0;
    // loop over image and detect changes
    for(int j = 1; j < motion.rows - 1; j+=2) {
        for(int i = 1; i < motion.cols - 1; i+=2) {
            if(static_cast<int>(motion.at<uchar>(j,i)) == 255) {
                if(min_x > i) min_x = i;
                if(max_x < i) max_x = i;
                if(min_y > j) min_y = j;
                if(max_y < j) max_y = j;
            }
        }
    }
    //check if not out of bounds
    if(min_x-10 > 0) min_x -= 10;
    if(min_y-10 > 0) min_y -= 10;
    if(max_x+10 < frame.cols-1) max_x += 10;
    if(max_y+10 < frame.rows-1) max_y += 10;
    // draw rectangle round the changed pixel
    Point x(min_x, min_y);
    Point y(max_x, max_y);
    Rect rect(x,y);
    cropped = frame(rect);
}

/* Loads the classification tree used for activity predication.
 */
void Monitor::loadDecisionTree(const char* filename) {
    dtree->clear();
    dtree->load(filename);
}

/* Predicts the current activity based off the motion and volume levels.
 */
int Monitor::getActivity(float motion, float noise) {
    Mat activity_sample = Mat(1, 2, CV_32FC1);
    CvDTreeNode *resultNode;
    activity_sample.at<float>(0, 0) = motion;
    activity_sample.at<float>(0, 1) = noise;
    resultNode = dtree->predict(activity_sample);
    return (int) resultNode->value;
}

/* Starts a new thread used for sampling the noise.
 */
void Monitor::startAudio() {
    running = true;
    aud = thread(&Monitor::audio, this);
}

/* Stops the thread that is measuring noise.
 */
void Monitor::stopAudio() {
    running = false;
    aud.join();
}

/* Initilises and measures the volume levels from the default microphone.
 */
int Monitor::audio() {
    PaStreamParameters inputParameters;
    PaStream *stream;
    PaError err;

    SAMPLE *recordedSamples;
    SAMPLE max, average;
    SAMPLE val, temp;
    int totalFrames;
    int numSamples;
    int numBytes;

    SAMPLE maxs [AVERAGE];
    int c = 0;
    for (int i = 0; i <AVERAGE; i++) {
        maxs[i] = 0;
    }


    totalFrames = NUM_SECONDS * SAMPLE_RATE; /* Record for a few seconds. */
    numSamples = totalFrames * NUM_CHANNELS;

    numBytes = numSamples * sizeof(SAMPLE);
    recordedSamples = (SAMPLE *) malloc( numBytes );
    if( recordedSamples == NULL )
    {
        printf("Could not allocate record array.\n");
        exit(1);
    }

    cout << "Initilising Audio..." << endl;

    err = Pa_Initialize();
    if( err != paNoError ) goto error;

    printf("Begin...\n");

    inputParameters.device = Pa_GetDefaultInputDevice(); /* default input device */
    if (inputParameters.device == paNoDevice) {
        fprintf(stderr,"Error: No default input device.\n");
        goto error;
    }
    inputParameters.channelCount = NUM_CHANNELS;
    inputParameters.sampleFormat = PA_SAMPLE_TYPE;
    inputParameters.suggestedLatency = Pa_GetDeviceInfo( inputParameters.device )->defaultLowInputLatency;
    inputParameters.hostApiSpecificStreamInfo = NULL;

   /* Record some audio. -------------------------------------------- */
    err = Pa_OpenStream(
            &stream,
            &inputParameters,
            NULL,                  /* &outputParameters, */
            SAMPLE_RATE,
            FRAMES_PER_BUFFER,
            paClipOff,      /* we won't output out of range samples so don't `ther clipping them */
            NULL, /* no callback, use blocking API */
            NULL ); /* no callback, so no callback userData */
    if(err != paNoError) goto error;

    err = Pa_StartStream(stream);
    if(err != paNoError) goto error;


    cout << "Recording..." << endl;

    while(running) {
        err = Pa_ReadStream(stream, recordedSamples, totalFrames);
        if(err != paNoError) goto error;

        temp = 0;
        average = 0;
        for(int i = 0; i < numSamples; i++) {
            val = recordedSamples[i];
            if(val < 0)
                val = -val; /* ABS */
            if(val > temp) {
                temp = val;
            }
            average += val;
        }

        average = average / numSamples;

        maxs[c] = temp;
        c = (c+1) % AVERAGE;

        max = 0;

        for (int i = 0; i < AVERAGE; i++) {
            if (maxs[i] > max) {
                max = maxs[i];
            }
        }

        //Update GUI
        w->setAudioLevel(max * 100);
        w->setAudioAverage(average * 100);

        std::cout << "hello" << std::endl;
    }

    err = Pa_Terminate();
    if( err != paNoError ) goto error;

    return 0;

    error:
        Pa_Terminate();
        fprintf( stderr, "An error occured while using the portaudio stream\n" )    ;
        fprintf( stderr, "Error number: %d\n", err );
        fprintf( stderr, "Error message: %s\n", Pa_GetErrorText( err ) );
        return -1;
}
