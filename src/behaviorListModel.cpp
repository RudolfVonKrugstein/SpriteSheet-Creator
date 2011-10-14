#include "behaviorListModel.h"

BehaviorListModel::BehaviorListModel(SpriteSheetData& f_data, QObject* parent) : QAbstractTableModel(parent), m_data(f_data) {
}

int BehaviorListModel::rowCount(const QModelIndex& parent) const {
  return m_data.m_behaviorNames.size();
}

QVariant BehaviorListModel::data(const QModelIndex& index, int role) const {
  if (!index.isValid()) {
    return QVariant();
  }
  switch (index.column()) {
    case 0:
      if (role == Qt::EditRole || role == Qt::DisplayRole) return QVariant(m_data.m_behaviorNames[index.row()]);
      break;
    case 1:
      if (role == Qt::EditRole || role == Qt::DisplayRole) return QVariant(m_data.m_behaviors.find(m_data.m_behaviorNames[index.row()])->second.m_frameRate);
      break;
    case 2:
      if (role == Qt::CheckStateRole) return QVariant(m_data.m_behaviors.find(m_data.m_behaviorNames[index.row()])->second.m_looped);
      if (role == Qt::DisplayRole) return QVariant(m_data.m_behaviors.find(m_data.m_behaviorNames[index.row()])->second.m_looped?QString("yes"):QString("no"));
      break;
  }
  return QVariant();
}

Qt::ItemFlags BehaviorListModel::flags(const QModelIndex & index) const {
  Qt::ItemFlags f_flags = QAbstractTableModel::flags(index) | Qt::ItemIsSelectable;
  switch (index.column()) {
    case 0:
      f_flags |= Qt::ItemIsEditable;
      break;
    case 1:
      f_flags |= Qt::ItemIsEditable;
      break;
    case 2:
      f_flags |= Qt::ItemIsUserCheckable;
      break;
  }
  return f_flags;
}

void BehaviorListModel::onDataChanged() {
  emit dataChanged(createIndex(0,0), createIndex(m_data.m_behaviorNames.size(),4)); 
}

void BehaviorListModel::onReset() {
	emit reset();
}

bool BehaviorListModel::setData(const QModelIndex& index, const QVariant& value, int role) {
  if (!index.isValid()) {
    return false;
  }
  switch (index.column()) {
    case 0:
      if (role == Qt::EditRole) m_data.renameBehavior(index.column(), value.value<QString>());
      break;
    case 1:
      if (role == Qt::EditRole) m_data.m_behaviors[m_data.m_behaviorNames[index.row()]].m_frameRate = std::max(value.value<int>(), 1);
      break;
    case 2:
      if (role == Qt::CheckStateRole) m_data.m_behaviors[m_data.m_behaviorNames[index.row()]].m_looped = value.value<bool>();
      break;
  }
}

int BehaviorListModel::columnCount(const QModelIndex& parent) const {
  return 3;
}

void BehaviorListModel::addBehavior(const Behavior b) {
  beginInsertRows(QModelIndex(), m_data.m_behaviorNames.size(), m_data.m_behaviorNames.size());
  m_data.addBehavior(b);
  endInsertRows();
}

void BehaviorListModel::deleteBehavior(const int index) {
  beginRemoveRows(QModelIndex(), index, index);
  m_data.deleteBehavior(index);
  endRemoveRows();
}


QVariant BehaviorListModel::headerData(int section, Qt::Orientation orientation, int role) const {
  if (role != Qt::DisplayRole)
    return QVariant();
  if (orientation == Qt::Vertical) {
    return QVariant();
  }
  if (role != Qt::DisplayRole) {
    return QVariant();
  }
  switch (section) {
    case 0:
      return QVariant(QString("Name"));
    case 1:
      return QVariant(QString("Framerate"));
    case 2:
      return QVariant(QString("Looped"));
  }
  return QVariant();
}
