//R. Raffin, M1 Informatique, "Surfaces 3D"
//tiré de CC-BY Edouard.Thiel@univ-amu.fr - 22/01/2019

#include "myopenglwidget.h"
#include <QDebug>
#include <QSurfaceFormat>
#include <QMatrix4x4>

#include <iostream>

#include "segment.h"

#include "utils.h"
#include "convertvbo.h"


static const QString vertexShaderFile   = ":/vertex.glsl";
static const QString fragmentShaderFile = ":/fragment.glsl";


myOpenGLWidget::myOpenGLWidget(QWidget *parent) :
	QOpenGLWidget(parent)
{
	qDebug() << "init myOpenGLWidget" ;

	QSurfaceFormat sf;
	sf.setDepthBufferSize(24);
	sf.setSamples(16);  // nb de sample par pixels : suréchantillonnage por l'antialiasing, en décalant à chaque fois le sommet
						// cf https://www.khronos.org/opengl/wiki/Multisampling et https://stackoverflow.com/a/14474260
	setFormat(sf);

	setEnabled(true);  // événements clavier et souris
	setFocusPolicy(Qt::StrongFocus); // accepte focus
	setFocus();                      // donne le focus

	m_timer = new QTimer(this);
	m_timer->setInterval(50);  // msec
	connect (m_timer, SIGNAL(timeout()), this, SLOT(onTimeout()));

    connect (this, SIGNAL(u_changed(double)), this, SLOT(on_set_u(double)));
    connect (this, SIGNAL(nbU_changed(int)), this, SLOT(on_set_nbU(int)));

    connect (this, SIGNAL(v_changed(double)), this, SLOT(on_set_v(double)));
    connect (this, SIGNAL(nbV_changed(int)), this, SLOT(on_set_nbV(int)));

    connect (this, SIGNAL(flag_pt_ctrl_changed(bool)), this, SLOT(on_pt_ctrl(bool)));
    connect (this, SIGNAL(flag_triangulation_changed(bool)), this, SLOT(on_triangulation(bool)));
    connect (this, SIGNAL(flag_vertices_changed(bool)), this, SLOT(on_vertices(bool)));
    connect (this, SIGNAL(flag_edges_changed(bool)), this, SLOT(on_edges(bool)));
    connect (this, SIGNAL(flag_carreaux_changed(bool)), this, SLOT(on_carreaux(bool)));
    connect (this, SIGNAL(flag_forme_changed(bool)), this, SLOT(on_forme(bool)));
}

myOpenGLWidget::~myOpenGLWidget()
{
	qDebug() << "destroy GLArea";

	delete m_timer;

	// Contrairement aux méthodes virtuelles initializeGL, resizeGL et repaintGL,
	// dans le destructeur le contexte GL n'est pas automatiquement rendu courant.
	makeCurrent();
	tearGLObjects();
	doneCurrent();
}


void myOpenGLWidget::initializeGL()
{
	qDebug() << __FUNCTION__ ;
	initializeOpenGLFunctions();
	glEnable(GL_DEPTH_TEST);

	makeGLObjects();

	//shaders
	m_program = new QOpenGLShaderProgram(this);
	m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, vertexShaderFile);  // compile
	m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, fragmentShaderFile);

	if (! m_program->link()) {  // édition de lien des shaders dans le shader program
		qWarning("Failed to compile and link shader program:");
		qWarning() << m_program->log();
	}
}

void myOpenGLWidget::doProjection()
{
	//m_mod.setToIdentity();
	//modelMatrix.ortho( -aratio, aratio, -1.0f, 1.0f, -1.0f, 1.0f );
}

