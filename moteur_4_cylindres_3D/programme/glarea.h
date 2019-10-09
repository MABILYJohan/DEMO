
#ifndef GLAREA_H
#define GLAREA_H
#ifndef M_PI
#define M_PI 3.14159
#endif

#include <QKeyEvent>
#include <QTimer>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>

#include <cmath>
#include "utils.h"

enum Dessin { DRAW_CYLINDRE, DRAW_DEMI_CYL, DRAW_RECTANGLE };

class GLArea : public QOpenGLWidget,
               protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit GLArea(QWidget *parent = 0);
    ~GLArea();

public slots:
    void setRadius(double radius);
    void setZ(double z);

signals:  // On ne les implémente pas, elles seront générées par MOC ;
          // les paramètres seront passés aux slots connectés.
    void radiusChanged(double newRadius);
    void zChanged(double newZ);

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

    void move_light(QMatrix4x4 *proj_mat, QMatrix4x4 *cam_mat);

    void draw_facette_bloc(int pointDepart);
    void draw_facette_base(int pointDepart);
    void draw_cote_figure(QMatrix4x4 proj_mat, QMatrix4x4 world_mat,
                          QMatrix4x4 cam_mat, QMatrix3x3 normal_mat, int forme, int face);
    void draw_figure(QMatrix4x4 proj_mat, QMatrix4x4 world_mat,
                     QMatrix4x4 cam_mat, QMatrix3x3 normal_mat, int forme);

    void build_coord_facette_bloc(int forme);
    void build_coord_facette_base(int forme);
    void build_coord_figure(int forme);
    void lisser_figure();

    void update_attributes(int orientation);
    void partie_vilebrequin(QMatrix4x4 proj_mat, QMatrix4x4 world_mat,
                            QMatrix4x4 cam_mat, QMatrix3x3 normal_mat,
                            float z1, float z2, float z3, float z4,
                            int orientation);
    void moteur(QMatrix4x4 proj, QMatrix4x4 world,
                QMatrix4x4 cam, QMatrix3x3 normalM);

private:
    double m_angle = 0;
    QTimer *m_timer = nullptr;
    double m_anim = 0;
    double m_radius = 0.5;
    double m_camZ = 1.0;
    double m_ratio = 1;

    float m_alpha = 0;

    QOpenGLShaderProgram *m_program;
    QOpenGLTexture *m_textures[3];

    void makeGLObjects();
    void tearGLObjects();
    QOpenGLBuffer m_vbo;

    QVector<GLfloat> v_vs;
    QVector<int> v_ind_ver;
    int v_nbPoints=0;
    QVector<GLfloat> v_normals;
    QVector<int> v_ind_nor;
    int v_pointDepBaseCyl=0;
    int v_pointDepBlocCyl=0;
    int v_pointDepBaseRect=0;
    int v_pointDepBlocRect=0;

    // MOTEUR
    float f_rayon = 1.f;
    float f_ep = 1.f;
    int cyl_nbFac = 40;
    float cyl_angle = 2.0*M_PI/float(cyl_nbFac);
    int rect_nbFac = 4;
    float rect_angle = 2.0*M_PI/float(rect_nbFac);

    float _factor = 0.5f;

    float _z0 = -1.f;
    float _x0 = 0.f;
    float _y0 = 0.f;
    float _z0_r = 1.5f*_factor,  _z0_ep = 0.1f*_factor;
    float _x3 = _x0+2.5f;
    float _y3 = _y0;

    float _piston_r = _z0_r * 0.15f;
    float _piston_ep = _z0_ep * 6.f;
    float _petitPist_ep = _z0_ep * 3.f;
    float _rect_ep = _piston_ep * 1.5f;
    float _cylInterne_r = _piston_r * 1.5f;
    float _cylInterne_ep = _piston_ep * 2.5f;
    float _demiCyl_r = _cylInterne_r*1.05;
    float _demiCyl_ep = _cylInterne_ep * 1.2f;
    float _cylLong_r = _piston_r * 0.5f;
    //float _cylLong_ep = rect_ep*2.f;
    float _cylLong_ep = _rect_ep*1.5f;

    float _dist0H = _z0_r*0.4f;
    float _distHJ = _dist0H*2.f;

    float _x2 = fabs(_x3 - (_x0+_dist0H)) - _demiCyl_ep/2.f;
    float _y2 = _y0;

    float _xH = _x0 + _dist0H * cos(m_alpha);
    float _yH= _y0 + _dist0H * sin(m_alpha);
    float _xI = _xH;
    float _yI = _y0;
    float _xJ = _xI + sqrt( pow(_distHJ, 2) -  pow((_dist0H*sin(m_alpha)), 2) );
    float _yJ = _y0;
    float _xK = _demiCyl_ep + _dist0H;
    float _yk = _yJ;
    float _beta;
    float _beta_animation;
};

#endif // GLAREA_H
