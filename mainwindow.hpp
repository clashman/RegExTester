#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>

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
    void inputChanged(QString text);
    void textChanged();

private:
    Ui::MainWindow *ui;
    QPalette palette;
    QRegExp rx;
};

#endif // MAINWINDOW_HPP
