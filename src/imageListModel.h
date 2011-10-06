#ifndef IMAGE_LIST_MODEL_INCLUDED__
#define IMAGE_LIST_MODEL_INCLUDED__

#include <QAbstractListModel>
#include <map>
#include <vector>
#include "data.h"

class ImageListModel : public QAbstractListModel
{
public:
  ImageListModel(SpriteSheetData& f_data, QObject* parent = 0);
  virtual int rowCount(const QModelIndex& parent = QModelIndex() ) const;
  virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
  virtual Qt::DropActions supportedDragActions() const;
  virtual Qt::ItemFlags flags(const QModelIndex &index) const;
  void onDataChanged();
  virtual QStringList mimeTypes() const;
  virtual QMimeData *mimeData(const QModelIndexList &indexes) const;

private:
  SpriteSheetData& m_data;
};

#endif
