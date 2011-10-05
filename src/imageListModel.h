#ifndef IMAGE_LIST_MODEL_INCLUDED__
#define IMAGE_LIST_MODEL_INCLUDED__

#include <QAbstractListModel>
#include <map>
#include <vector>
#include "image.h"

class ImageListModel : public QAbstractListModel
{
public:
  ImageListModel(QObject* parent = 0);
  void addImage(const Image image, const QString& name);
  virtual int rowCount(const QModelIndex& parent = QModelIndex() ) const;
  virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
  virtual Qt::DropActions supportedDragActions() const;
  virtual Qt::ItemFlags flags(const QModelIndex &index) const;
  void onDataChanged();
  const QPixmap getThumbnail(const QString& name) const;
  virtual QStringList mimeTypes() const;
  virtual QMimeData *mimeData(const QModelIndexList &indexes) const;

  void getImagePointerList(std::list<Image*>& f_list);
private:
  std::map<QString, Image>  m_images;
  std::vector<QString>      m_imageNames;
};

#endif
