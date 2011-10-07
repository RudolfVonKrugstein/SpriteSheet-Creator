#include <QtGui>
#include "mainWindow.h"
#include "imagePacker.h"
#include <iostream>

MainWindowImpl::MainWindowImpl(QWidget* parent) : m_imageModel(m_data), m_behaviorModel(m_data) ,m_currentBehaviorModel(m_data)
{
  setupUi(this);

  // Set models and item delegate
  imageList->setModel(&m_imageModel); 
  behaviorList->setModel(&m_behaviorModel);
  m_currentBehaviorModel.setBehaviorList(behaviorList);
  currentBehavior->setModel(&m_currentBehaviorModel);

  // Enable drag and drop
  imageList->setDragEnabled(true);
  currentBehavior->setAcceptDrops(true);
  currentBehavior->setDropIndicatorShown(true);

  // Create context menu
  imageList->setContextMenuPolicy(Qt::ActionsContextMenu);
  imageList->addAction(actionDelete_Image);
  imageList->addAction(actionImport_Images);
  imageList->addAction(actionAppend_Image);
  
  behaviorList->setContextMenuPolicy(Qt::ActionsContextMenu);
  behaviorList->addAction(actionAdd_Behavior);
  behaviorList->addAction(actionDelete_Behavior);
  behaviorList->addAction(actionEdit_Behavior);

  currentBehavior->setContextMenuPolicy(Qt::ActionsContextMenu);
  currentBehavior->addAction(actionMove_frame_right);
  currentBehavior->addAction(actionMove_frame_left);

  // Set toolbutton actions
  addImage->setDefaultAction(actionImport_Images);
  deleteImage->setDefaultAction(actionDelete_Image);
  appendImage->setDefaultAction(actionAppend_Image); 
  newBehavior->setDefaultAction(actionAdd_Behavior);
  editBehavior->setDefaultAction(actionEdit_Behavior);
  moveFrameRight->setDefaultAction(actionMove_frame_right);
  moveFrameLeft->setDefaultAction(actionMove_frame_left);
  
  // connect actions
  connect(actionImport_Images, SIGNAL(triggered()), this, SLOT(importImages()));
  connect(actionDelete_Image, SIGNAL(triggered()), this, SLOT(doDeleteImage()));
  connect(actionAppend_Image, SIGNAL(triggered()), this, SLOT(doAppendImage()));
  connect(actionAdd_Behavior, SIGNAL(triggered()), this, SLOT(doAddBehavior()));
  connect(actionEdit_Behavior, SIGNAL(triggered()), this, SLOT(doEditBehavior()));
  connect(actionMove_frame_right, SIGNAL(triggered()), this, SLOT(doMoveFrameRight()));
  connect(actionMove_frame_left, SIGNAL(triggered()), this, SLOT(doMoveFrameLeft()));
  
  /*connect(imageList->selectionModel(), SIGNAL(selectionChanged(QItemSelection, QItemSelection)), this, SLOT(imageListSelectionChanged(QItemSelection, QItemSelection)));
  connect(behaviorList->selectionModel(), SIGNAL(selectionChanged(QItemSelection, QItemSelection)), this, SLOT(behaviorListSelectionChanged(QItemSelection, QItemSelection)));
  connect(currentBehavior->selectionModel(), SIGNAL(selectionChanged(QItemSelection, QItemSelection)), this, SLOT(currentBehaviorSelectionChanged(QItemSelection, QItemSelection)));*/
  connect(imageList->selectionModel(), SIGNAL(selectionChanged(QItemSelection, QItemSelection)), this, SLOT(selectionChanged(QItemSelection, QItemSelection)));
  connect(behaviorList->selectionModel(), SIGNAL(selectionChanged(QItemSelection, QItemSelection)), this, SLOT(selectionChanged(QItemSelection, QItemSelection)));
  connect(currentBehavior->selectionModel(), SIGNAL(selectionChanged(QItemSelection, QItemSelection)), this, SLOT(selectionChanged(QItemSelection, QItemSelection)));
  
  // Initial state of actions
  actionDelete_Image->setEnabled(false);
  actionAppend_Image->setEnabled(false);
  actionEdit_Behavior->setEnabled(false);
  actionMove_frame_right->setEnabled(false);
  actionMove_frame_left->setEnabled(false);
}

