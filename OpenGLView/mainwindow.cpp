#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QKeyEvent>
#include <QMessageBox>
#include <QProgressDialog>
#include <QtConcurrent/QtConcurrent>

MainWindow::MainWindow( QWidget *parent ) : QMainWindow( parent ), ui( new Ui::MainWindow ) {
  ui->setupUi( this );
  QStringList args = QApplication::arguments( );
  if( args.size( ) == 2 ) {
    QFileInfo info( args.at( 1 ) );
    if( info.isFile( ) ) {
      ui->openGLWidget->LoadFile( info.absoluteFilePath( ) );
    }
    else {
      QMessageBox::warning( this, "ERROR", "File not found." );
      exit( 1 );
    }
  }
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
                                  tr(
                                    "All Supported files (*.nii *.nii.gz *.stl *.stl.gz) ;;STL files (*.stl *.stl.gz);; NIfTI Images (*.nii *.nii.gz) " ) );
  ui->openGLWidget->LoadFile( fileName );
}

void MainWindow::on_pushButton_clicked( ) {
//  QProgressDialog progress;
//  connect( ui->openGLWidget, &STLViewer::finishedMCubes, &progress, &QWidget::close );
  ui->openGLWidget->runMarchingCubes( ( double ) ui->spinBox->value( ), ui->doubleSpinBox->value( ) );
}
