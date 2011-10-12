#ifndef BEHAVIOR_H_INCLUDED__
#define BEHAVIOR_H_INCLUDED__
#include <QtGui>

typedef QString Frame;

class Behavior {
public:
  Behavior();
  std::vector<Frame> m_frames;
  QString m_name;
  int m_frameRate;
  bool m_looped; 
  Frame& operator[](size_t index);
  size_t size();
  void push_back(const Frame& f);
  void pop_back();
  void insert(int pos, const Frame& f);
  void remove(int pos);
  void erase(const Frame& f);
  void moveFrame(int srcPos, int dstPos);
};

#endif
