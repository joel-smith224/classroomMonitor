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

#include "mainwindow.h"
#include "classroom.h"

using namespace std;
using namespace cv;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
        ui->setupUi(this);
        this->videoMotion = false;
        questionAsked = false;
    }

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::UpdateFrame(cv::Mat *frame) {
    cvtColor(*frame, *frame, CV_BGR2RGB);
    QImage image = QImage((uchar *) frame->data, frame->cols, frame->rows, frame->step, QImage::Format_RGB888);
    ui->video_widget->setPixmap(QPixmap::fromImage(image));
}

void MainWindow::UpdateFrame(const IplImage *img) {
    QImage image = QImage((uchar *) img->imageData, img->width, img->height,  QImage::Format_Indexed8);
    ui->video_widget->setPixmap(QPixmap::fromImage(image));
}

QImage MainWindow::IplImage2QImage(const IplImage *iplImage) {
    int height = iplImage->height;
    int width = iplImage->width;

    const uchar *qImageBuffer = (const uchar*)iplImage->imageData;
    QImage img(qImageBuffer, width, height, QImage::Format_RGB888);
    return img.rgbSwapped();
}

void MainWindow::SetQuestion(Question *question) {
    this->question = question;
}

void MainWindow::SetClassroom(Classroom *classroom) {
    this->classroom = classroom;
}

void MainWindow::setAudioLevel(int level) {
    ui->audio_level->setText(QString::number(level));
}

void MainWindow::setAudioAverage(int level) {
    ui->average_audio->setText(QString::number(level));
}

void MainWindow::setMovementLevel(int level) {
    ui->movement_level->setText(QString::number(level));
}

void MainWindow::setMovementAverage(int level) {
    ui->movement_average->setText(QString::number(level));
}

string MainWindow::getAudioLevel() {
    return ui->audio_level->text().toStdString();
}

string MainWindow::getAudioAverage() {
    return ui->average_audio->text().toStdString();
}

string MainWindow::getMovementLevel() {
    return ui->movement_level->text().toStdString();
}

string MainWindow::getMovementAverage() {
    return ui->movement_average->text().toStdString();
}

void MainWindow::on_button_ask_clicked() {
    enableQuestions(true);
    ui->button_results->setEnabled(true);
    questionAsked = true;
    ui->results_a->setText("0");
    ui->results_b->setText("0");
    ui->results_c->setText("0");
    ui->results_d->setText("0");
    ui->label_hands->setText("Hands (Max)");
    ui->label_motion->setText("Motion (Mean)");
}

void MainWindow::SetResults(int question, int hands) {
    switch(question) {
    case 0:
        ui->results_a->setText(QString::number(hands));
        break;
    case 1:
        ui->results_b->setText(QString::number(hands));
        break;
    case 2:
        ui->results_c->setText(QString::number(hands));
        break;
    case 3:
        ui->results_d->setText(QString::number(hands));
        break;
    default:
        break;
    }
}

void MainWindow::SetResults(int question, double motion) {
    switch(question) {
    case 0:
        ui->motion_a->setText(QString::number(motion));
        break;
    case 1:
        ui->motion_b->setText(QString::number(motion));
        break;
    case 2:
        ui->motion_c->setText(QString::number(motion));
        break;
    case 3:
        ui->motion_d->setText(QString::number(motion));
        break;
    default:
        break;
    }
}

void MainWindow::setActivity(const char *activity) {
    ui->label_activity->setText(QString(activity));
}

void MainWindow::enableQuestions(bool enabled) {
    ui->button_a->setEnabled(enabled);
    ui->button_b->setEnabled(enabled);
    ui->button_c->setEnabled(enabled);
    ui->button_d->setEnabled(enabled);
}

void MainWindow::on_button_a_clicked() {
    this->question->calculateResponse(0);
    enableQuestions(false);
    ui->button_a->setEnabled(true);
}

void MainWindow::on_button_b_clicked() {
    this->question->calculateResponse(1);
    enableQuestions(false);
    ui->button_b->setEnabled(true);
}

void MainWindow::on_button_c_clicked() {
    this->question->calculateResponse(2);
    enableQuestions(false);
    ui->button_c->setEnabled(true);
}

