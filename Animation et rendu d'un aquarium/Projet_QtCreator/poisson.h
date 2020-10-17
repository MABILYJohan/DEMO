#ifndef POISSON_H
#define POISSON_H

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

class Poisson
{
public:
    explicit Poisson(QVector3D pos, QVector3D vit, float t);
    explicit Poisson(float xP, float yP, float zP,
                     float xV, float yV, float zV,
                     float t);

    virtual ~Poisson();

    virtual QVector3D _vitesse();
    virtual QVector3D _old_vit();
    virtual QVector3D _position();
    virtual float _vitesse_max();
    virtual float _rayon();
    virtual QOpenGLBuffer _vbo();
    virtual QVector<GLfloat> _vertData();

    virtual void _set_vitesse(QVector3D v);
    virtual void _set_old_vit(QVector3D v);
    virtual void _set_position(QVector3D p);

    virtual void anime(float dt);
    virtual bool dans_voisinage( QVector3D p, float coef = 1.f);
    virtual void rotate(QMatrix4x4 &modelMatrix);
    virtual void affiche(QOpenGLShaderProgram *sp, QMatrix4x4 modelMatrix);

    virtual void demi_tour (float coordType, float dist, bool forcer=false);
    virtual void demi_tour (float distX, float distY, float distZ);
    virtual void demi_tour ();

    float angle_de_test=0.f;


protected:
    QVector3D position;
    // QVector3D oldPos;
    QVector3D vitesse;
    QVector3D oldVit;
    float vitesseMax;   // val/sec  ("mètre par secondes")
    float taille;
    float rayon;
    QVector<GLfloat> vertData;
    QOpenGLBuffer vbo;

    virtual void build_vbo();
};

#endif // POISSON_H
