#include "stlviewer.h"

#include <QApplication>
#include <QDebug>
#include <QKeyEvent>
#include <QOpenGLFunctions>

STLViewer::STLViewer( QWidget *parent ) : QOpenGLWidget( parent ) {
  mesh = nullptr;
  verts = nullptr;
  norms = nullptr;
  tris = nullptr;
  setFocus( );
  setFocusPolicy( Qt::StrongFocus );
  QStringList args = QApplication::arguments( );
  if( args.size( ) == 2 ) {
    LoadStl( args.at( 1 ) );
  }
}

void STLViewer::LoadStl( QString stlFile ) {
  clear( );
  resetTransform( );
  COMMENT( "Loading stl file: " << stlFile.toStdString( ), 0 );
  mesh = Bial::TriangleMesh::ReadSTLB( stlFile.trimmed( ).toStdString( ) );
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
    }
  }
  else {
    norms = nullptr;
  }
  boundings[ 0 ] = xs;
  boundings[ 1 ] = ys;
  boundings[ 2 ] = zs;
}

void STLViewer::initializeGL( ) {
  glClearColor( 0, 0, 0, 1 );
  glEnable( GL_DEPTH_TEST );
  glEnable( GL_LIGHTING );
  glEnable( GL_LIGHT0 );
  glColorMaterial( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE );
  glEnable( GL_COLOR_MATERIAL );
}

void STLViewer::resizeGL( int w, int h ) {
  glViewport( 0, 0, w, h );
  glMatrixMode( GL_PROJECTION );
  glLoadIdentity( );
  gluPerspective( 60, ( float ) w / h, .01, 2.0 );
  glMatrixMode( GL_MODELVIEW );
  glLoadIdentity( );
  gluLookAt( 0, 0, -2, 0, 0, 0, 0, 1, 0 );
}


void STLViewer::paintGL( ) {

  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

  glLoadIdentity( ); /* Set up modelview transform, first cube. */
  glPushMatrix( );
  glTranslated( 0, 0, -1.5 );
  glScaled( zoom, zoom, zoom );
  glRotatef( rotateZ, 0, 0, 1 ); /* Apply rotations. */
  glRotatef( rotateY, 0, 1, 0 );
  glRotatef( rotateX, 1, 0, 0 );
  if( verts ) {
    glVertexPointer( 3, GL_DOUBLE, 0, verts );
  }
  if( norms ) {
    glNormalPointer( GL_DOUBLE, 0, norms );
  }
  glPushMatrix( );

  glScaled( 1.0 / boundings[ 0 ], 1.0 / boundings[ 1 ], 1.0 / boundings[ 2 ] );

  GLfloat red[] = { .8f, 0.f, 0.f, 1.f };
  glMaterialfv( GL_FRONT, GL_DIFFUSE, red );
  GLfloat blue[] = { 0.f, 0.f, 0.8f, 1.f };
  glMaterialfv( GL_BACK, GL_DIFFUSE, blue );

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
