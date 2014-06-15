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

#include "classroom.h"
#include "mainwindow.h"

Classroom::Classroom(MainWindow *window) {
    this->window = window;
    question = new Question();
    window->SetQuestion(question);
    window->SetClassroom(this);
    window->populateCameraList();
    loadHandCascade("cascade-lbp-small.xml");
}

Classroom::~Classroom() {
    cap.release();
    monitor->stopAudio();
    monitor->~Monitor();
    closeDataFile();
}

/* Loads the cascade used for hand detection.
 */
int Classroom::loadHandCascade(const string& filename) {
    if(!hand_cascade.load(filename)) {
        cout << "Could not load hand cascade file at \"." << filename << "\"" << endl;
        return -1;
    }
    return 0;
}

/* Loads the classificaiton tree used to classify classroom use.
 */
void Classroom::loadDecisionTree(const string& filename) {
    monitor->loadDecisionTree(filename.c_str());
}

/* Opens the camera of id.
 */
int Classroom::openCamera(int id) {
    if(cap.isOpened())
        cap.release();
    cap.open(id);
    if(!cap.isOpened())
        return -1;
    openMonitor();
    video = false;
    return 0;
}

/* Opens the video fileneame
 */
int Classroom::openVideo(const string &filename) {
    if(cap.isOpened())
        cap.release();
    cap.open(filename);
    if(!cap.isOpened())
        return -1;
    openMonitor();
    video = true;
    video_time = 16;
    return 0;
}

/* Creates and intilises an instance of the Monitor Class
 */
int Classroom::openMonitor() {
    cap >> frame;
    if(frame.empty()) {
        cout << "Could not capture video!" << endl;
        return -1;
    }
    monitor = new Monitor(frame, window);
    monitor->startAudio();
    monitor->loadDecisionTree("tree4.yaml");
    return 0;
}

/* Opens filename for recording data collection.
 */
int Classroom::openDataFile(const string& filename) {
    data.open(filename);
    if(!data.is_open()) {
        cout << "Could not open data file: \"" << filename << "\"." << endl;
        return -1;
    }
    return 0;
}

/*  Closes the data file.
 */
void Classroom::closeDataFile() {
    data.close();
}

/* Gets the next frame. If a video file is opened, a frame is taken once a second.
 */
int Classroom::getFrame() {
    if(video) {
        if(cap.get(CV_CAP_PROP_POS_FRAMES) >= cap.get(CV_CAP_PROP_FRAME_COUNT)) {
            cap.release();
            window->populateCameraList();
            return -1;
        }
        cap.set(CV_CAP_PROP_POS_MSEC, video_time * 1000);
        video_time += VIDEO_SAMPLE_RATE;
    }
    cap >> frame;
    if(frame.empty())
        return -1;
    return 0;
}

/* Runs when the classroom monitor is set to the Classroom Response System.
 * Detects hands and estimate motion to estimate voting patterns.
 */
void Classroom::vote() {
    difference = monitor->difference(&frame, motion, false);

    monitor->detectMotion(motion, frame, cropped);
    cvWaitKey(50);
    hands = detectAndDisplay(&cropped, &hand_cascade);
    cvWaitKey(50);
    if(question->isAsking()) {
        window->SetResults(question->currentQuestion(), question->currentResponse(hands));
        window->SetResults(question->currentQuestion(), question->currentMotion(difference));
    } else {
        if(window->isQuestionAsked())
            window->enableQuestions(true);
        else
            window->enableQuestions(false);
    }
    Mat dst;
    Size s(window->getVideoWidth(), window->getVideoHeight());
    cv::resize(frame, dst, s, 0, 0, CV_INTER_AREA );

    window->UpdateFrame(&dst);
}

/* Runs when the classroom monitor is set to the Activity Monitor.
 * Detects motion and measure volume level for activity predication.
 */
void Classroom::activity() {
    difference = monitor->difference(&frame, motion, true);
    average -= samples[sampleIndex];
    samples[sampleIndex] = difference / MOVEMENT_SAMPLES;
    average += samples[sampleIndex++];
    sampleIndex %= MOVEMENT_SAMPLES;

    window->setMovementLevel(difference);
    window->setMovementAverage(average);

    IplImage img = frame;
    window->UpdateFrame(&img);

    int act = monitor->getActivity(difference, stof(window->getAudioLevel()));
    const char *str;
    switch(act) {
    case 0:
        str = "No Activity";
        break;
    case 1:
        str = "Other";
        break;
    case 2:
        str = "Lecture";
        break;
    case 3:
        str = "Active Learning";
        break;
    default:
        str = "Unknown";
        break;
    }
    window->setActivity(str);
}

/* Called inside a loop.
 *
 */
void Classroom::update() {
    if(cap.isOpened()) {
        getFrame();
        if(window->getVideo())
            activity();
        else
            vote();
        recordData();
    }
    cvWaitKey(50);
}

void Classroom::setRecord(bool record) {
    this->record = record;
}

/* If data is being record write the current motion and volume levels to the data file.
 */
void Classroom::recordData() {
    if(record && data.is_open()) {
        time(&rawtime);
        timeinfo = localtime (&rawtime);

        data << myAsctime(timeinfo);
        data << std::fixed << std::setprecision(8) << difference;
        data << " ";
        data << std::fixed << std::setprecision(8) << average;
        data << " " << window->getAudioLevel() << " " << window->getAudioAverage() << endl;
    }
}

/* Converts time to a string.
 */
char* Classroom::myAsctime(const struct tm *timeptr) {
  static const char mon_name[][4] = {
    "Jan", "Feb", "Mar", "Apr", "May", "Jun",
    "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
  };
  static char result[26];
  sprintf(result, "%s%d-%.2d:%.2d:%.2d-%d: ",
    mon_name[timeptr->tm_mon],
    timeptr->tm_mday, timeptr->tm_hour,
    timeptr->tm_min, timeptr->tm_sec,
    1900 + timeptr->tm_year);
  return result;
}

/*  Counts the number of cameras connected to the computer.
 */
int Classroom::countCameras() {
    int n = 0;
    if(cap.isOpened())
        cap.release();
    while(true) {
       cap.open(n++);
       if(!cap.isOpened())
           break;
       cap.release();
    }
    cap.release();
    return n - 1;
}

