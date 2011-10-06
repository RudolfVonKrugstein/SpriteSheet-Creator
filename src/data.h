#ifndef DATA_H_INCLUED__
#define DATA_H_INCLUED__

#include <QtGui>
#include "image.h"
#include <QtXml/QDomDocument>

typedef std::vector<QString> Behavior;
// Data class for all spritesheet data
class SpriteSheetData {
public:
  void addImage(const Image image, const QString& name);
  const QPixmap getThumbnail(const QString& name) const;
  const QString getShortName(const QString& name) const;
  void getImagePointerList(std::list<Image*>& f_list);
  void recreatePackedTexture(bool f_autocrop);
  void exportXML(const QString outDir, const QString xmlFile, const QString pngFile, bool f_autocrop, QWidget* f_parent);
  void save(QDomDocument& doc, QDomElement& root);
  void load(QDomDocument& doc, QDomElement& root);

  std::map<QString, Image>    m_images;
  std::vector<QString>        m_imageNames;
  std::map<QString, Behavior> m_behaviors;
  std::vector<QString>        m_behaviorNames;

  QImage m_outImage;
};

#endif
