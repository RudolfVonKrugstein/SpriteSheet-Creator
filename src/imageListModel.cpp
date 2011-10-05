#include "imageListModel.h"

ImageListModel::ImageListModel(QObject* parent) : QAbstractListModel(parent) {
}

void ImageListModel::addImage(const Image image, const QString& name) {
  m_images[name] = image;
  m_imageNames.push_back(name); 
}

int ImageListModel::rowCount(const QModelIndex& parent) const {
  return m_imageNames.size();
}

Qt::DropActions ImageListModel::supportedDragActions() const {
  return Qt::CopyAction;
}

const QPixmap ImageListModel::getThumbnail(const QString& name) const {
  return m_images.find(name)->second.m_thumbnail;
}

const QString ImageListModel::getShortName(const QString& name) const {
  return m_images.find(name)->second.getName();
}

QVariant ImageListModel::data(const QModelIndex& index, int role) const {
  if (index.isValid()) {
    switch(role) {
    case Qt::DecorationRole:
      return QVariant(getThumbnail(m_imageNames[index.row()]));
    case Qt::DisplayRole:
        return QVariant(getShortName(m_imageNames[index.row()]));
    case Qt::ToolTipRole:
      return QVariant(m_imageNames[index.row()]);
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
             QString text = m_imageNames[index.row()];
             stream << text;
         }
     }
     mimeData->setData("application/image.name", encodedData);
     return mimeData;
 }

void ImageListModel::onDataChanged() {
  emit dataChanged(createIndex(0,0), createIndex(m_imageNames.size(), 1)); 
}

void ImageListModel::getImagePointerList(std::list<Image*>& f_images) {
  for (std::map<QString, Image>::iterator i = m_images.begin(); i != m_images.end(); ++i) {
    f_images.push_back(&(i->second));
  }
}
