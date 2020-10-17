#ifndef BANC_H
#define BANC_H

#include <iostream>
#include <QVector3D>
#include <QDebug>

#include <QOpenGLTexture>

#include <vector>
using namespace std;
#include "utils.h"

#include "poisson.h"
#include "obstacle.h"
#include "predateur.h"

class Banc
{
public:
    explicit Banc(int nbPois, int nbOb, int nbPred, int l, int h, int p);

    void affiche_poissons(QOpenGLShaderProgram *spText, QMatrix4x4 modelMatrix,
                          QOpenGLTexture *textPoisson);
    void affiche_obstacles(QOpenGLShaderProgram *spText, QMatrix4x4 modelMatrix,
                           QOpenGLTexture *textObstacle);
    void affiche_predateurs(QOpenGLShaderProgram *spText, QMatrix4x4 modelMatrix,
                           QOpenGLTexture *textPredateur);
    void affiche_aquarium(QOpenGLShaderProgram *spColor, QMatrix4x4 modelMatrix);
    void affiche(QOpenGLShaderProgram *spBoite, QOpenGLShaderProgram *spPoissons, QOpenGLShaderProgram *spObstacles,
                 QMatrix4x4 modelMatrix,
                 QOpenGLTexture *textObstacle, QOpenGLTexture *textPoisson, QOpenGLTexture *textPredateur);
    void anime(float dt);

    QOpenGLBuffer _vbo();
    void _set_coefSep(float newCoef);
    void _set_coefAl(float newCoef);
    void _set_coefCohes(float newCoef);

protected:
    QVector3D position;
    int largeurAqua;
    int hauteurAqua;
    int profondeurAqua;
    float moyenneAqua;
    int nbVert;
    float coefBoite = 3.f;
    float coefSep = 1.f;
    float coefAl = 1.f;
    float coefCohes = 1.f;

    int nbPoissons;
    int nbObstacles;
    int nbPredateurs;
    //list<Poisson> listPoissons;
    vector<Poisson> listPoissons;
    vector<Obstacle> listObstacles;
    vector<Poisson> listPredateurs;

    QOpenGLBuffer vbo;

    void build_aquarium();
    void build_obstacles(int posVal);
    void build_poissons(int posVal);
    void build_predateurs(int posVal);

    void regler_comportement_simple_poisson(int numPoisson);
    QVector3D separation(int numP);
    void regler_comportement_poisson(int numP, float intLin);
    void regler_comportement_predateur(int numPred);
    void remettre_poisson_dans_aquarium(Poisson &myPoisson, float coef);
    void dessiner_aquarium();
};

#endif // BANC_H
