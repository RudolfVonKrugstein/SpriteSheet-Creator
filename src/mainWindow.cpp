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
  connect(imageList->selectionModel(), SIGNAL(selectionChanged(QItemSelection, QItemSelection)), this, SLOT(imageListSelectionChanged(QItemSelection, QItemSelection)));
  actionDelete_Image->setEnabled(false);
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

void MainWindowImpl::imageListSelectionChanged(QItemSelection n, QItemSelection o) {
  if (n.count() == 0) {
    actionDelete_Image->setEnabled(false);
  } else {
    actionDelete_Image->setEnabled(true);
  }
}

