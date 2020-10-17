#include "banc.h"
#include "convertvbo.h"


//////////////////////////////// Constructeurs ///////////////////////////////////

Banc::Banc(int nbPois, int nbOb, int nbPred, int l, int h, int p) :
    largeurAqua(l), hauteurAqua(h), profondeurAqua(p),
    nbPoissons(nbPois), nbObstacles(nbOb), nbPredateurs(nbPred)
{
    qsrand(time(NULL));
    position = QVector3D(0.f, 0.f, 0.f);
    moyenneAqua = ((l+h+p)/3.f);

    int val_pos = 10;
    build_aquarium();
    build_poissons(val_pos);
    build_obstacles(val_pos*8);
    build_predateurs(val_pos*8);
}

void Banc::build_aquarium()
{
    float x = largeurAqua;
    float y = hauteurAqua;
    float z = profondeurAqua;
    vector<float> vertices = {
        // Plan X
        -x, y, -z,  // A
        x, y, -z,   // B
        x, y, z,    // C
        -x, y, z,   // D
        -x, -y, z,  // E
        x, -y, z,   // F
        x, -y, -z,  // G
        -x, -y, -z, // H

        // Plan Y
        -x, y, -z,  // A
        -x, y, z,   // D
        -x, -y, z,  // E
        -x, -y, -z, // H
        x, -y, -z,  // G
        x, -y, z,   // F
        x, y, z,    // C
        x, y, -z    // B
    };
    nbVert = (int)vertices.size()/3;
    vector<float> colors = {
        0.f, 0.f, 1.f,
    };
    QVector<GLfloat> vertData;
    ConvertVbo::convert_coords_to_vbo(vertData,
                                      vertices, 3,
                                      colors, 3, true);
    vbo.create();
    vbo.bind();
    vbo.allocate(vertData.constData(), vertData.count() * int(sizeof(GLfloat)));
}

void Banc::build_obstacles(int posVal)
{
    for (int i=0; i<nbObstacles; i++)
    {
        float xP = (float)Utils::randInt(largeurAqua*(-posVal), largeurAqua*posVal) / 100.f;
        float yP = (float)Utils::randInt(hauteurAqua*(-posVal), hauteurAqua*posVal) / 100.f;
        float zP = (float)Utils::randInt(profondeurAqua*(-posVal), profondeurAqua*posVal) / 100.f;
        QVector3D posOb (xP, yP, zP);
        Obstacle myOb(posOb, moyenneAqua/5.f);
        listObstacles.push_back(myOb);
    }
}

void Banc::build_predateurs(int posVal)
{
    qDebug() << "<" << __FUNCTION__ << ">";
//    qDebug() << "\tnbPredateurs = " << nbPredateurs;
    for (int i=0; i<nbPredateurs; i++)
    {
        float xP = (float)Utils::randInt(largeurAqua*(-posVal), largeurAqua*posVal) / 100.f;
        float yP = (float)Utils::randInt(hauteurAqua*(-posVal), hauteurAqua*posVal) / 100.f;
        float zP = (float)Utils::randInt(profondeurAqua*(-posVal), profondeurAqua*posVal) / 100.f;
        QVector3D posPred (xP, yP, zP);

        bool b_vit_fixe = true;
        float vit = 0;
        if (b_vit_fixe)
            vit = +moyenneAqua*2 / 10.f;
        else
            while (vit==0) {vit = (float)Utils::randInt(-moyenneAqua*2, +moyenneAqua*2)/10.f;}
        QVector3D vitPred(vit, vit, vit);

        float taille = moyenneAqua/10.f;
        Predateur myPred(posPred, vitPred, taille);
        listPredateurs.push_back(myPred);
    }

//    qDebug() << "\tlistPredateurs.size() = " << listPredateurs.size();
    qDebug() << "</" << __FUNCTION__ << ">";
}

