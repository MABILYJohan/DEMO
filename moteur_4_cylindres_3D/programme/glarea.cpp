
#include "glarea.h"
#include <QDebug>
#include <QSurfaceFormat>
#include <QMatrix4x4>
#include <QVector3D>

static const QString vertexShaderFile   = ":/vertex.glsl";
static const QString fragmentShaderFile = ":/fragment.glsl";


GLArea::GLArea(QWidget *parent) :
    QOpenGLWidget(parent)
{
    qDebug() << "init GLArea" ;

    QSurfaceFormat sf;
    sf.setDepthBufferSize(24);
    sf.setSamples(16);  // nb de sample par pixels : suréchantillonnage por l'antialiasing, en décalant à chaque fois le sommet
                        // cf https://www.khronos.org/opengl/wiki/Multisampling et https://stackoverflow.com/a/14474260
    setFormat(sf);
    qDebug() << "Depth is"<< format().depthBufferSize();

    setEnabled(true);  // événements clavier et souris
    setFocusPolicy(Qt::StrongFocus); // accepte focus
    setFocus();                      // donne le focus

    m_timer = new QTimer(this);
    m_timer->setInterval(50);  // msec
    connect (m_timer, SIGNAL(timeout()), this, SLOT(onTimeout()));
    connect (this, SIGNAL(radiusChanged(double)), this, SLOT(setRadius(double)));
    connect (this, SIGNAL(zChanged(double)), this, SLOT(setZ(double)));
}

GLArea::~GLArea()
{
    qDebug() << "destroy GLArea";

    delete m_timer;

    // Contrairement aux méthodes virtuelles initializeGL, resizeGL et repaintGL,
    // dans le destructeur le contexte GL n'est pas automatiquement rendu courant.
    makeCurrent();
    tearGLObjects();
    doneCurrent();
}

void GLArea::initializeGL()
{
    qDebug() << __FUNCTION__ ;
    initializeOpenGLFunctions();
    glEnable(GL_DEPTH_TEST);

    makeGLObjects();

    // shaders
    m_program = new QOpenGLShaderProgram(this);
    m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, vertexShaderFile);  // compile
    m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, fragmentShaderFile);
    if (! m_program->link()) {  // édition de lien des shaders dans le shader program
        qWarning("Failed to compile and link shader program:");
        qWarning() << m_program->log();
    }

    // récupère identifiants de "variables" dans les shaders
    //m_posAttr = m_program->attributeLocation("posAttr");
    //m_colAttr = m_program->attributeLocation("colAttr");
    //m_matrixUniform = m_program->uniformLocation("matrix");
}

