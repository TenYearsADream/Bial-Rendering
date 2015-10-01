#ifndef TESTMARCHINGCUBES_H
#define TESTMARCHINGCUBES_H

#include <QTest>

class TestMarchingCubes : public QObject {
  Q_OBJECT
private slots:

  void testMarchingCube();

  void testPolygonize();

  void testBoxAdj();

};

#endif // TESTMARCHINGCUBES_H
