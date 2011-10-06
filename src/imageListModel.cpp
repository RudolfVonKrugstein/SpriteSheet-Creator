#include "imageListModel.h"

ImageListModel::ImageListModel(SpriteSheetData& f_data, QObject* parent) : QAbstractListModel(parent), m_data(f_data) {
}


int ImageListModel::rowCount(const QModelIndex& parent) const {
  return m_data.m_imageNames.size();
}

Qt::DropActions ImageListModel::supportedDragActions() const {
  return Qt::CopyAction;
}

QVariant ImageListModel::data(const QModelIndex& index, int role) const {
  if (index.isValid()) {
    switch(role) {
    case Qt::DecorationRole:
      return QVariant(m_data.getThumbnail(m_data.m_imageNames[index.row()]));
    case Qt::DisplayRole:
        return QVariant(m_data.getShortName(m_data.m_imageNames[index.row()]));
    case Qt::ToolTipRole:
      return QVariant(m_data.m_imageNames[index.row()]);
    default:
      return QVariant();
    }
  } else {
    return QVariant();
  }
}

Qt::ItemFlags ImageListModel::flags(const QModelIndex& index) const {
   Qt::ItemFlags defaultFlags = QAbstractListModel::flags(index);

   if (index.isValid())
     return Qt::ItemIsDragEnabled | defaultFlags;
   else
     return defaultFlags;
}

QStringList ImageListModel::mimeTypes() const
{
  QStringList types;
  types << "application/image.name";
  return types;
}

QMimeData *ImageListModel::mimeData(const QModelIndexList &indexes) const {
     QMimeData *mimeData = new QMimeData();
     QByteArray encodedData;

     QDataStream stream(&encodedData, QIODevice::WriteOnly);

     foreach (QModelIndex index, indexes) {
         if (index.isValid()) {
             QString text = m_data.m_imageNames[index.row()];
             stream << text;
         }
     }
     mimeData->setData("application/image.name", encodedData);
     return mimeData;
 }

void ImageListModel::onDataChanged() {
  emit dataChanged(createIndex(0,0), createIndex(m_data.m_imageNames.size(), 1)); 
}

