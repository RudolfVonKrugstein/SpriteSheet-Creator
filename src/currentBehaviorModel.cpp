#include "currentBehaviorModel.h"

CurrentBehaviorModel::CurrentBehaviorModel(SpriteSheetData& f_data, QObject* parent) : QAbstractListModel(parent), m_data(f_data), m_behaviorListView(0){
	m_selectedBehavior = -1;
}
  
int CurrentBehaviorModel::rowCount(const QModelIndex& parent) const {
  if  ( m_selectedBehavior == -1) {
    return 0;
  }
  return m_data.m_behaviors[m_data.m_behaviorNames[m_selectedBehavior]].size();
}
QVariant CurrentBehaviorModel::data(const QModelIndex& index, int role) const {
  if (!index.isValid()) {
    return QVariant();
  }
  if  ( m_selectedBehavior == -1) {
    return QVariant();
  }
  switch(role) {
  case Qt::DisplayRole:
    return QVariant();
  case Qt::DecorationRole:
    return m_data.m_images[m_data.m_behaviors[m_data.m_behaviorNames[m_selectedBehavior]][index.row()]].m_thumbnail;
  case Qt::ToolTipRole:
    return m_data.m_images[m_data.m_behaviors[m_data.m_behaviorNames[m_selectedBehavior]][index.row()]].getName();
  }
  return QVariant();
}

void CurrentBehaviorModel::testSelectedBehaviorChanged() {
  int l_newSelectedBehavior = -1;
  if  ( m_behaviorListView->selectionModel()->selectedIndexes().count() != 0) {
	l_newSelectedBehavior = m_behaviorListView->selectionModel()->selectedIndexes().first().row();
  }
  if (l_newSelectedBehavior != m_selectedBehavior) {
    m_selectedBehavior = l_newSelectedBehavior;
	emit reset();
  }
}

void CurrentBehaviorModel::onDataChanged() {
  if  (m_selectedBehavior == -1) {
    return;
  }
  size_t l_numFrames = m_data.m_behaviors[m_data.m_behaviorNames[m_selectedBehavior]].size();
  emit dataChanged(createIndex(0,0), createIndex(l_numFrames, 1)); 
}

Qt::ItemFlags CurrentBehaviorModel::flags(const QModelIndex &index) const
{
  if (index.isValid()) {
    return (Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled);
  }

  if  ( m_selectedBehavior == -1) {
    return 0;
  }
  return Qt::ItemIsDropEnabled;
}

bool CurrentBehaviorModel::removeRows(int row, int count, const QModelIndex& parent) {
  if  ( m_selectedBehavior == -1) {
    return false; // Nothing in the listview
  }

  if (parent.isValid()) {
    return false; // This can not be in a list view
  }
  if (row >= m_data.m_behaviors[m_data.m_behaviorNames[m_selectedBehavior]].size()) {
    return false; // Non existing row
  }
  // Get range
  size_t numElements = m_data.m_behaviors[m_data.m_behaviorNames[m_selectedBehavior]].size();
  int beginRow = qMax(0, row);
  int endRow = qMin(row + count, (int)numElements);
  count = endRow - beginRow;

  beginRemoveRows(parent, beginRow, endRow);
  for (size_t i = beginRow; i < numElements - count; ++i) {
	  m_data.m_behaviors[m_data.m_behaviorNames[m_selectedBehavior]][i] =
		  m_data.m_behaviors[m_data.m_behaviorNames[m_selectedBehavior]][i+count];
  }
  for (size_t i = 0; i < count; ++i) {
    m_data.m_behaviors[m_data.m_behaviorNames[m_selectedBehavior]].pop_back();
  }
  endRemoveRows();
  return true;
}

bool CurrentBehaviorModel::dropMimeData(const QMimeData *data, Qt::DropAction action,
    int row, int column, const QModelIndex &parent) {
  if  ( m_selectedBehavior == -1) {
    return false;
  }
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
      row = m_data.m_behaviors[m_data.m_behaviorNames[m_selectedBehavior]].size();
    } else {
      row = qMin(row, (int)m_data.m_behaviors[m_data.m_behaviorNames[m_selectedBehavior]].size());
    }
  }

  QByteArray encodedData = data->data("application/image.name");
  QDataStream stream(&encodedData, QIODevice::ReadOnly);

  while (!stream.atEnd()) {
    QString name;
    stream >> name;;

    beginInsertRows(QModelIndex(), row, row);
    m_data.m_behaviors[m_data.m_behaviorNames[m_selectedBehavior]].insert(row, name);
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

QMimeData *CurrentBehaviorModel::mimeData(const QModelIndexList &indexes) const {
  if  ( m_selectedBehavior == -1) {
    return 0;
  }

  QMimeData *mimeData = new QMimeData();
  QByteArray encodedData;

  QDataStream stream(&encodedData, QIODevice::WriteOnly);

  foreach (QModelIndex index, indexes) {
    if (index.isValid()) {
	  QString text = m_data.m_behaviors[m_data.m_behaviorNames[m_selectedBehavior]][index.row()];
	  stream << text;
    }
  }
  mimeData->setData("application/image.name", encodedData);
  return mimeData;
}


Qt::DropActions CurrentBehaviorModel::supportedDragActions() const
{
      return Qt::MoveAction;
}

Qt::DropActions CurrentBehaviorModel::supportedDropActions() const
{
      return Qt::CopyAction | Qt::MoveAction;
}
