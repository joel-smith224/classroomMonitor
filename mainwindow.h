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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QVBoxLayout>
#include <QKeyEvent>
#include <QFileDialog>
#include <opencv/cv.h>
#include <opencv/highgui.h>

#include "QOpenCVWidget.h"
#include "ui_mainwindow.h"
#include "record.h"
#include "response.h"


class Classroom;

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = 0);
        ~MainWindow();
        void UpdateFrame(cv::Mat *frame);
        void UpdateFrame(const IplImage *img);
        void SetQuestion(Question *question);
        void SetClassroom(Classroom *classroom);
        void SetResults(int question, int hands);
        void SetResults(int question, double motion);
        void enableQuestions(bool enabled);
        void setAudioLevel(int level);
        void setAudioAverage(int level);
        void setMovementLevel(int level);
        void setMovementAverage(int level);
        void setActivity(const char* activity);
        string getAudioLevel();
        string getAudioAverage();
        string getMovementLevel();
        string getMovementAverage();
        bool getVideo();
        bool isQuestionAsked();
        void populateCameraList();
        int getVideoHeight();
        int getVideoWidth();

    private slots:
        QImage IplImage2QImage(const IplImage *iplImage);
        void on_button_ask_clicked();
        void on_button_a_clicked();
        void on_button_b_clicked();
        void on_button_c_clicked();
        void on_button_d_clicked();

        void on_button_results_clicked();

        void closeEvent(QCloseEvent *event);

        void on_tabWidget_tabBarClicked(int index);

        void on_actionExit_triggered();

        void on_actionOpen_Cascade_triggered();

        void on_actionRecord_triggered();

        void on_cameraList_itemDoubleClicked(QListWidgetItem *item);

        void on_actionOpen_Video_triggered();

        void on_button_selectcamera_clicked();

        void on_button_openvideo_clicked();

        void on_actionOpen_Classifier_triggered();

private:
        Ui::MainWindow *ui;
        QOpenCVWidget *cvwidget;
        Question *question;
        Classroom *classroom;
        bool videoMotion;
        bool questionAsked;
};



#endif // MAINWINDOW_H
