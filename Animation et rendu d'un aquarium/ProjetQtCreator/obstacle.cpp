#include "obstacle.h"

#include "convertvbo.h"


Obstacle::Obstacle(QVector3D pos, float r) :
    position(pos), rayon(r)
{
    init();
}

Obstacle::Obstacle(float xP, float yP, float zP, float r) :
    position(xP, yP, zP), rayon(r)
{
    init();
}


void Obstacle::init()
{
//    QVector3D pA(0.f, 0.f, 0.f);
//    QVector3D pB(1.f, 0.f, 0.f);
//    QVector3D pC(1.f, 1.f, 0.f);
//    QVector3D pD(0.f, 1.f, 0.f);

//    QVector3D pAz(0.f, 0.f, 0.5f);
//    QVector3D pBz(1.f, 0.f, 0.5f);
//    QVector3D pCz(1.f, 1.f, 0.5f);
//    QVector3D pDz(0.f, 1.f, 0.5f);

    //float z=0.f;
    // vertices
    vector<float> vertices = {
        /////////////////   2D  //////////////////

        0.f, 0.f, 0.f,
        1.f, 0.f, 0.f,
        1.f, 1.f, 0.f,
        0.f, 1.f, 0.f

    };
    vector<float> texCoords = {
        0.f, 1.f,
        1.f, 1.f,
        1.f, 0.f,
        0.f, 0.f
    };

    QVector<GLfloat> vertData;

    ConvertVbo::convert_coords_to_vbo(vertData,
                                      vertices, 3,
                                      texCoords, 2);
    vbo.create();
    vbo.bind();
    vbo.allocate(vertData.constData(), vertData.count() * int(sizeof(GLfloat)));
}

QVector3D Obstacle::_position() {   return position;    }
QOpenGLBuffer Obstacle::_vbo()  {   return vbo;         }

void Obstacle::affiche(QOpenGLShaderProgram *sp, QMatrix4x4 modelMatrix)
{
    //glEnable(GL_BLEND);
    vbo.bind();
    sp->bind();
    modelMatrix.translate(position);
    //modelMatrix.rotate(1.f, vitesse);
    sp->setUniformValue("modelMatrix", modelMatrix);
    sp->setUniformValue("particleSize", rayon);

    //glDrawArrays(GL_TRIANGLES, 0, 3);
    glDrawArrays(GL_POLYGON, 0, 4);

    sp->release();
    vbo.release();
    //glDisable(GL_BLEND);
}









