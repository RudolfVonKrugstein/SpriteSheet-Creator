#ifndef DATA_H_INCLUED__
#define DATA_H_INCLUED__

#include <QtGui>
#include "image.h"
#include "behavior.h"
#include <QtXml/QDomDocument>

// Data class for all spritesheet data
class SpriteSheetData {
public:
  void addImage(const Image& image, const QString& name);
  bool loadImage(const QString& fileName, const QString& name);
  const QPixmap getThumbnail(const QString& name) const;
  const QString getShortName(const QString& name) const;
  void getImagePointerList(std::list<Image*>& f_list);
  void recreatePackedTexture(bool f_autocrop);
  void exportXML(const QString& xmlFile, const QString pngFile, bool f_autocrop, QWidget* f_parent);
  void save(QDomDocument& doc, QDomElement& root);
  bool load(QDomDocument& doc, QDomNode& root);
  void clear();

  void deleteImage(int f_index);
  void appendImage(int f_index, int f_behaviorIndex);
  int addBehavior(Behavior b);
  void deleteBehavior(const int index);
  void moveFrameRight(int f_behaviorIndex, int f_frameIndex);
  void moveFrameLeft(int f_behaviorIndex, int f_frameIndex);

  void renameBehavior(int index, const QString newName);

  std::map<QString, Image>    m_images;
  std::vector<QString>        m_imageNames;
  std::map<QString, Behavior> m_behaviors;
  std::vector<QString>        m_behaviorNames;

  QImage m_outImage;

};

#endif
