#include "behaviorListModel.h"

BehaviorListModel::BehaviorListModel(ImageListModel* f_ilm, QObject* parent) : QAbstractListModel(parent), m_imageListModel(f_ilm) {
}

int BehaviorListModel::rowCount(const QModelIndex& parent) const {
  return m_imageNames.size();
}

QVariant BehaviorListModel::data(const QModelIndex& index, int role) const {
  if (!index.isValid()) {
    return QVariant();
  }
  switch(role) {
    case Qt::DecorationRole:
      return QVariant(m_imageListModel->getThumbnail(m_imageNames[index.row()]));
    case Qt::ToolTipRole:
      return QVariant(m_imageNames[index.row()]);
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