void MainWindowImpl::importImages() {
  QStringList names = QFileDialog::getOpenFileNames(this, tr("Select images to import"), QString::null, tr("Images (*.png *.xpm *.jpg)"));
  for (QStringList::iterator n = names.begin(); n != names.end(); ++n) {
    importImage(*n);
  }
  m_imageModel.onDataChanged();
  m_data.recreatePackedTexture(autocrop->checkState() == Qt::Checked);
  // Set image
  outTexture->setPixmap(QPixmap::fromImage(m_data.m_outImage));
}

void MainWindowImpl::importImage(const QString& name) {
  if (!m_data.loadImage(name, QFileInfo(name).baseName())) {
    QMessageBox::warning(this, "Unable to load image", "Unable to load " + name);
  }
}

void MainWindowImpl::selectionChanged(QItemSelection n, QItemSelection o) {
  bool l_imageSelected = imageList->selectionModel()->selectedIndexes().count() != 0;
  bool l_behaviorSelected = behaviorList->selectionModel()->selectedIndexes().count() != 0;
  bool l_currentBeSelected = currentBehavior->selectionModel()->selectedIndexes().count() != 0;
  actionDelete_Image->setEnabled(l_imageSelected);
  actionAppend_Image->setEnabled(l_imageSelected && l_behaviorSelected);
  actionEdit_Behavior->setEnabled(l_behaviorSelected);
  actionMove_frame_right->setEnabled(l_currentBeSelected);
  actionMove_frame_left->setEnabled(l_currentBeSelected);
  m_currentBehaviorModel.onDataChanged();
}

/*void MainWindowImpl::imageListSelectionChanged(QItemSelection n, QItemSelection o) {
  if (n.count() == 0) {
    actionDelete_Image->setEnabled(false);
  } else {
    actionDelete_Image->setEnabled(true);
  }
}

void MainWindowImpl::behaviorListSelectionChanged(QItemSelection n, QItemSelection o) {
  if (n.count() == 0) {
    actionEdit_Behavior->setEnabled(false);
    actionMove_frame_right->setEnabled(false);
    actionMove_frame_left->setEnabled(false);
  } else {
    actionEdit_Behavior->setEnabled(true);
    actionMove_frame_right->setEnabled(true);
    actionMove_frame_left->setEnabled(true);
  }
  m_currentBehaviorModel.onDataChanged();
}

void MainWindowImpl::currentBehaviorSelectionChanged(QItemSelection n, QItemSelection o) {
  if (n.count() == 0) {
    actionMove_frame_right->setEnabled(false);
    actionMove_frame_left->setEnabled(false);
  } else {
    actionMove_frame_right->setEnabled(true);
    actionMove_frame_left->setEnabled(true);
  }
}*/

void MainWindowImpl::doDeleteImage() {
  m_data.deleteImage(imageList->selectionModel()->selectedIndexes().front().row());
  m_imageModel.onDataChanged();
}

void MainWindowImpl::doAppendImage() {
  m_data.appendImage(imageList->selectionModel()->selectedIndexes().front().row(),
      behaviorList->selectionModel()->selectedIndexes().front().row());
  m_currentBehaviorModel.onDataChanged();
}

void MainWindowImpl::doAddBehavior() {
  m_data.addBehavior("StrangeName");
  m_behaviorModel.onDataChanged();
}

void MainWindowImpl::doEditBehavior() {
}

void MainWindowImpl::doMoveFrameRight() {
  m_data.moveFrameRight(
      behaviorList->selectionModel()->selectedIndexes().front().row(),
      currentBehavior->selectionModel()->selectedIndexes().front().row());
  m_currentBehaviorModel.onDataChanged();
}

void MainWindowImpl::doMoveFrameLeft() {
  m_data.moveFrameLeft(
      behaviorList->selectionModel()->selectedIndexes().front().row(),
      currentBehavior->selectionModel()->selectedIndexes().front().row());
  m_currentBehaviorModel.onDataChanged();
}

