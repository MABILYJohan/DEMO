#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "dialoghelp.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

private:
	Ui::MainWindow *ui;

private slots:
    void on_check(bool value);
    void on_slider(int value);
    void on_dbl_spinBox(double value);

protected slots:
    void on_help();

};

#endif // MAINWINDOW_H
