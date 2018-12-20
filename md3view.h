#ifndef MD3VIEW_H
#define MD3VIEW_H
#include <QGraphicsView>
#include <QOpenGLFunctions>
#include <QWidget>
#include <QGraphicsScene>
#include <QGLWidget>
#include <QGraphicsItem>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#define MIN(a,b)  ((a < b)? a : b)
#define PI            3.14159265358979323846

class CEasyGLWidget : public QGLWidget
{
    Q_OBJECT
public:
    CEasyGLWidget( const QGLFormat& format, QWidget* parent = 0  );

protected:
    virtual void initializeGL();
    virtual void resizeGL( int w, int h );
    virtual void paintGL();

    virtual void keyPressEvent( QKeyEvent* e );

private:
    bool prepareShaderProgram( const QString& vertexShaderPath,
                               const QString& geometryShaderPath,
                               const QString& fragmentShaderPath );

    QOpenGLShaderProgram m_shader;
    QOpenGLBuffer m_vertexBuffer;
    bool inited;

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

class Md3View:public QGraphicsView,
        protected QOpenGLFunctions
{
public:
    Md3View( QWidget* pParent = 0 );
    virtual ~Md3View( void );
private:
    QGraphicsScene  *m_pMainGraphicsScene;
    //QGraphicsView   *m_pMainGraphicsView;
    QGLWidget       *m_pMainGLWidget;

    CEasyGraphicsItem *m_pItem;

    CEasyGLWidget *m_OGLItem;
};

#endif // MD3VIEW_H
