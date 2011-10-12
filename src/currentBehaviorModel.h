#ifndef CURRENT_BEHAVIOR_INCLUDED__
#define CURRENT_BEHAVIOR_INCLUDED__

#include "data.h"

class CurrentBehaviorModel : public QAbstractListModel {
public:
  CurrentBehaviorModel(SpriteSheetData& f_data, QObject* parent = 0);
  inline void setBehaviorList(QTableView* f_list) {m_behaviorListView = f_list;};
  
  virtual int rowCount(const QModelIndex& parent = QModelIndex() ) const;
  virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;

  bool dropMimeData(const QMimeData *data, Qt::DropAction action,
      int row, int column, const QModelIndex &parent);
  QStringList mimeTypes() const;
  virtual QMimeData *mimeData(const QModelIndexList &indexes) const;
  Qt::DropActions supportedDropActions() const;
  Qt::DropActions supportedDragActions() const;
  Qt::ItemFlags flags(const QModelIndex &index) const;
  virtual bool removeRows(int row, int count, const QModelIndex &parent);

  void onDataChanged();
  void testSelectedBehaviorChanged();
private:
  // Behavior selected on the last testSelectedBehaviroChanged
  int m_selectedBehavior;

  SpriteSheetData& m_data;
  QTableView* m_behaviorListView;
};

#endif
