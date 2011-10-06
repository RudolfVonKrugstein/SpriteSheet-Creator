#ifndef MAINWINDOW_H__
#define MAINWINDOW_H__

#include "ui_gui.h"
#include "imageListModel.h"
#include "behaviorListModel.h"
#include "currentBehaviorModel.h"
#include "data.h"

class MainWindowImpl : public QMainWindow, private Ui::MainWindow
{
  Q_OBJECT

public:
  MainWindowImpl(QWidget *parent = 0);

private:
  SpriteSheetData m_data;
  ImageListModel m_imageModel;
  BehaviorListModel m_behaviorModel;
  CurrentBehaviorModel m_currentBehaviorModel;
  
  void importImage(const QString& name);
  
public slots:
  void importImages();
  void imageListSelectionChanged(QItemSelection n, QItemSelection o);
};

#endif
