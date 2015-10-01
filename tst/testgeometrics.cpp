#include "testgeometrics.h"
#include <Common.hpp>
#include <Geometrics.hpp>
#include <TestHelper.hpp>

using namespace Bial;
using namespace std;

void TestGeometrics::testVector3D( ) {
  const Vector3D v1;
  QCOMPARE( v1.x, 0.0 );
  QCOMPARE( v1.y, 0.0 );
  QCOMPARE( v1.z, 0.0 );
  const Vector3D v2( 1.0, 2.0, 3.0 );
  QCOMPARE( v2.x, 1.0 );
  QCOMPARE( v2.y, 2.0 );
  QCOMPARE( v2.z, 3.0 );
  const Vector3D v3( 4.65f, 5.45f, 6.54f );
  Vector3D v4 = v1 + v2 + v3;
  QCOMPARE( v4.x, 5.65f );
  QCOMPARE( v4.y, 7.45f );
  QCOMPARE( v4.z, 9.54f );
  QVERIFY_EXCEPTION_THROWN( Vector3D( 0.0 / 0.0, 0, 0 ), runtime_error );
  v4 += v2;
  QCOMPARE( v4.x, 6.65f );
  QCOMPARE( v4.y, 9.45f );
  QCOMPARE( v4.z, 12.54f );
  v4 -= v2;
  QCOMPARE( v4.x, 5.65f );
  QCOMPARE( v4.y, 7.45f );
  QCOMPARE( v4.z, 9.54f );
  v4 = v4 - v3;
  QCOMPARE( v4.x, 1.0 );
  QCOMPARE( v4.y, 2.0 );
  QCOMPARE( v4.z, 3.0 );
  v4 = 1.0 * v2 * 2.0;
  QCOMPARE( v4.x, 2.0 );
  QCOMPARE( v4.y, 4.0 );
  QCOMPARE( v4.z, 6.0 );
  v4 *= 1.2f;
  QCOMPARE( v4.x, 2.4f );
  QCOMPARE( v4.y, 4.8f );
  QCOMPARE( v4.z, 7.2f );
  v4 /= 1.2f;
  QCOMPARE( v4.x, 2.0 );
  QCOMPARE( v4.y, 4.0 );
  QCOMPARE( v4.z, 6.0 );
  v4 = v4 / 2.0;
  QCOMPARE( v4.x, 1.0 );
  QCOMPARE( v4.y, 2.0 );
  QCOMPARE( v4.z, 3.0 );
  QVERIFY_EXCEPTION_THROWN( v4 /= 0, runtime_error );
  QVERIFY_EXCEPTION_THROWN( v4 / 0, runtime_error );
  QCOMPARE( Dot( v1, v2 ), 0.0 );
  QCOMPARE( Dot( v2, v2 ), 14.0 );
  QCOMPARE( Dot( v2, -v2 ), -14.0 );
  QCOMPARE( AbsDot( v2, -v2 ), 14.0 );
  Vector3D cross = Cross( v2, v3 );
  QCOMPARE( cross.x, -3.27f );
  QCOMPARE( cross.y, 7.41f );
  QCOMPARE( cross.z, -3.85f );
  Vector3D v5( 1.0, 1.0, 1.0 );
  Vector3D norm = v5.Normalized( );
  QCOMPARE( norm.x, 0.57735f );
  QCOMPARE( norm.y, 0.57735f );
  QCOMPARE( norm.z, 0.57735f );
  Vector3D norm2 = Vector3D( 3, 0, 0 ).Normalized( );
  QCOMPARE( norm2, Vector3D( 1, 0, 0 ) );
  QCOMPARE( v1.Length( ), 0.0 );
  QCOMPARE( v2.Length( ), 3.74166f );
  QCOMPARE( v2.LengthSquared( ), 14.0 );
}


void TestGeometrics::testCoordinateSystem( ) {
  Vector3D v1( 1.0, 2.0, 3.0 ), v2, v3;
  v1 = v1.Normalized( );
  CoordinateSystem( v1, &v2, &v3 );
  QCOMPARE( v2.x, 0.0 );
  QCOMPARE( v2.y, 0.83205f );
  QCOMPARE( v2.z, -0.5547f );
  QCOMPARE( v3.x, -0.963624f );
  QCOMPARE( v3.y, 0.14825f );
  QCOMPARE( v3.z, 0.222375f );
}

