#include "stlviewer.h"

#include <QApplication>
#include <QDebug>
#include <QKeyEvent>
#include <QOpenGLFunctions>


int rotateX = 15, rotateY = -15, rotateZ = 0; /* rotation amounts about axes, controlled by keyboard */


/* Arrays for use with glDrawElements.  This is the data for a cube with 6 different
 * colors at the six vertices.  (Looks kind of strange without lighting.)
 */

float vertexCoords[ 24 ] = { /* Coordinates for the vertices of a cube. */
  1, 1, 1, 1, 1, -1, 1, -1, -1, 1, -1, 1,
  -1, 1, 1, -1, 1, -1, -1, -1, -1, -1, -1, 1
};

float vertexColors[ 24 ] = { /* An RGB color value for each vertex */
  1, 1, 1, 1, 0, 0, 1, 1, 0, 0, 1, 0,
  0, 0, 1, 1, 0, 1, 0, 0, 0, 0, 1, 1
};

int elementArray[ 24 ] = { /* Vertex number for the six faces. */
  0, 1, 2, 3, 0, 3, 7, 4, 0, 4, 5, 1,
  6, 2, 1, 5, 6, 5, 4, 7, 6, 7, 3, 2
};


/* We will draw edges for the first cube using this array with glDrawElements.
 * (It looks pretty bad without lighting if edges aren't drawn.
 */

int edgeElementArray[ 24 ] = {
  0, 1, 1, 5, 5, 4, 4, 0,         /* edges of the top face */
  7, 3, 3, 2, 2, 6, 6, 7,         /* edges of the bottom face */
  1, 2, 0, 3, 4, 7, 5, 6
};                                /* edges connecting top face to bottom face */

/* Arrays for use with glDrawArrays.  The coordinate array contains four sets of vertex
 * coordinates for each face.  The color array must have a color for each vertex.  Since
 * the color of each face is solid, there is a lot of redundancy in the color array.
 * There is also redundancy in the coordinate array, compared to using glDrawElements.
 * But note that it is impossible to use a single call to glDrawElements to draw a cube
 * with six faces where each face has a different solid color, since with glDrawElements,
 * the colors are associated with the vertices, not the faces.
 */

float cubeCoords[ 72 ] = {
  1, 1, 1, -1, 1, 1, -1, -1, 1, 1, -1, 1,          /* face #1 */
  1, 1, 1, 1, -1, 1, 1, -1, -1, 1, 1, -1,          /* face #2 */
  1, 1, 1, 1, 1, -1, -1, 1, -1, -1, 1, 1,          /* face #3 */
  -1, -1, -1, -1, 1, -1, 1, 1, -1, 1, -1, -1,      /* face #4 */
  -1, -1, -1, -1, -1, 1, -1, 1, 1, -1, 1, -1,      /* face #5 */
  -1, -1, -1, 1, -1, -1, 1, -1, 1, -1, -1, 1
};                                                 /* face #6 */

float cubeFaceColors[ 72 ] = {
  1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0,       /* face #1 is red */
  0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0,       /* face #2 is green */
  0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1,       /* face #3 is blue */
  1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0,       /* face #4 is yellow */
  0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1,       /* face #5 is cyan */
  1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1,
};                                          /* face #6 is red */


STLViewer::STLViewer( QWidget *parent ) : QOpenGLWidget( parent ) {
  QStringList args = QApplication::arguments( );
  if( args.size( ) == 2 ) {
    QString fname = args.at( 1 );
    COMMENT( "Loading stl file: " << fname.toStdString( ), 0 );
    mesh = Bial::TriangleMesh::ReadSTLB( fname.trimmed( ).toStdString( ) );
/*    mesh->Print( std::cout ); */
    size_t *vertexIndex = mesh->getVertexIndex( );
    Bial::Point3D *p = mesh->getP( );

    verts = new GLdouble[ mesh->getNverts( ) * 3 ];
    tris = new GLuint[ mesh->getNtris( ) * 3 ];
    for( size_t vtx = 0; vtx < ( mesh->getNtris( ) * 3 ); ++vtx ) {
      tris[ vtx ] = static_cast< GLuint >( vertexIndex[ vtx ] );
    }
    for( size_t pt = 0; pt < mesh->getNverts( ); ++pt ) {
      const Bial::Point3D &point = p[ pt ];
      verts[ pt * 3 ] = static_cast< GLdouble >( point.x / 100.0 );
      verts[ pt * 3 + 1 ] = static_cast< GLdouble >( point.y / 100.0 );
      verts[ pt * 3 + 2 ] = static_cast< GLdouble >( point.z / 100.0 );
    }
  }
  setFocusPolicy(Qt::ClickFocus);
}

