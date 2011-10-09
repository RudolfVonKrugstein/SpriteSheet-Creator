#ifndef CURRENT_BEHAVIOR_INCLUDED__
#define CURRENT_BEHAVIOR_INCLUDED__

#include "data.h"

class CurrentBehaviorModel : public QAbstractListModel {
public:
  CurrentBehaviorModel(SpriteSheetData& f_data, QObject* parent = 0);
  inline void setBehaviorList(QTableView* f_list) {m_behaviorListView = f_list;};
  
  virtual int rowCount(const QModelIndex& parent = QModelIndex() ) const;
  virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;

  void onDataChanged();
private:
  SpriteSheetData& m_data;
  QTableView* m_behaviorListView;
};

#endif
