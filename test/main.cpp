#include <QtTest>

#include "testdraw.h"
#include "testgeometrics.h"
#include "testmarchingcubes.h"
using namespace std;

int main( int argc, char **argv ) {
  QFileInfo testPath(QDir::current().filePath("res"));
  if( !testPath.exists() || !testPath.isDir()) {
    throw std::runtime_error("Test files path not found!");
  }

  TestGeometrics testGeometrics;
  TestDraw testDraw;
  TestMarchingCubes testMarch;
  int status = 0;
  status |= QTest::qExec( &testMarch, argc, argv );
  status |= QTest::qExec( &testDraw, argc, argv );
  status |= QTest::qExec( &testGeometrics, argc, argv );
  return( status );
}
