// Basé sur :
// CC-BY Edouard.Thiel@univ-amu.fr - 22/01/2019

#ifndef GLAREA_H
#define GLAREA_H

#include <QKeyEvent>
#include <QTimer>
#include <QElapsedTimer>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>

#include "poisson.h"
#include "banc.h"

class GLArea : public QOpenGLWidget,
               protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit GLArea(QWidget *parent = nullptr);
    ~GLArea() override;

public slots:
    void on_set_separation(double newSep);
    void on_set_alignement(double newAl);
    void on_set_cohesion(double newCohes);

    void on_set_Red(int newR);
    void on_set_Green(int newG);
    void on_set_Blue(int newB);


signals:  // On ne les implémente pas, elles seront générées par MOC ;
          // les paramètres seront passés aux slots connectés.
    void separation_changed(double newSep);
    void alignement_changed(double newAl);
    void cohesion_changed(double newCohes);

    void red_changed(int newR);
    void green_changed(int newG);
    void blue_changed(int newB);

protected slots:
    void onTimeout();

protected:
    void initializeGL() override;
    void doProjection();
    void resizeGL(int w, int h) override;
    void paintGL() override;
    void keyPressEvent(QKeyEvent *ev) override;
    void keyReleaseEvent(QKeyEvent *ev) override;
    void mousePressEvent(QMouseEvent *ev) override;
    void mouseReleaseEvent(QMouseEvent *ev) override;
    void mouseMoveEvent(QMouseEvent *ev) override;

private:
    float xRot=20.0f, yRot=0.0f, zRot=0.0f;
    float xPos=0.0f,  yPos=0.0f, zPos=-50.0f;
    QTimer *myTimer = nullptr;
    QElapsedTimer elapsedTimer;
    float dt = 0;
    float windowRatio = 1.0f;
    QPoint lastPos;

    // OLD
    //QVector3D posSmoke;
//    float coefSep = 1.f;
//    float coefAl = 1.f;
//    float coefCohes = 1.f;
    QColor color;

    // NEW
    Poisson * myPoisson;
    Banc *myBanc;

    QOpenGLShaderProgram *program_sol;
    QOpenGLShaderProgram *program_particule;
    QOpenGLShaderProgram *program_poisson;
    QOpenGLShaderProgram *program_couleur;
    QOpenGLBuffer vbo_sol;
    QOpenGLBuffer vbo_particule;
    QOpenGLBuffer vbo_poisson;
    QOpenGLTexture *textures[3];

    void makeGLObjects();
    void tearGLObjects();

    void get_vbo();
};

#endif // GLAREA_H
