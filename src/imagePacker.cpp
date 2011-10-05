#include "imagePacker.h"

ImagePacker::ImagePacker(const std::list<Image*>& f_images, bool f_autoCrop) : m_untouchedImages(f_images), m_dimensions(1,1), m_autoCrop(f_autoCrop) {
  // First anchor
  m_anchors.push_back(QPoint(0,0)); 
}

// Increase one dimensions (the smaller one) by factor 2
void ImagePacker::increaseDimensions() {
  if (m_dimensions.width() < m_dimensions.height()) {
    m_dimensions.setWidth(m_dimensions.width() * 2);
  } else {
    m_dimensions.setHeight(m_dimensions.height() * 2);
  }
}

// Insert an untouched image into the images
bool ImagePacker::insertImage(Image* im) {
  for (std::list<QPoint>::iterator i = m_anchors.begin(); i != m_anchors.end(); ++i) {
    // Set the anchor and test if it fits
    im->m_anchor = *i;
    if (fits(im)) {
      m_insertedImages.push_back(im);
      m_anchors.erase(i);
      m_anchors.push_back(QPoint(im->m_anchor.x() + im->width(m_autoCrop), im->m_anchor.y()));
      m_anchors.push_back(QPoint(im->m_anchor.x(), im->m_anchor.y() + im->height(m_autoCrop)));
      return true;
    }
  }
  // Loop finished nothing found
  return false;
}

bool ImagePacker::fits(Image* im) {
  if (im->m_anchor.x() + im->width(m_autoCrop) > m_dimensions.width())
    return false;
  if (im->m_anchor.y() + im->height(m_autoCrop) > m_dimensions.height())
    return false;
  for (std::list<Image*>::iterator i = m_insertedImages.begin(); i != m_insertedImages.end(); ++i) {
    if (overlap(*i, im)) {
      return false;
    }
  }
  return true;
}

bool ImagePacker::overlap(Image* i1, Image* i2) {
  return QRect(i1->m_anchor.x(), i1->m_anchor.y(), i1->width(m_autoCrop), i1->height(m_autoCrop)).intersects(
      QRect(i2->m_anchor.x(), i2->m_anchor.y(), i2->width(m_autoCrop), i2->height(m_autoCrop))
  );
}

void ImagePacker::crop() {
  m_dimensions = QSize(0,0);
  for (std::list<Image*>::iterator i = m_insertedImages.begin(); i != m_insertedImages.end(); ++i) {
    m_dimensions = QSize(std::max(m_dimensions.width(), (*i)->m_anchor.x() + (*i)->width(m_autoCrop)),
        std::max(m_dimensions.height(), (*i)->m_anchor.y() + (*i)->height(m_autoCrop)));
  } 
}

void ImagePacker::insertAnchor(const QPoint anchor) {
  for (std::list<QPoint>::iterator i = m_anchors.begin(); i != m_anchors.end(); ++i) {
    if (i->x() + i->y() > anchor.x() + anchor.y()) {
      m_anchors.insert(i, anchor);
      return;
    }
  }
  m_anchors.push_back(anchor);
} 

// For sorting by height, biggest height first
bool g_autoCrop;
bool compareImagePointers(Image *i1, Image *i2) {
  return i1->height(g_autoCrop) > i2->height(g_autoCrop);
}

void ImagePacker::packImages() {
  g_autoCrop = m_autoCrop;
  // Sort by height
  m_untouchedImages.sort(compareImagePointers);
  for (std::list<Image*>::iterator i = m_untouchedImages.begin(); i != m_untouchedImages.end(); ++i) {
    while(!insertImage(*i)) {
      increaseDimensions();
    }
  }
}
