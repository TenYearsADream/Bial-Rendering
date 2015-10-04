#ifndef TESTGEOMETRICS_H
#define TESTGEOMETRICS_H

#include <QTest>

class TestGeometrics : public QObject {
  Q_OBJECT

private slots:

  void testVector3D( );

  void testCoordinateSystem( );

  void testPoint3D( );

  void testNormal( );

  void testRay( );

  void textBBox( );

  void testTransform3D( );

  void testImageTransform( );
};

#endif /* TESTGEOMETRICS_H */
