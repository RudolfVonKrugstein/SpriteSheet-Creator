#ifndef CURRENT_BEHAVIOR_INCLUDED__
#define CURRENT_BEHAVIOR_INCLUDED__

#include "data.h"

class CurrentBehaviorModel : public QAbstractListModel {
public:
  CurrentBehaviorModel(SpriteSheetData& f_data, QObject* parent = 0);
  inline void setBehaviorList(QListView* f_list) {m_behaviorListView = f_list;};
  
  virtual int rowCount(const QModelIndex& parent = QModelIndex() ) const;
  virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;

private:
  SpriteSheetData& m_data;
  QListView* m_behaviorListView;
};

#endif