void GLArea::build_coord_facette_bloc(int forme)
{
//    qDebug() << "<" << __FUNCTION__ << ">";
    float rC = f_rayon;
    float zC = f_ep/2.f;
    int points = 4;
    int dim = 3;
    int oldSize = v_vs.size();

//    qDebug() << "forme : " << forme;

    float angle;
    if (forme == DRAW_CYLINDRE) {
        angle = cyl_angle;
    }
    else {
        angle = rect_angle;
    }

    Utils::add_float_to_vector(&v_vs, points*dim,
        rC, 0.f, zC,                            // A 3
        rC * cos(angle), rC*sin(angle), zC,     // B 4
        rC * cos(angle), rC*sin(angle), -zC,    // B' 5
        rC, 0.f, -zC);                          // A' 6
    // Pour ajouter les dernières coords ajoutées au QVector vs
    int ind = 0;
    if (oldSize != 0) {
        ind = oldSize/dim;
    }
    //qDebug() << "ind : " << ind;
    // On trace dans l'ordre de déclaration donc on remplit itérativement
    for (int decal=0; decal<points; decal++) {
        v_ind_ver.append(ind+decal);
    }

    /******* NORMALE ********/
    int idA;
    int idB;
    int idC;
    int idD;
    //qDebug() << "v_vs.size() : " << v_vs.size();
    if (v_vs.size() == 0) {
       qDebug() << "ERROR in " << __FUNCTION__
                << " : QVector v_vs vide";
       exit (1);
    }
    else {
        // On instancie en sens inverse pour obtenir les bons id
        // si on a déjà déclaré des normales avant.
        idD = v_vs.size()-dim;
        idC = idD-dim;
        idB = idC-dim;
        idA = idB-dim;
    }

    /*
    qDebug() << "idA : " << idA;
    qDebug() << "idB : " << idB;
    qDebug() << "idC : " << idC;
    qDebug() << "idD : " << idD;
    qDebug() << "v_vs.at(idA)" << v_vs.at(idA);
    qDebug() << "v_vs.at(idB)" << v_vs.at(idB);
    qDebug() << "v_vs.at(idC)" << v_vs.at(idC);
    qDebug() << "v_vs.at(idD)" << v_vs.at(idD);
    */

    GLfloat x = fabs(v_vs.at(idA) - v_vs.at(idB));
    GLfloat y = fabs(v_vs.at(idA+1) - v_vs.at(idB+1));
    GLfloat z = fabs(v_vs.at(idA+2) - v_vs.at(idB+2));
    QVector3D vAB(x, y, z);
    x = fabs(v_vs.at(idA) - v_vs.at(idC));
    y = fabs(v_vs.at(idA+1) - v_vs.at(idC+1));
    z = fabs(v_vs.at(idA+2) - v_vs.at(idC+2));
    QVector3D vAC(x, y, z);

    // normal() calcule la normale normalisée
    QVector3D nABC = QVector3D::normal(vAB, vAC);
    Utils::add_float_to_vector(&v_normals, dim,
                               nABC.x(), nABC.y(), nABC.z());

    // Indices
    ind = 0;
    if (v_ind_nor.size() != 0) {
        ind = v_ind_nor.size()/dim;
    }
    // Chaque point de la facette sera relié à la normale de la
    // ligne @idNor de @v_normals
    for (int p=0; p<points; p++) {
        v_ind_nor.append(ind);
    }

    v_nbPoints+=points;

//    qDebug() << "</" << __FUNCTION__ << ">";
}

void GLArea::build_coord_facette_base(int forme)
{
//    qDebug() << "<" << __FUNCTION__ << ">";
    float r = f_rayon;
    int points = 3;
    int dim = 3;
    int oldSize = v_vs.size();
    float angle;
    if (forme == DRAW_CYLINDRE) {
        angle = cyl_angle;
    }
    else {
        angle = rect_angle;
    }
//    qDebug() << "forme : " << forme;

    Utils::add_float_to_vector(&v_vs, points*dim, // 9
        0.f, 0.f, 0.f,    // O    0
        r, 0.f, 0.f,      // A    1
        r * cos(angle), r*sin(angle), 0.f); // B    2

    // Pour ajouter les dernières coords ajoutées au QVector vs
    int ind = 0;
    if (oldSize != 0) {
        ind = oldSize/dim;
    }
    //qDebug() << "ind : " << ind;
    // On trace dans l'ordre de déclaration donc on remplit itérativement
    for (int decal=0; decal<points; decal++) {
        v_ind_ver.append(ind+decal);
    }

    /******* NORMALE ********/
    int idB;
    int idA;
    int idO;
    //qDebug() << "vs->size() : " << v_vs.size();
    if (v_vs.size() == 0) {
       qDebug() << "ERROR in " << __FUNCTION__
                << " : QVector v_vs vide";
       exit (1);
    }
    else {
        // On instancie en sens inverse pour obtenir les bons id
        // si on a déjà déclaré des normales avant.
        idB = v_vs.size()-dim;
        idA = idB-(dim);
        idO = idA-(dim);
    }
    /*
    qDebug() << "idO : " << idO;
    qDebug() << "idA : " << idA;
    qDebug() << "idB : " << idB;
    qDebug() << "v_vs.at(idO)" << v_vs.at(idO);
    qDebug() << "v_vs.at(idA)" << v_vs.at(idA);
    qDebug() << "v_vs.at(idB)" << v_vs.at(idB);
    */

    GLfloat x = fabs(v_vs.at(idO) - v_vs.at(idA));
    GLfloat y = fabs(v_vs.at(idO+1) - v_vs.at(idA+1));
    GLfloat z = fabs(v_vs.at(idO+2) - v_vs.at(idA+2));
    QVector3D vOA(x, y, z);
    x = fabs(v_vs.at(idO) - v_vs.at(idB));
    y = fabs(v_vs.at(idO+1) - v_vs.at(idB+1));
    z = fabs(v_vs.at(idO+2) - v_vs.at(idB+2));
    QVector3D vOB(x, y, z);

    /*
    qDebug() << "vOA.x=" << vOA.x()
            << "\tvOA.y=" << vOA.y()
            << "\tvOA.z=" << vOA.z();
    qDebug() << "vOB.x=" << vOB.x()
            << "\tvOB.y=" << vOB.y()
            << "\tvOB.z=" << vOB.z();
    */

    // normal() calcule la normale normalisée
    QVector3D nOAB = QVector3D::normal(vOA, vOB);
    Utils::add_float_to_vector(&v_normals, dim,
                               nOAB.x(), nOAB.y(), nOAB.z());
    // Indices
    ind = 0;
    if (v_ind_nor.size() != 0) {
        ind = v_ind_nor.size()/dim;
    }
    // Chaque point de la facette sera relié à la normale de la
    // ligne @idNor de @v_normals
    Utils::add_int_to_vector(&v_ind_nor, points,
                  ind, ind, ind);

    v_nbPoints+=points;

//    qDebug() << "</" << __FUNCTION__ << ">";
}


