/*************************************************************************
 * RegExTester: matches a regular expression against a specifiable text  *
 *              as you type                                              *
 *                                                                       *
 * Copyright (C) 2011 Clemens Werther                                    *
 *                                                                       *
 * This program is free software: you can redistribute it and/or modify  *
 * it under the terms of the GNU General Public License as published by  *
 * the Free Software Foundation; either version 3 of the License, or     *
 * (at your option) any later version.                                   *
 *                                                                       *
 * This program is distributed in the hope that it will be useful,       *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 * GNU General Public License for more details.                          *
 *                                                                       *
 * You should have received a copy of the GNU General Public License     *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 *************************************************************************/

#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QTextEdit>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void inputChanged();

    void patternSyntaxChanged(int index);
    void caseSensitivityToggled(bool toggled);
    void minimalToggled(bool toggled);
private:
    void updateView();
    QTextEdit::ExtraSelection createExtraSelection(int start, int length, QColor foreground, QColor background);

    Ui::MainWindow *ui;
    QPalette palette;
    QRegExp rx;
};

#endif // MAINWINDOW_HPP
