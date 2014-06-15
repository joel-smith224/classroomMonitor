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

#include "MyCameraWindow.h"

MyCameraWindow::MyCameraWindow(QWidget *parent) : QWidget(parent) {
    QVBoxLayout *layout = new QVBoxLayout;
    cvwidget = new QOpenCVWidget(this);
    layout->addWidget(cvwidget);
    setLayout(layout);
    resize(500, 400);

    startTimer(100);  // 0.1-second timer
 }


void MyCameraWindow::UpdateFrame(cv::Mat *frame) {
    cvwidget->putImage(frame);
}