void myOpenGLWidget::makeGLObjects()
{
//    qDebug() << "<" << __FUNCTION__ << ">";

    _n=4;
    _m=4;
    _carBez = new CarreauxBezier(_n, _m);
    qDebug() << _carBez->degre_n();
    qDebug() << _carBez->degre_m();
//    _carBez->display();

    vector<Point> pointControles = Utils::tab_to_list(_carBez->coords(), _n, _m);
    _vertData1 = ConvertVbo::convert_to_grid(pointControles, _n, _m,
                                                 100.f, 0.f, 0.f);

    _nbU=4;
    _nbV=4;

    m_vbo1.create();
    m_vbo2.create();
    m_vbo3.create();

    maj_discretisation_carBez();

//    qDebug() << "</" << __FUNCTION__ << ">";
}

/*-------------------------------------------------------------------------------------
 * Met à jour le carreaux de bézier en fonction des paramètres
 * @_nbU et @_nbV
 * -----------------------------------------------------------------------------------*/
void myOpenGLWidget::maj_discretisation_carBez()
{
//    qDebug() << "<" << __FUNCTION__ << ">";

    _carBez->generer_carreaux(0, _nbU, _nbV);
    vector<Point> coordsCarreaux = Utils::tab_to_list( _carBez->coordsCarreaux(), _nbU, _nbV );

    if (_flagTriangle)
    {
        _vertData2 = ConvertVbo::convert_triangulation(coordsCarreaux, _nbU, _nbV,
                                                 0.f, 100.f, 0.f);
    }
    else {
        _vertData2 = ConvertVbo::convert_to_grid(coordsCarreaux, _nbU, _nbV,
                                                 0.f, 100.f, 0.f);
    }

    update();
//    qDebug() << "</" << __FUNCTION__ << ">";
}

/*-------------------------------------------------------------------------------------
 * Met à jour les coordonnées du point donné par les paramètres
 * @_pointU et @_pointV
 * -----------------------------------------------------------------------------------*/
void myOpenGLWidget::maj_point()
{
//    qDebug() << "<" << __FUNCTION__ << ">";
    if (_pointU>=0.f && _pointU<=1.f
            && _pointV>=0.f && _pointV<=1.f)
    {
        _vertData3.clear();
        vector<Point> tmp;
        tmp.push_back(_carBez->eval(_pointU, _pointV));
        Point pTmp; pTmp.set(-1.f, 1.f, 1.f);
        tmp.push_back(pTmp);
        _vertData3 = ConvertVbo::convert_listPoints_to_vbo(tmp, 0.f, 0.f, 100.f);
        update();
    }
//    qDebug() << "</" << __FUNCTION__ << ">";
}

void myOpenGLWidget::tearGLObjects()
{
    m_vbo1.destroy();
    m_vbo2.destroy();
    m_vbo3.destroy();
}

void myOpenGLWidget::resizeGL(int w, int h)
{
	qDebug() << __FUNCTION__ << w << h;

	//C'est fait par défaut
	glViewport(0, 0, w, h);

	m_ratio = (double) w / h;
	//doProjection();
}

/*------------------------------------------
 * Change le vbo courant pour le vbo @idVbo
 * ----------------------------------------*/
void myOpenGLWidget::switch_vbo(int idVbo)
{
    if (idVbo==1)
    {
        m_vbo1.bind();
        m_vbo1.allocate(_vertData1.constData(), _vertData1.count() * sizeof(GLfloat));
        //m_vbo1.release();
    }
    else if (idVbo==2)
    {
        m_vbo2.bind();
        m_vbo2.allocate(_vertData2.constData(), _vertData2.count() * sizeof(GLfloat));
        //m_vbo2.release();
    }
    else if (idVbo>=3)
    {
        m_vbo3.bind();
        m_vbo3.allocate(_vertData3.constData(), _vertData3.count() * sizeof(GLfloat));
    }
}

/*----------------------------------------
 * Dessine le carreaux par triangulation
 * --------------------------------------*/
