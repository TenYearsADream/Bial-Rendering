#ifndef STLMODEL_H
#define STLMODEL_H


#include "MarchingCubes.hpp"
#include "glassert.h"
#include <Draw.hpp>
#include <GL/glu.h>
#include <GL/glut.h>
#include <QOpenGLWidget>

#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QString>


class StlModel {
  Bial::TriangleMesh *mesh;
  GLdouble *verts;
  GLdouble *norms;
  GLuint *tris;
  float boundings[ 3 ];
  QOpenGLVertexArrayObject mVAO;
  QOpenGLBuffer mVertexPositionBuffer;
  QOpenGLBuffer mVertexColorBuffer;

  void prepareVertexBuffers( QOpenGLShaderProgram &mShaderProgram );

public:
  StlModel( QString fileName );
  ~StlModel( );
  void reload( );
  void draw( );
};

#endif /* STLMODEL_H */
