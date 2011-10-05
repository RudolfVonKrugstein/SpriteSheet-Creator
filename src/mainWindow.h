#ifndef MAINWINDOW_H__
#define MAINWINDOW_H__

#include "ui_gui.h"
#include "imageListModel.h"
#include "behaviorListModel.h"

class MainWindowImpl : public QMainWindow, private Ui::MainWindow
{
  Q_OBJECT

public:
  MainWindowImpl(QWidget *parent = 0);

private:
  ImageListModel m_imageModel;
  BehaviorListModel m_behaviorModel;
  
  void importImage(const QString& name);
  
public slots:
  void importImages();
  void imageListSelectionChanged(QItemSelection n, QItemSelection o);
};

#endif