void GLArea::lisser_figure()
{
//    qDebug() << "<" << __FUNCTION__ << ">";
    QVector3D nOAB;
    QVector3D nABC;

    nOAB.setX(v_normals[0]);
    nOAB.setY(v_normals[1]);
    nOAB.setZ(v_normals[2]);
    nABC.setX(v_normals[3]);
    nABC.setY(v_normals[4]);
    nABC.setZ(v_normals[5]);

    //QVector3D nAB = (nOAB + nOAB) / 2.0;
    QVector3D nAB = (nOAB + nABC) / 2.0;

    v_normals.append(nAB.x());
    v_normals.append(nAB.y());
    v_normals.append(nAB.z());

    v_ind_nor[1] = 2;
    v_ind_nor[2] = 2;
    v_ind_nor[3] = 2;
    v_ind_nor[4] = 2;
    v_ind_nor[5] = 2;
    v_ind_nor[6] = 2;

//    qDebug() << "</" << __FUNCTION__ << ">";
}

void GLArea::build_coord_figure(int forme)
{
//    qDebug() << "<" << __FUNCTION__ << ">";

    if (forme <= DRAW_CYLINDRE) v_pointDepBaseCyl = v_nbPoints;
    else    v_pointDepBaseRect = v_nbPoints;
    build_coord_facette_base(forme);

    if (forme <= DRAW_CYLINDRE) v_pointDepBlocCyl = v_nbPoints;
    else    v_pointDepBlocRect = v_nbPoints;
    build_coord_facette_bloc(forme);

    //lisser_figure();


//    qDebug() << "_nbPoints = " << v_nbPoints;
//    qDebug() << "vs : ";
//    for (int v=0; v<v_vs.size(); v++) {
//        qDebug() << ' ' << v_vs[v];
//    }
//    qDebug() << "ind_ver : ";
//    for (int iV=0; iV<v_ind_ver.size(); iV++) {
//        qDebug() << ' ' << v_ind_ver[iV];
//    }
//    qDebug() << "normals : ";
//    for (int n=0; n<v_normals.size(); n++) {
//        qDebug() << ' ' << v_normals[n];
//    }
//    qDebug() << "ind_nor : ";
//    for (int iN=0; iN<v_ind_nor.size(); iN++) {
//        qDebug() << ' ' << v_ind_nor[iN];
//    }

    /*
    qDebug() << "v_pointDepBaseCyl : " << v_pointDepBaseCyl;
    qDebug() << "v_pointDepBlocCyl : " << v_pointDepBlocCyl;
    qDebug() << "v_pointDepBaseRect : " << v_pointDepBaseRect;
    qDebug() << "v_pointDepBlocRect : " << v_pointDepBlocRect;
    */

//    qDebug() << "</" << __FUNCTION__ << ">";
}


