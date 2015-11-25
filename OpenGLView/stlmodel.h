#ifndef STLMODEL_H
#define STLMODEL_H


#include "MarchingCubes.hpp"
#include "glassert.h"
#include <Draw.hpp>
#include <GL/glu.h>
#include <GL/glut.h>
#include <QOpenGLWidget>
#include <QString>
#include <array>


class StlModel {
  Bial::TriangleMesh *mesh = nullptr;
  Bial::Vector< GLdouble > verts;
  Bial::Vector< GLdouble > norms;
  Bial::Vector< GLuint > tris;
  std::array< float, 3 > boundings;

public:
  StlModel( Bial::TriangleMesh *mesh );
  ~StlModel( );
  void reload( );
  void draw( bool drawNorm );
  void drawNormals( );

  static StlModel* loadStl( QString fileName );
  static StlModel* marchingCubes( QString fileName, float isolevel, float scale );
  void simplifyMesh(Bial::TriangleMesh * mesh);
};

#endif /* STLMODEL_H */
