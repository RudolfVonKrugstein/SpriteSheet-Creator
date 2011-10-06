#include "data.h"
#include "imagePacker.h"
#include <QtXml/QDomDocument>

void SpriteSheetData::clear() {
  m_behaviorNames.clear();
  m_behaviors.clear();
  m_images.clear();
  m_imageNames.clear();
}

void SpriteSheetData::addImage(const Image& image, const QString& name) {
  m_images[name] = image;
  m_imageNames.push_back(name); 
}

bool SpriteSheetData::loadImage(const QString& fileName, const QString& name) {
  QImage image(fileName); 
  if (image.isNull()) {
    return false;
  } 
  image.convertToFormat(QImage::Format_ARGB32);
  addImage(Image(image, name), fileName);
  return true;
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

void SpriteSheetData::save(QDomDocument& doc, QDomElement& root) {
  // Export images
  QDomElement images = doc.createElement("images");
  for (std::vector<QString>::iterator i = m_imageNames.begin(); i != m_imageNames.end(); ++i) {
    QDomElement image = doc.createElement("image");
    image.setAttribute("fileName", *i);
    image.setAttribute("shortName", m_images[*i].getName());
    images.appendChild(image);
  }  
  root.appendChild(images);
  // Export behaviors
  QDomElement behaviors = doc.createElement("behaviors");
  for (std::vector<QString>::iterator i = m_behaviorNames.begin(); i != m_behaviorNames.end(); ++i) {
    QDomElement behavior = doc.createElement("behavior");
    behavior.setAttribute("name", *i);
    for (std::vector<QString>::iterator j = m_behaviors[*i].begin(); j != m_behaviors[*i].end(); ++j) {
      QDomElement frame = doc.createElement("frame");
      frame.setAttribute("image", *j);
      behavior.appendChild(frame);
    }
    behaviors.appendChild(behavior);
  }  
  root.appendChild(behaviors);
}

bool SpriteSheetData::load(QDomDocument& doc, QDomNode& root) {
  // Extract images and behaviors
  QDomNode images = root.namedItem("images");
  if (images.isNull()) {
    return false;
  }
  QDomNode behaviors = root.namedItem("behaviors");
  if (behaviors.isNull()) {
    return false;
  }

  // Load all images
  QDomNode imageNode = images.firstChild();
  while( !imageNode.isNull() )
  {
    QDomElement e = imageNode.toElement();
    if( e.tagName() != "image" )
    {
      return false;
    }
    QString fileName = e.attribute("fileName");
    QString shortName = e.attribute("shortName");
    if (!loadImage(fileName, shortName)) {
      return false;
    }
    imageNode = imageNode.nextSibling();
  }
  
  // Load all behaviors
  QDomNode behaviorNode = behaviors.firstChild();
  while( !behaviorNode.isNull() )
  {
    QDomElement e = behaviorNode.toElement();
    if( e.tagName() != "behavior" )
    {
      return false;
    }
    QString name = e.attribute("name");
    m_behaviors[name] = Behavior();
    m_behaviorNames.push_back(name);
    QDomNode frameNode = behaviorNode.firstChild();
    while ( !frameNode.isNull() ) {
      QDomElement e2 = frameNode.toElement();
      if ( e2.tagName() != "frame") {
        return false;
      }
      QString imageName = e2.attribute("image");
      m_behaviors[name].push_back(imageName);
      frameNode = frameNode.nextSibling();
    }

    behaviorNode = behaviorNode.nextSibling();
  }
  return true;
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

  QDomElement sprites = doc.createElement("sprites");
  
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
    sprites.appendChild(sprite);
  }
  root.appendChild(sprites);

  // Export behaviors
  QDomElement behaviors = doc.createElement("behaviors");
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
    behaviors.appendChild(behavior);
  }
  root.appendChild(behaviors);

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
