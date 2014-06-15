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

#include "record.h"
#include "ui_record.h"

Record::Record(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Record)
{
    ui->setupUi(this);
}

Record::~Record() {
    delete ui;
}

void Record::setClassroom(Classroom *classroom) {
    this->classroom = classroom;
}

void Record::on_button_close_clicked() {
    record = false;
    classroom->setRecord(false);
    classroom->closeDataFile();
    Record::close();
}

void Record::on_button_browse_clicked() {
    QString filename = QFileDialog::getSaveFileName(this, tr("Save As"), "", tr("Files (*.*)"));
    if(classroom->openDataFile(filename.toStdString()) == 0) {
        QFileInfo file(filename);
        ui->label_filename->setText(file.fileName());
        ui->button_record->setEnabled(true);
    } else
        ui->label_filename->setText("Could not open File");
}

void Record::on_button_record_clicked() {
    if(record)
        ui->button_record->setText("Record");
    else
        ui->button_record->setText("Stop");
    classroom->setRecord(!record);
    record = !record;
}
