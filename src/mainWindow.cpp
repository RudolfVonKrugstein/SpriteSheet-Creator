#include <QtGui>
#include "mainWindow.h"

MainWindowImpl::MainWindowImpl(QWidget* parent) : m_imageModel(), m_behaviorModel(&m_imageModel)
{
  setupUi(this);

  // Set models and item delegate
  imageList->setModel(&m_imageModel); 
  currentBehavior->setModel(&m_behaviorModel);

  // Enable drag and drop
  imageList->setDragEnabled(true);
  currentBehavior->setAcceptDrops(true);
  currentBehavior->setDropIndicatorShown(true);

  // Create context menu
  imageList->setContextMenuPolicy(Qt::ActionsContextMenu);
  imageList->addAction(actionDelete_Image);
  
  // connect actions
  connect(actionImport_Images, SIGNAL(triggered()), this, SLOT(importImages()));
  connect(imageList->selectionModel(), SIGNAL(selectionChanged(QItemSelection, QItemSelection)), this, SLOT(imageListSelectionChanged(QItemSelection, QItemSelection)));
  actionDelete_Image->setEnabled(false);
}

void MainWindowImpl::importImages() {
  QStringList names = QFileDialog::getOpenFileNames(this, tr("Select images to import"), QString::null, tr("Images (*.png, *.xpm, *.jpg)"));
  for (QStringList::iterator n = names.begin(); n != names.end(); ++n) {
    importImage(*n);
  }
  m_imageModel.onDataChanged();
}

void MainWindowImpl::importImage(const QString& name) {
  QImage image(name);
  if (image.isNull()) {
    QMessageBox::warning(this, "Unable to load image", "Unable to load " + name);
    return;
  }
  image.convertToFormat(QImage::Format_ARGB32);
  m_imageModel.addImage(Image(image), QFileInfo(name).baseName());
}

void MainWindowImpl::imageListSelectionChanged(QItemSelection n, QItemSelection o) {
  if (n.count() == 0) {
    actionDelete_Image->setEnabled(false);
  } else {
    actionDelete_Image->setEnabled(true);
  }
}
