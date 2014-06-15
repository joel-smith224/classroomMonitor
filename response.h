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

#ifndef RESPONSE_H_
#define RESPONSE_H_

#include <ctime>

#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#define MAXIMUM_QUESTION_TIME 1000


using namespace cv;

class Question {
    private:
        std::clock_t start;
        double duration;
        bool asking;
        int question;
        int result;
        double motion;
        int count;
    public:
        Question ();
        ~Question ();
        int calculateResponse(int question);
        bool isAsking();
        int currentResponse(int hands);
        double currentMotion(double motion);
        int currentQuestion();
};


/**
 *  Detect the raised hands in the image, draw rectangles around them and then display the image.
 */

int detectAndDisplay( Mat *frame, CascadeClassifier *cas);


#endif /* RESPONSE_H_ */