void myOpenGLWidget::paint_carreaux_triangle()
{
    // 6 = nb de vertices dessinés par i.
    if (_flagVertices) glDrawArrays(GL_POINTS, 0, (_nbU-1)*(_nbV-1)*6);
    //glDrawArrays(GL_LINE_STRIP, 4, 4);

    if (!_flagEdges) return;
    for (int i=0; i<(_nbU-1)*(_nbV-1)*6; i+=3)
    {
        if (_flagForme) glDrawArrays(GL_TRIANGLES, i, 3);
        else            glDrawArrays(GL_LINE_LOOP, i, 3);
    }
}

/*----------------------------------------
 * Dessine le carreaux par lignes
 * !!! _flagForme ne marche pas très bien
 *  actuellement pour cette fonction
 * --------------------------------------*/
void myOpenGLWidget::paint_carreaux_lines()
{
    if (_flagVertices)glDrawArrays(GL_POINTS, 0, _nbU*_nbV);

    if (!_flagEdges) return;
    for (int i=0; i<_nbU*_nbV; i+=_nbV)
    {
        if (_flagForme) glDrawArrays(GL_POLYGON, i, _nbV);
        else            glDrawArrays(GL_LINE_STRIP, i, _nbV);

    }

    for (int i=_nbU*_nbV; i<(_nbU*_nbV)*2; i+=_nbU)
    {
        if (_flagForme) glDrawArrays(GL_POLYGON, i, _nbU);
        else            glDrawArrays(GL_LINE_STRIP, i, _nbU);
    }
}

/*----------------------------------------
 * Peindre en fonction du vbo @idVbo
 * --------------------------------------*/
void myOpenGLWidget::paint_vbo(int idVbo, QMatrix4x4 *m)
{
    switch_vbo(idVbo);

    m_program->setUniformValue("matrix", *m);

    // posAttr taille 3 + colAttr taille 3
    int taille = 6;
    m_program->setAttributeBuffer("posAttr",
                                  GL_FLOAT, 0 * sizeof(GLfloat), 3, taille * sizeof(GLfloat));
    m_program->setAttributeBuffer("colAttr",
                                  GL_FLOAT, 3 * sizeof(GLfloat), 3, taille * sizeof(GLfloat));
    m_program->enableAttributeArray("posAttr");
    m_program->enableAttributeArray("colAttr");

    glPointSize (5.0f);

    // Point de controle
    if (idVbo==1)
    {
        if (_flagVertices)glDrawArrays(GL_POINTS, 0, _n*_m);

        if (!_flagEdges) return;
        for (int i=0; i<_n*_m; i+=_n)
        {
            //qDebug() << "i=" << i << "à" << i+(_n-1);
            glDrawArrays(GL_LINE_STRIP, i, _n);
        }

        for (int i=_n*_m; i<(_n*_m)*2; i+=_n)
        {
            //qDebug() << "i=" << i << "à" << i+(_n-1);
            glDrawArrays(GL_LINE_STRIP, i, _n);
        }
    }
    // Carreaux bezier
    else if (idVbo==2)
    {
        if (_flagTriangle)   paint_carreaux_triangle();
        else                paint_carreaux_lines();
    }
    // Point courant
    else if (idVbo>=3) {
        glDrawArrays(GL_POINTS, 0, 2);
        glDrawArrays(GL_LINE_STRIP, 0, 2);
    }

    m_program->disableAttributeArray("posAttr");
    m_program->disableAttributeArray("colAttr");

}

void myOpenGLWidget::paintGL()
{
	qDebug() << __FUNCTION__ ;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_program->bind(); // active le shader program


	/// Ajout RR pour gérer les 3 matrices utiles
	/// à mettre dans doProjection() pour clarifier
	/// -----------------------------------------
		m_modelView.setToIdentity();
		m_modelView.lookAt(QVector3D(0.0f, 0.0f, 3.0f),    // Camera Position
						 QVector3D(0.0f, 0.0f, 0.0f),    // Point camera looks towards
						 QVector3D(0.0f, 1.0f, 0.0f));   // Up vector

		m_projection.setToIdentity ();
		m_projection.perspective(70.0, width() / height(), 0.1, 100.0); //ou m_ratio

        //m_model.translate(0, 0, -3.0);

		// Rotation de la scène pour l'animation
        if (m_timer->isActive()) {
            m_model.rotate(m_angle, 0, 1, 0);
        }

		QMatrix4x4 m = m_projection * m_modelView * m_model;
	///----------------------------

    if (_flagPtCtrl) paint_vbo(1, &m);
    if (_flagCarreaux) paint_vbo(2, &m);
    paint_vbo(3, &m);

    m_program->release();
}

