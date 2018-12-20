#include <QtWidgets>

#include "window.h"
#include "glwidget.h"
#include "myglwidget.h"
CEasyGLWidget::CEasyGLWidget(QWidget *parent)
: QGLWidget(QGLFormat(QGL::SampleBuffers/*|QGL::AlphaChannel|QGL::HasOverlay|QGL::Rgba*/),parent)

{
    m_pOGLObject = 0;
}

CEasyGLWidget::~CEasyGLWidget()
{

}


/*! \brief Draws a single circular or elliptical arc.
 *
 *\param  cx 			Upper Left corner of the in x coordinates
 *\param  cy 			Upper Left corner of the in y coordinates
 *\param  r				Radius of the arc.
 *\param  start_angle 	Start position in angles (radian)
 *\param  arc_angle 	Angle of the Arc in radian
 *\param  num_segments  Point count, which represent the arc
 */
void CEasyGLWidget::DrawArc(GLdouble cx, GLdouble cy, GLdouble r, GLdouble start_angle, GLdouble arc_angle, int num_segments)
{
    //Theta is calculated from the arc angle, the -1 bit comes from the fact that the arc is open
    GLdouble theta = arc_angle / GLdouble(num_segments -1);
    GLdouble tangetial_factor = tanf(theta);

    GLdouble radial_factor = cosf(theta);

    //start at the start angle
    GLdouble  x = r * cosf(start_angle);
    GLdouble  y =  r * sinf(start_angle);

    glBegin(GL_LINE_STRIP);

    for(int i = 0; i < num_segments; i++)
    {
        glVertex2f(x + cx + r, y + cy +r); //, 0.5);

        GLdouble tx = -y;
        GLdouble ty = x;

        x += tx * tangetial_factor;
        y += ty * tangetial_factor;

        x *= radial_factor;
        y *= radial_factor;
    }

    glEnd();
}


/*! \brief Draws a single line.
 *
 * \param[in] cStartPoint Start Point of the line to be drawn.
 * \param[in] cDestPoint  Destination Point of the line to be drawn.
 */
void CEasyGLWidget::DrawLine(QPointF cStartPoint, QPointF cDestPoint)
{
    glBegin(GL_LINE_STRIP);

    glVertex2f(cStartPoint.rx(), cStartPoint.ry());
    glVertex2f(cDestPoint.rx(), cDestPoint.ry());

    glEnd();
}

void CEasyGLWidget::initializeGL()
{
    qglClearColor( QColor(90,125,164,0)); //255 set background color

    qglColor(QColor(255,0,0,255));

    //create a display list containing information about the object we want to display
    m_pOGLObject = glGenLists(1);

    glNewList(m_pOGLObject, GL_COMPILE);
    DrawArc(0.0, 0.0, 30.0, 0.0, 2*PI, 60);

    DrawLine(QPointF(0.0,0.0), QPointF(200.0,200.0));
    glEndList();

    //activate alpha values for transparent colors of the bitmaps
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER,0.1);

    //set up the rendering process to use a particular shading model and rendering flags:
    glShadeModel(GL_FLAT);
}

void CEasyGLWidget::resizeGL(int iWidth, int iHeight)
{
     // Set size of the view port (display size)
     glViewport(0,0,(int)MIN(iWidth,iHeight), (int)MIN(iWidth,iHeight)); // always use width=height

     glMatrixMode(GL_PROJECTION); //select protection matrix
     glLoadIdentity();  //reset matrix position to start

     // Open gl always draws on the same matrix (width x height = fix defined hud size)
     // the size is defined by the Ortho Matrix.
     // This matrix is scaled to the destination size.
     glOrtho(0, 400,400, 0, -1,1);

     glMatrixMode(GL_MODELVIEW);  //select model view (result) matrix
}

void CEasyGLWidget::paintGL()
{

     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear background using background color
     glLoadIdentity();              //reset matrix position to start
     glTranslatef(0.0, 0.0, 0.0) ; // move object to dest position
     glCallList(m_pOGLObject);     // call the display list containing the rendering commands for the object

     renderText(20,20,"test");

}


/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////// ITEM

CEasyGraphicsItem::CEasyGraphicsItem(QWidget *parent)
{
    m_OGLItem = new CEasyGLWidget();
    m_OGLItem->resize(200,200);
    m_Pixmap = m_OGLItem->renderPixmap(600,600);
}
CEasyGraphicsItem::~CEasyGraphicsItem()
{

}

void CEasyGraphicsItem::paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
     QRectF rect(0.0,0.0,200.0,200.0);

     //Works fine !!
     //painter->drawArc(rect,0,16*360);

     // Open gl element is not displayed!!!
    //m_pPixmap = QPixmap::grabWidget(m_OGLItem,0,0,600,600);
    m_Pixmap = m_OGLItem->renderPixmap(600,600);
    painter->drawPixmap(0,0,600,600,m_Pixmap);
    //m_Image = m_OGLItem->grabFrameBuffer();
    //painter->drawImage(rect, m_Image);
}

QRectF CEasyGraphicsItem::boundingRect() const
{
    return QRectF(0,0,600,600); //m_CurrWindowWidth,m_CurrWindowHeight);
}

Window::Window(QWidget *parent)
    : QWidget(parent)
{
     m_pMainGraphicsScene = new QGraphicsScene();
     m_pMainGraphicsScene->setSceneRect(QRectF(0,0,630,630)); // Dummy window size
     m_pMainGraphicsView = new QGraphicsView(m_pMainGraphicsScene);

     m_pMainGLWidget = new QGLWidget();
     //m_pMainGraphicsView->setViewport(m_pMainGLWidget);

     //setCentralWidget(m_pMainGraphicsView);

     QLayout * layout = new QVBoxLayout();
     layout->addWidget(m_pMainGraphicsView);
     setLayout(layout);
     show();


     // Add open gl item  to view (not part of the scene!!) works fine!
     /*m_OGLItem = new CEasyGLWidget(m_pMainGraphicsView);
     m_OGLItem->resize(400,400);
     m_pMainGraphicsScene->addWidget(m_OGLItem);
     */

     // add graphics item which contain a open gl element
     m_pItem = new CEasyGraphicsItem();
     m_pMainGraphicsScene->addItem(m_pItem);


}

Window::~Window()
{

}
