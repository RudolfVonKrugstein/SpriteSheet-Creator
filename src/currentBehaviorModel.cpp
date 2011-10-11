#include "currentBehaviorModel.h"

CurrentBehaviorModel::CurrentBehaviorModel(SpriteSheetData& f_data, QObject* parent) : QAbstractListModel(parent), m_data(f_data), m_behaviorListView(0){
}
  
int CurrentBehaviorModel::rowCount(const QModelIndex& parent) const {
  if  ( m_behaviorListView->selectionModel()->selectedIndexes().count() == 0) {
    return 0;
  }
  int l_selectedBehavior = m_behaviorListView->selectionModel()->selectedIndexes().front().row();
  return m_data.m_behaviors[m_data.m_behaviorNames[l_selectedBehavior]].size();
}
QVariant CurrentBehaviorModel::data(const QModelIndex& index, int role) const {
  if (!index.isValid()) {
    return QVariant();
  }
  if  ( m_behaviorListView->selectionModel()->selectedIndexes().count() == 0) {
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

void CurrentBehaviorModel::onDataChanged() {
  if  ( m_behaviorListView->selectionModel()->selectedIndexes().count() == 0) {
    return;
  }
  int l_selectedBehavior = m_behaviorListView->selectionModel()->selectedIndexes().front().row();
  size_t l_numFrames = m_data.m_behaviors[m_data.m_behaviorNames[l_selectedBehavior]].size();
  emit dataChanged(createIndex(0,0), createIndex(l_numFrames, 1)); 
}

Qt::ItemFlags CurrentBehaviorModel::flags(const QModelIndex &index) const
{
  if (index.isValid())
    return (Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled);

  if  ( m_behaviorListView->selectionModel()->selectedIndexes().count() == 0) {
    return 0;
  }
  return Qt::ItemIsDropEnabled;
}

bool CurrentBehaviorModel::dropMimeData(const QMimeData *data, Qt::DropAction action,
    int row, int column, const QModelIndex &parent) {
  if  ( m_behaviorListView->selectionModel()->selectedIndexes().count() == 0) {
    return false;
  }
  int selected = m_behaviorListView->selectionModel()->selectedIndexes().first().row();
  if (action == Qt::IgnoreAction) {
    return true;
  }
  if (!data->hasFormat("application/image.name")) {
    return false;
  }
  if (column > 0) {
    return false;
  }

  if (!parent.isValid()) {
    if (row < 0) {
      row = m_data.m_behaviors[m_data.m_behaviorNames[selected]].size();
    } else {
      row = qMin(row, (int)m_data.m_behaviors[m_data.m_behaviorNames[selected]].size());
    }
  }

  QByteArray encodedData = data->data("application/image.name");
  QDataStream stream(&encodedData, QIODevice::ReadOnly);

  while (!stream.atEnd()) {
    QString name;
    stream >> name;;

    beginInsertRows(QModelIndex(), row, row);
    m_data.m_behaviors[m_data.m_behaviorNames[selected]].insert(row, name);
    endInsertRows();
    ++row;
  }
  return true;
}

QStringList CurrentBehaviorModel::mimeTypes() const {
  QStringList types;
  types << "application/image.name";
  return types;
}

Qt::DropActions CurrentBehaviorModel::supportedDropActions() const
{
      return Qt::CopyAction;// | Qt::MoveAction;
}