void GLArea::makeGLObjects()
{
    //qDebug() << "<" << __FUNCTION__ << ">";
    //qDebug() << "v_nbPoints : " << v_nbPoints;

//    qDebug() << "\n******** CYLINDRE *********";
    build_coord_figure(DRAW_CYLINDRE);
//    qDebug() << "\n******** RECTANGLE *********";
    build_coord_figure(DRAW_RECTANGLE);

    GLfloat texCoords[] = {
        0.0, 1.0,
        1.0, 1.0,
        0.5, 0.0,
        0.0, 0.0,
        1.0, 0.0,
        0.5, 1.0
    };

    GLfloat colors[] = {
        1.0, 1.0, 1.0,      // triangle 1
        1.0, 0.6, 0.6,      // triangle 0
    };
    //int ind_col[] = { 0, 0, 0, 1, 1, 1, 0 };
    QVector<int> ind_col;
    for (int p=0; p<v_nbPoints; p++) {
        ind_col.append(0);
    }

//#if 0
    // Normales des triangles
    /*
    GLfloat normals[] = {
        nOAB.x(), nOAB.y(), nOAB.z()
    };
    int ind_nor[] = { 0, 0, 0 };
    */
//#else
    /*
    // Pour lissage de Phong
    QVector3D nAB = (nACB + nABD) / 2.0;

    GLfloat normals[] = { nACB.x(), nACB.y(), nACB.z(),
                          nABD.x(), nABD.y(), nABD.z(),
                          nAB.x(), nAB.y(), nAB.z()};
                   // A  C  B  A  B  D
    int ind_nor[] = { 2, 0, 2, 2, 2, 1 };
    */
//#endif


    QVector<GLfloat> vertData;
    for (int i = 0; i < v_nbPoints; ++i)
    {

        // coordonnées sommets
        for (int j = 0; j < 3; j++)
            vertData.append(v_vs[v_ind_ver[i]*3+j]);

        // couleurs sommets
        for (int j = 0; j < 3; j++)
            vertData.append(colors[ind_col[i]*3+j]);

        // coordonnées texture
        for (int j = 0; j < 2; j++)
            vertData.append(texCoords[i*2+j]);
        // normales sommets
        for (int j = 0; j < 3; j++)
            vertData.append(v_normals[v_ind_nor[i]*3+j]);
    }


    m_vbo.create();
    m_vbo.bind();
    m_vbo.allocate(vertData.constData(), vertData.count() * sizeof(GLfloat));

    for (int i = 0; i < 2; i++) {
        QString nom = QString(":/side%1.png").arg(i+1);
        QImage image(nom);
        if (image.isNull())
             qDebug() << "load image" << nom << "failed";
        else qDebug() << "load image" << nom << image.size();
        m_textures[i] = new QOpenGLTexture(image);
    }
    QString nom = QString(":/metalStrie.jpg");
    //QString nom = QString(":/orange.jpg");
    QImage image(nom);
    if (image.isNull())
         qDebug() << "load image" << nom << "failed";
    else qDebug() << "load image" << nom << image.size();
    m_textures[2] = new QOpenGLTexture(image);


    qDebug() << "\nHELP";
    qDebug() << "press a ton animate";
    qDebug() << "press space button ton rotate";

//    qDebug() << "</" << __FUNCTION__ << ">";
}


void GLArea::tearGLObjects()
{
    m_vbo.destroy();
    for (int i = 0; i < 3; i++)
        delete m_textures[i];
}


void GLArea::resizeGL(int w, int h)
{
//    qDebug() << __FUNCTION__ << w << h;

    // C'est fait par défaut
    glViewport(0, 0, w, h);

    m_ratio = (double) w / h;
    // doProjection();
}

void GLArea::draw_facette_bloc(int pointDepart)
{
    int nbPoints=4;
    glDrawArrays(GL_QUADS, pointDepart, nbPoints);
}