void Banc::build_poissons(int posVal)
{
    bool b_vit_fixe = false;
    for (int i=0; i<nbPoissons; i++)
    {
        float xP = (float)Utils::randInt(largeurAqua*(-posVal), largeurAqua*posVal) / 100.f;
        float yP = (float)Utils::randInt(hauteurAqua*(-posVal), hauteurAqua*posVal) / 100.f;
        float zP = (float)Utils::randInt(profondeurAqua*(-posVal), profondeurAqua*posVal) / 100.f;
        float vit = 0;
        if (b_vit_fixe)
            vit = +moyenneAqua*2 / 10.f;
        else
            while (vit==0) {vit = (float)Utils::randInt(-moyenneAqua*2, +moyenneAqua*2)/10.f;}

        QVector3D posPoisson (xP, yP, zP);
        QVector3D vitPoisson(vit, vit, vit);
        // float taille = moyenneAqua/20.f;
        float taille = moyenneAqua/40.f;
        Poisson p = Poisson(posPoisson,
                            vitPoisson,
                            taille);
        listPoissons.push_back(p);
    }
}


//////////////////////////////// Accesseurs ///////////////////////////////////

QOpenGLBuffer Banc::_vbo()                  {    return vbo;        }
void Banc::_set_coefSep(float newCoef)      {   coefSep = newCoef;  }
void Banc::_set_coefAl(float newCoef)       {   coefAl = newCoef;   }
void Banc:: _set_coefCohes(float newCoef)   {   coefCohes = newCoef;}


//////////////////////////////// Méthodes /////////////////////////////////////



void Banc::affiche_poissons(QOpenGLShaderProgram *spText, QMatrix4x4 modelMatrix,
                            QOpenGLTexture *textPoisson)
{
    textPoisson->bind();
    /* En attendant de pouvoir utiliser un seul vbo pour chaque objet,
     * on utilise cette astuce pour n'écrire ces instructions qu'une seule fois */
    if (!listPoissons.empty())  listPoissons[0]._vbo().bind();
    else return;
    spText->bind();

    int tailleVb = 5;   // 3 + 2
    spText->setAttributeBuffer("in_position", GL_FLOAT, 0, 3, tailleVb * sizeof(GLfloat));
    spText->setAttributeBuffer("in_uv", GL_FLOAT, 3 * sizeof(GLfloat), 2, tailleVb * sizeof(GLfloat));
    spText->enableAttributeArray("in_position");
    spText->enableAttributeArray("in_uv");
    listPoissons[0]._vbo().release();

    modelMatrix.translate(position);
    for (unsigned i=0; i<listPoissons.size(); i++)
    {
        listPoissons[i].affiche(spText, modelMatrix);
    }

    spText->disableAttributeArray("in_position");
    spText->disableAttributeArray("in_uv");

    spText->release();
    textPoisson->release();
}

void Banc::affiche_obstacles(QOpenGLShaderProgram *spText, QMatrix4x4 modelMatrix,
                             QOpenGLTexture *textObstacle)
{
    textObstacle->bind();
    /* En attendant de pouvoir utiliser un seul vbo pour chaque objet,
     * on utilise cette astuce pour n'écrire ces instructions qu'une seule fois */
    if (!listObstacles.empty())  listObstacles[0]._vbo().bind();
    else return;
    spText->bind();

    int tailleVb = 5;   // 3 + 2
    spText->setAttributeBuffer("in_position", GL_FLOAT, 0, 3, tailleVb * sizeof(GLfloat));
    spText->setAttributeBuffer("in_uv", GL_FLOAT, 3 * sizeof(GLfloat), 2, tailleVb * sizeof(GLfloat));
    spText->enableAttributeArray("in_position");
    spText->enableAttributeArray("in_uv");
    listObstacles[0]._vbo().release();

    for (unsigned i=0; i<listObstacles.size(); i++)
    {
        listObstacles[i].affiche(spText, modelMatrix);
    }

    spText->release();
    textObstacle->release();
}

