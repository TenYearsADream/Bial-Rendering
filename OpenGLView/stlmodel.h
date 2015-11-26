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

using namespace Bial;

class StlModel {
  TriangleMesh *mesh = nullptr;
  Vector< GLdouble > verts;
  Vector< GLdouble > norms;
  Vector< GLuint > tris;
  std::array< float, 3 > boundings;

public:
  StlModel(TriangleMesh *amesh );
  ~StlModel( );
  void reload( );
  void draw( bool drawNorm );
  void drawNormals( );
  void save(QString fileName);
  static StlModel* loadStl( QString fileName );
  static StlModel* marchingCubes(QString fileName, QString maskFileName, float isolevel, float scale );

private:
  void RemoveLittleComponents( Vector< size_t > &vertexIndex, size_t numVerts );
  void SimplifyMesh( Vector< size_t > &vertexIndex, Vector< Normal > &n, Vector< Point3D > &p );
};

#endif /* STLMODEL_H */
