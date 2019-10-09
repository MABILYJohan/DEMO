/* R. Raffin
 * M1 Informatique, Aix-Marseille Université
 * Fenêtre principale
 * Au cas où, l'UI contient une barre de menu, une barre de status, une barre d'outils (cf QMainWindow).
 * Une zone est laissée libre à droite du Widget OpenGL pour mettre de futurs contrôles ou informations.
 */

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

    connect ( ui->doubleSpinBoxU, SIGNAL(valueChanged(double)), this, SLOT(on_dbl_spinBox(double)) );
    connect ( ui->doubleSpinBoxV, SIGNAL(valueChanged(double)), this, SLOT(on_dbl_spinBox(double) ));

    connect ( ui->horizontalSliderU, SIGNAL(valueChanged(int)), this, SLOT(on_slider(int)) );
    connect ( ui->horizontalSliderV, SIGNAL(valueChanged(int)), this, SLOT(on_slider(int)) );

    connect ( ui->checkBox_PtCtrl, SIGNAL(clicked(bool)), this, SLOT(on_check(bool)) );
    connect ( ui->checkBoxTrianguler, SIGNAL(clicked(bool)), this, SLOT(on_check(bool)) );
    connect ( ui->checkBox_Points, SIGNAL(clicked(bool)), this, SLOT(on_check(bool)) );
    connect ( ui->checkBox_Aretes, SIGNAL(clicked(bool)), this, SLOT(on_check(bool)) );
    connect ( ui->checkBox_Carreaux, SIGNAL(clicked(bool)), this, SLOT(on_check(bool)) );
    connect ( ui->checkBox_surface, SIGNAL(clicked(bool)), this, SLOT(on_check(bool)) );

    connect( ui->actionA_propos, SIGNAL(triggered()), this, SLOT(on_help()));
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::on_check(bool value)
{
    qDebug() << __FUNCTION__ << "The event sender is" << sender();

    // PTS DE CONTROLE
    if(sender()->objectName() == "checkBox_PtCtrl")
    {
        qDebug() << "emit ui->openGLWidget->flag_pt_ctrl_changed("
                 << value << ");";
        emit ui->openGLWidget->flag_pt_ctrl_changed(value);
    }
    // TRIANGULER
    else if(sender()->objectName() == "checkBoxTrianguler")
    {
        qDebug() << "emit ui->openGLWidget->flag_triangulation_changed("
                 << value << ");";
        emit ui->openGLWidget->flag_triangulation_changed(value);
    }
    // VERTICES
    else if(sender()->objectName() == "checkBox_Points")
    {
        qDebug() << "emit ui->openGLWidget->flag_points_changed("
                 << value << ");";
        emit ui->openGLWidget->flag_vertices_changed(value);
    }
    // EDGES
    else if(sender()->objectName() == "checkBox_Aretes")
    {
        qDebug() << "emit ui->openGLWidget->flag_edges_changed("
                 << value << ");";
        emit ui->openGLWidget->flag_edges_changed(value);
    }
    // CARREAUX
    else if(sender()->objectName() == "checkBox_Carreaux")
    {
        qDebug() << "emit ui->openGLWidget->flag_carreaux_changed("
                 << value << ");";
        emit ui->openGLWidget->flag_carreaux_changed(value);
    }
    // SURFACE
    else if(sender()->objectName() == "checkBox_surface")
    {
        qDebug() << "emit ui->openGLWidget->flag_forme_changed("
                 << value << ");";
        emit ui->openGLWidget->flag_forme_changed(value);
    }
}

void MainWindow::on_slider(int value)
{
    qDebug() << __FUNCTION__ << "The event sender is" << sender();
    if (sender()->objectName() == "horizontalSliderU")
    {
        qDebug() << "emit ui->openGLWidget->nbU_changed(value)";
        emit ui->openGLWidget->nbU_changed(value);
    }
    else
    {
        qDebug() << "emit ui->openGLWidget->nbV_changed(value)";
        emit ui->openGLWidget->nbV_changed(value);
    }
}

void MainWindow::on_dbl_spinBox(double value)
{
    qDebug() << __FUNCTION__ << "The event sender is" << sender();
    if (sender()->objectName() == "doubleSpinBoxU") {
        qDebug() << "  emit ui->openGLWidget->u_changed(" << value <<")";
        emit ui->openGLWidget->u_changed(value);
    }
    else if (sender()->objectName() == "doubleSpinBoxV") {
        qDebug() << "  emit ui->openGLWidget->v_changed(" << value <<")";
        emit ui->openGLWidget->v_changed(value);
    }
}

void MainWindow::on_help()
{
    DialogHelp dlh;
    if (dlh.exec()) {
    }
    else {
        ;
    }
}
