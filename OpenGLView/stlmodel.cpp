#include "Sorting.hpp"
#include "stlmodel.h"
#include <QDebug>
#include <QOpenGLContext>
#include <QTime>

using namespace Bial;

bool comparePts( const Point3D &pt1, const Point3D &pt2 ) {
  if( pt1.x != pt2.x ) {
    return( pt1.x < pt2.x );
  }
  if( pt1.y != pt2.y ) {
    return( pt1.y < pt2.y );
  }
  return( pt1.z < pt2.z );
}

template< typename T >
Vector< std::size_t > sort_permutation( const Vector< T > &vec ) {
  Vector< std::size_t > p( vec.size( ) );
  std::iota( p.begin( ), p.end( ), 0 );
  std::sort( p.begin( ), p.end( ),
             [ & ]( std::size_t i, std::size_t j ) { return( comparePts( vec[ i ], vec[ j ] ) ); } );
  return( p );
}

template< typename T >
Vector< T > apply_permutation( const Vector< T > &vec, const Vector< std::size_t > &p ) {
  Vector< T > sorted_vec( p.size( ) );
  std::transform( p.begin( ), p.end( ), sorted_vec.begin( ),
                  [ & ]( std::size_t i ) { return( vec[ i ] ); } );
  return( sorted_vec );
}

StlModel::StlModel( TriangleMesh *mesh ) {
  this->mesh = mesh;
  simplifyMesh( mesh );
  QTime t;
  t.start( );

  /*    mesh->Print( std::cout ); */

  Vector< Point3D > p = mesh->getP( );
  Vector< size_t > vertexIndex = mesh->getVertexIndex( );
  Vector< Normal > n = mesh->getN( );

  Vector< size_t > order = sort_permutation( p );
  p = apply_permutation( p, order );
  if( n.size( ) == p.size( ) ) {
    n = apply_permutation( n, order );
  }
  Vector< size_t > invOrder( order.size( ) );
  for( size_t i = 0; i < order.size( ); ++i ) {
    invOrder[ order[ i ] ] = i;
  }
  tris = Vector< GLuint >( vertexIndex.size( ) );
  for( size_t vtx = 0; vtx < vertexIndex.size( ); ++vtx ) {
    tris[ vtx ] = invOrder[ vertexIndex[ vtx ] ];
  }
  verts.resize( p.size( ) * 3 );
  double xs( 0.0 ), ys( 0.0 ), zs( 0.0 );
  for( size_t i = 0; i < p.size( ); ++i ) {
    const Point3D &point = p[ i ];
    verts[ i * 3 ] = static_cast< GLdouble >( point.x );
    verts[ i * 3 + 1 ] = static_cast< GLdouble >( point.y );
    verts[ i * 3 + 2 ] = static_cast< GLdouble >( point.z );
    xs = std::max( xs, point.x );
    ys = std::max( ys, point.y );
    zs = std::max( zs, point.z );
  }
  if( !n.empty( ) ) {
    COMMENT( "Reading normals.", 0 );
    norms.resize( p.size( ) * 3 );
    for( size_t i = 0; i < n.size( ); ++i ) {
      const Normal &norm = n[ i ];
      norms[ i * 3 ] = 0.0 - static_cast< GLdouble >( norm.x );
      norms[ i * 3 + 1 ] = 0.0 - static_cast< GLdouble >( norm.y );
      norms[ i * 3 + 2 ] = 0.0 - static_cast< GLdouble >( norm.z );
    }
  }
  boundings[ 0 ] = xs;
  boundings[ 1 ] = ys;
  boundings[ 2 ] = zs;

  qDebug( ) << "Elapsed: " << t.elapsed( ) << " ms";
}

StlModel::~StlModel( ) {
  if( mesh ) {
    delete mesh;
  }
}

void StlModel::reload( ) {
  if( !verts.empty( ) ) {
/*    qDebug( ) << mesh->getNverts( ) << " verts found"; */
    glEnableClientState( GL_VERTEX_ARRAY );
    glVertexPointer( 3, GL_DOUBLE, 0, &verts[ 0 ] );
    glDisableClientState( GL_VERTEX_ARRAY );
  }
  if( !norms.empty( ) ) {
/*    qDebug( ) << mesh->getNverts( ) << " norms found"; */
    glEnableClientState( GL_NORMAL_ARRAY );
    glNormalPointer( GL_DOUBLE, 0, &norms[ 0 ] );
    glDisableClientState( GL_NORMAL_ARRAY );
  }
/*  qDebug( ) << mesh->getNtris( ) << " triangles found"; */
/*  qDebug( ) << "Loaded dada to OpenGL."; */
}

void StlModel::draw( bool drawNorm ) {
/*  qDebug( ) << "Drawing Model"; */
  if( mesh ) {
    reload( );
  }
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
  if( !tris.empty( ) ) {
/*    qDebug( ) << "Drawing Triangles."; */
    glAssert( glDrawElements( GL_TRIANGLES, tris.size( ), GL_UNSIGNED_INT, &tris[ 0 ] ) );
/*    qDebug( ) << "Drawing Normals."; */
    if( drawNorm ) {
      drawNormals( );
    }
  }
  glDisable( GL_POLYGON_OFFSET_FILL );
  glDisableClientState( GL_VERTEX_ARRAY );
  glDisableClientState( GL_NORMAL_ARRAY );
  glPopMatrix( );
}

void StlModel::drawNormals( ) {
  if( !norms.empty( ) ) {
    glBegin( GL_LINES );
    glColor3f( 1, 0, 0 );
    for( size_t i = 0; i < mesh->getNverts( ); ++i ) {
      int x1 = verts[ i * 3 ];
      int y1 = verts[ i * 3 + 1 ];
      int z1 = verts[ i * 3 + 2 ];
      int x2 = x1 + norms[ i * 3 ];
      int y2 = y1 + norms[ i * 3 + 1 ];
      int z2 = z1 + norms[ i * 3 + 2 ];
      glVertex3f( x1, y1, z1 );
      glVertex3f( x2, y2, z2 );

    }
    glEnd( );
  }
}

StlModel* StlModel::loadStl( QString fileName ) {
  COMMENT( "Loading stl file: " << fileName.toStdString( ), 0 );
  return( new StlModel( TriangleMesh::ReadSTLB( fileName.trimmed( ).toStdString( ) ) ) );
}

StlModel* StlModel::marchingCubes( QString fileName, float isolevel, float scale ) {
  if( fileName.isEmpty( ) ) {
    return( nullptr );
  }
  qDebug( ) << "Loading image.";
  Image< int > img = File::Read< int >( fileName.trimmed( ).toStdString( ) );
  if( scale != 1.0 ) {
    qDebug( ) << "Resizing image.";
    img = Geometrics::Scale( img, scale, true );
  }
  qDebug( ) << "Running marching cubes algorithm.";
  TriangleMesh *mesh = MarchingCubes::exec( img, isolevel * img.Maximum( ) );
  if( !mesh ) {
    qDebug( ) << "Failed to generate model.";
    return( nullptr );
  }
  qDebug( ) << "Returning a new STL Model.";
  return( new StlModel( mesh ) );

}

#include <algorithm>
#include <iostream>

void StlModel::simplifyMesh( TriangleMesh *mesh ) {
/*  std::vector<Point3D>points */
}
