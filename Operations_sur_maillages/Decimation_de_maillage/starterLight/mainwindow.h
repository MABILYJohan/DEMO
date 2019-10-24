#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QFileDialog>
#include <QMainWindow>
#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>
#include <QProgressDialog>

namespace Ui {
class MainWindow;
}

using namespace OpenMesh;
using namespace OpenMesh::Attributes;

enum enumParamSeuil {PARAM_TAILLE, PARAM_ANGLE, PARAM_PLAN};

struct reductInfos
{
    int nbReduct;    // Nb de fois ou on réduit le maillage
    int nbEdges;    // Nb d'eges restantes après simplification
    int precision;  // La précision avec laquelle on a simplifié le maillage
};

struct MyTraits : public OpenMesh::DefaultTraits
{
    // use vertex normals and vertex colors
    VertexAttributes( OpenMesh::Attributes::Normal | OpenMesh::Attributes::Color | OpenMesh::Attributes::Status);
    // store the previous halfedge
    HalfedgeAttributes( OpenMesh::Attributes::PrevHalfedge );
    // use face normals face colors
    FaceAttributes( OpenMesh::Attributes::Normal | OpenMesh::Attributes::Color | OpenMesh::Attributes::Status);
    EdgeAttributes( OpenMesh::Attributes::Color | OpenMesh::Attributes::Status );
    // vertex thickness
    VertexTraits{float thickness; float value;};
    // edge thickness
    EdgeTraits{float thickness;};
};
typedef OpenMesh::TriMesh_ArrayKernelT<MyTraits> MyMesh;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    // les fonctions à compléter
    void showEdgeSelection(MyMesh* _mesh);
    void collapseEdge(MyMesh* _mesh, int edgeID);

    void decimationPlaneite(MyMesh* _mesh, int percent);
    void decimationAngle(MyMesh* _mesh, int percent);
    void decimationTailleQS(MyMesh* _mesh, int percent);
    void decimationTaille(MyMesh* _mesh, int percent);
    void decimationAleatoire(MyMesh* _mesh, int percent);
    void decimation(MyMesh* _mesh, int percent, QString method);

    reductInfos maj_RI(int nbEdges, int precision, int nbReduct);

    void updateEdgeSelectionIHM();

    void displayMesh(MyMesh *_mesh, bool isTemperatureMap = false, float mapRange = -1);
    void resetAllColorsAndThickness(MyMesh* _mesh);

private:

    bool modevoisinage;

    MyMesh mesh;

    int vertexSelection;
    int edgeSelection;
    int faceSelection;

    Ui::MainWindow *ui;

    reductInfos _riTaille;
    reductInfos _riAngle;
    reductInfos _riPlan;
    int _nbReduct=0;

    int _nbAreteAleatoire = 0;
    float _precision=0.f;

    QProgressDialog *_progress;

protected:
    float angleFF(MyMesh* _mesh, int faceID0, int faceID1, int vertID0, int vertID1);
    int get_edge_with_min_angle_between_neighb_face(MyMesh *_mesh, float limite);
    int min_edge_extremites_plan(MyMesh *_mesh, float limite);
    float calc_moyenne_angle_point(MyMesh *_mesh, int vertID);

    float param_plan(MyMesh *_mesh, float precision);
    float param_angle(MyMesh *_mesh, float precision);
    float param_taille(MyMesh *_mesh, float precision);
    float param_seuil(MyMesh *_mesh, int param, float precision);

    float faceArea(MyMesh* _mesh, int faceID);
    float aire_barycentrique(MyMesh* _mesh, int vertID);
    float angleEE(MyMesh* _mesh, int vertexID, int faceID);
    float H_Curv(MyMesh* _mesh, int vertID);
    float K_Curv(MyMesh* _mesh, int vertID);

private slots:

    void on_pushButton_chargement_clicked();
    void on_pushButton_edgeMoins_clicked();
    void on_pushButton_edgePlus_clicked();
    void on_pushButton_delSelEdge_clicked();

    void on_pushButton_decimate_clicked();

    void on_spinBox_precision(int value);
};

#endif // MAINWINDOW_H