void Banc::affiche_predateurs(QOpenGLShaderProgram *spText, QMatrix4x4 modelMatrix,
                              QOpenGLTexture *textPredateur)
{
    textPredateur->bind();
    /* En attendant de pouvoir utiliser un seul vbo pour chaque objet,
         * on utilise cette astuce pour n'écrire ces instructions qu'une seule fois */
    if (!listPredateurs.empty())  listPredateurs[0]._vbo().bind();
    else return;
    spText->bind();

    int tailleVb = 5;   // 3 + 2
    spText->setAttributeBuffer("in_position", GL_FLOAT, 0, 3, tailleVb * sizeof(GLfloat));
    spText->setAttributeBuffer("in_uv", GL_FLOAT, 3 * sizeof(GLfloat), 2, tailleVb * sizeof(GLfloat));
    spText->enableAttributeArray("in_position");
    spText->enableAttributeArray("in_uv");
    listPredateurs[0]._vbo().release();

    for (unsigned i=0; i<listPredateurs.size(); i++)
    {
        listPredateurs[i].affiche(spText, modelMatrix);
    }

    spText->release();
    textPredateur->release();
}

void Banc::dessiner_aquarium()
{
    int depart = 0; int pas=4;
    for (depart=0; depart<nbVert; depart+=pas) {
        glDrawArrays(GL_LINE_LOOP, depart, pas);
    }
}

void Banc::affiche_aquarium(QOpenGLShaderProgram *spColor, QMatrix4x4 modelMatrix)
{
    vbo.bind();
    spColor->bind();
    spColor->setUniformValue("modelMatrix", modelMatrix);
    int tailleVb = 6;   // 3 + 3
    spColor->setAttributeBuffer("posAttr", GL_FLOAT, 0, 3, tailleVb * sizeof(GLfloat));
    spColor->setAttributeBuffer("colAttr", GL_FLOAT, 3 * sizeof(GLfloat), 3, tailleVb * sizeof(GLfloat));
    spColor->enableAttributeArray("posAttr");
    spColor->enableAttributeArray("colAttr");

    dessiner_aquarium();

    spColor->disableAttributeArray("posAttr");
    spColor->disableAttributeArray("colAttr");
    spColor->release();
    vbo.release();
}

void Banc::affiche(QOpenGLShaderProgram *spBoite, QOpenGLShaderProgram *spPoissons, QOpenGLShaderProgram *spObstacles,
                   QMatrix4x4 modelMatrix,
                   QOpenGLTexture *textObstacle, QOpenGLTexture *textPoisson, QOpenGLTexture *textPredateur)
{
    //glDrawArrays(GL_POLYGON, 0, 4);
    //modelMatrix.setToIdentity();
    modelMatrix.translate(position);

    affiche_aquarium(spBoite, modelMatrix);
    affiche_obstacles(spObstacles, modelMatrix, textObstacle);
    affiche_predateurs(spPoissons, modelMatrix, textPredateur);
    affiche_poissons(spPoissons, modelMatrix, textPoisson);
}

void Banc::remettre_poisson_dans_aquarium(Poisson &myPoisson, float coef)
{
    QVector3D myPoint;
    // point B - point A
    myPoint = myPoisson._position() - position;
    //float coef = 2.f;

    if (abs(myPoint[0]) >= largeurAqua - (myPoisson._rayon()/coefBoite)) {
        myPoisson.demi_tour(0, largeurAqua/100.f, true);
    }
    else if(abs(myPoint[1]) >= hauteurAqua - (myPoisson._rayon()/coefBoite)) {
        myPoisson.demi_tour(1, hauteurAqua/100.f, true);
    }
    else if(abs(myPoint[2]) >= profondeurAqua - (myPoisson._rayon()/coefBoite)) {
        myPoisson.demi_tour(2, profondeurAqua/100.f, true);
    }

    /*
    if (abs(myPoint[0]) >= largeurAqua - myPoisson._rayon()) {
        myPoisson.demi_tour(0, largeurAqua/100.f, false);
    }
    else if(abs(myPoint[1]) >= hauteurAqua - myPoisson._rayon()) {
        myPoisson.demi_tour(1, hauteurAqua/100.f, false);
    }
    else if(abs(myPoint[2]) >= profondeurAqua - myPoisson._rayon()) {
        myPoisson.demi_tour(2, profondeurAqua/100.f, false);
    }
    */

    /*
    if (listPoissons[i].dans_voisinage(myPoint))
    {
        myList[i].demi_tour(0, largeurAqua/100.f);
        myList[i].demi_tour(1, hauteurAqua/100.f);
        myList[i].demi_tour(2, profondeurAqua/100.f);
    }
    */
}