void TestGeometrics::testPoint3D( ) {
  const Point3D p1;
  QCOMPARE( p1.x, 0.0 );
  QCOMPARE( p1.y, 0.0 );
  QCOMPARE( p1.z, 0.0 );
  const Point3D p2( 1.0, 2.0, 3.0 );
  QCOMPARE( p2.x, 1.0 );
  QCOMPARE( p2.y, 2.0 );
  QCOMPARE( p2.z, 3.0 );
  const Vector3D v1( 1.0, 2.0, 3.0 );
  Point3D p3 = p1 + v1;
  QCOMPARE( p3.x, 1.0 );
  QCOMPARE( p3.y, 2.0 );
  QCOMPARE( p3.z, 3.0 );
  p3 += v1;
  QCOMPARE( p3.x, 2.0 );
  QCOMPARE( p3.y, 4.0 );
  QCOMPARE( p3.z, 6.0 );
  Vector3D v2 = p3 - p2;
  QCOMPARE( v2.x, 1.0 );
  QCOMPARE( v2.y, 2.0 );
  QCOMPARE( v2.z, 3.0 );
  v2 = p3 - v2;
  QCOMPARE( v2.x, 1.0 );
  QCOMPARE( v2.y, 2.0 );
  QCOMPARE( v2.z, 3.0 );
  p3 -= v2;
  QCOMPARE( p3.x, 1.0 );
  QCOMPARE( p3.y, 2.0 );
  QCOMPARE( p3.z, 3.0 );
  QCOMPARE( Distance( p1, p1 ), 0.0 );
  QCOMPARE( DistanceSquared( p1, p1 ), 0.0 );
  QCOMPARE( Distance( p1, p2 ), v1.Length( ) );
  QCOMPARE( DistanceSquared( p1, p2 ), v1.LengthSquared( ) );
}


void TestGeometrics::testNormal( ) {
  const Normal n1;
  QCOMPARE( n1.x, 0.0 );
  QCOMPARE( n1.y, 0.0 );
  QCOMPARE( n1.z, 0.0 );
  const Normal n2( 1.0, 2.0, 3.0 );
  QCOMPARE( n2.x, 1.0 );
  QCOMPARE( n2.y, 2.0 );
  QCOMPARE( n2.z, 3.0 );
  const Normal n3( 4.65f, 5.45f, 6.54f );
  Normal n4 = n1 + n2 + n3;
  QCOMPARE( n4.x, 5.65f );
  QCOMPARE( n4.y, 7.45f );
  QCOMPARE( n4.z, 9.54f );
  QVERIFY_EXCEPTION_THROWN( Normal( 0.0 / 0.0, 0, 0 ), runtime_error );
  n4 += n2;
  QCOMPARE( n4.x, 6.65f );
  QCOMPARE( n4.y, 9.45f );
  QCOMPARE( n4.z, 12.54f );
  n4 -= n2;
  QCOMPARE( n4.x, 5.65f );
  QCOMPARE( n4.y, 7.45f );
  QCOMPARE( n4.z, 9.54f );
  n4 = n4 - n3;
  QCOMPARE( n4.x, 1.0 );
  QCOMPARE( n4.y, 2.0 );
  QCOMPARE( n4.z, 3.0 );
  n4 = 1.0 * n2 * 2.0;
  QCOMPARE( n4.x, 2.0 );
  QCOMPARE( n4.y, 4.0 );
  QCOMPARE( n4.z, 6.0 );
  n4 *= 1.2f;
  QCOMPARE( n4.x, 2.4f );
  QCOMPARE( n4.y, 4.8f );
  QCOMPARE( n4.z, 7.2f );
  n4 /= 1.2f;
  QCOMPARE( n4.x, 2.0 );
  QCOMPARE( n4.y, 4.0 );
  QCOMPARE( n4.z, 6.0 );
  n4 = n4 / 2.0;
  QCOMPARE( n4.x, 1.0 );
  QCOMPARE( n4.y, 2.0 );
  QCOMPARE( n4.z, 3.0 );
  QVERIFY_EXCEPTION_THROWN( n4 /= 0, runtime_error );
  QVERIFY_EXCEPTION_THROWN( n4 / 0, runtime_error );
  QCOMPARE( Dot( n1, n2 ), 0.0 );
  QCOMPARE( Dot( n2, n2 ), 14.0 );
  QCOMPARE( Dot( n2, -n2 ), -14.0 );
  QCOMPARE( AbsDot( n2, -n2 ), 14.0 );
  Normal v5( 1.0, 1.0, 1.0 );
  Normal norm = v5.Normalized( );
  QCOMPARE( norm.x, 0.57735f );
  QCOMPARE( norm.y, 0.57735f );
  QCOMPARE( norm.z, 0.57735f );
  QCOMPARE( n1.Length( ), 0.0 );
  QCOMPARE( n2.Length( ), 3.74166f );
  QCOMPARE( n2.LengthSquared( ), 14.0 );
  const Vector3D vec( 1.0, 2.0, 3.0 );
  QCOMPARE( Dot( n2, -vec ), -14.0 );
  QCOMPARE( Dot( vec, -n2 ), -14.0 );
  QCOMPARE( AbsDot( n2, -vec ), 14.0 );
  QCOMPARE( AbsDot( vec, -n2 ), 14.0 );
}

