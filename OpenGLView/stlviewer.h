#ifndef STLVIEWER_H
#define STLVIEWER_H

#include "stlmodel.h"

#include <Draw.hpp>
#include <GL/glu.h>
#include <GL/glut.h>
#include <QOpenGLWidget>
#include <QWidget>

class Light {
public:
  int lightNbr = GL_LIGHT0;
  GLfloat lightPos[ 4 ] = { 0.0f, 0.0f, 1.0f, 0 };
  GLfloat lightDir[ 4 ] = { 0.0f, 0.0f, -1.0f, 10 };
  GLfloat specular[ 4 ] = { 1.0f, 1.0f, 1.0f, 1.0f };
  GLfloat specref[ 4 ] = { 1.0f, 1.0f, 1.0f, 1.0f };
  GLfloat diffuse[ 4 ] = { 0.4f, 0.4f, 0.4f, 1.0f };
  GLfloat ambient[ 4 ] = { 0.0f, 0.0f, 0.0f, 1.0f };
  double cutoff = 15.0f;
  void enable( ) {
    glLightfv( lightNbr, GL_DIFFUSE, diffuse );
    glLightfv( lightNbr, GL_SPECULAR, specular );
    glLightfv( lightNbr, GL_POSITION, lightPos );
    glLightfv( lightNbr, GL_AMBIENT, ambient );
/*
 *    glLightfv( lightNbr, GL_SPOT_DIRECTION, lightDir );
 *    glLightf( lightNbr, GL_SPOT_CUTOFF, cutoff );
 *    glLightf( lightNbr, GL_SPOT_EXPONENT, 1.0f );
 */
    glEnable( lightNbr );
  }
  void draw( ) {
    glPushMatrix( );
    glBegin( GL_POINTS );
    glVertex3fv( lightPos );
    glEnd( );
    glPopMatrix( );
  }
};

class STLViewer : public QOpenGLWidget {
  Q_OBJECT
  Light light1;
  double zoom = 1.0;
  int rotateX = 0;
  int rotateY = 0;
  int rotateZ = 0;
  bool dragging = false;
  QPoint lastPoint;
  StlModel *model = nullptr;
  QString fileName;
  bool drawNormals = false;

public:
  explicit STLViewer( QWidget *parent = 0 );
  void LoadFile( QString stlFile );

  void drawLines( );

  void runMarchingCubes( float isolevel, float scale );

  StlModel* getModel( ) const;

  bool getDrawNormals( ) const;
  void setDrawNormals( bool value );

signals:
  void finishedMCubes( );
protected:
  void resetTransform( );
  void initializeGL( );
  void resizeGL( int w, int h );
  void paintGL( );
  void clear( );

  /* QWidget interface */
protected:
  void keyPressEvent( QKeyEvent *evt );
  void mousePressEvent( QMouseEvent *evt );
  void mouseReleaseEvent( QMouseEvent *evt );
  void mouseMoveEvent( QMouseEvent *evt );
  void wheelEvent( QWheelEvent *evt );
  void mouseDoubleClickEvent( QMouseEvent *evt );
};

#endif /* STLVIEWER_H */