void Banc::regler_comportement_simple_poisson(int numPoisson)
{

    remettre_poisson_dans_aquarium(listPoissons[numPoisson], coefBoite);
    unsigned np = numPoisson;
    for (unsigned i=0; i<listPoissons.size(); i++)
    {
        if (i==np)  continue;
        if (listPoissons[np].dans_voisinage(listPoissons[i]._position())) {
            listPoissons[i].demi_tour();
            qDebug() << "Poisson" << np << ": Aîe! regarde où tu vas poisson" << i << "!";
            //qDebug () << "listPoissons[i].distance = " << listPoissons[i]._distance();
        }
    }
}

/*
QVector3D Banc::separation(int numP)
{
    int nbObVois=0;
    QVector3D p, pV;
    QVector3D v1(0.f, 0.f, 0.f);
    vector<Poisson> &lP = listPoissons;
    for (unsigned i=0; i<lP.size(); i++)
    {
        if (i==numP)  continue;

        if (lP[numP].dans_voisinage(lP[i]._position()))
        {
            p = lP[numP]._position();
            pV = lP[i]._position();
            if (p.distanceToPoint(pV) <=  lP[numP]._rayon())
            {
                nbObVois++;
                v1 += lP[i]._position();
            }
        }
    }
    return v1;
}
*/

void Banc::regler_comportement_poisson(int numP, float intLin)
{
    vector<Poisson> &lP = listPoissons;
    vector<Obstacle> &lO = listObstacles;
    vector<Poisson> &lPred = listPredateurs;
    QVector3D v1(0.f, 0.f, 0.f),
            v2(0.f, 0.f, 0.f),
            v3(0.f, 0.f, 0.f);
    int nbPVois=0;
    int nbObVois=0;

//    if (collision_aquarium(numP)) {
//        listPoissons[numP].demi_tour();
//    }

    // remettre_poisson_dans_aquarium(listPoissons[numP], coefBoite);

    for (unsigned i=0; i<lP.size(); i++)
    {
        if (i==numP)  continue;
        if (lP[numP].dans_voisinage(lP[i]._position())) {
            nbPVois++;
            nbObVois++;
            v1 += lP[i]._position();
            v2 += lP[i]._vitesse();
            v3 += lP[i]._position();
        }
    }
    for (unsigned i=0; i<lO.size(); i++)
    {
        if (lP[numP].dans_voisinage(lO[i]._position())) {
            nbObVois++;
            v1 += lO[i]._position();
        }
    }
    for (unsigned i=0; i<lPred.size(); i++)
    {
        if (lP[numP].dans_voisinage(lPred[i]._position())) {
            nbObVois++;
            v1 += lPred[i]._position();
        }
    }

    if (nbObVois <= 0)    return;

    ///// Séparation /////
    v1 = v1 / nbObVois;
    //v1 = lp[numP]._position() - v1;
    v1 = - (v1 - lP[numP]._position());
    //v1.normalize();

    ///// Alignement /////
    v2 = v2 / nbPVois;
    v2 = lP[numP]._vitesse() - v2;
    //v2.normalize();


    ///// Cohésion  /////
    v3 = v3 / nbPVois;
    v3 = v3 - lP[numP]._position();
    //v3.normalize();

    if (nbObVois>0)
    {
        //qDebug() << "Poisson" lis.tPoissons[i].anime(dt);<< numP << ": j'ai des voisins! ";
        QVector3D vP = listPoissons[numP]._vitesse();
//        float coefA = (float)Utils::randInt(1, 100) / 100.f;
//        float coefB = (float)Utils::randInt(1, 100) / 100.f;
//        float coefC = (float)Utils::randtaille*5.fInt(1, 100) / 100.f;
//        float coefA = 1.f;
//        float coefB = 2.f;
//        float coefC = 1.3f;
        QVector3D newVP =
                coefSep*v1
                + coefAl*v2
                + coefCohes*v3;

        // Limiteur vitesse
        if (newVP.length() >= lP[numP]._vitesse_max()) {
            newVP.normalize();
            newVP *= lP[numP]._vitesse_max();
        }

        if (intLin<0.f)   intLin = 0.f;
        if (intLin>1.f)   intLin = 1.f;
        newVP = (1-intLin)*vP + intLin*newVP;
        lP[numP]._set_vitesse(newVP);
    }
}

