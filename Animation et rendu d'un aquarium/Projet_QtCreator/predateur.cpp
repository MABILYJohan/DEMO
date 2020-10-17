#include "predateur.h"

#include "convertvbo.h"

Predateur::Predateur(QVector3D pos, QVector3D vit, float taille) :
    Poisson(pos, vit, taille)
{
    build_vbo();
}

Predateur::Predateur(float xP, float yP, float zP,
                     float xV, float yV, float zV,
                     float taille) :
    Poisson(xP, yP, zP, xV, yV, zV, taille)
{
    build_vbo();
    vitesseMax *= 1.f;
    rayon = taille * 2.f;
}


//void Predateur::build_vbo()
//{
//    // VERSION 1
//    /*
//    //qDebug() << "\t<Predateur::" << __FUNCTION__ << ">";
//    //rayon = taille*5.f;
//    rayon = 10.f;
//    float z=0.f;
//    // vertices
//    vector<float> vertices = {
//        0.f, 0.f, z,
//        1.f, 0.f, z,
//        1.f, 1.f, z,
//        0.f, 1.f, z
//    };
//    vector<float> texCoords = {
//        0.f, 1.f,
//        1.f, 1.f,
//        1.f, 0.f,
//        0.f, 0.f
//    };

//    ConvertVbo::convert_coords_to_vbo(vertData,
//                                      vertices, 3,
//                                      texCoords, 2);
//    vbo.create();
//    vbo.bind();
//    vbo.allocate(vertData.constData(), vertData.count() * int(sizeof(GLfloat)));
//    //qDebug() << "\t</Predateur::" << __FUNCTION__ << ">";
//    */

//    // VERSION 2
//    //    qDebug() << "\t<Poisson::" << __FUNCTION__ << ">";

//    //oldPos = position;
//    //    rayon = taille * 3.f;
//    rayon = 10.f;
//    vitesseMax = 1.f;
//    vitesse[0] = 0.5f;
//    //    vitesse[0] = 0.0f;
//    //    vitesse[1] = 0.0f;
//    //    vitesse[2] = 0.0f;

//    float z = 0.f;
//    // vertices
//    QVector3D pA(0.f, 1.f, 0.f);
//    QVector3D pB(0.f, 0.f, 0.f);
//    QVector3D pC(1.f, 0.f, 0.f);

//    QVector3D pAz(0.f, 1.f, 0.2f);
//    QVector3D pBz(0.f, 0.f, 0.2f);
//    QVector3D pCz(1.f, 0.f, 0.2f);

//    vector<float> vertices = {
//        /////////////////   2D  //////////////////
//        /*
//        pA[0], pA[1], pA[2],
//        pB[0], pB[1], pB[2],
//        pC[0], pC[1], pC[2],
//        */

//        /////////////////   3D  //////////////////
//        // Face 1
//        pA[0], pA[1], pA[2],
//        pB[0], pB[1], pB[2],
//        pC[0], pC[1], pC[2],
//        // Face 2
//        pAz[0], pAz[1], pAz[2],
//        pBz[0], pBz[1], pBz[2],
//        pCz[0], pCz[1], pCz[2],

//        // Cote 1
//        pA[0], pA[1], pA[2],
//        pAz[0], pAz[1], pAz[2],
//        pCz[0], pCz[1], pCz[2],
//        pC[0], pC[1], pC[2],
//        // Cote 2
//        pA[0], pA[1], pA[2],
//        pB[0], pB[1], pB[2],
//        pBz[0], pBz[1], pBz[2],
//        pAz[0], pAz[1], pAz[2],
//        // Cote 3
//        pB[0], pB[1], pB[2],
//        pBz[0], pBz[1], pBz[2],
//        pCz[0], pCz[1], pCz[2],
//        pC[0], pC[1], pC[2],
//    };
//    vector<float> texCoords = {
//        /////////////////   2D  //////////////////
//        /*
//        0.f, 1.f,
//        0.f, 0.f,
//        1.f, 0.f,
//        */

//        /////////////////   3D  //////////////////
//        // Face 1
//        0.f, 1.f,
//        0.f, 0.f,
//        1.f, 0.f,
//        // Face 2
//        0.f, 1.f,
//        0.f, 0.f,
//        1.f, 0.f,

//        // Cote 1
//        0.6f, 0.6f,
//        0.4f, 0.4f,
//        0.4f, 0.6f,
//        0.6f, 0.4f,
//        // Cote 2
//        0.6f, 0.6f,
//        0.4f, 0.4f,
//        0.4f, 0.6f,
//        0.6f, 0.4f,
//        // Cote 3
//        0.6f, 0.6f,
//        0.4f, 0.4f,
//        0.4f, 0.6f,
//        0.6f, 0.4f,
//    };

//    ConvertVbo::convert_coords_to_vbo(vertData,
//                                      vertices, 3,
//                                      texCoords, 2);
//    vbo.create();
//    vbo.bind();
//    vbo.allocate(vertData.constData(), vertData.count() * int(sizeof(GLfloat)));
//    //    qDebug() << "\t</Poisson::" << __FUNCTION__ << ">";

//}

Predateur::~Predateur()
{
    ;
}