void TestGeometrics::testRay( ) {
  Ray r1;
  QCOMPARE( r1.o, Point3D( 0, 0, 0 ) );
  QCOMPARE( r1.d, Vector3D( 0, 0, 0 ) );
  QCOMPARE( r1.mint, 0.0 );
/*  QCOMPARE(r1.maxt,INFINITY); */
  QCOMPARE( r1.time, 0.0 );
  QCOMPARE( r1.depth, 0 );
  Point3D o( 1.0, 2.0, 3.0 );
  Vector3D d( 1.0, 2.0, 3.0 );
  Ray r2( o, d, 10.0, 1000.0, 10.0, 110 );
  QCOMPARE( r2.o, o );
  QCOMPARE( r2.d, d );
  QCOMPARE( r2.mint, 10.0 );
  QCOMPARE( r2.maxt, 1000.0 );
  QCOMPARE( r2.time, 10.0 );
  QCOMPARE( r2.depth, 110 );
  QCOMPARE( r2( 0 ), o );
  QCOMPARE( r2( 1 ), o + d );
}

void TestGeometrics::textBBox( ) {
  const BBox emptybox;
  QCOMPARE( emptybox.pMin, Point3D( INFINITY, INFINITY, INFINITY ) );
  QCOMPARE( emptybox.pMax, Point3D( -INFINITY, -INFINITY, -INFINITY ) );
  const Point3D p1( 0, 1, 0 ), p2( 1, 0, 1 );
  BBox box2( p1 );
  QCOMPARE( box2.pMin, p1 );
  QCOMPARE( box2.pMax, p1 );
  BBox box3( p1, p2 );
  QCOMPARE( box3.pMin, Point3D( 0, 0, 0 ) );
  QCOMPARE( box3.pMax, Point3D( 1, 1, 1 ) );
  BBox box4( Point3D( -1, -1, -1 ), Point3D( ) );
  BBox box5 = Union( box3, Point3D( -1, -1, -1 ) );
  QCOMPARE( box5.pMin, Point3D( -1, -1, -1 ) );
  QCOMPARE( box5.pMax, Point3D( 1, 1, 1 ) );
  box5 = Union( box3, box4 );
  QCOMPARE( box5.pMin, Point3D( -1, -1, -1 ) );
  QCOMPARE( box5.pMax, Point3D( 1, 1, 1 ) );
  QCOMPARE( box2.Overlaps( box2 ), true );
  QCOMPARE( box3.Overlaps( emptybox ), false );
  QCOMPARE( box5.Inside( Point3D( ) ), true );
  QCOMPARE( box5.Inside( p1 ), true );
  QCOMPARE( box5.Inside( Point3D( 1, 1, 1 ) ), true );
  QCOMPARE( box5.Inside( Point3D( -10, -10, -10 ) ), false );
  QCOMPARE( box5.SurfaceArea( ), 24.0 );
  QCOMPARE( box5.Volume( ), 8.0 );
}

