// Basé sur :
// CC-BY Edouard.Thiel@univ-amu.fr - 22/01/2019

#include "glarea.h"
#include <QDebug>
#include <QSurfaceFormat>
#include <QMatrix4x4>



GLArea::GLArea(QWidget *parent) :
    QOpenGLWidget(parent)
{
    QSurfaceFormat sf;
    sf.setDepthBufferSize(24);
    sf.setSamples(16);
    setFormat(sf);

    setEnabled(true);                   // événements clavier et souris
    setFocusPolicy(Qt::StrongFocus);    // accepte focus
    setFocus();                         // donne le focus

    myTimer = new QTimer(this);
    myTimer->setInterval(50);           // msec
    connect (myTimer, SIGNAL(timeout()), this, SLOT(onTimeout()));
    myTimer->start();
    elapsedTimer.start();

    //posSmoke.setX(0.f);    posSmoke.setY(0.f);    posSmoke.setZ(0.f);
    //color.setRgb(1.f, 1.f, 1.f);

    connect (this, SIGNAL(separation_changed(double)), this, SLOT(on_set_separation(double)));
    connect (this, SIGNAL(alignement_changed(double)), this, SLOT(on_set_alignement(double)));
    connect (this, SIGNAL(cohesion_changed(double)), this, SLOT(on_set_cohesion(double)));

    connect (this, SIGNAL(red_changed(int)), this, SLOT(on_set_Red(int)));
    connect (this, SIGNAL(green_changed(int)), this, SLOT(on_set_Green(int)));
    connect (this, SIGNAL(blue_changed(int)), this, SLOT(on_set_Blue(int)));
}


GLArea::~GLArea()
{
    delete myTimer;
    delete myPoisson;
    delete myBanc;

    // Contrairement aux méthodes virtuelles initializeGL, resizeGL et repaintGL,
    // dans le destructeur le contexte GL n'est pas automatiquement rendu courant.
    makeCurrent();
    tearGLObjects();
    doneCurrent();
}


void GLArea::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(0.5f, 0.5f, 1.f, 1.f);
    glEnable(GL_DEPTH_TEST);

    makeGLObjects();

    // shader du sol
    program_sol = new QOpenGLShaderProgram(this);
    program_sol->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/simple.vsh");
    program_sol->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/simple.fsh");
    if (! program_sol->link()) {  // édition de lien des shaders dans le shader program
        qWarning("Failed to compile and link shader program:");
        qWarning() << program_sol->log();
    }
    program_sol->setUniformValue("texture", 0);

    // shader de billboard
    program_particule = new QOpenGLShaderProgram(this);
    program_particule->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/billboard.vsh");
    program_particule->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/billboard.fsh");
    if (! program_particule->link()) {  // édition de lien des shaders dans le shader program
        qWarning("Failed to compile and link shader program:");
        qWarning() << program_particule->log();
    }
    program_particule->setUniformValue("texture", 0);

    // Shader Poisson
    program_poisson = new QOpenGLShaderProgram(this);
    program_poisson->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/shaderPoisson.vsh");
    program_poisson->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/shaderPoisson.fsh");
    if (! program_poisson->link()) {  // édition de lien des shaders dans le shader program
        qWarning("Failed to compile and link shader program:");
        qWarning() << program_poisson->log();
    }
    program_poisson->setUniformValue("texture", 0);

    // Shader couleur
    program_couleur = new QOpenGLShaderProgram(this);
    program_couleur->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/basicColor.vsh");
    program_couleur->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/basicColor.fsh");
    if (! program_couleur->link()) {  // édition de lien des shaders dans le shader program
        qWarning("Failed to compile and link shader program:");
        qWarning() << program_couleur->log();
    }
}


void GLArea::makeGLObjects()
{
    //////////////////////////// Création poisson /////////////////////////

    myPoisson = new Poisson (0.f, 0.f, 0.7f,
                            0.f, 0.1f, 0.f,
                            3.f);
    int nbPoissons = 300;
    int nbOb = 5;
    int nbPred = 3;
    int tailleAqua = 12;
    myBanc = new Banc (nbPoissons, nbOb, nbPred, tailleAqua, tailleAqua, tailleAqua);
//    myBanc = new Banc (10, 5, 5, 5);

    // Création de textures
    QImage image_meduse(":/textures/meduse.png");
    if (image_meduse.isNull())
        qDebug() << "load image meduse.png failed";
    textures[0] = new QOpenGLTexture(image_meduse);

//    QImage image_particule(":/textures/poisson2.png");
//    QImage image_poisson(":/textures/poysson.png");
    QImage image_poisson(":/textures/poisson.png");
    if (image_poisson.isNull())
        qDebug() << "load image poisson.png failed";
    textures[1] = new QOpenGLTexture(image_poisson);

    QImage image_requin(":/textures/requin.png");
    if (image_requin.isNull())
        qDebug() << "load image requin.png failed";
    textures[2] = new QOpenGLTexture(image_requin);
}

void GLArea::tearGLObjects()
{
    vbo_sol.destroy();
    vbo_poisson.destroy();
    vbo_particule.destroy();
    for (int i = 0; i < 3; i++)
        delete textures[i];
}

void GLArea::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
    windowRatio = float(w) / h;
}


