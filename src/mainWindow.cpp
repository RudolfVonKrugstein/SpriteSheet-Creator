#include <QtGui>
#include "mainWindow.h"
#include "imagePacker.h"
#include <iostream>

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
  QStringList names = QFileDialog::getOpenFileNames(this, tr("Select images to import"), QString::null, tr("Images (*.png *.xpm *.jpg)"));
  for (QStringList::iterator n = names.begin(); n != names.end(); ++n) {
    importImage(*n);
  }
  m_imageModel.onDataChanged();
  recreatePackedTexture();
}

void MainWindowImpl::importImage(const QString& name) {
  QImage image(name);
  if (image.isNull()) {
    QMessageBox::warning(this, "Unable to load image", "Unable to load " + name);
    return;
  }
  image.convertToFormat(QImage::Format_ARGB32);
  m_imageModel.addImage(Image(image, QFileInfo(name).baseName()), name);
}

void MainWindowImpl::imageListSelectionChanged(QItemSelection n, QItemSelection o) {
  if (n.count() == 0) {
    actionDelete_Image->setEnabled(false);
  } else {
    actionDelete_Image->setEnabled(true);
  }
}

void MainWindowImpl::recreatePackedTexture() {
  // Create a list of images
  std::list<Image*> l_images;
  m_imageModel.getImagePointerList(l_images);
  // Find image positions
  ImagePacker ip(l_images,  autocrop->checkState() == Qt::Checked);
  ip.packImages();
  // Create destionation image
  QImage outImage(ip.getDim(), QImage::Format_ARGB32);
  QPainter painter(&outImage);
  for (std::list<Image*>::iterator i = l_images.begin(); i != l_images.end(); ++i) {
    if (autocrop->checkState() != Qt::Checked)
      painter.drawImage((*i)->m_anchor, (*i)->getImage());
    else
      painter.drawImage((*i)->m_anchor, (*i)->getCroppedImage());
  }  
  // Set image
  outTexture->setPixmap(QPixmap::fromImage(outImage));
}
