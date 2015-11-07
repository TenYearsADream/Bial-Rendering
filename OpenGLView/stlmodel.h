#ifndef STLMODEL_H
#define STLMODEL_H


#include "MarchingCubes.hpp"
#include "glassert.h"
#include <Draw.hpp>
#include <GL/glu.h>
#include <GL/glut.h>
#include <QOpenGLWidget>

#include <QString>


class StlModel {
  Bial::TriangleMesh *mesh = nullptr;
  GLdouble *verts = nullptr;
  GLdouble *norms = nullptr;
  GLuint *tris = nullptr;
  float boundings[ 3 ];

public:
  StlModel( Bial::TriangleMesh *mesh );
  ~StlModel( );
  void reload( );
  void draw( );
  void drawNormals( );

  static StlModel* loadStl( QString fileName );
  static StlModel* marchingCubes( QString fileName, float isolevel, float scale );
};

#endif /* STLMODEL_H */