void STLViewer::initializeGL( ) {
//  glClearColor( 0, 0, 0, 1 );
//  glEnable( GL_DEPTH_TEST );
//  glEnable( GL_LIGHT0 );
//  glEnable( GL_LIGHTING );
//  glColorMaterial( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE );
//  glEnable( GL_COLOR_MATERIAL );

  glMatrixMode( GL_PROJECTION );
  glOrtho( -4, 4, -2, 2, -2, 2 );    /* simple orthographic projection */
  glMatrixMode( GL_MODELVIEW );
  glEnable( GL_DEPTH_TEST );
  glClearColor( 0.5, 0.5, 0.5, 1 );

}

void STLViewer::resizeGL( int w, int h ) {
  glViewport( 0, 0, w, h );
//  glMatrixMode( GL_PROJECTION );
//  gluPerspective( 45, ( float ) w / h, 0.01, 10000.0 );
//  glMatrixMode( GL_MODELVIEW );
//  glLoadIdentity( );
//  gluLookAt( 0, 0, 5, 0, 0, 0, 0, 1, 0 );
//  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  glMatrixMode( GL_PROJECTION );
  glLoadIdentity( );
  glOrtho( -4, 4, -2, 2, -2, 2 );    /* simple orthographic projection */
  glMatrixMode( GL_MODELVIEW );
  glEnable( GL_DEPTH_TEST );
  glClearColor( 0.5, 0.5, 0.5, 1 );

}


void STLViewer::paintGL( ) {

  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

  glLoadIdentity( ); /* Set up modelview transform, first cube. */
  glTranslatef( -2, 0, 0 ); /* Move cube to left half of window. */

  glRotatef( rotateZ, 0, 0, 1 ); /* Apply rotations. */
  glRotatef( rotateY, 0, 1, 0 );
  glRotatef( rotateX, 1, 0, 0 );

  glVertexPointer( 3, GL_FLOAT, 0, cubeCoords ); /* Set data type and location, first cube. */
  glColorPointer( 3, GL_FLOAT, 0, cubeFaceColors );

  glEnableClientState( GL_VERTEX_ARRAY );
  glEnableClientState( GL_COLOR_ARRAY );

  glDrawArrays( GL_QUADS, 0, 24 ); /* Draw the first cube! */


  /*
   * Second cube, using glDrawElements.  Also draw the cube edges, and enable polygon offset
   * while the faces of the cube are being drawn.
   */

  glLoadIdentity( ); /* Set up modelview transform, first cube. */

  glTranslatef( 2, 0, 0 ); /* Move cube to right half of window. */

  glRotatef( rotateZ, 0, 0, 1 ); /* Apply rotations. */
  glRotatef( rotateY, 0, 1, 0 );
  glRotatef( rotateX, 1, 0, 0 );

  glVertexPointer( 3, GL_FLOAT, 0, vertexCoords ); /* Set data type and location, second cube. */
  glColorPointer( 3, GL_FLOAT, 0, vertexColors );


  glEnable( GL_POLYGON_OFFSET_FILL );
  glPolygonOffset( 1, 1 );
  glDrawElements( GL_QUADS, 24, GL_UNSIGNED_INT, elementArray ); /* Draw the second cube! */
  glDisable( GL_POLYGON_OFFSET_FILL );

  glDisableClientState( GL_COLOR_ARRAY ); /* Don't use color array for the edges. */
  glColor3f( 0, 0, 0 ); /* The edges will be black. */
  glLineWidth( 2 );

  glDrawElements( GL_LINES, 24, GL_UNSIGNED_INT, edgeElementArray ); /* Draw the edges! */

  //  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  //  glColor3i(1,1,1);
  //  glLoadIdentity();
  //  glTranslatef(-100, -100, -100);
  //  glVertexPointer(3, GL_DOUBLE, 0, verts);
  //  glEnable(GL_POLYGON_OFFSET_FILL);
  //  glPolygonOffset(1,1);
  //  glDrawElements( GL_TRIANGLES, mesh->getNtris(), GL_UNSIGNED_INT, tris );
  //  glDisable(GL_POLYGON_OFFSET_FILL);

}


void STLViewer::keyPressEvent(QKeyEvent * evt){
  switch( evt->key() ){
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
    rotateX = rotateY = rotateZ = 0;
    break;
  }
  update();
}
