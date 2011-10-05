#ifndef IMAGE_INCLUDED__
#define IMAGE_INCLUDED__
#include <QtGui>


// Class for images including dimensions
class Image
{
public:
  Image();
  Image(QImage& image, const QString f_name);

  QPixmap m_thumbnail;
  int width(bool f_cropped);
  int height(bool f_cropped);

  QPoint m_anchor; // Anker postion in the final texture
  inline const QImage& getImage() const { return m_image;};
  inline const QImage& getCroppedImage() const { return m_croppedImage;};
  inline const QString& getName() const { return m_name;}
private:
  QImage m_image;
  QImage m_croppedImage;
  QPoint m_croppedOffset;
  QString m_name;
  void autoCropImage();
};

#endif
