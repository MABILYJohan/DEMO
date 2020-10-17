#ifndef PREDATEUR_H
#define PREDATEUR_H

#include <iostream>
#include <QVector3D>
#include <QDebug>

#include "poisson.h"
#include <vector>
using namespace std;
#include "utils.h"

#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QMatrix4x4>

class Predateur : public Poisson
{
public:
    explicit Predateur(QVector3D pos, QVector3D vit, float taille);
    explicit Predateur(float xP, float yP, float zP, float xV, float yV, float zV, float taille);

    ~Predateur();

protected:
    //void build_vbo();
};

#endif // PREDATEUR_H
