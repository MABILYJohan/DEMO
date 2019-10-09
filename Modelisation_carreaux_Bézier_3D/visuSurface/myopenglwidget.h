#ifndef MYOPENGLWIDGET_H
#define MYOPENGLWIDGET_H

#include <QObject>
#include <QWidget>

#include <QKeyEvent>
#include <QTimer>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>

#include "surface.h"
#include "carreauxbezier.h"

class myOpenGLWidget : public QOpenGLWidget,
			   protected QOpenGLFunctions
{
	Q_OBJECT

public:
	explicit myOpenGLWidget(QWidget *parent = nullptr);
	~myOpenGLWidget();

public slots:
    void on_set_u(double newU);
    void on_set_nbU(int newNbU);
    void on_set_v(double newV);
    void on_set_nbV(int newNbV);

    void on_pt_ctrl(bool value);
    void on_triangulation(bool value);
    void on_vertices(bool value);
    void on_edges(bool value);
    void on_carreaux(bool value);
    void on_forme(bool value);

signals:  // On ne les implémente pas, elles seront générées par MOC ;
          // les paramètres seront passés aux slots connectés.
    void u_changed(double newU);
    void nbU_changed(int newNbU);
    void v_changed(double newV);
    void nbV_changed(int newNbV);

    void flag_pt_ctrl_changed(bool value);
    void flag_triangulation_changed(bool value);
    void flag_vertices_changed(bool value);
    void flag_edges_changed(bool value);
    void flag_carreaux_changed(bool value);
    void flag_forme_changed(bool value);

protected slots:
	void onTimeout();

protected:
	void initializeGL() override;
	void doProjection();
	void resizeGL(int w, int h) override;
	void paintGL() override;
	void keyPressEvent(QKeyEvent *ev) override;
	void keyReleaseEvent(QKeyEvent *ev) override;
	void mousePressEvent(QMouseEvent *ev) override;
	void mouseReleaseEvent(QMouseEvent *ev) override;
	void mouseMoveEvent(QMouseEvent *ev) override;

private:

    // BASE OPENGL
    double m_angle = 1;
	QTimer *m_timer = nullptr;
	double m_ratio = 1;

	//RR matrices utiles
	QMatrix4x4 m_modelView;
	QMatrix4x4 m_projection;
	QMatrix4x4 m_model;

	QOpenGLShaderProgram *m_program;
    QOpenGLBuffer m_vbo1;
    QOpenGLBuffer m_vbo2;
    QOpenGLBuffer m_vbo3;

	void makeGLObjects();
	void tearGLObjects();

    // MY OPENGL
    CarreauxBezier *_carBez;
    QVector<GLfloat> _vertData1;
    QVector<GLfloat> _vertData2;
    QVector<GLfloat> _vertData3;
    void switch_vbo(int idVbo);
    void paint_carreaux_triangle();
    void paint_carreaux_lines();
    void paint_vbo(int idVbo, QMatrix4x4 *m);
    void maj_discretisation_carBez();
    void maj_point();

    // UTILS
    bool _flagPtCtrl=false;
    bool _flagTriangle=true;
    bool _flagVertices=false;
    bool _flagEdges=true;
    bool _flagCarreaux=true;
    bool _flagForme=false;
    int _n=0;
    int _m=0;
    int _nbU=0;
    int _nbV=0;
    float _pointU=0;
    float _pointV=0;
};


#endif // MYOPENGLWIDGET_H
