#include "md3view.h"
#include <QKeyEvent>

typedef void (*PglGenVertexArrays) (GLsizei n,  GLuint *arrays);
typedef void (*PglBindVertexArray) (GLuint array);

CEasyGLWidget::CEasyGLWidget(const QGLFormat& format, QWidget* parent)
: QGLWidget(QGLFormat(QGL::SampleBuffers/*|QGL::AlphaChannel|QGL::HasOverlay|QGL::Rgba*/),parent),//QGLWidget( format, parent ),
  m_vertexBuffer(QOpenGLBuffer::VertexBuffer),//
  inited(false)
{
}
void CEasyGLWidget::initializeGL()
{
    if(inited==false){
        inited= true;
        // get context opengl-version
           qDebug() << "Widget OpenGl: " << format().majorVersion() << "." << format().minorVersion();
           qDebug() << "Context valid: " << context()->isValid();
           qDebug() << "Really used OpenGl: " << context()->format().majorVersion() << "." << context()->format().minorVersion();
           qDebug() << "OpenGl information: VENDOR:       " << (const char*)glGetString(GL_VENDOR);
           qDebug() << "                    RENDERDER:    " << (const char*)glGetString(GL_RENDERER);
           qDebug() << "                    VERSION:      " << (const char*)glGetString(GL_VERSION);
           qDebug() << "                    GLSL VERSION: " << (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION);

           QGLFormat glFormat = QGLWidget::format();
           if ( !glFormat.sampleBuffers() )
               qWarning() << "Could not enable sample buffers";

           // Set the clear color to black
           glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );

           // Prepare a complete shader program...
           if ( !prepareShaderProgram( ":/std.vert", ":/std.geom",":/std.frag" ) )
               return;

           // we need a VAO in core!
           GLuint VAO;
           PglGenVertexArrays glGenVertexArrays = (PglGenVertexArrays) context()->getProcAddress("glGenVertexArrays");
           PglBindVertexArray glBindVertexArray = (PglBindVertexArray) context()->getProcAddress("glBindVertexArray");
           glGenVertexArrays(1, &VAO);
           glBindVertexArray(VAO);

           // We need us some vertex data. Start simple with a triangle ;-)
           float points[] = { -0.5f, -0.5f, 0.0f, 1.0f,
                               0.5f, -0.5f, 0.0f, 1.0f,
                               0.0f,  0.5f, 0.0f, 1.0f };
           m_vertexBuffer.create();
           m_vertexBuffer.setUsagePattern( QOpenGLBuffer::StaticDraw );
           if ( !m_vertexBuffer.bind() )
           {
               qWarning() << "Could not bind vertex buffer to the context";
               return;
           }
           m_vertexBuffer.allocate( points, 3 * 4 * sizeof( float ) );

           // Bind the shader program so that we can associate variables from
           // our application to the shaders
           if ( !m_shader.bind() )
           {
               qWarning() << "Could not bind shader program to context";
               return;
           }

           // Enable the "vertex" attribute to bind it to our currently bound
           // vertex buffer.
           m_shader.setAttributeBuffer( "vertex", GL_FLOAT, 0, 4 );
           m_shader.enableAttributeArray( "vertex" );
    }
}

void CEasyGLWidget::resizeGL( int w, int h )
{
    // Set the viewport to window dimensions
    glViewport( 0, 0, w, qMax( h, 1 ) );
}

void CEasyGLWidget::paintGL()
{
    // Clear the buffer with the current clearing color
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    // Draw stuff
    glDrawArrays( GL_TRIANGLES, 0, 3 );

}

void CEasyGLWidget::keyPressEvent( QKeyEvent* e )
{
    switch ( e->key() )
    {
        case Qt::Key_Escape:
            break;

        default:
            QGLWidget::keyPressEvent( e );
    }
}

bool CEasyGLWidget::prepareShaderProgram( const QString& vertexShaderPath,
                                     const QString& geometryShaderPath,
                                     const QString& fragmentShaderPath
                                     )
{
    // First we load and compile the vertex shader...
    bool result = m_shader.addShaderFromSourceFile( QOpenGLShader::Vertex, vertexShaderPath );
    if ( !result )
        qWarning() << m_shader.log();


    result = m_shader.addShaderFromSourceFile( QOpenGLShader::Geometry, geometryShaderPath );
    if ( !result )
        qWarning() << m_shader.log();



    // ...now the fragment shader...
    result = m_shader.addShaderFromSourceFile( QOpenGLShader::Fragment, fragmentShaderPath );
    if ( !result )
        qWarning() << m_shader.log();

    // ...and finally we link them to resolve any references.
    result = m_shader.link();
    if ( !result )
        qWarning() << "Could not link shader program:" << m_shader.log();

    return result;
}

/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////// ITEM

CEasyGraphicsItem::CEasyGraphicsItem(QWidget *parent)
{
    QGLFormat glFormat;
    glFormat.setVersion( 3, 2 );
    glFormat.setProfile( QGLFormat::CoreProfile ); // Requires >=Qt-4.8.0
    glFormat.setSampleBuffers( true );

    m_OGLItem = new CEasyGLWidget(glFormat);
    m_OGLItem->resize(200,200);
    m_Pixmap = m_OGLItem->renderPixmap(300,300);
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
    m_Pixmap = m_OGLItem->renderPixmap(300,300);
    painter->drawPixmap(0,0,300,300,m_Pixmap);
    //m_Image = m_OGLItem->grabFrameBuffer();
    //painter->drawImage(rect, m_Image);
}

QRectF CEasyGraphicsItem::boundingRect() const
{
    return QRectF(0,0,300,300); //m_CurrWindowWidth,m_CurrWindowHeight);
}

Md3View::Md3View(QWidget *parent)
    : QGraphicsView(parent)
{
    m_pMainGraphicsScene = new QGraphicsScene();
    m_pMainGraphicsScene->setSceneRect(QRectF(0,0,300,300)); // Dummy window size
    //m_pMainGraphicsView = new QGraphicsView(m_pMainGraphicsScene);
    setScene(m_pMainGraphicsScene);

    m_pMainGLWidget = new QGLWidget();
    //m_pMainGraphicsView->setViewport(m_pMainGLWidget);

    //setCentralWidget(m_pMainGraphicsView);

    //QLayout * layout = new QVBoxLayout();
    //layout->addWidget(m_pMainGraphicsView);
    //setLayout(layout);
    //show();


    // Add open gl item  to view (not part of the scene!!) works fine!
    /*m_OGLItem = new CEasyGLWidget(m_pMainGraphicsView);
    m_OGLItem->resize(400,400);
    m_pMainGraphicsScene->addWidget(m_OGLItem);
    */

    // add graphics item which contain a open gl element
    m_pItem = new CEasyGraphicsItem();
    m_pMainGraphicsScene->addItem(m_pItem);
}

Md3View::~Md3View()
{

}


