#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <QElapsedTimer>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>

#include <QMatrix4x4>

#include <iostream>
#include <QVector3D>
#include <QDebug>

#include <vector>
using namespace std;
#include "utils.h"

class Obstacle
{
public:
    explicit Obstacle(QVector3D pos, float r);
    explicit Obstacle(float xP, float yP, float zP, float r);

    QOpenGLBuffer _vbo();
    QVector3D _position();

    void affiche(QOpenGLShaderProgram *sp, QMatrix4x4 modelMatrix);

protected:
    QVector3D position;
    float rayon;
    QOpenGLBuffer vbo;

    void init();
};

#endif // OBSTACLE_H
