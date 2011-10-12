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
  void selectionChanged(QItemSelection n, QItemSelection o);
  /*void imageListSelectionChanged(QItemSelection n, QItemSelection o);
  void behaviorListSelectionChanged(QItemSelection n, QItemSelection o);
  void currentBehaviorSelectionChanged(QItemSelection n, QItemSelection o);*/
  void doDeleteImage();
  void doAppendImage();
  void doAddBehavior();
  void doDeleteBehavior();
  void doEditBehavior();
  void doMoveFrameRight();
  void doMoveFrameLeft();
  void doExportSpriteSheet();
  void doSave();
  void doLoad();
};

#endif
