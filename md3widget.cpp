#include "md3widget.h"

Md3Widget::Md3Widget(QWidget *parent)
    : QOpenGLWidget(parent)
{
    setAttribute(Qt::WA_TranslucentBackground,true);
    //setAttribute(Qt::WA_OpaquePaintEvent);
    //setAttribute(Qt::WA_NoBackground, false);
    //setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint );
    /*parent->setAttribute(Qt::WA_TranslucentBackground);
    parent->setAttribute(Qt::WA_NoSystemBackground, false);

    QSurfaceFormat fmt = format();
    fmt.setAlphaBufferSize(8);
    setFormat(fmt);*/

}

Md3Widget::~Md3Widget()
{

}

void Md3Widget::initializeGL()
{
    glClearColor(0,0,0,0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
}

void Md3Widget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBegin(GL_TRIANGLES);
        glColor3f(1.0, 0.0, 0.0);
        glVertex3f(-0.5, -0.5, 0);
        glColor3f(0.0, 1.0, 0.0);
        glVertex3f( 0.5, -0.5, 0);
        glColor3f(0.0, 0.0, 1.0);
        glVertex3f( 0.0,  0.5, 0);
    glEnd();
}

void Md3Widget::resizeGL(int w, int h)
{
    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, (float)w/h, 0.01, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0,0,5,0,0,0,0,1,0);
}