void GLArea::draw_facette_base(int pointDepart)
{
    int nbPoints=3;
    //glDrawArrays(GL_TRIANGLES, pointDepart, nbPoints);

    glDrawArrays(GL_POLYGON, pointDepart, nbPoints);
}

void GLArea::draw_cote_figure(QMatrix4x4 proj_mat, QMatrix4x4 world_mat, QMatrix4x4 cam_mat,
                                QMatrix3x3 normal_mat, int forme, int face)
{
    //qDebug() << "<" << __FUNCTION__ << ">";

    int nbFacettes;
    int pointDepart;
    float angle = 360.f;
    if (forme == DRAW_CYLINDRE)  {
        nbFacettes = cyl_nbFac;
        pointDepart = v_pointDepBaseCyl;
    }
    else if (forme == DRAW_DEMI_CYL) {
        if (face == 0) return;
        angle = 180.f;
        nbFacettes = cyl_nbFac/2;
        pointDepart = v_pointDepBaseCyl;
    }
    else {
        nbFacettes = rect_nbFac;
        pointDepart = v_pointDepBaseRect;
    }


    for (int facette=0; facette<nbFacettes; facette++)
    {
        world_mat.rotate(angle/nbFacettes, 0.f, 0.f, 1.f);
        normal_mat = world_mat.normalMatrix();
        m_program->setUniformValue("projMatrix", proj_mat);
        m_program->setUniformValue("mvMatrix", cam_mat*world_mat);
        m_program->setUniformValue("norMatrix", normal_mat);
        //m_program->setUniformValue("norMatrix", normal_mat);
        // TMP
        draw_facette_base(pointDepart);
    }
    //qDebug() << "</" << __FUNCTION__ << ">";

}

void GLArea::draw_figure(QMatrix4x4 proj_mat, QMatrix4x4 world_mat, QMatrix4x4 cam_mat,
                           QMatrix3x3 normal_mat, int forme)
{
    QMatrix4x4 world1 = world_mat;
    world1.translate(0.f, 0.f, f_ep/2.f);
    draw_cote_figure(proj_mat, world1, cam_mat, normal_mat, forme, 0);
    QMatrix4x4 world2 = world_mat;
    world2.translate(0.f, 0.f, -f_ep/2.f);
    draw_cote_figure(proj_mat, world2, cam_mat, normal_mat, forme, 1);
    QMatrix4x4 world3 = world_mat;

    int nbFacettes;
    int pointDepart;
    float angle = 360.f;
    if (forme == DRAW_CYLINDRE)  {
        nbFacettes = cyl_nbFac;
        pointDepart = v_pointDepBlocCyl;
    }
    else if (forme == DRAW_DEMI_CYL) {
        angle = 180.f;
        nbFacettes = cyl_nbFac/2;
        pointDepart = v_pointDepBlocCyl;
    }
    else {
        nbFacettes = rect_nbFac;
        pointDepart = v_pointDepBlocRect;
    }

    for (int facette=0; facette<nbFacettes; facette++)
    {
        world3 = world_mat;
        if (facette>0)  world3.rotate((facette*angle)/float(nbFacettes), 0.f, 0.f, 1.f);
        normal_mat = world3.normalMatrix();
        m_program->setUniformValue("projMatrix", proj_mat);
        m_program->setUniformValue("mvMatrix", cam_mat*world3);
        m_program->setUniformValue("norMatrix", normal_mat);
        draw_facette_bloc(pointDepart);
    }
}


void GLArea::move_light(QMatrix4x4 *proj_mat, QMatrix4x4 *cam_mat)
{
    proj_mat->translate(0.8f, 0.f, 0.f);
    cam_mat->translate(0.8f, 0.f, 0.f);
    //proj_mat->rotate(angle, 0.f, 1.f, 0.f);
    //cam_mat->rotate(angle, 0.f, 1.f, 0.f);
    proj_mat->rotate(45.f, 0.f, 1.f, 0.f);
    cam_mat->rotate(45.f, 0.f, 1.f, 0.f);
}

