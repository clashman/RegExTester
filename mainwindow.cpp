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

#include "mainwindow.hpp"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QStringList items;
    items << "RegExp" << "RegExp2" << "Wildcard" << "WildcardUnix" << "FixedString" << "W3CXmlSchema11";
    ui->patternSyntax->insertItems(0, items);
    updateView();

    connect(ui->lineEdit, SIGNAL(textChanged(QString)), this, SLOT(inputChanged()));
    connect(ui->textEdit, SIGNAL(textChanged()), this, SLOT(inputChanged()));

    connect(ui->caseSensitivity, SIGNAL(toggled(bool)), this, SLOT(caseSensitivityToggled(bool)));
    connect(ui->minimal, SIGNAL(toggled(bool)), this, SLOT(minimalToggled(bool)));
    connect(ui->patternSyntax, SIGNAL(currentIndexChanged(int)), this, SLOT(patternSyntaxChanged(int)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateView()
{
    ui->caseSensitivity->setCheckState(rx.caseSensitivity() ? Qt::Checked : Qt::Unchecked);
    ui->minimal->setCheckState(rx.isMinimal() ? Qt::Checked : Qt::Unchecked);
    ui->patternSyntax->setCurrentIndex(rx.patternSyntax());

    inputChanged();
}

void MainWindow::inputChanged()
{
    rx.setPattern(ui->lineEdit->text());
    ui->statusBar->showMessage(rx.errorString());

    QColor backgroundColor = Qt::green;

    if (!rx.isValid() || rx.pattern().isEmpty()) {
        backgroundColor = Qt::red;
        QList<QTextEdit::ExtraSelection> empty;
        ui->textEdit->setExtraSelections(empty);
    }

    if (rx.exactMatch(QString::null)) {
        ui->statusBar->showMessage("pattern matches empty strings");
        backgroundColor = Qt::yellow;
    }

    if (rx.pattern().isEmpty())
        backgroundColor = Qt::white;

    palette.setColor(QPalette::Base, backgroundColor);
    ui->lineEdit->setPalette(palette);


    if (rx.isValid() && !rx.pattern().isEmpty()) {
        QList<QTextEdit::ExtraSelection> matches;

        QColor backgroundColor = Qt::black;
        QColor foregroundColor = Qt::gray;

        int count = 0;
        int pos = 0;
        while ((pos = rx.indexIn(ui->textEdit->toPlainText(), pos)) != -1) {
            matches << createExtraSelection(pos, rx.matchedLength(), foregroundColor, backgroundColor);

            QColor capForegroundColor = Qt::cyan;
            for (int n = 1; n <= rx.captureCount(); n++) {
                matches << createExtraSelection(rx.pos(n), rx.cap(n).size(), capForegroundColor, backgroundColor);
                capForegroundColor = capForegroundColor == Qt::cyan ? Qt::magenta : Qt::cyan;
            }

            pos += rx.matchedLength();
            //no deadlocks in case of matching empty string
            if (!rx.matchedLength())
                pos++;

            count++;
            backgroundColor = backgroundColor == Qt::gray ? Qt::black : Qt::gray;
            foregroundColor = foregroundColor == Qt::gray ? Qt::black : Qt::gray;
        }

        QString foundMuliple = count == 1 ? QString() : "es";
        ui->statusBar->showMessage(QString::number(count) + " match" + foundMuliple);
        ui->textEdit->setExtraSelections(matches);
    }
}

QTextEdit::ExtraSelection MainWindow::createExtraSelection(int start, int length, QColor foreground, QColor background)
{
    QTextEdit::ExtraSelection highlight;
    highlight.cursor = ui->textEdit->textCursor();
    highlight.cursor.setPosition(start);
    highlight.cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, length);
    highlight.format.setBackground(background);
    highlight.format.setForeground(foreground);

    return highlight;
}

void MainWindow::patternSyntaxChanged(int index)
{
    rx.setPatternSyntax((QRegExp::PatternSyntax)index);
    updateView();
}

void MainWindow::caseSensitivityToggled(bool toggled)
{
    rx.setCaseSensitivity(toggled ? Qt::CaseSensitive : Qt::CaseInsensitive);
    updateView();
}

void MainWindow::minimalToggled(bool toggled)
{
    rx.setMinimal(toggled);
    updateView();
}
