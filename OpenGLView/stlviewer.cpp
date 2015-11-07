#include <QApplication>
#include <QDebug>
#include <QKeyEvent>
#include <QOpenGLFunctions>
#include <QTime>

#include "MarchingCubes.hpp"
#include "glassert.h"
#include "stlviewer.h"


STLViewer::STLViewer( QWidget *parent ) : QOpenGLWidget( parent ) {

  setFocus( );
  setFocusPolicy( Qt::StrongFocus );
  model = nullptr;
}

void STLViewer::LoadFile( QString stlFile ) {
  clear( );
  resetTransform( );
/*  model = new StlModel( stlFile ); */
  if( fileName.endsWith( ".stl" ) || fileName.endsWith( ".stl.gz" ) ) {
    model = StlModel::loadStl( fileName );
  }
  fileName = stlFile;
}

GLfloat ambientLight[] = { 0.5f, 0.5f, 0.5f, 1.0f };

void STLViewer::initializeGL( ) {
  glEnable( GL_LIGHTING );
  glClearColor( 0, 0, 0, 1 );
  glShadeModel( GL_SMOOTH );
  glLightModelfv( GL_LIGHT_MODEL_AMBIENT, ambientLight );
  light1.enable( );
  glEnable( GL_DEPTH_TEST );
  /* Enable color */
  glEnable( GL_COLOR_MATERIAL );
  /* Enable opacity */
  glEnable( GL_BLEND );
  glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
  /* Enable OpenGL Antialiasing functions. */
  glEnable( GL_LINE_SMOOTH );
/*  glEnable( GL_POLYGON_SMOOTH ); */

  /* Enable specular reflection according to viewer. */
  glLightModeli( GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE );
  glLightModeli( GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE );

  glCheckError( );
}

void STLViewer::resizeGL( int w, int h ) {
  glViewport( 0, 0, w, h );
  glMatrixMode( GL_PROJECTION );
  glLoadIdentity( );
  gluPerspective( 60, ( float ) w / h, .01, 2.0 );
  glMatrixMode( GL_MODELVIEW );
  glLoadIdentity( );
/*  gluLookAt( 0, 0, -2, 0, 0, 0, 0, 1, 0 ); */
  if( model ) {
    model->reload( );
  }
  glCheckError( );
}


void STLViewer::drawLines( ) {
  glBegin( GL_LINES );
  glColor3f( 1, 0, 0 );
  glVertex3d( 0, 0, 0 );
  glVertex3d( 1, 0, 0 );
  glColor3f( 0, 1, 0 );
  glVertex3d( 0, 0, 0 );
  glVertex3d( 0, 1, 0 );
  glColor3f( 0, 0, 1 );
  glVertex3d( 0, 0, 0 );
  glVertex3d( 0, 0, 1 );
  glEnd( );
}

void STLViewer::clear( ) {
  if( model ) {
    delete model;
  }
}


void STLViewer::keyPressEvent( QKeyEvent *evt ) {
  switch( evt->key( ) ) {
      case Qt::Key_Left:
      rotateY -= 15;
      break;
      case Qt::Key_Right:
      rotateY += 15;
      break;
      case Qt::Key_Down:
      rotateX -= 15;
      break;
      case Qt::Key_Up:
      rotateX += 15;
      break;
      case Qt::Key_PageUp:
      rotateZ -= 15;
      break;
      case Qt::Key_PageDown:
      rotateZ += 15;
      break;
      case Qt::Key_Home:
      resetTransform( );
      break;
  }
  update( );
}


void STLViewer::mousePressEvent( QMouseEvent *evt ) {
  if( evt->button( ) == Qt::LeftButton ) {
    dragging = true;
    lastPoint = evt->pos( );
  }
  evt->accept( );
}

void STLViewer::mouseReleaseEvent( QMouseEvent *evt ) {
  if( evt->button( ) == Qt::LeftButton ) {
    dragging = false;
    QPoint diff = evt->pos( ) - lastPoint;
    rotateX += diff.y( );
    rotateY += diff.x( );
    lastPoint = evt->pos( );
  }
  evt->accept( );
}

void STLViewer::mouseMoveEvent( QMouseEvent *evt ) {
  if( dragging ) {
    QPoint diff = evt->pos( ) - lastPoint;
    rotateX += diff.y( );
    rotateY += diff.x( );
    lastPoint = evt->pos( );
  }
  evt->accept( );
  update( );
}

void STLViewer::wheelEvent( QWheelEvent *evt ) {
  QPoint numDegrees = evt->angleDelta( ) / 8;
  if( !numDegrees.isNull( ) ) {
    QPoint numSteps = numDegrees / 15;
    zoom += 0.1 * numSteps.ry( );
  }
  zoom = std::max( 1.0, zoom );
  evt->accept( );
  update( );
}

void STLViewer::resetTransform( ) {
  rotateX = rotateY = rotateZ = 0;
  zoom = 1.0;
}

void STLViewer::mouseDoubleClickEvent( QMouseEvent *evt ) {
  resetTransform( );
  evt->accept( );
  update( );
}

void STLViewer::runMarchingCubes( float isolevel, float scale ) {
  if( model ) {
    delete model;
    model = nullptr;
  }
  model = StlModel::marchingCubes( fileName, isolevel, scale );
  update();
  emit finishedMCubes();
}

void STLViewer::paintGL( ) {
  /* Cleaning screen */
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  glClearColor( 0, 0, 0, 1 );

  /* Loading modelView matrix */
  glMatrixMode( GL_MODELVIEW );
  glLoadIdentity( );

  Bial::Transform3D transf;
  transf.Translate( 0, 0, -1.5 ).Scale( zoom, zoom, zoom );
  transf.Rotate( rotateX, 0 ).Rotate( rotateY, 1 ).Rotate( rotateZ, 2 );
  glLoadMatrixd( &transf.getAffineMatrix( ).Transposed( )[ 0 ] );
  if( model ) {
    model->draw( );
  }
/*  light1.draw( ); */

/*  drawLines( ); */

  glCheckError( );
}
