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

#include <QApplication>
#include <cinttypes>

#include <assert.h>
#include <QWidget>
#include <QVBoxLayout>
#include "QOpenCVWidget.h"
#include "MyCameraWindow.h"

#include "ui_mainwindow.h"
#include "mainwindow.h"
#include "response.h"
#include "classroom.h"

#define MOVEMENT_SAMPLES 127


int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    MainWindow w;
    w.show();
    Classroom classroom(&w);

    while(true) {
        classroom.update();
        if(!w.isVisible())
            break;
    }
    classroom.~Classroom();
    cout << "Done" << endl;
    return 0;
}
