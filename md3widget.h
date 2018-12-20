#ifndef MD3WIDGET_H
#define MD3WIDGET_H
#include <QWidget>
#include <QOpenGLWidget>
#include <gl/GLU.h>
#include <gl/GL.h>

class Md3Widget: public QOpenGLWidget
{
public:
    Md3Widget(QWidget *parent = 0);
    ~Md3Widget();

protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
};

#endif // MD3WIDGET_H
