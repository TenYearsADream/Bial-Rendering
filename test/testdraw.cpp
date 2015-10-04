#include "Draw.hpp"
#include "testdraw.h"

#include <QProcess>
using namespace Bial;
using namespace std;

void TestDraw::testTriangleMesh( ) {
  shared_ptr< Transform3D > o2w( new Transform3D( ) );
/*  o2w->Translate(10,10,10); */
  o2w->Rotate( 35, Transform3D::X );
  o2w->Rotate( 35, Transform3D::Y );
  o2w->Scale( 1.5, 2.2, 3.5 );
  shared_ptr< Transform3D > w2o( new Transform3D( o2w->Inverse( ) ) );
  /* TODO Study about vertex normals :). */
  Point3D p[ 4 ] = { Point3D( 1, 0, 0 ), Point3D( 0, 1, 0 ), Point3D( 0, 0, 1 ), Point3D( 1, 1, 1 ) };
/*  Normal n[4] = {Normal(2,1,1),Normal(1,2,1),Normal(1,1,2),Normal(2,2,2)}; */
  size_t vi[ 12 ] = { 2, 1, 0, 0, 1, 3, 1, 2, 3, 3, 2, 0 };
  shared_ptr< TriangleMesh > mesh( new TriangleMesh( o2w.get( ), w2o.get( ), false, 4, 4, &vi[ 0 ],
                                                     &p[ 0 ] /*,&n[0]*/ ) );
  QVERIFY_EXCEPTION_THROWN( mesh->Area( ), runtime_error ); /* Unimplemented method */
  QVERIFY( !mesh->CanIntersect( ) );
/*  QCOMPARE(mesh->ObjectBound(),BBox(Point3D(0,0,0),Point3D(1,1,1))); */
  mesh->Print( cout );
//  cout << endl << "STL: " << endl;
//  mesh->ExportSTLA( cout );
  mesh->ExportSTLA( "/tmp/tst.stl" );
  QProcess view;
  view.start( "glc_view", QStringList( ) << "/tmp/tst.stl" );
  view.waitForFinished( );
}
