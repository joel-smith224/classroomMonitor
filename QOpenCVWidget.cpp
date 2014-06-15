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

#include "QOpenCVWidget.h"

QOpenCVWidget::QOpenCVWidget(QWidget *parent) : QWidget(parent) {
    layout = new QVBoxLayout;
    imagelabel = new QLabel;
    QImage dummy(100,100,QImage::Format_RGB32);
    image = dummy;
    layout->addWidget(imagelabel);
    for (int x = 0; x < 100; x ++) {
        for (int y =0; y < 100; y++) {
            image.setPixel(x,y,qRgb(x, y, y));
        }
    }
    imagelabel->setPixmap(QPixmap::fromImage(image));

    setLayout(layout);
}

QOpenCVWidget::~QOpenCVWidget(void) {

}

void QOpenCVWidget::putImage(cv::Mat *frame) {
    cvtColor(*frame, *frame, CV_BGR2RGB);
    QImage image = QImage((uchar*) frame->data, frame->cols, frame->rows, frame->step, QImage::Format_RGB888);
    imagelabel->setPixmap(QPixmap::fromImage(image));
}