void TestGeometrics::testTransform3D( ) {
  Transform3D t1;
  QCOMPARE( t1.getAffineMatrix( ).Dims( ), ( size_t ) 2 );
  QCOMPARE( t1.getAffineMatrix( ).size( 0 ), ( size_t ) 4 );
  QCOMPARE( t1.getAffineMatrix( ).size( 1 ), ( size_t ) 4 );
  QCOMPARE( t1.getAffineMatrix( ).CompareToIdentity( ), 0.0 );
  Matrix< float > mat;
  QVERIFY_EXCEPTION_THROWN( Transform3D t2( mat ), logic_error );
  QVERIFY_EXCEPTION_THROWN( Transform3D t2( Matrix< float >( 2, 2 ) ), logic_error );
  QVERIFY( t1 == t1 );
  QVERIFY( !( t1 != t1 ) );
  Matrix< float > mat2( 4, 4 );
  Transform3D t3( mat2 );
  QVERIFY( !( t1 == t3 ) );
  QVERIFY( t1 != t3 );
  t1.Translate( 10.0, 9.0, 8.0 );
  QCOMPARE( t1.getAffineMatrix( ).at( 3, 1 ), 9.0 );
  QCOMPARE( t1.getInverseMatrix( ).at( 3, 1 ), -9.0 );
  QCOMPARE( t1( Point3D( 0, 1, 2 ) ), Point3D( 10, 10, 10 ) );
  QCOMPARE( t1( Vector3D( 1, 2, 3 ) ), Vector3D( 1, 2, 3 ) );
  Ray ray( Point3D( 0, 1, 2 ), Vector3D( 1, 2, 3 ) );
  ray = t1( ray );
  QCOMPARE( ray.o, Point3D( 10, 10, 10 ) );
  QCOMPARE( ray.d, Vector3D( 1, 2, 3 ) );
  t1.reset( );
  QCOMPARE( t1.getAffineMatrix( ).at( 3, 1 ), 0.0 );
  QCOMPARE( t1.getInverseMatrix( ).at( 3, 1 ), 0.0 );
  QVERIFY_EXCEPTION_THROWN( t1.Scale( 0, 0, 0 ), logic_error );
  t1.reset( );
  QVERIFY( !t1.HasScale( ) );
  t1.Scale( 2.0, 4.0, 8.0 );
  QVERIFY( t1.HasScale( ) );
  QCOMPARE( t1.getAffineMatrix( ).at( 0, 0 ), 2.0 );
  QCOMPARE( t1( Point3D( 1, 2, 3 ) ), Point3D( 2, 8, 24 ) );
  QCOMPARE( t1( Vector3D( 1, 2, 3 ) ), Vector3D( 2, 8, 24 ) );
  QCOMPARE( t1( Normal( 1, 2, 3 ) ), Normal( 2, 8, 24 ) );
  for( int axis = 0; axis < 3; ++axis ) {
    t1.reset( );
    t1.Rotate( 30.0, axis );
    Transform3D t4;
    Vector3D vec( 0, 0, 0 );
    vec[ axis ] = 3;
    t4.Rotate( 30.0, vec );
    QVERIFY2( t4 == t1, "Rotations matrices should match." );
  }
  t1.reset( );
  t1.Shear( 1, 1, 1, 1, 1, 1 );
  QCOMPARE( t1( Point3D( 1, 1, 1 ) ), Point3D( 3, 3, 3 ) );
  QCOMPARE( t1( Vector3D( 1, 1, 1 ) ), Vector3D( 3, 3, 3 ) );
/* /  QCOMPARE(t1(Normal(1,1,1)),Normal(1,1,1)); */
  t1.reset( );
/* /  t1.LookAt(Point3D(10,10,10),Point3D(0,0,0),Vector3D(0,1,0)); // Don't know how to test it */
}

