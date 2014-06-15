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

#include "response.h"

Question::Question() {
    this->asking = false;
    this->question = 0;
    this->result = 0;
}

Question::~Question() {

}

/* Begins asking a question.
 */
int Question::calculateResponse(int question) {
    if(this->asking)
        this->asking = false;
    else {
        this->asking = true;
        result = 0;
        motion = 0;
        count = 0;
        this->start = std::clock();
        this->question = question;
    }
    return 0;
}

/* Returns if a question is being asked or not.
 */
bool Question::isAsking() {
    duration = (std::clock() - start) / (double) CLOCKS_PER_SEC;
    if(duration >= MAXIMUM_QUESTION_TIME)
        asking = false;
    return asking;
}

/* Returns the current maximum number of hands that has been detected for this question.
 */
int Question::currentResponse(int hands) {
    if(hands > result)
        result = hands;
    return result;
}

/* Returns the average motion level for this question.
 */
double Question::currentMotion(double motion) {
    this->motion += motion;
    count++;
    return motion / count;
}

/* Returns the current question.
 */
int Question::currentQuestion() {
    return this->question;
}

/*  Detect the raised hands in the image, draw rectangles around them and then display the image.
 */
int detectAndDisplay(Mat *frame, CascadeClassifier *cas) {
    std::vector<Rect> hands;
    int count;

    // Detect Hands
    cas->detectMultiScale(*frame, hands, 1.1, 3, 0|CV_HAAR_SCALE_IMAGE, Size(10, 10), Size(80, 80));
    count = hands.size();

    for( int i = 0; i < count; i++ ) {
        Point pt1(hands[i].x + hands[i].width, hands[i].y + hands[i].height);
        Point pt2(hands[i].x, hands[i].y);

        rectangle(*frame, pt1, pt2, cvScalar(0, 255, 0, 0));
    }
    return count;
}
