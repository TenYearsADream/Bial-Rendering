#include <QApplication>
#include <QDebug>
#include <QKeyEvent>
#include <QOpenGLFunctions>

#include "glassert.h"
#include "stlviewer.h"
#include "MarchingCubes.hpp"

STLViewer::STLViewer( QWidget *parent ) : QOpenGLWidget( parent ) {
  mesh = nullptr;
  verts = nullptr;
  norms = nullptr;
  tris = nullptr;
  setFocus( );
  setFocusPolicy( Qt::StrongFocus );
}

void STLViewer::LoadFile( QString fileName ) {
  clear( );
  resetTransform( );
  COMMENT( "Loading stl file: " << fileName.toStdString( ), 0 );
  if( fileName.endsWith( ".stl" ) || fileName.endsWith( ".stl.gz" ) ) {
    mesh = Bial::TriangleMesh::ReadSTLB( fileName.trimmed( ).toStdString( ) );
  } else {
    Bial::Image<int> img = Bial::Geometrics::Scale(Bial::File::Read<int>(fileName.toStdString()),0.25,true);

    mesh = Bial::MarchingCubes::exec( img, 50.f );

  }
  /*    mesh->Print( std::cout ); */
  size_t *vertexIndex = mesh->getVertexIndex( );
  Bial::Point3D *p = mesh->getP( );
  Bial::Normal *n = mesh->getN( );
  verts = new GLdouble[ mesh->getNverts( ) * 3 ];
  tris = new GLuint[ mesh->getNtris( ) * 3 ];
  for( size_t vtx = 0; vtx < ( mesh->getNtris( ) * 3 ); ++vtx ) {
    tris[ vtx ] = static_cast< GLuint >( vertexIndex[ vtx ] );
  }
  double xs( 0.0 ), ys( 0.0 ), zs( 0.0 );
  for( size_t pt = 0; pt < mesh->getNverts( ); ++pt ) {
    const Bial::Point3D &point = p[ pt ];
    verts[ pt * 3 ] = static_cast< GLdouble >( point.x );
    verts[ pt * 3 + 1 ] = static_cast< GLdouble >( point.y );
    verts[ pt * 3 + 2 ] = static_cast< GLdouble >( point.z );

    xs = std::max( xs, point.x );
    ys = std::max( ys, point.y );
    zs = std::max( zs, point.z );
  }
  if( n != nullptr ) {
    COMMENT( "Reading normals.", 0 );
    norms = new GLdouble[ mesh->getNtris( ) * 3 ];
    for( size_t t = 0; t < mesh->getNtris( ); ++t ) {
      const Bial::Normal &norm = n[ t ];
      norms[ t * 3 ] = static_cast< GLdouble >( norm.x );
      norms[ t * 3 + 1 ] = static_cast< GLdouble >( norm.y );
      norms[ t * 3 + 2 ] = static_cast< GLdouble >( norm.z );
      /*      std::cout << norm << std::endl; */
    }
  } else {
    norms = nullptr;
  }
  boundings[ 0 ] = xs;
  boundings[ 1 ] = ys;
  boundings[ 2 ] = zs;
}

GLfloat lightPos[] = { 0.0f, 0.0f, 500.0f, 10 };
GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat specref[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat ambientLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };

void STLViewer::initializeGL( ) {
  glEnable( GL_LIGHTING );
  glClearColor( 0, 0, 0, 1 );
  glShadeModel( GL_SMOOTH );
  glLightModelfv( GL_LIGHT_MODEL_AMBIENT, ambientLight );
  glLightfv( GL_LIGHT0, GL_DIFFUSE, ambientLight );
  glLightfv( GL_LIGHT0, GL_SPECULAR, specular );
  glLightfv( GL_LIGHT0, GL_POSITION, lightPos );
  glLightf( GL_LIGHT0, GL_SPOT_CUTOFF, 60.0f );
  glEnable( GL_LIGHT0 );
  glEnable( GL_DEPTH_TEST );
  //Enable color
  glEnable( GL_COLOR_MATERIAL );
  // Enable opacity
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  // Enable OpenGL Antialiasing functions.
  glEnable(GL_LINE_SMOOTH);
  glEnable(GL_POLYGON_SMOOTH);

  glCheckError();
}

void STLViewer::resizeGL( int w, int h ) {
  glViewport( 0, 0, w, h );
  glMatrixMode( GL_PROJECTION );
  glLoadIdentity( );
  gluPerspective( 60, ( float ) w / h, .01, 2.0 );
  glMatrixMode( GL_MODELVIEW );
  glLoadIdentity( );
  gluLookAt( 0, 0, -2, 0, 0, 0, 0, 1, 0 );
  if( verts ) {
    glVertexPointer( 3, GL_DOUBLE, 0, verts );
  }
  if( norms ) {
    glNormalPointer( GL_DOUBLE, 0, norms );
  }
  glCheckError();
}


void STLViewer::paintGL( ) {

  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  glClearColor( 0, 0, 0, 1 );

  glLoadIdentity( ); /* Set up modelview transform, first cube. */
  glPushMatrix( );
  glTranslated( 0, 0, -1.5 );
  glScaled( zoom, zoom, zoom );
  glRotatef( rotateZ, 0, 0, 1 ); /* Apply rotations. */
  glRotatef( rotateY, 0, 1, 0 );
  glRotatef( rotateX, 1, 0, 0 );

  glPushMatrix( );

  glScaled( 1.0 / boundings[ 0 ], 1.0 / boundings[ 1 ], 1.0 / boundings[ 2 ] );

  /*
   *  GLfloat red[] = { 0.2f, 0.2f, 0.2f, 1.f };
   *  glMaterialfv( GL_FRONT, GL_DIFFUSE, red );
   *
   *  GLfloat blue[] = { 0.f, 0.f, 0.8f, 1.f };
   *  glMaterialfv( GL_BACK, GL_DIFFUSE, blue );
   */

  glColor4f(1,1,1,0.5);

  glTranslated( -boundings[ 0 ] / 2.0, -boundings[ 1 ] / 2.0, -boundings[ 2 ] / 2.0 );
  glEnableClientState( GL_VERTEX_ARRAY );
  glEnable( GL_POLYGON_OFFSET_FILL );
  glPolygonOffset( 1, 1 );
  if( tris ) {
    glDrawElements( GL_TRIANGLES, mesh->getNtris( ) * 3, GL_UNSIGNED_INT, tris );
  }
  glDisable( GL_POLYGON_OFFSET_FILL );
  glDisableClientState( GL_VERTEX_ARRAY );

  glPopMatrix( );

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

  glPopMatrix( );

  glCheckError();
}

void STLViewer::clear( ) {
  if( mesh ) {
    delete mesh;
  }
  if( verts ) {
    delete[] verts;
  }
  if( norms ) {
    delete[] norms;
  }
  if( tris ) {
    delete[] tris;
  }
  mesh = nullptr;
  verts = nullptr;
  norms = nullptr;
  tris = nullptr;
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
