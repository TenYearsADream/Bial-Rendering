#include <QApplication>
#include <QDebug>
#include <QKeyEvent>
#include <QOpenGLFunctions>
#include <QTime>

#include "MarchingCubes.hpp"
#include "glassert.h"
#include "stlviewer.h"


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
  }
  else {
    QTime t;
    t.start( );
    qDebug( ) << "Running marching cubes algorithm.";
    Bial::Image< int > img = Bial::Geometrics::Scale( Bial::File::Read< int >( fileName.toStdString( ) ), 0.3, true );

    mesh = Bial::MarchingCubes::exec( img, 50.f );
    qDebug( ) << "Elapsed: " << t.elapsed( ) << " ms";
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
    norms = new GLdouble[ mesh->getNverts( ) * 3 ];
    for( size_t t = 0; t < mesh->getNverts( ); ++t ) {
      const Bial::Normal &norm = n[ t ];
      norms[ t * 3 ] = 1.0 - static_cast< GLdouble >( norm.x );
      norms[ t * 3 + 1 ] = 1.0 - static_cast< GLdouble >( norm.y );
      norms[ t * 3 + 2 ] = 1.0 - static_cast< GLdouble >( norm.z );
      /*      std::cout << norm << std::endl; */
    }
  }
  else {
    norms = nullptr;
  }
  boundings[ 0 ] = xs;
  boundings[ 1 ] = ys;
  boundings[ 2 ] = zs;
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
  glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
  glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

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
  if( verts ) {
    qDebug( ) << "Verts found";
    glEnableClientState( GL_VERTEX_ARRAY );
    glVertexPointer( 3, GL_DOUBLE, 0, verts );
    glDisableClientState( GL_VERTEX_ARRAY );
  }
  if( norms ) {
    qDebug( ) << "Norms found";
    glEnableClientState( GL_NORMAL_ARRAY );
    glNormalPointer( GL_DOUBLE, 0, norms );
    glDisableClientState( GL_NORMAL_ARRAY );
  }
  glCheckError( );
}


void STLViewer::drawTriangleMesh( ) {
  glPushMatrix( );
  glScaled( 1.0 / boundings[ 0 ], 1.0 / boundings[ 1 ], 1.0 / boundings[ 2 ] );
  GLfloat diffuseCoeff[] = { 0.2f, 0.4f, 0.9f, 1.0f };
  GLfloat specularCoeff[] = { 0.2f, 0.4f, 0.9f, 1.0f };
  glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, diffuseCoeff );
  glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, specularCoeff );
  glMaterialf( GL_FRONT_AND_BACK, GL_SHININESS, 25.0 );
  glColor4f( 1, 1, 1, 1 );
  glTranslated( -boundings[ 0 ] / 2.0, -boundings[ 1 ] / 2.0, -boundings[ 2 ] / 2.0 );
  glEnable( GL_NORMALIZE );
  glEnableClientState( GL_NORMAL_ARRAY );
  glEnableClientState( GL_VERTEX_ARRAY );
  glEnable( GL_POLYGON_OFFSET_FILL );
  glPolygonOffset( 1, 1 );
  if( tris ) {
    glAssert( glDrawElements( GL_TRIANGLES, mesh->getNtris( ) * 3, GL_UNSIGNED_INT, tris ) );
  }
  glDisable( GL_POLYGON_OFFSET_FILL );
  glDisableClientState( GL_VERTEX_ARRAY );
  glDisableClientState( GL_NORMAL_ARRAY );
  glPopMatrix( );
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


  drawTriangleMesh( );

//  glBegin( GL_QUADS );
//  glVertex3f( -1, -1, 0 );
//  glVertex3f( -1, 1, 0 );
//  glVertex3f( 1, 1, 0 );
//  glVertex3f( 1, -1, 0 );
//  glEnd( );

  light1.draw( );

  drawLines( );

  glCheckError( );
}
