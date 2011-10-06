#include "currentBehaviorModel.h"

CurrentBehaviorModel::CurrentBehaviorModel(SpriteSheetData& f_data, QListView* f_behaviorListView, QObject* parent) : QAbstractListModel(parent), m_data(f_data), m_behaviorListView(f_behaviorListView){
}
  
int CurrentBehaviorModel::rowCount(const QModelIndex& parent) const {
  int l_selectedBehavior = m_behaviorListView->selectionModel()->selectedIndexes().front().row();
  return m_data.m_behaviors[m_data.m_behaviorNames[l_selectedBehavior]].size();
}
QVariant CurrentBehaviorModel::data(const QModelIndex& index, int role) const {
  if (!index.isValid()) {
    return QVariant();
  }
  int l_selectedBehavior = m_behaviorListView->selectionModel()->selectedIndexes().front().row();
  switch(role) {
  case Qt::DisplayRole:
    return QVariant();
  case Qt::DecorationRole:
    return m_data.m_images[m_data.m_behaviors[m_data.m_behaviorNames[l_selectedBehavior]][index.row()]].m_thumbnail;
  case Qt::ToolTipRole:
    return m_data.m_images[m_data.m_behaviors[m_data.m_behaviorNames[l_selectedBehavior]][index.row()]].getName();
  }
  return QVariant();
}
