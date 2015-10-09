#include "testmarchingcubes.h"

#include <Adjacency.hpp>
#include <Draw.hpp>
#include <MarchingCubes.hpp>
#include <QProcess>

using namespace Bial;

void TestMarchingCubes::testMarchingCube( ) {
  QTime time;
  time.start( );
  Image<int> img = Geometrics::Scale(File::Read<int>("res/0.nii.gz"),0.25,true);
//  Image< int > img = File::Read<int>( "res/0.nii.gz" );

  TriangleMesh *mesh = MarchingCubes::exec( img, 50.f );

  // mesh->Print(std::cout);
  // std::cout << std::endl << "STL: " << std::endl;
  // mesh->ExportSTLA(std::cout);
  time.restart( );

  mesh->ExportSTLB( "/tmp/marching2.stl" );
}

void TestMarchingCubes::testPolygonize( ) {
  Cell cell;
  BBox box( Point3D( 0, 0, 0 ), Point3D( 1, 1, 1 ) );
  for( size_t vtx = 0; vtx < 8; ++vtx ) {
    cell.p[ vtx ] = box.at( vtx ).toVector( );
    cell.val[ vtx ] = 1.f;
  }
  cell.val[ 2 ] = 0.f;
  cell.val[ 3 ] = 0.f;
  cell.calcIdx( 0.5 );
  COMMENT("Teste",0)
  QCOMPARE( cell.idx, (uchar)12 );
  cell.printIdx( );
  QCOMPARE( MarchingCubes::edgeTable[ cell.idx ], 0xc0a );
  MarchingCubes::printEdges( cell.idx );
  MarchingCubes::printTris( cell.idx );

  std::array< Vector3D, 12 > vertexList;
  MarchingCubes::getVertexList( vertexList, cell, 0.5 );
  MarchingCubes::printVertexList( vertexList );

  Vector< size_t > tris;
  Vector< Point3D > vertices;
  Vector< Normal > normals;
  std::cout << "Tris = " << MarchingCubes::Polygonize( cell, 0.5, tris, vertices, normals ) << std::endl;
  std::shared_ptr< TriangleMesh > mesh( new TriangleMesh( new Transform3D( ), new Transform3D( ),
                                                          false, tris.size( ) / 3,
                                                          vertices.size( ), &tris[ 0 ], &vertices[ 0 ],
                                                          &normals[ 0 ] ) );

  // mesh->Print(std::cout);
  // std::cout << std::endl << "STL: " << std::endl;
// mesh->ExportSTLA(std::cout);

  mesh->ExportSTLA( "/tmp/marching.stl" );

  std::cout << "Vertices  = " << vertices << std::endl;
  std::cout << "Triangles = " << tris << std::endl;
//  QProcess view;
//  view.start( "glc_player", QStringList( ) << "/tmp/marching.stl" );
//  view.waitForFinished( -1 );
}

void TestMarchingCubes::testBoxAdj( ) {
  Image< int > img( 2, 2, 2 );
  img( 0, 0, 1 ) = 0;
  img( 1, 0, 1 ) = 1;
  img( 1, 0, 0 ) = 2;
  img( 0, 0, 0 ) = 3;
  img( 0, 1, 1 ) = 4;
  img( 1, 1, 1 ) = 5;
  img( 1, 1, 0 ) = 6;
  img( 0, 1, 0 ) = 7;
  Adjacency adj = Adjacency::MarchingCube( );
  AdjacencyIterator itr = adj.begin( img, 0 );
  for( int vtx = 0; vtx < 8; ++vtx ) {
    QCOMPARE( img[ *( itr++ ) ], vtx );
  }
}