void MainWindow::on_button_d_clicked() {
    this->question->calculateResponse(3);
    enableQuestions(false);
    ui->button_d->setEnabled(true);
}

bool MainWindow::getVideo() {
    return videoMotion;
}

bool MainWindow::isQuestionAsked() {
    return questionAsked;
}

void MainWindow::on_button_results_clicked(){
    questionAsked = false;
    double totalH = 0;
    double totalM = 0;
    double resultsH [4], percentH[4];
    double resultsM [4], percentM[4];
    resultsH[0] = ui->results_a->text().toDouble();
    resultsH[1] = ui->results_b->text().toDouble();
    resultsH[2] = ui->results_c->text().toDouble();
    resultsH[3] = ui->results_d->text().toDouble();
    resultsM[0] = ui->motion_a->text().toDouble();
    resultsM[1] = ui->motion_b->text().toDouble();
    resultsM[2] = ui->motion_c->text().toDouble();
    resultsM[3] = ui->motion_d->text().toDouble();
    for(int i = 0; i < 4; i++) {
        totalH += resultsH[i];
        totalM += resultsM[i];
    }
    for(int i =0; i < 4; i++) {
        percentH[i] = 100 * resultsH[i] / totalH;
        percentM[i] = 100 * resultsM[i] / totalM;
    }
    ui->results_a->setText(QString::number((percentH[0] + percentM[0]) / 2) + "%");
    ui->results_b->setText(QString::number((percentH[1] + percentM[1]) / 2) + "%");
    ui->results_c->setText(QString::number((percentH[2] + percentM[2]) / 2) + "%");
    ui->results_d->setText(QString::number((percentH[3] + percentM[3]) / 2) + "%");
    ui->motion_a->setText("");
    ui->motion_b->setText("");
    ui->motion_c->setText("");
    ui->motion_d->setText("");
    ui->label_hands->setText("Results (%)");
    ui->label_motion->setText("");
    enableQuestions(false);
    ui->button_results->setEnabled(false);
    question->~Question();
}

void MainWindow::closeEvent(QCloseEvent *event) {
    event->accept();
}

void MainWindow::on_tabWidget_tabBarClicked(int index) {
    if(index)
        videoMotion = true;
    else
        videoMotion = false;
}

void MainWindow::on_actionExit_triggered() {
    MainWindow::close();
}

void MainWindow::on_actionOpen_Cascade_triggered() {
    QString filename = QFileDialog::getOpenFileName(this, tr("Open Cascade"), "", tr("Files (*.*)"));
    classroom->loadHandCascade(filename.toStdString());
}

void MainWindow::on_actionRecord_triggered() {
    Record *recordDialog = new Record();
    recordDialog->setClassroom(classroom);
    recordDialog->show();
}

void MainWindow::on_cameraList_itemDoubleClicked(QListWidgetItem *item) {
    on_button_selectcamera_clicked();
}

void MainWindow::populateCameraList() {
    ui->cameraList->clear();
    for(int i = 0; i < classroom->countCameras(); i++)
        ui->cameraList->addItem("Camera " + QString::number(i));
    ui->video_widget->hide();
    ui->select_camera_widget->show();
}

void MainWindow::on_actionOpen_Video_triggered() {
    populateCameraList();
}

void MainWindow::on_button_selectcamera_clicked() {
    if(classroom->openCamera(ui->cameraList->currentIndex().row()) == 0) {
        ui->select_camera_widget->hide();
        ui->video_widget->show();
    }
}

void MainWindow::on_button_openvideo_clicked() {
    QString filename = QFileDialog::getOpenFileName(this, tr("Open Video"), "", tr("Files (*.*)"));
    if(classroom->openVideo(filename.toStdString()) == 0) {
        ui->select_camera_widget->hide();
        ui->video_widget->show();
    }
}


int MainWindow::getVideoHeight() {
    return ui->video_widget->height();
}

int MainWindow::getVideoWidth() {
    return ui->video_widget->width();
}

void MainWindow::on_actionOpen_Classifier_triggered() {
    QString filename = QFileDialog::getOpenFileName(this, tr("Open Clasifier"), "", tr("Files (*.*)"));
    classroom->loadDecisionTree(filename.toStdString());
}
