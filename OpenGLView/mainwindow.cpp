#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QKeyEvent>

MainWindow::MainWindow( QWidget *parent ) : QMainWindow( parent ), ui( new Ui::MainWindow ) {
  ui->setupUi( this );
}

MainWindow::~MainWindow( ) {
  delete ui;
}


void MainWindow::keyPressEvent( QKeyEvent *e ) {
  if( e->key( ) == Qt::Key_Escape ) {
    close( );
  }
  else {
    QWidget::keyPressEvent( e );
  }
}

void MainWindow::on_actionOpen_files_triggered( ) {
  QString fileName =
    QFileDialog::getOpenFileName( this, "Open STL files.", QDir::homePath( ),
                                  tr( "All Supported files (*.nii *.nii.gz *.stl *.stl.gz) ;;STL files (*.stl *.stl.gz);; NIfTI Images (*.nii *.nii.gz) ") );
  ui->openGLWidget->LoadFile( fileName );
}
