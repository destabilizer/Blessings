#pragma once

namespace blessings {
  class Resolution {
  public:
    int width;
    int height;

    Resolution(int w=0, int h=0) : width(w), height(h) {}
  };

  struct GridPos {
    int x;
    int y;

    GridPos(int X=0, int Y=0) : x(X), y(Y) {};
  };

  inline bool operator==(const Resolution& a, const Resolution& b) {
    return a.width==b.width && a.height==b.height;
  }

  inline bool operator!=(const Resolution& a, const Resolution& b) {
    return a.width!=b.width || a.height!=b.height;
  }

  inline bool operator<(const Resolution& a, const Resolution& b) {
    return a.width<b.width && a.height<b.height;
  }

  inline bool operator<=(const Resolution& a, const Resolution& b) {
    return a.width<=b.width && a.height<=b.height;
  }

  inline bool operator>(const Resolution& a, const Resolution& b) {
    return a.width>b.width && a.height>b.height;
  }

  inline bool operator>=(const Resolution& a, const Resolution& b) {
    return a.width>=b.width && a.height>=b.height;
  }

  inline bool operator==(const GridPos& a, const GridPos& b) {
    return a.x==b.x && a.y==b.y;
  }

  inline bool operator!=(const GridPos& a, const GridPos& b) {
    return a.x!=b.x || a.y!=b.y;
  }

  inline GridPos operator+(const GridPos& a, const GridPos& b) {
    return GridPos(a.x+b.x, a.y+b.y);
  }
}
