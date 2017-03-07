#pragma once

#include <queue>

#include "additional_structs.hpp"
#include "terminal_io.hpp"

namespace blessings {
  template <typename InS, typename OutS, typename Prop>
  class Monitor {
  public:
    Monitor() {}
    Monitor(int MaxSize);
    ~Monitor();
    Monitor(const Monitor&);
    Monitor& operator=(const Monitor&);

    class Cell {
    private:
      OutS symb;
      Prop prop;
      bool unstaged = false;

    public:
      Cell() {}
      Cell(OutS s, Prop p) : symb(s), prop(p) {}
      Cell(OutS s) : symb(s), prop(Prop::empty) {}
      ~Cell() {}
      Cell(const Cell&);
      Cell& operator=(const Cell&);

      OutS symbol() {return symb;}
      Prop property() {return prop;}

      static bool hardopt;

      bool isUnstaged();

      void setStaged();
      void setUnstaged();
    };

    class Iterator {
    private:
      Monitor::Cell* grid;
      int pointer;
      int stopPos;

      bool comparable(const Iterator&);

    public:
      Iterator() {}
      Iterator(Cell* Grid, int ptr, int bound);
      //Iterator(const Iterator&);
      Iterator& operator=(const Iterator&);
      ~Iterator() {}

      Cell& operator*();

      Iterator& operator++();
      Iterator operator++(int);
      Iterator& operator--();
      Iterator operator--(int);

      bool operator==(const Iterator&);
      bool operator!=(const Iterator&);
      bool operator>=(const Iterator&);
      bool operator<=(const Iterator&);
      bool operator>(const Iterator&);
      bool operator<(const Iterator&);

      Iterator operator+(int);
      Iterator operator-(int);
      void operator+=(int);
      void operator-=(int);

      int index();
      bool isBegin();
      bool isEnd();

      void rewind();

      class Error {};
      class EndError : public Error {};
    };

    void connect(TerminalIO<InS, OutS, Prop>*);
    void disconnect();

    void startWork();
    void endWork();

    GridPos positionOf(int) const;
    int indexOf(GridPos) const;
    int currentBound() const;

    Cell & operator[] (int p);
    Cell operator[] (int p) const;
    Cell & operator()(int x, int y);
    Cell operator()(int x, int y) const;
    Cell & operator()(GridPos);
    Cell operator()(GridPos) const;

    Cell & at(int p);
    Cell at (int p) const;
    Cell & at(int x, int y);
    Cell at (int x, int y) const;
    Cell & at(GridPos);
    Cell at(GridPos) const;

    Iterator begin();
    Iterator end();

    void tile(OutS, Prop);
    void tile(OutS);
    //void tileRectangle(OutS, Prop, GridPos p1, GridPos p2);
    //void tileRectangle(OutS, GridPos p1, GridPos p2);
    //void tileRectangle(OutS, Prop, int x1, int y1, int x2, int y2);
    //void tileRectangle(OutS, int x1, int y1, int x2, int y2);

    Resolution getCurrentResolution();
    Resolution getTerminalResolution();
    void setResolution(Resolution);
    void setResolution(int w, int h);
    void updateResolution();

    void moveCursor(int x, int y);
    //void moveCursor(int p);
    void moveCursorTo(int x, int y);
    void moveCursorTo(int p);
    void moveCursorTo(GridPos);

    void resetCursor();
    void hideCursor();
    void showCursor();
    bool isCursorVisible();

    void clearInputBuffer();

    std::queue<InS> getSymbol(int n=1);
    void printSymbol(OutS, Prop);
    void printSymbol(OutS);

    enum resChange {
      alarm,
      ignore,
      ignoreExtension
    };

    void clearScreen();
    void printPage();
    void draw(resChange drawMode=alarm);
    void lazyDraw(resChange drawMode=alarm);
    void hardOptimization(bool);

    class Error {};
    class DrawError : public Error {};
    class ResolutionDisparity : public DrawError {};
    class TerminalModeError : public DrawError {};

  private:
    Cell* grid;
    TerminalIO<InS, OutS, Prop>* termIO;

    int maxSize;
    Resolution res;

    bool isDrawn=false;
    bool cursorVisible=false;
    bool cursorVisibleSlot;

    void saveCursor();
    void restoreCursor();

    void checkMode();
    void checkResolution(resChange);
    //void checkPos(GridPos);
    //void checkPos(int x, int y);
    //void checkPos(int p);
  };

}
