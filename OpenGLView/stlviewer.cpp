#include "stlviewer.h"

#include <QApplication>
#include <QOpenGLFunctions>

STLViewer::STLViewer( QWidget *parent ) : QOpenGLWidget( parent ) {
  QStringList args = QApplication::arguments();
  if( args.size() == 2 ) {
    QString fname = args.at(1);
    COMMENT("Loading stl file: " << fname.toStdString(), 0);
    mesh = Bial::TriangleMesh::ReadSTLB(fname.trimmed().toStdString());
    mesh->Print(std::cout);
  }
}

void STLViewer::initializeGL( ) {
  glClearColor( 0, 0, 0, 1 );
  glEnable( GL_DEPTH_TEST );
  glEnable( GL_LIGHT0 );
  glEnable( GL_LIGHTING );
  glColorMaterial( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE );
  glEnable( GL_COLOR_MATERIAL );
}

void STLViewer::resizeGL( int w, int h ) {
  glViewport( 0, 0, w, h );
  glMatrixMode( GL_PROJECTION );
  glLoadIdentity( );
  gluPerspective( 45, ( float ) w / h, 0.01, 100.0 );
  glMatrixMode( GL_MODELVIEW );
  glLoadIdentity( );
  gluLookAt( 0, 0, 5, 0, 0, 0, 0, 1, 0 );
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
}

void STLViewer::paintGL( ) {

  glBegin( GL_TRIANGLES );
  glColor3f( 1.0, 0.0, 0.0 );

  glEnd( );
}
