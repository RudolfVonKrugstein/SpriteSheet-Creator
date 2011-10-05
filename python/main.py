#!/usr/bin/python

import os
import sys
from PIL import Image
from rectanglePacker import RectStorage
from rectanglePacker import Rect

def autocrop(image):
  image = image.convert("RGBA")
  bbox = image.split()[list(image.getbands()).index('A')].getbbox()
  return image.crop(bbox)

if len(sys.argv) != 2:
   raise Exception("Expected one argument, and one argument only")

imageRects = []

# load all images
for f in os.listdir(sys.argv[1]):
  if f.endswith(".png"):
    i = Image.open(sys.argv[1] + os.sep + f)
    i.load()
    i = i.convert("RGBA")
    r = Rect()
    r.image = i
    imageRects.append(r) 

# auto crop all images and set dimensions
for ri in imageRects:
  ri.image = autocrop(ri.image)
  ri.width, ri.height = ri.image.size

rs = RectStorage()
rs.packRectangles(imageRects)

# create a new image
outImage = Image.new("RGBA", rs.dimensions)

for ri in rs.rects:
  outImage.paste(ri.image, (ri.x, ri.y, ri.x + ri.width, ri.y + ri.height))

# save the image
outImage.save("texture.png")

