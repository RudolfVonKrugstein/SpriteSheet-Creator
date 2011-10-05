#include "image.h"

Image::Image() {}
  
Image::Image(QImage& f_image) : m_anchor(0,0), m_image(f_image) {
  QImage tmp = f_image.scaled(64, 64);
  m_thumbnail = QPixmap::fromImage(tmp);
  autoCropImage();
}

int Image::width(bool f_cropped) {
  if (f_cropped) {
    return m_croppedImage.width();
  } else {
    return m_image.width();
  }
}

int Image::height(bool f_cropped) {
  if (f_cropped) {
    return m_croppedImage.height();
  } else {
    return m_image.height();
  }
}

void Image::autoCropImage() {
  QRect l_rect(0, 0, m_image.width(), m_image.height());
  // Cut off from the left
  bool l_nonTranspFound = false;
  while (!l_nonTranspFound) {
    for (int i = 0; i < m_image.height(); ++i) {
      if (qAlpha(m_image.pixel(l_rect.x(), i)) != 0) {
        l_nonTranspFound = true;
        break;
      }
    }
    if (!l_nonTranspFound) l_rect.setX(l_rect.x() +1);
  }
  // Cut off from the above
  l_nonTranspFound = false;
  while (!l_nonTranspFound) {
    for (int i = 0; i < m_image.width(); ++i) {
      if (qAlpha(m_image.pixel(i, l_rect.y())) != 0) {
        l_nonTranspFound = true;
        break;
      }
    }
    if (!l_nonTranspFound) l_rect.setY(l_rect.y() +1);
  }
  // Cut off from the right
  l_nonTranspFound = false;
  while (!l_nonTranspFound) {
    for (int i = 0; i < m_image.height(); ++i) {
      if (qAlpha(m_image.pixel(l_rect.width()-1, i)) != 0) {
        l_nonTranspFound = true;
        break;
      }
    }
    if (!l_nonTranspFound) l_rect.setWidth(l_rect.width() -1);
  }
  // Cut off from below
  l_nonTranspFound = false;
  while (!l_nonTranspFound) {
    for (int i = 0; i < m_image.width(); ++i) {
      if (qAlpha(m_image.pixel(i, l_rect.height()-1)) != 0) {
        l_nonTranspFound = true;
        break;
      }
    }
    if (!l_nonTranspFound) l_rect.setHeight(l_rect.height() -1);
  }

  m_croppedImage = m_image.copy(l_rect);
  m_croppedOffset = QPoint(l_rect.x(), l_rect.y());
}