void myOpenGLWidget::keyPressEvent(QKeyEvent *ev)
{
	qDebug() << __FUNCTION__ << ev->text();

	switch(ev->key()) {
		case Qt::Key_Z :
            qDebug() << "m_angle+1";
            m_angle += 1;
            if (m_angle >= 360) m_angle -= 360;
			update();
			break;
		case Qt::Key_A :
			if (m_timer->isActive())
				m_timer->stop();
			else m_timer->start();
			break;
		case Qt::Key_R :
			break;
	}
}

void myOpenGLWidget::keyReleaseEvent(QKeyEvent *ev)
{
	qDebug() << __FUNCTION__ << ev->text();
}

void myOpenGLWidget::mousePressEvent(QMouseEvent *ev)
{
	qDebug() << __FUNCTION__ << ev->x() << ev->y() << ev->button();
}

void myOpenGLWidget::mouseReleaseEvent(QMouseEvent *ev)
{
	qDebug() << __FUNCTION__ << ev->x() << ev->y() << ev->button();
}

void myOpenGLWidget::mouseMoveEvent(QMouseEvent *ev)
{
	qDebug() << __FUNCTION__ << ev->x() << ev->y();
}

void myOpenGLWidget::onTimeout()
{
	qDebug() << __FUNCTION__ ;

	update();
}



///////////////////////////// SIGNAUX //////////////////////////////////////////

void myOpenGLWidget::on_set_nbU(int newNbU)
{
    qDebug() << __FUNCTION__ << "The event sender is" << sender();
    _nbU = newNbU;
//    qDebug() << "_nbU = " << _nbU;
    maj_discretisation_carBez();
}

void myOpenGLWidget::on_set_nbV(int newNbV)
{
    qDebug() << __FUNCTION__ << "The event sender is" << sender();
    _nbV = newNbV;
//    qDebug() << "_myNbV = " << _nbV;
    maj_discretisation_carBez();
}

void myOpenGLWidget::on_set_u(double newU)
{
    qDebug() << __FUNCTION__ << "The event sender is" << sender();
    _pointU = float(newU);
    maj_point();
}

void myOpenGLWidget::on_set_v(double newV)
{
    qDebug() << __FUNCTION__ << "The event sender is" << sender();
    _pointV = float(newV);
    maj_point();
}

void myOpenGLWidget::on_pt_ctrl(bool value)
{
    qDebug() << __FUNCTION__ << "The event sender is" << sender();
    _flagPtCtrl = value;
    update();
}

void myOpenGLWidget::on_triangulation(bool value)
{
    qDebug() << __FUNCTION__ << "The event sender is" << sender();
    _flagTriangle = value;
    maj_discretisation_carBez();
}

void myOpenGLWidget::on_vertices(bool value)
{
    qDebug() << __FUNCTION__ << "The event sender is" << sender();
    _flagVertices = value;
    update();
}

void myOpenGLWidget::on_edges(bool value)
{
    qDebug() << __FUNCTION__ << "The event sender is" << sender();
    _flagEdges = value;
    update();
}

void myOpenGLWidget::on_carreaux(bool value)
{
    qDebug() << __FUNCTION__ << "The event sender is" << sender();
    _flagCarreaux = value;
    update();
}

void myOpenGLWidget::on_forme(bool value)
{
    qDebug() << __FUNCTION__ << "The event sender is" << sender();
    _flagForme = value;
    update();
}
