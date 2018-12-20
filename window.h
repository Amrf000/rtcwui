#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGLWidget>
#include <QGraphicsItem>
#define MIN(a,b)  ((a < b)? a : b)
#define PI            3.14159265358979323846

class CEasyGLWidget : public QGLWidget
{
    Q_OBJECT

public:

    CEasyGLWidget(QWidget *parent = 0);
    ~CEasyGLWidget();

     void initializeGL();
     void resizeGL(int w, int h);
     void paintGL();

     void DrawArc(GLdouble cx, GLdouble cy, GLdouble r, GLdouble start_angle, GLdouble arc_angle, int num_segments);
     void DrawLine(QPointF cStartPoint, QPointF cDestPoint);
private:

    GLuint             m_pOGLObject;

};


class CEasyGraphicsItem : public QGraphicsItem
{
    //Q_OBJECT

public:
    CEasyGraphicsItem(QWidget *parent = 0);
    ~CEasyGraphicsItem();

    virtual QRectF boundingRect() const;
    virtual void paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0 );

private:
    CEasyGLWidget *m_OGLItem;
    QPixmap m_Pixmap;
    QImage m_Image;

};

class Window : public QWidget
{
  Q_OBJECT

public:
  Window(QWidget *parent = 0);
  ~Window();

private:
  QGraphicsScene  *m_pMainGraphicsScene;
  QGraphicsView   *m_pMainGraphicsView;
  QGLWidget       *m_pMainGLWidget;

  CEasyGraphicsItem *m_pItem;

  CEasyGLWidget *m_OGLItem;
};

#endif
