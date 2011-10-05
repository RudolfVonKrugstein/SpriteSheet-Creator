#ifndef IMAGE_INCLUDED__
#define IMAGE_INCLUDED__
#include <QtGui>


// Class for images including dimensions
class Image
{
public:
  Image();
  Image(QImage& image);

  QPixmap m_thumbnail;
  int width(bool f_cropped);
  int height(bool f_cropped);

  QPoint m_anchor; // Anker postion in the final texture
  inline const QImage& getImage() { return m_image;};
  inline const QImage& getCroppedImage() { return m_croppedImage;};
private:
  QImage m_image;
  QImage m_croppedImage;
  QPoint m_croppedOffset;
  void autoCropImage();
};

#endif