void GLArea::paintGL()
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_program->bind(); // active le shader program

    QMatrix4x4 proj_mat;
    GLfloat hr = m_radius, wr = hr * m_ratio;
    proj_mat.frustum(-wr, wr, -hr, hr, m_camZ, 5.0);

    QMatrix4x4 cam_mat;
    //cam_mat.translate(0, 0, -3.0);
    cam_mat.translate(0, 0, -3.0);
    cam_mat.rotate(m_angle, 0, 1, 0);


    //move_light(&proj_mat, &cam_mat);

    QMatrix4x4 world_mat;
    //world_mat.translate(-1, 0, 0);
    //world_mat.rotate(m_angle, 0, 1, 0);


    QMatrix3x3 normal_mat = world_mat.normalMatrix();

    m_program->setUniformValue("projMatrix", proj_mat);
    m_program->setUniformValue("mvMatrix", cam_mat*world_mat);
    m_program->setUniformValue("norMatrix", normal_mat);

    int taille = 11;
    m_program->setAttributeBuffer("posAttr",
        GL_FLOAT, 0 * sizeof(GLfloat), 3, taille * sizeof(GLfloat));

    m_program->setAttributeBuffer("colAttr",
        GL_FLOAT, 3 * sizeof(GLfloat), 3, taille * sizeof(GLfloat));

    m_program->setAttributeBuffer("texAttr",
            GL_FLOAT, 6 * sizeof(GLfloat), 2, taille * sizeof(GLfloat));

    m_program->setAttributeBuffer("norAttr",
        GL_FLOAT, 9 * sizeof(GLfloat), 3, taille * sizeof(GLfloat));
    m_program->enableAttributeArray("posAttr");
    m_program->enableAttributeArray("colAttr");
    m_program->enableAttributeArray("norAttr");

    m_textures[2]->bind();

    moteur(proj_mat, world_mat, cam_mat, normal_mat);

    m_textures[2]->release();

    m_program->disableAttributeArray("posAttr");
    m_program->disableAttributeArray("colAttr");
    m_program->disableAttributeArray("texAttr");
    m_program->disableAttributeArray("norAttr");

    m_program->release();
}

void GLArea::keyPressEvent(QKeyEvent *ev)
{
    //qDebug() << __FUNCTION__ << ev->text();

    switch(ev->key()) {
        case Qt::Key_Space :
            m_angle += 2;
            if (m_angle >= 360) m_angle -= 360;
            update();
            break;
        case Qt::Key_C :
            m_angle -= 2;
            if (m_angle >= 360) m_angle -= 360;
            update();
            break;
        case Qt::Key_A :
            if (m_timer->isActive())
                m_timer->stop();
            else m_timer->start();
            break;
        case Qt::Key_R :
            if (ev->text() == "r")
                 setRadius(m_radius-0.05);
            else setRadius(m_radius+0.05);
            break;
        case Qt::Key_Z :
            if (ev->text() == "z") setZ(m_camZ-0.05);
            else setZ(m_camZ+0.05);
            break;
    }
}

void GLArea::keyReleaseEvent(QKeyEvent *ev)
{
    (void) ev;
    //qDebug() << __FUNCTION__ << ev->text();
}

void GLArea::mousePressEvent(QMouseEvent *ev)
{
    qDebug() << __FUNCTION__ << ev->x() << ev->y() << ev->button();
}

void GLArea::mouseReleaseEvent(QMouseEvent *ev)
{
    qDebug() << __FUNCTION__ << ev->x() << ev->y() << ev->button();
}

void GLArea::mouseMoveEvent(QMouseEvent *ev)
{
    qDebug() << __FUNCTION__ << ev->x() << ev->y();
}

void GLArea::onTimeout()
{
    qDebug() << __FUNCTION__ ;
    m_anim += 0.01;
    if (m_anim > 1) m_anim = 0;
    m_alpha += 0.05f;
    if (m_alpha > 360.f) m_alpha = 0.f;

    update();
}

void GLArea::setRadius(double radius)
{
    qDebug() << __FUNCTION__ << radius << sender();
    if (radius != m_radius && radius > 0.01 && radius <= 10) {
        m_radius = radius;
        qDebug() << "  emit radiusChanged()";
        emit radiusChanged(radius);
        update();
    }
}

