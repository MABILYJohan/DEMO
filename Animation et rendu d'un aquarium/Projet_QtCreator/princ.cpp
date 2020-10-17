// Basé sur :
// CC-BY Edouard.Thiel@univ-amu.fr - 22/01/2019

#include "princ.h"
#include <QDebug>

Princ::Princ(QWidget *parent) : QMainWindow(parent)
{
    setupUi(this);

    connect ( doubleSpinBox_separation, SIGNAL(valueChanged(double)), this, SLOT(on_dbl_spinBox(double)) );
    connect ( doubleSpinBox_alignement, SIGNAL(valueChanged(double)), this, SLOT(on_dbl_spinBox(double)) );
    connect ( doubleSpinBox_cohesion, SIGNAL(valueChanged(double)), this, SLOT(on_dbl_spinBox(double)) );

//    connect ( spinBox_R, SIGNAL(valueChanged(int)), this, SLOT(on_spinBox(int)) );
//    connect ( spinBox_G, SIGNAL(valueChanged(int)), this, SLOT(on_spinBox(int)) );
//    connect ( spinBox_B, SIGNAL(valueChanged(int)), this, SLOT(on_spinBox(int)) );

}

void Princ::on_dbl_spinBox(double value)
{
    qDebug() << __FUNCTION__ << "The event sender is" << sender();

    if (sender()->objectName() == "doubleSpinBox_separation") {
        qDebug() << "  emit glarea->separation_changed(" << value <<")";
        emit glarea->separation_changed(value);
    }
    else if (sender()->objectName() == "doubleSpinBox_alignement") {
        qDebug() << "  emit glarea->alignement_changed(" << value <<")";
        emit glarea->alignement_changed(value);
    }
    else if (sender()->objectName() == "doubleSpinBox_cohesion") {
        qDebug() << "  emit glarea->cohesion_changed(" << value <<")";
        emit glarea->cohesion_changed(value);
    }
}

void Princ::on_spinBox(int value)
{
    qDebug() << __FUNCTION__ << "The event sender is" << sender();

    if (sender()->objectName() == "spinBox_R") {
        qDebug() << "  emit glarea->red_changed(" << value <<")";
        emit glarea->red_changed(value);
    }
    else if (sender()->objectName() == "spinBox_G") {
        qDebug() << "  emit glarea->green_changed(" << value <<")";
        emit glarea->green_changed(value);
    }
    else if (sender()->objectName() == "spinBox_B") {
        qDebug() << "  emit glarea->blue_changed(" << value <<")";
        emit glarea->blue_changed(value);
    }
}
