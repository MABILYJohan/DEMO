/****************************************************************************
** Meta object code from reading C++ file 'myopenglwidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../visuSurface/myopenglwidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'myopenglwidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_myOpenGLWidget_t {
    QByteArrayData data[28];
    char stringdata0[340];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_myOpenGLWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_myOpenGLWidget_t qt_meta_stringdata_myOpenGLWidget = {
    {
QT_MOC_LITERAL(0, 0, 14), // "myOpenGLWidget"
QT_MOC_LITERAL(1, 15, 9), // "u_changed"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 4), // "newU"
QT_MOC_LITERAL(4, 31, 11), // "nbU_changed"
QT_MOC_LITERAL(5, 43, 6), // "newNbU"
QT_MOC_LITERAL(6, 50, 9), // "v_changed"
QT_MOC_LITERAL(7, 60, 4), // "newV"
QT_MOC_LITERAL(8, 65, 11), // "nbV_changed"
QT_MOC_LITERAL(9, 77, 6), // "newNbV"
QT_MOC_LITERAL(10, 84, 20), // "flag_pt_ctrl_changed"
QT_MOC_LITERAL(11, 105, 5), // "value"
QT_MOC_LITERAL(12, 111, 26), // "flag_triangulation_changed"
QT_MOC_LITERAL(13, 138, 21), // "flag_vertices_changed"
QT_MOC_LITERAL(14, 160, 18), // "flag_edges_changed"
QT_MOC_LITERAL(15, 179, 21), // "flag_carreaux_changed"
QT_MOC_LITERAL(16, 201, 18), // "flag_forme_changed"
QT_MOC_LITERAL(17, 220, 8), // "on_set_u"
QT_MOC_LITERAL(18, 229, 10), // "on_set_nbU"
QT_MOC_LITERAL(19, 240, 8), // "on_set_v"
QT_MOC_LITERAL(20, 249, 10), // "on_set_nbV"
QT_MOC_LITERAL(21, 260, 10), // "on_pt_ctrl"
QT_MOC_LITERAL(22, 271, 16), // "on_triangulation"
QT_MOC_LITERAL(23, 288, 11), // "on_vertices"
QT_MOC_LITERAL(24, 300, 8), // "on_edges"
QT_MOC_LITERAL(25, 309, 11), // "on_carreaux"
QT_MOC_LITERAL(26, 321, 8), // "on_forme"
QT_MOC_LITERAL(27, 330, 9) // "onTimeout"

    },
    "myOpenGLWidget\0u_changed\0\0newU\0"
    "nbU_changed\0newNbU\0v_changed\0newV\0"
    "nbV_changed\0newNbV\0flag_pt_ctrl_changed\0"
    "value\0flag_triangulation_changed\0"
    "flag_vertices_changed\0flag_edges_changed\0"
    "flag_carreaux_changed\0flag_forme_changed\0"
    "on_set_u\0on_set_nbU\0on_set_v\0on_set_nbV\0"
    "on_pt_ctrl\0on_triangulation\0on_vertices\0"
    "on_edges\0on_carreaux\0on_forme\0onTimeout"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_myOpenGLWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      21,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      10,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,  119,    2, 0x06 /* Public */,
       4,    1,  122,    2, 0x06 /* Public */,
       6,    1,  125,    2, 0x06 /* Public */,
       8,    1,  128,    2, 0x06 /* Public */,
      10,    1,  131,    2, 0x06 /* Public */,
      12,    1,  134,    2, 0x06 /* Public */,
      13,    1,  137,    2, 0x06 /* Public */,
      14,    1,  140,    2, 0x06 /* Public */,
      15,    1,  143,    2, 0x06 /* Public */,
      16,    1,  146,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      17,    1,  149,    2, 0x0a /* Public */,
      18,    1,  152,    2, 0x0a /* Public */,
      19,    1,  155,    2, 0x0a /* Public */,
      20,    1,  158,    2, 0x0a /* Public */,
      21,    1,  161,    2, 0x0a /* Public */,
      22,    1,  164,    2, 0x0a /* Public */,
      23,    1,  167,    2, 0x0a /* Public */,
      24,    1,  170,    2, 0x0a /* Public */,
      25,    1,  173,    2, 0x0a /* Public */,
      26,    1,  176,    2, 0x0a /* Public */,
      27,    0,  179,    2, 0x09 /* Protected */,

 // signals: parameters
    QMetaType::Void, QMetaType::Double,    3,
    QMetaType::Void, QMetaType::Int,    5,
    QMetaType::Void, QMetaType::Double,    7,
    QMetaType::Void, QMetaType::Int,    9,
    QMetaType::Void, QMetaType::Bool,   11,
    QMetaType::Void, QMetaType::Bool,   11,
    QMetaType::Void, QMetaType::Bool,   11,
    QMetaType::Void, QMetaType::Bool,   11,
    QMetaType::Void, QMetaType::Bool,   11,
    QMetaType::Void, QMetaType::Bool,   11,

 // slots: parameters
    QMetaType::Void, QMetaType::Double,    3,
    QMetaType::Void, QMetaType::Int,    5,
    QMetaType::Void, QMetaType::Double,    7,
    QMetaType::Void, QMetaType::Int,    9,
    QMetaType::Void, QMetaType::Bool,   11,
    QMetaType::Void, QMetaType::Bool,   11,
    QMetaType::Void, QMetaType::Bool,   11,
    QMetaType::Void, QMetaType::Bool,   11,
    QMetaType::Void, QMetaType::Bool,   11,
    QMetaType::Void, QMetaType::Bool,   11,
    QMetaType::Void,

       0        // eod
};

void myOpenGLWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        myOpenGLWidget *_t = static_cast<myOpenGLWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->u_changed((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 1: _t->nbU_changed((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->v_changed((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 3: _t->nbV_changed((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->flag_pt_ctrl_changed((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 5: _t->flag_triangulation_changed((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 6: _t->flag_vertices_changed((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 7: _t->flag_edges_changed((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 8: _t->flag_carreaux_changed((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 9: _t->flag_forme_changed((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 10: _t->on_set_u((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 11: _t->on_set_nbU((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 12: _t->on_set_v((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 13: _t->on_set_nbV((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 14: _t->on_pt_ctrl((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 15: _t->on_triangulation((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 16: _t->on_vertices((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 17: _t->on_edges((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 18: _t->on_carreaux((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 19: _t->on_forme((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 20: _t->onTimeout(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (myOpenGLWidget::*_t)(double );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&myOpenGLWidget::u_changed)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (myOpenGLWidget::*_t)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&myOpenGLWidget::nbU_changed)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (myOpenGLWidget::*_t)(double );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&myOpenGLWidget::v_changed)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (myOpenGLWidget::*_t)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&myOpenGLWidget::nbV_changed)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (myOpenGLWidget::*_t)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&myOpenGLWidget::flag_pt_ctrl_changed)) {
                *result = 4;
                return;
            }
        }
        {
            typedef void (myOpenGLWidget::*_t)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&myOpenGLWidget::flag_triangulation_changed)) {
                *result = 5;
                return;
            }
        }
        {
            typedef void (myOpenGLWidget::*_t)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&myOpenGLWidget::flag_vertices_changed)) {
                *result = 6;
                return;
            }
        }
        {
            typedef void (myOpenGLWidget::*_t)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&myOpenGLWidget::flag_edges_changed)) {
                *result = 7;
                return;
            }
        }
        {
            typedef void (myOpenGLWidget::*_t)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&myOpenGLWidget::flag_carreaux_changed)) {
                *result = 8;
                return;
            }
        }
        {
            typedef void (myOpenGLWidget::*_t)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&myOpenGLWidget::flag_forme_changed)) {
                *result = 9;
                return;
            }
        }
    }
}

const QMetaObject myOpenGLWidget::staticMetaObject = {
    { &QOpenGLWidget::staticMetaObject, qt_meta_stringdata_myOpenGLWidget.data,
      qt_meta_data_myOpenGLWidget,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *myOpenGLWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *myOpenGLWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_myOpenGLWidget.stringdata0))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "QOpenGLFunctions"))
        return static_cast< QOpenGLFunctions*>(this);
    return QOpenGLWidget::qt_metacast(_clname);
}

int myOpenGLWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QOpenGLWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 21)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 21;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 21)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 21;
    }
    return _id;
}

// SIGNAL 0
void myOpenGLWidget::u_changed(double _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void myOpenGLWidget::nbU_changed(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void myOpenGLWidget::v_changed(double _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void myOpenGLWidget::nbV_changed(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void myOpenGLWidget::flag_pt_ctrl_changed(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void myOpenGLWidget::flag_triangulation_changed(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void myOpenGLWidget::flag_vertices_changed(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void myOpenGLWidget::flag_edges_changed(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void myOpenGLWidget::flag_carreaux_changed(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void myOpenGLWidget::flag_forme_changed(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
