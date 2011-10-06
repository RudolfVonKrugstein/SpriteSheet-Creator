#ifndef CURRENT_BEHAVIOR_INCLUDED__
#define CURRENT_BEHAVIOR_INCLUDED__

#include "data.h"

class CurrentBehaviorModel : public QAbstractListModel {
  CurrentBehaviorModel(SpriteSheetData& f_data, QListView* f_bahviorListView, QObject* parent = 0);
  
  virtual int rowCount(const QModelIndex& parent = QModelIndex() ) const;
  virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;

private:
  SpriteSheetData& m_data;
  QListView* m_behaviorListView;
};

#endif
