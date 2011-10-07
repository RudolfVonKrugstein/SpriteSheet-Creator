#ifndef BEHAVIOR_LIST_MODEL_INCLUDED__
#define BEHAVIRO_LIST_MODEL_INCLUDED__

#include "data.h"
  
class BehaviorListModel : public QAbstractListModel {
public:
  BehaviorListModel(SpriteSheetData& f_data, QObject* parent = 0);
  virtual int rowCount(const QModelIndex& parent = QModelIndex() ) const;
  virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
  virtual Qt::DropActions supportedDropActions() const;
  virtual QStringList mimeTypes() const;
  virtual bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent);

  void onDataChanged();
private:
  SpriteSheetData& m_data;
};

#endif
