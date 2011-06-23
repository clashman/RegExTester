#include "mainwindow.hpp"
#include "ui_mainwindow.h"

#include <QDebug>

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

        if (rx.pattern().isEmpty())
            backgroundColor = Qt::white;
    }

    palette.setColor(QPalette::Base, backgroundColor);
    ui->lineEdit->setPalette(palette);

    if (rx.exactMatch(QString::null))
        ui->statusBar->showMessage("pattern matches empty strings");

    if (rx.isValid() && !rx.pattern().isEmpty()) {
        QList<QTextEdit::ExtraSelection> matches;

        int pos = 0;
        while ((pos = rx.indexIn(ui->textEdit->toPlainText(), pos)) != -1) {
            QTextEdit::ExtraSelection highlight;
            highlight.cursor = ui->textEdit->textCursor();
            highlight.cursor.setPosition(pos);
            highlight.cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, rx.matchedLength());
            highlight.format.setBackground(Qt::green);

            matches << highlight;

            pos += rx.matchedLength();
            //no deadlocks in case of matching empty string
            if (!rx.matchedLength())
                pos++;
        }
        ui->textEdit->setExtraSelections(matches);
    }
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
