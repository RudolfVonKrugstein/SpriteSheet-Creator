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
  m_outImage = QImage(ip.getDim(), QImage::Format_ARGB32);
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

void MainWindowImpl::export(const QString outDir, const QString xmlFile, const QString pngFile) {
  // Write the png
  QImageWriter l_writer;
  l_writer.setFormat("png");
  l_write.setFileName(outDir + pngFile);
  if (!l_writer.write(m_outImage)) {
    QMessageBox.error(this, "Unable to write PNG", l_writer.errorString());
    return;
  }
  // Write the xml
  QDomDocument doc("SpriteSheet");  
  QDomElement root = doc.createElement("spritesheet");
  doc.appendChild(root);
  QDomElement texture = doc.createElement("texture");
  texture.setAttribute("name", pngFile);
  root.appendChild(texture);
  
  std::list<Image*> l_images;
  m_imageModel.getImagePointerList(l_images);
  
  for (std::list<Image*>::iterator i = l_images.begin(); i != l_images.end(); ++i) {
    QDomElement sprite = doc.createElement("sprite");
    // Set name
    sprite.setAttribute("name", (*i)->getShortName());
    // Set color offset
    bool l_autoCrop = autocrop->checkState() == Qt::Checked;
    if (!l_autoCrop){
      sprite.setAttribute("colorOffset", "0,0");
    } else {
      sprite.setAttribute("colorOffset", QString::number((*i)->getCroppedOffset().x()) + "," + QString::number((*i)->getCroppedOffset().y()));
    }
    // Set texture rect
    sprite.setAttribute("textureRect", QString::number((*i)->m_anchor.x()) + "," + QString::number((*i)->m_anchor.y()) + "," + QString((*i)->width(l_autoCrop)) + "," + QString((*i)->height(l_autoCrop)));
    // Add the sprite
    root.appendChild(sprite);
  }
}
