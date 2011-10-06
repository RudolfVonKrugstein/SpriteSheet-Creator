#include "behaviorListModel.h"

BehaviorListModel::BehaviorListModel(SpriteSheetData& f_data, QObject* parent) : QAbstractListModel(parent), m_data(f_data) {
}

int BehaviorListModel::rowCount(const QModelIndex& parent) const {
  return m_data.m_behaviorNames.size();
}

QVariant BehaviorListModel::data(const QModelIndex& index, int role) const {
  if (!index.isValid()) {
    return QVariant();
  }
  switch(role) {
    case Qt::DisplayRole:
      return m_data.m_behaviorNames[index.row()];
  }
  return QVariant();
}

QStringList BehaviorListModel::mimeTypes() const {
  QStringList types;
  types << "application/image.name";
   return types;
}

Qt::DropActions BehaviorListModel::supportedDropActions() const {
  return Qt::CopyAction;
}

bool BehaviorListModel::dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) {
  return true;  
}