void GLArea::setZ(double z)
{
    qDebug() << __FUNCTION__ << z << sender();
    if (z != m_camZ && z > 0.01 && z <= 10) {
        m_camZ = z;
        qDebug() << "  emit zChanged()";
        emit zChanged(z);
        update();
    }
}

void GLArea::update_attributes(int orientation)
{
    if (orientation == 0) {
        _xH = _x0 + _dist0H * cos(m_alpha);
        _yH= _y0 + _dist0H * sin(m_alpha);
    }
    else {
        _xH = _x0 - _dist0H * cos(m_alpha);
        _yH= _y0 - _dist0H * sin(m_alpha);
    }
    _xI = _xH;
    _yI = _y0;
    _xJ = _xI + sqrt( pow(_distHJ, 2) -  pow((_dist0H*sin(m_alpha)), 2) );
    _yJ = _y0;
    _xK = _demiCyl_ep + _dist0H;
    _yk = _yJ;

    if (fabs(_xJ-_xH) != 0)    _beta = -(atan(_yH / fabs(_xJ-_xH)));
    else _beta = 0;
    _beta_animation = (_beta*180.0)/(M_PI);
}

void GLArea::partie_vilebrequin(QMatrix4x4 proj_mat, QMatrix4x4 world_mat,
                                QMatrix4x4 cam_mat, QMatrix3x3 normal_mat,
                                float z1, float z2, float z3, float z4, int orientation)
{
    float angle_animation = (m_alpha*180.0)/(M_PI);
    update_attributes(orientation);

    QMatrix4x4 proj;
    QMatrix4x4 world;
    QMatrix4x4 cam;
    QMatrix3x3 normalM;

    proj = proj_mat;
    cam = cam_mat;
    normalM = normal_mat;

    // Piston z1
    world = world_mat;
    world.translate(_x0, _y0, z1);
    world.rotate(angle_animation, 0.f, 0.f, 1.f);
    world.scale(_piston_r, _piston_r, _piston_ep);
    normalM = world.normalMatrix();
    draw_figure(proj, world, cam, normalM, DRAW_CYLINDRE);

    // Rect z2
    world = world_mat;
    world.translate(_x0, _y0, z2);
    world.rotate(angle_animation, 0.f, 0.f, 1.f);
    if (orientation == 0) {
        world.translate(_piston_ep/2.f, 0.f, 0.f);
    }
    else {
        world.translate(-_piston_ep/2.f, 0.f, 0.f);
    }
    world.rotate(90.f, 0.f, 0.f, 1.f);
    world.rotate(90.f, 1.f, 0.f, 0.f);
    world.rotate(45.f, 0.f, 0.f, 1.f);
    world.scale(_piston_r, _piston_r, _rect_ep);
    normalM = world.normalMatrix();
    draw_figure(proj, world, cam, normalM, DRAW_RECTANGLE);

    // Petit piston z3
    world = world_mat;
    world.translate(_xH, _yH, z3);
    //world.rotate(angle_animation, 0.f, 0.f, 1.f);
    world.scale(_piston_r, _piston_r, _petitPist_ep);
    normalM = world.normalMatrix();
    draw_figure(proj, world, cam, normalM, DRAW_CYLINDRE);

    // CylindreInterne z3
    world = world_mat;
    world.translate(_xJ+_cylInterne_ep/2, _yJ, z3);
    world.rotate(180, 0.f, 0.f, 1.f);
    world.rotate(90, 0.f, 1.f, 0.f);
    world.scale(_cylInterne_r, _cylInterne_r, _cylInterne_ep);
    normalM = world.normalMatrix();
    draw_figure(proj, world, cam, normalM, DRAW_CYLINDRE);

    // CylindreLong z3
    world = world_mat;
    world.translate(_xJ-(_distHJ/2.0)*cos(_beta),
                    _yJ-(_distHJ/2.0)*sin(_beta),
                    z3);
    //world.rotate(180.f, 0.f, 1.f, 0.f);
    world.rotate(_beta_animation, 0.f, 0.f, 1.f);
    world.rotate(180, 0.f, 0.f, 1.f);
    world.rotate(90, 0.f, 1.f, 0.f);
    world.scale(_cylLong_r, _cylLong_r, _cylLong_ep);
    normalM = world.normalMatrix();
    draw_figure(proj, world, cam, normalM, DRAW_CYLINDRE);

    // Demi_cylindre z3
    world = world_mat;
    world.translate(_xK, _yk, z3);
    world.rotate(180, 0.f, 0.f, 1.f);
    world.rotate(90, 0.f, 1.f, 0.f);
    world.scale(_demiCyl_r, _demiCyl_r, _demiCyl_ep);
    normalM = world.normalMatrix();
    draw_figure(proj, world, cam, normalM, DRAW_DEMI_CYL);

    // Rect z4
    world = world_mat;
    world.translate(_x0, _y0, z4);
    world.rotate(angle_animation, 0.f, 0.f, 1.f);
    if (orientation == 0) {
        world.translate(_piston_ep/2.f, 0.f, 0.f);
    }
    else {
        world.translate(-_piston_ep/2.f, 0.f, 0.f);
    }
    world.rotate(-90.f, 0.f, 0.f, 1.f);
    world.rotate(90.f, 1.f, 0.f, 0.f);
    world.rotate(45.f, 0.f, 0.f, 1.f);
    world.scale(_piston_r, _piston_r, _rect_ep);
    normalM = world.normalMatrix();
    draw_figure(proj, world, cam, normalM, DRAW_RECTANGLE);
}

