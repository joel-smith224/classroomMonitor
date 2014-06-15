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

#ifndef RECORD_H
#define RECORD_H

#include <QDialog>
#include <QFileDialog>
#include <QFileInfo>

#include "classroom.h"

namespace Ui {
class Record;
}

class Record : public QDialog
{
    Q_OBJECT

public:
    explicit Record(QWidget *parent = 0);
    ~Record();
    void setClassroom(Classroom *classroom);

private slots:
    void on_button_close_clicked();

    void on_button_browse_clicked();

    void on_button_record_clicked();

private:
    Ui::Record *ui;
    Classroom *classroom;
    bool record = false;
};

#endif // RECORD_H