void Banc::regler_comportement_predateur(int numPred)
{
    vector<Poisson> &lP = listPoissons;
    Poisson &pred = listPredateurs[numPred];
    //remettre_poisson_dans_aquarium(pred, coefBoite);
    float min = INT_MAX*2.f;
    int pMin = -1;
    for (unsigned i=0; i<listPoissons.size(); i++)
    {
        float distance = pred._position().distanceToPoint(lP[i]._position());
//        qDebug ()<< "Poisson" << i << "distance of"
//                  << distance;
        //if (distance < 0)   distance = INT_MAX;
        if (distance < min)
        {
            min = distance;
            pMin = i;
        }
    }
    if (pMin < 0)
    {
        qDebug() << "pMin = " << pMin;
        qDebug() << "distance = " << pred._position().distanceToPoint(lP[0]._position());
        qDebug() << "Poisson 0 position ->\tx =" << lP[0]._position()[0]
                << "\ty =" << lP[0]._position()[1]
                << "\tz =" << lP[0]._position()[2];
        qDebug() << "poisson.size() = " << listPoissons.size();
        qWarning () << "problem in " << __FUNCTION__ << ": aucun poisson trouvé, pMin < 0";
        exit(1);
    }
    //qDebug() << "Predateur" << numPred << ": Je veux manger poisson" << pMin;
    QVector3D v1(lP[pMin]._position() - pred._position());
    //v1 = pred._vitesse() + v1;
    // Limiteur vitesse
    if (v1.length() >= pred._vitesse_max()) {
        v1.normalize();
        v1 *= pred._vitesse_max();
    }
    pred._set_vitesse(v1);
}

void Banc::anime(float dt)
{
    // POISSON
    for (unsigned i=0; i<listPoissons.size(); i++)
    {
        //listPoissons[i].anime(dt);
        //regler_comportement_simple_poisson(i);
        //listPoissons[i]._set_old_vit(listPoissons[i]._vitesse());
        regler_comportement_poisson(i, 0.7f);
    }
    for (unsigned i=0; i<listPoissons.size(); i++)
    {
        remettre_poisson_dans_aquarium(listPoissons[i], coefBoite);
    }
    for (unsigned i=0; i<listPoissons.size(); i++)
    {
        listPoissons[i].anime(dt);
    }

    // PREDATEUR
    for (unsigned i=0; i<listPredateurs.size(); i++)
    {
        remettre_poisson_dans_aquarium(listPredateurs[i], coefBoite);
    }
    for (unsigned i=0; i<listPredateurs.size(); i++)
    {
        regler_comportement_predateur(i);
    }
    for (unsigned i=0; i<listPredateurs.size(); i++)
    {
        listPredateurs[i].anime(dt);
    }
}