void TestGeometrics::testImageTransform( ) {
  Image< int > img( "res/0.nii.gz" );
  /* Getting orientation string from file */
/*
 *  const NiftiHeader nifti("res/0.nii.gz");
 *  const string orientation = nifti.Orientation();
 */

/*
 *  cout << orientation << endl;
 *  cout << "AxialDimension    = " << NiftiHeader::AxialDimension(orientation) << endl;
 *  cout << "CoronalDimension  = " << NiftiHeader::CoronalDimension(orientation) << endl;
 *  cout << "SagittalDimension = " << NiftiHeader::SagittalDimension(orientation) << endl;
 *  cout << nifti.AffineMatrix() << endl;
 */

  QTime time;
  time.start( );

  {
    Transform3D axialTransform;
    axialTransform.Rotate( 90.0, Transform3D::X ).Rotate( 90.0, Transform3D::Y );
    Point3D start, end( img.size( 0 ), img.size( 1 ), img.size( 2 ) );
    axialTransform( start, &start );
    axialTransform( end, &end );
    BBox box( start, end );
    axialTransform = axialTransform.Inverse( );
    axialTransform.Translate( box.pMin.x, box.pMin.y, box.pMin.z );
    box = box.Normalized( );
    size_t z = box.pMax.z / 2.0;
    Image< int > axial( ( size_t ) abs( round( box.pMax.x ) ), ( size_t ) abs( round( box.pMax.y ) ) );
    for( size_t y = 0; y < axial.size( 1 ); ++y ) {
      for( size_t x = 0; x < axial.size( 0 ); ++x ) {
        Point3D pos = axialTransform( Point3D( x, y, z ) );
        if( img.ValidPixel( pos.x, pos.y, pos.z ) ) {
          axial( x, y ) = img( pos.x, pos.y, pos.z );
        }
      }
    }
    axial.Write( "dat/axial.pgm" );
    QVERIFY2( TestHelper::CompareImages( "dat/axial.pgm", "res/axial.pgm" ), "Image do not match to template." );
  } {
    Transform3D coronalTransform;
    coronalTransform.Rotate( 180.0, Transform3D::Z ).Rotate( 90.0, Transform3D::Y );
    Point3D start, end( img.size( 0 ), img.size( 1 ), img.size( 2 ) );
    coronalTransform( start, &start );
    coronalTransform( end, &end );
    BBox box( start, end );
    coronalTransform = coronalTransform.Inverse( );
    coronalTransform.Translate( box.pMin.x, box.pMin.y, box.pMin.z );
    box = box.Normalized( );
    size_t z = box.pMax.z / 2.0;
    Image< int > coronal( ( size_t ) abs( round( box.pMax.x ) ), ( size_t ) abs( round( box.pMax.y ) ) );
    for( size_t y = 0; y < coronal.size( 1 ); ++y ) {
      for( size_t x = 0; x < coronal.size( 0 ); ++x ) {
        const Point3D pos = coronalTransform( Point3D( x, y, z ) );
        if( img.ValidPixel( pos.x, pos.y, pos.z ) ) {
          coronal( x, y ) = img( pos.x, pos.y, pos.z );
        }
      }
    }
    coronal.Write( "dat/coronal.pgm" );
    QVERIFY2( TestHelper::CompareImages( "dat/coronal.pgm", "res/coronal.pgm" ), "Image do not match to template." );
  } {
    Transform3D sagittalTransform;
    sagittalTransform.Rotate( 180.0, Transform3D::Z );
    Point3D start, end( img.size( 0 ), img.size( 1 ), img.size( 2 ) );
    sagittalTransform( start, &start );
    sagittalTransform( end, &end );
    BBox box( start, end );
    sagittalTransform = sagittalTransform.Inverse( );
    sagittalTransform.Translate( box.pMin.x, box.pMin.y, box.pMin.z );
    box = box.Normalized( );
    size_t z = box.pMax.z / 2.0;
    Image< int > sagittal( ( size_t ) abs( round( box.pMax.x ) ), ( size_t ) abs( round( box.pMax.y ) ) );
    for( size_t y = 0; y < sagittal.size( 1 ); ++y ) {
      for( size_t x = 0; x < sagittal.size( 0 ); ++x ) {
        Point3D pos = sagittalTransform( Point3D( x, y, z ) );
        if( img.ValidPixel( pos.x, pos.y, pos.z ) ) {
          sagittal( x, y ) = img( pos.x, pos.y, pos.z );
        }
      }
    }
    sagittal.Write( "dat/sagittal.pgm" );
    QVERIFY2( TestHelper::CompareImages( "dat/sagittal.pgm", "res/sagittal.pgm" ), "Image do not match to template." );
  } {
    Transform3D shear;
/*    shear.Translate( img.size(0)/2.0, img.size(1)/2.0, img.size(2)/2.0); */
    shear.Shear( 0.0, 0.0, 0.0, 0.0, 0.0, 0.2 );
/*    shear.Translate(-img.size(0)/2.0,-img.size(1)/2.0,-img.size(2)/2.0); */
    shear = shear.Inverse( );
    const Image< int > img( "res/0-brain.nii.gz" );
    Image< int > res( img.Dim( ) );
    for( size_t z = 0; z < res.size( 2 ); ++z ) {
      for( size_t y = 0; y < res.size( 1 ); ++y ) {
        for( size_t x = 0; x < res.size( 0 ); ++x ) {
          Point3D pos = shear( Point3D( x, y, z ) );
          if( img.ValidPixel( pos.x, pos.y, pos.z ) ) {
            res( x, y, z ) = img( round( pos.x ), round( pos.y ), round( pos.z ) );
          }
        }
      }
    }
    res.Write( "dat/shear.nii.gz", "res/0-brain.nii.gz" );
    QVERIFY2( TestHelper::CompareImages( "dat/shear.nii.gz", "res/shear.nii.gz" ), "Image do not match to template." );
  }
  cout << "Elapsed time: " << time.elapsed( ) << endl;
}
