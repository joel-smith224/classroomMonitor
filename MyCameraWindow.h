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

#ifndef MYCAMERAWINDOW_H_
#define MYCAMERAWINDOW_H_

#include <QWidget>
#include <QVBoxLayout>
#include "QOpenCVWidget.h"
#include <opencv/cv.h>
#include <opencv/highgui.h>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>


class MyCameraWindow : public QWidget
{
    Q_OBJECT
    private:
        QOpenCVWidget *cvwidget;
        CvCapture *camera;

    public:
        MyCameraWindow(QWidget *parent=0);

    public:
        void UpdateFrame(cv::Mat *frame);
};


#endif /*MYCAMERAWINDOW_H_*/
