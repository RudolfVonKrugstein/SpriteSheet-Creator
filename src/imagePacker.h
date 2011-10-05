#ifndef IMAGE_PACKER_INCLUDED__
#define IMAGE_PACKER_INCLUDED__

#include "image.h"

class ImagePacker {
public:
  ImagePacker(const std::list<Image*>& f_images, bool f_autoCrop = true);

  // Pack the images, this changes the anchor positions of all images and the dimenions returned by getDim
  void packImages();

  inline QSize getDim() {return m_dimensions;};
private:
  // Increase contained dimension
  void increaseDimensions();
  // Try to insert an image
  bool insertImage(Image* im);
  // Test if an iamge fits
  bool fits(Image* im);
  // Test if two images overlap
  bool overlap(Image* i1, Image* i2);
  // Crop the texture
  void crop();
  // insert an anchor, where an image could be placed
  void insertAnchor(const QPoint anchor);

  std::list<Image*> m_untouchedImages;
  QSize m_dimensions;
  std::list<Image*> m_insertedImages;
  std::list<QPoint> m_anchors;
  bool m_autoCrop;
};

#endif
