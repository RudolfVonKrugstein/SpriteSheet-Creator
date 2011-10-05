#!/usr/bin/python

import sys
import os
from PIL import Image as PILImage
from PIL.ImageQt import ImageQt
from PyQt4 import QtCore, QtGui
from guidata.gui import Ui_MainWindow
from rectanglePacker import RectStorage
from rectanglePacker import Rect

def PILImageToQImage(pilimage):
  imageq = ImageQt(pilimage)
  qimage = QtGui.QImage(imageq)
  return qimage

class Image:
  def __init__(self, image):
    self.image = image
    self.x, self.y, self.width, self.height = 0, 0, image.size[0], image.size[1]
    # create thumbnail
    self.thumbnail = image.copy()
    self.thumbnail.thumbnail((32,32))
    self.thumbnail = PILImageToQImage(self.thumbnail)
    self.selectedThumbnail = self.thumbnail
    self.thumbnail = QtGui.QPixmap.fromImage(self.thumbnail)
    self.selectedThumbnail.invertPixels()
    self.selectedThumbnail = QtGui.QPixmap.fromImage(self.selectedThumbnail)

class ImageDelegate(QtGui.QItemDelegate):
  
  def __init__(self, parent = None):
    QtGui.QItemDelegate.__init__(self, parent)

  def getThumbnail(self, option, index):
    pixmap = index.data(QtCore.Qt.DisplayRole)
    pixmap = pixmap.toPyObject()
    if option.state & QtGui.QStyle.State_Selected:
      return pixmap.selectedThumbnail
    else:
      return pixmap.thumbnail
    
  def paint(self, painter, option, index):
    painter.save()
    p = self.getThumbnail(option, index)
    painter.drawPixmap(option.rect.topLeft(), p)
    option.rect = option.rect.translated(p.width(), 0)
    QtGui.QItemDelegate.paint(self, painter, option, index)
    painter.restore()

  def sizeHint(self, option, index):
    p = self.getThumbnail(option, index)
    return p.size()

class ImageListModel(QtCore.QAbstractListModel):
  def __init__(self, parent = None, *args):
    QtCore.QAbstractListModel.__init__(self, parent, *args)
    self.images = {}
    self.imageNames = []

  def addImage(self, image, name):
    self.images[name] = image
    self.imageNames.append(name)
  
  def rowCount(self, parent=QtCore.QModelIndex()):
    return len(self.imageNames)

  def data(self, index, role):
    if index.isValid() and role == QtCore.Qt.DisplayRole:
      return QtCore.QVariant(self.images[self.imageNames[index.row()]])
    else:
      return QtCore.QVariant()

  def flags(self, index):
    return QtCore.QAbstractListModel.flags(self, index) | QtCore.Qt.ItemIsDragEnabled

  def onDataChanged(self):
    self.dataChanged.emit(self.createIndex(0,0), self.createIndex(len(self.images),1))

  def mimeTypes(self):
    return ["application/vnd.text.list"]

class BehaviorListModel(QtCore.QAbstractListModel):
  def __init__(self, imageListModel, parent = None, *args):
    QtCore.QAbstractListModel.__init__(self, parent, *args)
    self.imageListModel = imageListModel
    self.imageNames = []

  def addImage(self, name):
    self.imageNames.append(name)
  
  def rowCount(self, parent = QtCore.QModelIndex()):
    return len(self.imageNames)
  
  def data(self, index, role):
    if index.isValid() and role == QtCore.Qt.DisplayRole:
      return QtCore.QVariant(self.imageListModel.images[self.imageNames[index.row()]])
    else:
      return QtCore.QVariant()

  def mimeTypes(self):
    return ["application/vnd.text.list"]

  def dropMimeData(self, data, action, row, column, parent = None):
    print "dropMimeData"
  

class MainWindow(QtGui.QMainWindow):
  def __init__(self, parent=None):
    QtGui.QWidget.__init__(self, parent)
    self.ui = Ui_MainWindow()
    self.ui.setupUi(self)
    
    # set the list model
    self.imageModel = ImageListModel()
    self.ui.imageList.setModel(self.imageModel)
    self.itemDelegate = ImageDelegate()
    self.ui.imageList.setItemDelegate(self.itemDelegate)
    # also for current behavior
    self.behaviorModel = BehaviorListModel(self.imageModel)
    self.ui.currentBehavior.setModel(self.behaviorModel)
    self.ui.currentBehavior.setItemDelegate(self.itemDelegate)

    # create context menu for image list
    self.ui.imageList.setContextMenuPolicy(QtCore.Qt.ActionsContextMenu)
    self.ui.imageList.addAction(self.ui.actionDelete_Image)
    
    # enable drag and drop
    self.ui.imageList.setDragEnabled(True)
    self.ui.currentBehavior.setAcceptDrops(True)
    self.ui.currentBehavior.setDropIndicatorShown(True)
    self.ui.currentBehavior.setDragDropMode(QtGui.QAbstractItemView.DropOnly)

    # connect actions
    self.connect(self.ui.actionImport_Images, QtCore.SIGNAL('triggered()'), self.importImages)
    self.connect(self.ui.imageList.selectionModel(), QtCore.SIGNAL("selectionChanged(QItemSelection, QItemSelection)"), self.imageListSelectionChanged)
    self.ui.actionDelete_Image.setEnabled(False)

  def importImages(self):
    images = QtGui.QFileDialog.getOpenFileNames(self, "Select images to import", "", "Images (*.png *.xpm *.jpg)")
    for i in images:
      self.importImage(i)
    #self.imageModel.emit(QtCore.SIGNAL("dataChanged(const QModelIndex &, const QModelIndex &)"), 1, 0)
    self.imageModel.onDataChanged()
    self.recreatePackedTexture()

  def imageListSelectionChanged(self, newSel, oldSel):
    if newSel.count() == 0:
      self.ui.actionDelete_Image.setEnabled(False)
    else:
      self.ui.actionDelete_Image.setEnabled(True)

  def importImage(self, fileName):
    try:
      image = PILImage.open(str(fileName))
      image.load()
      image = image.convert("RGBA")
    except:
      QtGui.QMessageBox.warning(self, "Unable to load image", "Unable to load " + fileName)
      return
    self.imageModel.addImage(Image(image), os.path.basename(str(fileName)))

  def recreatePackedTexture(self):
    #create list of images
    imageRects = []
    for k,v in self.imageModel.images.iteritems():
      imageRects.append(v)
    rs = RectStorage()
    rs.packRectangles(imageRects)
    outImage = PILImage.new("RGBA", rs.dimensions) 
    for ri in rs.rects:
      outImage.paste(ri.image, (ri.x, ri.y, ri.x + ri.width, ri.y + ri.height))
    qimage = PILImageToQImage(outImage)
    pixmap = QtGui.QPixmap(qimage)
    self.ui.outTexture.setPixmap(pixmap)
    

if __name__ == "__main__":
  app = QtGui.QApplication(sys.argv)
  window = MainWindow()
  window.show()
  sys.exit(app.exec_())

