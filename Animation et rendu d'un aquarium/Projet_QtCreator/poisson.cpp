#include "poisson.h"
#include <cmath>
#include "convertvbo.h"

//////////////////////// CONSTRUCTEURS ///////////////////////////////////

Poisson::Poisson(QVector3D pos, QVector3D vit, float t) :
    position(pos), vitesse(vit), taille(t)
{
//    this ->position = pos;
//    this ->vitesse = vitesse;
//    this->taille = taille;
    build_vbo();
}

Poisson::Poisson(float xP, float yP, float zP,
                 float xV, float yV, float zV,
                 float t) :
    position(xP, yP, zP), vitesse(xV, yV, zV), taille(t)
{
    build_vbo();
}

void Poisson::build_vbo()
{
//    qDebug() << "\t<Poisson::" << __FUNCTION__ << ">";

    //oldPos = position;
//    rayon = taille * 3.f;
    rayon = 10.f;
    vitesseMax = 1.f;
    //vitesse[0] = 0.5f;
//    vitesse[0] = 0.0f;
//    vitesse[1] = 0.0f;
//    vitesse[2] = 0.0f;

    float z = 0.f;
    // vertices
    QVector3D pA(0.f, 1.f, 0.f);
    QVector3D pB(0.f, 0.f, 0.f);
    QVector3D pC(1.f, 0.f, 0.f);

    QVector3D pAz(0.f, 1.f, 0.2f);
    QVector3D pBz(0.f, 0.f, 0.2f);
    QVector3D pCz(1.f, 0.f, 0.2f);

    vector<float> vertices = {
        /////////////////   2D  //////////////////
        /*
        pA[0], pA[1], pA[2],
        pB[0], pB[1], pB[2],
        pC[0], pC[1], pC[2],
        */

        /////////////////   3D  //////////////////
        // Face 1
        pA[0], pA[1], pA[2],
        pB[0], pB[1], pB[2],
        pC[0], pC[1], pC[2],
        // Face 2
        pAz[0], pAz[1], pAz[2],
        pBz[0], pBz[1], pBz[2],
        pCz[0], pCz[1], pCz[2],

        // Cote 1
        pA[0], pA[1], pA[2],
        pAz[0], pAz[1], pAz[2],
        pCz[0], pCz[1], pCz[2],
        pC[0], pC[1], pC[2],
        // Cote 2
        pA[0], pA[1], pA[2],
        pB[0], pB[1], pB[2],
        pBz[0], pBz[1], pBz[2],
        pAz[0], pAz[1], pAz[2],
        // Cote 3
        pB[0], pB[1], pB[2],
        pBz[0], pBz[1], pBz[2],
        pCz[0], pCz[1], pCz[2],
        pC[0], pC[1], pC[2],
    };
    vector<float> texCoords = {
        /////////////////   2D  //////////////////
        /*
        0.f, 1.f,
        0.f, 0.f,
        1.f, 0.f,
        */

        /////////////////   3D  //////////////////
        // Face 1
        0.f, 1.f,
        0.f, 0.f,
        1.f, 0.f,
        // Face 2
        0.f, 1.f,
        0.f, 0.f,
        1.f, 0.f,

        // Cote 1
        0.f, 0.1f,
        0.f, 0.1f,
        0.f, 0.1f,
        0.f, 0.1f,
        // Cote 2
        0.f, 0.1f,
        0.f, 0.1f,
        0.f, 0.1f,
        0.f, 0.1f,
        // Cote 3
        0.f, 0.1f,
        0.f, 0.1f,
        0.f, 0.1f,
        0.f, 0.1f,
    };

    ConvertVbo::convert_coords_to_vbo(vertData,
                                      vertices, 3,
                                      texCoords, 2);
    vbo.create();
    vbo.bind();
    vbo.allocate(vertData.constData(), vertData.count() * int(sizeof(GLfloat)));
//    qDebug() << "\t</Poisson::" << __FUNCTION__ << ">";
}

Poisson::~Poisson ()
{
    ;
}


QVector3D Poisson::_position()          {   return position;    }
QVector3D Poisson::_vitesse()           {   return vitesse;     }
QVector3D Poisson::_old_vit()           {   return oldVit;      }
float Poisson::_vitesse_max()           {   return vitesseMax;  }
float Poisson::_rayon()                 {   return rayon;       }
QOpenGLBuffer Poisson::_vbo()           {   return vbo;         }
QVector<GLfloat> Poisson::_vertData()   {   return vertData;    }

