#ifndef BEHAVIOR_LIST_MODEL_INCLUDED__
#define BEHAVIRO_LIST_MODEL_INCLUDED__

#include "imageListModel.h"
  
class BehaviorListModel : public QAbstractListModel {
public:
  BehaviorListModel(ImageListModel* f_ilm, QObject* parent = 0);
  virtual int rowCount(const QModelIndex& parent = QModelIndex() ) const;
  virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
  virtual Qt::DropActions supportedDropActions() const;
  virtual QStringList mimeTypes() const;
  virtual bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent);

private:
  ImageListModel* m_imageListModel;
  std::vector<QString> m_imageNames;
};

#endif
