#include "data.h"
#include "imagePacker.h"
#include <QtXml/QDomDocument>

void SpriteSheetData::addImage(const Image image, const QString& name) {
  m_images[name] = image;
  m_imageNames.push_back(name); 
}

const QPixmap SpriteSheetData::getThumbnail(const QString& name) const {
  return m_images.find(name)->second.m_thumbnail;
}

const QString SpriteSheetData::getShortName(const QString& name) const {
  return m_images.find(name)->second.getName();
}

void SpriteSheetData::getImagePointerList(std::list<Image*>& f_images) {
  for (std::map<QString, Image>::iterator i = m_images.begin(); i != m_images.end(); ++i) {
    f_images.push_back(&(i->second));
  }
}

void SpriteSheetData::recreatePackedTexture(bool f_autocrop) {
  // Create a list of images
  std::list<Image*> l_images;
  getImagePointerList(l_images);
  // Find image positions
  ImagePacker ip(l_images,  f_autocrop);
  ip.packImages();
  // Create destionation image
  m_outImage = QImage(ip.getDim(), QImage::Format_ARGB32);
  QPainter painter(&m_outImage);
  for (std::list<Image*>::iterator i = l_images.begin(); i != l_images.end(); ++i) {
    if (!f_autocrop)
      painter.drawImage((*i)->m_anchor, (*i)->getImage());
    else
      painter.drawImage((*i)->m_anchor, (*i)->getCroppedImage());
  }  
}

void SpriteSheetData::exportXML(const QString outDir, const QString xmlFile, const QString pngFile, bool f_autocrop, QWidget* f_parent){
  // Write the png
  QImageWriter l_writer;
  l_writer.setFormat("png");
  l_writer.setFileName(outDir + pngFile);
  if (!l_writer.write(m_outImage)) {
    QMessageBox::critical(f_parent, "Unable to write PNG", l_writer.errorString());
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
  getImagePointerList(l_images);
  
  // Export images
  for (std::list<Image*>::iterator i = l_images.begin(); i != l_images.end(); ++i) {
    QDomElement sprite = doc.createElement("sprite");
    // Set name
    sprite.setAttribute("name", (*i)->getName());
    // Set color offset
    if (!f_autocrop){
      sprite.setAttribute("colorOffset", "0,0");
    } else {
      sprite.setAttribute("colorOffset", QString::number((*i)->getCroppedOffset().x()) + "," + QString::number((*i)->getCroppedOffset().y()));
    }
    // Set texture rect
    sprite.setAttribute("textureRect", QString::number((*i)->m_anchor.x()) + "," + QString::number((*i)->m_anchor.y()) + "," + QString((*i)->width(f_autocrop)) + "," + QString((*i)->height(f_autocrop)));
    // Add the sprite
    root.appendChild(sprite);
  }

  // Export behviors
  for (std::vector<QString>::iterator i = m_behaviorNames.begin(); i != m_behaviorNames.end(); ++i) {
    QDomElement behavior = doc.createElement("behavior");
    // Set name
    behavior.setAttribute("name", *i);
    Behavior& l_behavior = m_behaviors[*i];
    for (std::vector<QString>::iterator j = l_behavior.begin(); j != l_behavior.end(); ++j) {
      QDomElement frame = doc.createElement("frame");
      frame.setAttribute("sprite", *j); 
      behavior.appendChild(frame);
    }
    root.appendChild(behavior);
  }

  // Write xml out
  QFile file( outDir + xmlFile);
  if( !file.open( QFile::WriteOnly ) ) {
    QMessageBox::critical(f_parent, "Unable to write XML", file.errorString());
    return;
  }

  QTextStream ts( &file );
  ts << doc.toString();

  file.close();
}
