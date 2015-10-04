#ifndef STLVIEWER_H
#define STLVIEWER_H

#include <QWidget>
#include <QOpenGLWidget>
#include <GL/glut.h>
#include <GL/glu.h>
#include <Draw.hpp>

class STLViewer : public QOpenGLWidget {
  Q_OBJECT

  Bial::TriangleMesh * mesh;
  GLdouble * verts;
  GLdouble * norms;
  GLuint * tris;
  float boundings[3];
public:
  explicit STLViewer(QWidget * parent = 0 );
protected:
  void initializeGL();
  void resizeGL(int w, int h);
  void paintGL();

  // QWidget interface
protected:
  void keyPressEvent(QKeyEvent *evt);
};

#endif /* STLVIEWER_H */
