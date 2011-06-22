#include "mainwindow.hpp"
#include "ui_mainwindow.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->lineEdit, SIGNAL(textChanged(QString)), this, SLOT(inputChanged(QString)));
    connect(ui->textEdit, SIGNAL(textChanged()), this, SLOT(textChanged()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::inputChanged(QString text)
{
    rx.setPattern(text);
    QColor backgroundColor = Qt::green;

    if (!rx.isValid() || text.isEmpty()) {
        backgroundColor = Qt::red;
        QList<QTextEdit::ExtraSelection> empty;
        ui->textEdit->setExtraSelections(empty);
    }

    if (text.isEmpty())
        backgroundColor = Qt::white;

    palette.setColor(QPalette::Base, backgroundColor);
    ui->lineEdit->setPalette(palette);

    if (rx.isValid() && !text.isEmpty()) {
        QList<QTextEdit::ExtraSelection> matches;

        int pos = 0;
        while ((pos = rx.indexIn(ui->textEdit->toPlainText(), pos)) != -1) {
            qDebug() << pos;

            QTextEdit::ExtraSelection highlight;
            highlight.cursor = ui->textEdit->textCursor();
            highlight.cursor.setPosition(pos);
            highlight.cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, rx.matchedLength());
            highlight.format.setBackground(Qt::green);

            matches << highlight;

            pos += rx.matchedLength();
        }
        ui->textEdit->setExtraSelections(matches);
    }
}

void MainWindow::textChanged()
{
    inputChanged(ui->lineEdit->text());
}
