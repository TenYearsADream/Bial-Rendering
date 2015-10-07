#ifndef STLVIEWER_H
#define STLVIEWER_H

#include <Draw.hpp>
#include <GL/glu.h>
#include <GL/glut.h>
#include <QOpenGLWidget>
#include <QWidget>

class STLViewer : public QOpenGLWidget {
  Q_OBJECT

  Bial::TriangleMesh *mesh;
  GLdouble *verts;
  GLdouble *norms;
  GLuint *tris;
  float boundings[ 3 ];
  double zoom = 1.0;
  int rotateX = 0;
  int rotateY = 0;
  int rotateZ = 0;
  bool dragging = false;
  QPoint lastPoint;
public:
  explicit STLViewer( QWidget *parent = 0 );
  void LoadStl(QString stlFile);
  void resetTransform();
protected:
  void initializeGL( );
  void resizeGL( int w, int h );
  void paintGL( );

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
