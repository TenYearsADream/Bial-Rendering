#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
  class MainWindow;
}

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  explicit MainWindow( QWidget *parent = 0 );
  ~MainWindow( );

private:
  Ui::MainWindow *ui;

  /* QWidget interface */
protected:
  void keyPressEvent( QKeyEvent* );
private slots:
  void on_actionOpen_files_triggered( );
  void on_pushButton_clicked( );
  void on_checkBox_clicked( bool checked );
};

#endif /* MAINWINDOW_H */
