# inspired by http://www.iguanademos.com/Jare/Articles.php?view=RectPlace

class Rect:
  def __init__(self):
    self.x = 0
    self.y = 0
    self.width = 0
    self.height = 0

class RectStorage:
  def __init__(self):
    self.rects = []
    self.dimensions = (1,1)
    self.anchors = [(0,0)] # potential points to insert rectangles
  
  # increase the dimensions of the containing rectangle
  def increaseDimensions(self):
    if (self.dimensions[1] < self.dimensions[2]):
      self.dimensions[1] = self.dimensions[1] * 2;
    else:
      self.dimensions[2] = self.dimensions[2] * 2;

  def insertRect(self, rect):
    # try to insert rectangle at any anchor point
    for a in self.anchors:
      # set the position of the rect
      (rect.x, rect.y) = a
      if self.fits(rect):
        # found something
        self.rects.append(rect)
        self.anchors.remove(a)
        self.insertAnchor( (a[0], a[1] + rect.height) )
        self.insertAnchor( (a[0] + rect.width, a[1]) )
        return True
    # end of loop, found nothing
    return False
  
  def fits(self, rect):
    for i in xrange(len(self.rects)):
      if self.overlap(rect, self.rects[i]):
        return False
    return True

  def overlap(self, r1, r2):
    if r1.x >= r2.x + r2.width:
      return False
    if r1.y >= r2.y + r2.height:
      return False
    if r2.x >= r1.x + r1.width:
      return False
    if r2.y >= r1.y + r1.height:
      return False
    return True

  # crop to containing rectangles
  def crop(self):
    self.dimensions = (0,0)
    for r in self.rects:
      self.dimensions = (max(r.x + r.width, self.dimensions[0]), max(r.y + r.height, self.dimensions[1]))

  # insert anchor at good position
  def insertAnchor(self, anchor):
    for i in xrange(len(self.anchors)):
      if (anchor[0] + anchor[1] < self.anchors[i][0] + self.anchors[i][1]):
        self.anchors.insert(i, anchor)
        return
    #nothing found
    self.anchors.append(anchor)

  def packRectangles(self,rects):
    # sort rectangles by highed
    rects.sort(key = lambda rect: rect.height)
    rects.reverse()
    
    # insert all rects
    for r in rects:
      while not self.insertRect(r):
        self.increaseDimensions()
    
    # crop the storage
    self.crop()