void GLArea::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Matrice de projection
    QMatrix4x4 projectionMatrix;
    projectionMatrix.perspective(45.0f, windowRatio, 1.0f, 1000.0f);

    // Matrice de vue (caméra)
    QMatrix4x4 viewMatrix;
    viewMatrix.translate(xPos, yPos, zPos);
    viewMatrix.rotate(xRot, 1, 0, 0);
    viewMatrix.rotate(yRot, 0, 1, 0);
    viewMatrix.rotate(zRot, 0, 0, 1);

    QMatrix4x4 modelMatrix;
    modelMatrix.translate(0.f, 1.f, 0.f);

    ////////////////// Affichage poisson ////////////////////////////////////
    //myPoisson->_vbo().bind();
    //glDepthMask(GL_FALSE);

    program_poisson->bind(); // active le shader program des particules
    program_poisson->setUniformValue("projectionMatrix", projectionMatrix);
    program_poisson->setUniformValue("viewMatrix", viewMatrix);
    program_poisson->setUniformValue("modelMatrix", modelMatrix);
    program_poisson->release();

    program_couleur->bind();
    program_couleur->setUniformValue("projectionMatrix", projectionMatrix);
    program_couleur->setUniformValue("viewMatrix", viewMatrix);
    program_couleur->setUniformValue("modelMatrix", modelMatrix);
    program_couleur->release();

    program_particule->bind();
    program_particule->setUniformValue("projectionMatrix", projectionMatrix);
    program_particule->setUniformValue("viewMatrix", viewMatrix);
    program_particule->setUniformValue("modelMatrix", modelMatrix);
    program_particule->release();

    // textures[1]->bind();
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    // myPoisson->affiche(program_poisson, modelMatrixPoisson);
    // myPoisson->anime(dt);
    myBanc->affiche(program_couleur, program_poisson, program_particule,
                    modelMatrix,
                    textures[0], textures[1], textures[2]);
    myBanc->anime(dt);

    glDisable(GL_BLEND);
    // textures[1]->release();
    //glDepthMask(GL_TRUE);
}


void GLArea::keyPressEvent(QKeyEvent *ev)
{
    float da = 1.0f;

    switch(ev->key())
    {
    case Qt::Key_A :
        xRot -= da;
        update();
        break;

    case Qt::Key_Q :
        xRot += da;
        update();
        break;

    case Qt::Key_Z :
        yRot -= da;
        update();
        break;

    case Qt::Key_S :
        yRot += da;
        update();
        break;

    case Qt::Key_E :
        zRot -= da;
        update();
        break;

    case Qt::Key_D :
        zRot += da;
        update();
        break;

    case Qt::Key_P :
        update();
        break;

    case Qt::Key_Up :
        //posSmoke[2] -= 0.5f;
        update();
        break;

    case Qt::Key_Down :
        //posSmoke[2] += 0.5f;
        update();
        break;

    case Qt::Key_Right :
        //posSmoke[0] += 0.5f;
        update();
        break;

    case Qt::Key_Left :
        //posSmoke[0] -= 0.5f;
        update();
        break;
    }
}


void GLArea::keyReleaseEvent(QKeyEvent *ev)
{
    qDebug() << __FUNCTION__ << ev->text();
}


void GLArea::mousePressEvent(QMouseEvent *ev)
{
    lastPos = ev->pos();
}


void GLArea::mouseReleaseEvent(QMouseEvent *ev)
{
    qDebug() << __FUNCTION__ << ev->x() << ev->y() << ev->button();
}


void GLArea::mouseMoveEvent(QMouseEvent *ev)
{
    int dx = ev->x() - lastPos.x();
    int dy = ev->y() - lastPos.y();

    if (ev->buttons() & Qt::LeftButton) {
        xRot += dy;
        yRot += dx;
        update();
    } else if (ev->buttons() & Qt::RightButton) {
        xPos += dx/10.0f;
        yPos -= dy/10.0f;
        update();
    } else if (ev->buttons() & Qt::MidButton) {
        xPos += dx/10.0f;
        zPos += dy;
        update();
    }

    lastPos = ev->pos();
}


void GLArea::onTimeout()
{
    static qint64 old_chrono = elapsedTimer.elapsed(); // static : initialisation la première fois et conserve la dernière valeur
    qint64 chrono = elapsedTimer.elapsed();
    dt = (chrono - old_chrono) / 1000.0f;
    old_chrono = chrono;
//    mySmoke->animate(dt);
    //myParticule->animate(dt);
    update();
}

////////////////////////// My signals /////////////////////////

void GLArea::on_set_separation(double newSep)
{
    qDebug() << __FUNCTION__ << "The event sender is" << sender();
    //coefSep = float(newSep);
    myBanc->_set_coefSep(newSep);
}

void GLArea::on_set_alignement(double newAl)
{
    qDebug() << __FUNCTION__ << "The event sender is" << sender();
    //coefAl = float(newAl);
    myBanc->_set_coefAl(newAl);
}

void GLArea::on_set_cohesion(double newCohes)
{
    qDebug() << __FUNCTION__ << "The event sender is" << sender();
    //coefCohes = float(newCohes);
    myBanc->_set_coefCohes(newCohes);
}

void GLArea::on_set_Red(int newR)
{
    color.setRed(newR);
}

void GLArea::on_set_Blue(int newB)
{
    color.setBlue(newB);
}

void GLArea::on_set_Green(int newG)
{
    color.setGreen(newG);
}

