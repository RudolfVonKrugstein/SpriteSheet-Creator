#ifndef BEHAVIOR_LIST_MODEL_INCLUDED__
#define BEHAVIRO_LIST_MODEL_INCLUDED__

#include "data.h"
  
class BehaviorListModel : public QAbstractTableModel {
public:
  BehaviorListModel(SpriteSheetData& f_data, QObject* parent = 0);
  virtual int rowCount(const QModelIndex& parent = QModelIndex() ) const;
  virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
  virtual bool setData(const QModelIndex& index,const QVariant& value, int role = Qt::DisplayRole);
  virtual int columnCount(const QModelIndex& parent = QModelIndex() ) const;
  QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
  void rowInserted(int row);
  Qt::ItemFlags flags(const QModelIndex & index) const;
  void addBehavior(const Behavior f_b);
  void deleteBehavior(const int index);

  void onDataChanged();
  void onReset();
private:
  SpriteSheetData& m_data;
};

#endif
