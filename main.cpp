#include <QtTest>

#include "tst/testdraw.h"
#include "tst/testgeometrics.h"
#include "tst/testmarchingcubes.h"
using namespace std;

int main( int argc, char **argv ) {
  TestGeometrics testGeometrics;
  TestDraw testDraw;
  TestMarchingCubes testMarch;
  int status = 0;
  status |= QTest::qExec( &testMarch, argc, argv );
  status |= QTest::qExec( &testDraw, argc, argv );
  status |= QTest::qExec( &testGeometrics, argc, argv );
  return( status );
}
