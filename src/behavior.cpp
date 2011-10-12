#include "behavior.h"
#include <QtGui>

Behavior::Behavior() {
  m_looped = false;
  m_frameRate = 30;
}

size_t Behavior::size() {
  return m_frames.size();
}
 
Frame& Behavior::operator[](size_t index) {
  return m_frames[index];
}

void Behavior::push_back(const Frame& f) {
  m_frames.push_back(f);
}

void Behavior::pop_back() {
  m_frames.pop_back();
}

void Behavior::insert(int row, const Frame& f) {
  m_frames.push_back(f);
  for (size_t i = m_frames.size()-1; i != row; --i) {
    std::swap(m_frames[i], m_frames[i-1]);
  }
}

void Behavior::remove(int pos) {
	for (size_t i = pos; i < m_frames.size() -1; ++i) {
		m_frames[i] = m_frames[i+1];
	}
	m_frames.pop_back();
}

void Behavior::moveFrame(int srcPos, int dstPos) {
  // Moves the frame at srcPos before the frame at dstPos
  if (srcPos < dstPos) {
    for (int i = srcPos; i < dstPos-1; ++i) {
	  std::swap(m_frames[i], m_frames[i+1]);
	}		
  }
  if (srcPos > dstPos) {
    for(int i = srcPos; i > dstPos; --i) {
	  std::swap(m_frames[i], m_frames[i-1]);
	}
  }
}

void Behavior::erase(const Frame& f) {
  bool l_found = true;
  while(l_found) {
    l_found = false;
    size_t j;
    for (j = 0; j < m_frames.size(); ++j) {
      if (m_frames[j] == f) {
        l_found = true;
         break;
      }
    }
    if (l_found) {
      // Move all after j
      for(;j < m_frames.size()-1; ++j) {
        m_frames[j] = m_frames[j+1];
      }
    }
  }  
}