void Poisson::_set_vitesse(QVector3D v) {   vitesse=v;  }
void Poisson::_set_old_vit(QVector3D v) {   oldVit=v;   }
void Poisson::_set_position(QVector3D p){   position=p; }


//////////////////////// METHODES ///////////////////////////////////

void Poisson::anime(float dt)
{
    //oldPos = position;
    position = position + vitesse * dt;
}

bool Poisson::dans_voisinage(QVector3D p, float coef)
{
    if (position.distanceToPoint(p) <= rayon*coef) {
        //qDebug() << __FUNCTION__ << ": " << position.distanceToPoint(p) ;
        return true;
    }
    return false;
}

void Poisson::demi_tour(float coordType, float dist, bool forcer)
{
    if (forcer) {
        if (position[coordType] > 0) position[coordType] -= dist;
        else if (position[coordType] <= 0) position[coordType] += dist;
    }
    vitesse[coordType] = -vitesse[coordType];
}

void Poisson::demi_tour(float distX, float distY, float distZ)
{
    demi_tour(0, distX);
    demi_tour(1, distY);
    demi_tour(2, distZ);
}

void Poisson::demi_tour()
{
    demi_tour(0, rayon);
    demi_tour(1, rayon);
    demi_tour(2, rayon);
}

void Poisson::rotate(QMatrix4x4 &modelMatrix)
{
    // SOLUCE 1
    QVector3D myP0 = position + position;
    myP0 = myP0 - position;
    //myP0 = QVector3D::crossProduct(position, myP0);
    myP0.normalize();
    QVector3D myP1 = position + vitesse;
    myP1 = myP1 - position;
    //myP1 = QVector3D::crossProduct(position, myP1);
    myP1.normalize();

    float angle = QVector3D::dotProduct(myP0, myP1);
    angle = angle * 180.f/M_PI;
    //angle_de_test += angle;
    //qDebug() << "angle = " << angle;
    //float angle = acos((myP0 | myP1));
    //    modelMatrix.rotate(angle_de_test, 0.f, 1.f, 0.f);
    //    modelMatrix.rotate(angle_de_test, 0.f, 0.f, 1.f);
    modelMatrix.rotate(angle-90.f, 1.f , 0.f , 1.f);
    // modelMatrix.rotate(angle, 1.f , 0.f , 1.f);


    // SOLUCE 2
    /*
    float angle_x, angle_y, angle_z;
    if (position[0] + vitesse[0] < position[0]) angle_x = 180.f;
    else                                        angle_x = 0;

    if (position[1] + vitesse[1] < position[1]) angle_y = -90.f;
    else                                        angle_y = 0;

    if (position[2] + vitesse[2] < position[2]) angle_z = 90.f;
    else                                        angle_z = -90.f;

    float angle1 = (angle_x + angle_z) / 2.f;
    modelMatrix.rotate(angle1, 0.f, 1.f, 0.f);
    //qDebug () << "angle1 = " << angle1;

//    float angle2 = (angle_x + angle_y +  angle_z) / 3.f;;
//    modelMatrix.rotate(angle2, 0.f, 0.f, 1.f);

    // modelMatrix.rotate(angle_z, 1.f, 0.f, 0.f);
    // modelMatrix.rotate(1.f, vitesse);
    */


    // SOLUCE 3
    /*
    QMatrix4x4 tmpM;
    tmpM.setToIdentity();

    float angle = static_cast<float>(atan2(vitesse.x(), -vitesse.y()) * 180 / M_PI);
    tmpM.rotate(angle-90.f, 1.f , 0.f , 1.f);

    modelMatrix.translate(position);

    modelMatrix *= tmpM;
    */
}



void Poisson::affiche(QOpenGLShaderProgram *sp, QMatrix4x4 modelMatrix)
{
    vbo.bind();
    sp->bind();
    modelMatrix.translate(position);

    rotate(modelMatrix);

    sp->setUniformValue("modelMatrix", modelMatrix);
    sp->setUniformValue("poissonTaille", taille);

    /////////////   Version 2D    /////////////
    //glDrawArrays(GL_TRIANGLES, 0, 3);

    /////////////   Version 3D    /////////////
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glDrawArrays(GL_TRIANGLES, 3, 3);
    glDrawArrays(GL_POLYGON, 6, 4);
    glDrawArrays(GL_POLYGON, 10, 4);
    glDrawArrays(GL_POLYGON, 14, 4);

    //glDrawArrays(GL_POLYGON, 0, 4);

    sp->release();
    vbo.release();
}