void GLArea::moteur(QMatrix4x4 proj_mat, QMatrix4x4 world_mat,
                    QMatrix4x4 cam_mat, QMatrix3x3 normal_mat)
{
    QMatrix4x4 proj;
    QMatrix4x4 world;
    QMatrix4x4 cam;
    QMatrix3x3 normalM;

    proj = proj_mat;
    cam = cam_mat;
    normalM = normal_mat;

    float angle_animation = (m_alpha*180.0)/(M_PI);

    // Roue z0
    world = world_mat;
    world.translate(_x0, _y0, _z0);
    world.rotate(angle_animation, 0.f, 0.f, 1.f);
    world.scale(_z0_r, _z0_r, _z0_ep);
    normalM = world.normalMatrix();
    draw_figure(proj, world, cam, normalM, DRAW_CYLINDRE);


    update_attributes(0);

    // Piston z00
    float z00 = _z0-0.5f;
    world = world_mat;
    world.translate(_x0, _y0, z00);
    world.rotate(angle_animation, 0.f, 0.f, 1.f);
    world.scale(_piston_r, _piston_r, 1.f);
    normalM = world.normalMatrix();
    draw_figure(proj, world, cam, normalM, DRAW_CYLINDRE);


    float z1, z2, z3, z4;
    z1 = _z0 + (_piston_ep/2.f);
    z2 = z1+(_rect_ep/2.f);
    z3 = z2+(_piston_ep)/2;
    z4 = z3+(_piston_ep)/2;
    partie_vilebrequin(proj_mat, world_mat, cam_mat, normal_mat,
                       z1, z2, z3, z4, 0);
    z1 = z4 + (_piston_ep*(3.f/4.f));
    z2 = z1+(_rect_ep/2.f);
    z3 = z2+(_piston_ep)/2;
    z4 = z3+(_piston_ep)/2;
    partie_vilebrequin(proj_mat, world_mat, cam_mat, normal_mat,
                       z1, z2, z3, z4, 1);
    z1 = z4 + (_piston_ep*(3.f/4.f));
    z2 = z1+(_rect_ep/2.f);
    z3 = z2+(_piston_ep)/2;
    z4 = z3+(_piston_ep)/2;
    partie_vilebrequin(proj_mat, world_mat, cam_mat, normal_mat,
                       z1, z2, z3, z4, 1);
    z1 = z4 + (_piston_ep*(3.f/4.f));
    z2 = z1+(_rect_ep/2.f);
    z3 = z2+(_piston_ep)/2;
    z4 = z3+(_piston_ep)/2;
    partie_vilebrequin(proj_mat, world_mat, cam_mat, normal_mat,
                       z1, z2, z3, z4, 0);

}


