#include "Symbol.hpp"
#include "Error.hpp"

#include <string>
#include <iostream>
#include <cstring>
#include <cstdio>

namespace Blessings_ns {
  SymbolUTF8::SymbolUTF8(const char* sym) {
    if(sym==nullptr) throw Error("nullptr given as const char* string");

    int size;
    if(!(sym[0]&0b10000000)) size=1;
    else if(!((sym[0]&0b11100000)^0b11000000)) size=2;
    else if(!((sym[0]&0b11110000)^0b11100000)) size=3;
    else if(!((sym[0]&0b11111000)^0b11110000)) size=4;
    else throw Error("non-UTF-8 symbol given");

    if(strlen(sym)!=size) throw Error("non-UTF-8 symbol given");

    switch(size) {
    case 4:
      if(((sym[3]&0b11000000)^0b10000000)) throw Error("non-UTF-8 symbol given");
    case 3:
      if(((sym[2]&0b11000000)^0b10000000)) throw Error("non-UTF-8 symbol given");
    case 2:
      if(((sym[1]&0b11000000)^0b10000000)) throw Error("non-UTF-8 symbol given");
    }

    for(int i=0; i<size; ++i) arr[i]=sym[i];
  }

  SymbolUTF8::SymbolUTF8(std::string sym) {
    int size;
    if(!(sym[0]&0b10000000)) size=1;
    else if(!((sym[0]&0b11100000)^0b11000000)) size=2;
    else if(!((sym[0]&0b11110000)^0b11100000)) size=3;
    else if(!((sym[0]&0b11111000)^0b11110000)) size=4;
    else throw Error("non-UTF-8 symbol given");

    if(sym.size()!=size) throw Error("non-UTF-8 symbol given");

    switch(size) {
    case 4:
      if(((sym[3]&0b11000000)^0b10000000)) throw Error("non-UTF-8 symbol given");
    case 3:
      if(((sym[2]&0b11000000)^0b10000000)) throw Error("non-UTF-8 symbol given");
    case 2:
      if(((sym[1]&0b11000000)^0b10000000)) throw Error("non-UTF-8 symbol given");
    }

    for(uint8_t i=0; i<size; ++i) arr[i]=sym[i];
  }

  char& SymbolUTF8::operator[](int pos) {
    if(pos>=4 || pos<0) throw Error("bad access position");

    return arr[pos];
  }

  char SymbolUTF8::operator[](int pos) const {
    if(pos>=4 || pos<0) throw Error("bad access position");

    return arr[pos];
  }

  int SymbolUTF8::getSize() const {
    if(!(arr[0]&0b10000000)) return 1;
    else if(!((arr[0]&0b11100000)^0b11000000)) return 2;
    else if(!((arr[0]&0b11110000)^0b11100000)) return 3;
    else return 4;
  }

  std::ostream& operator<<(std::ostream& stream, const SymbolUTF8& sym) {
    int size;
    if(!(sym.arr[0]&0b10000000)) size=1;
    else if(!((sym.arr[0]&0b11100000)^0b11000000)) size=2;
    else if(!((sym.arr[0]&0b11110000)^0b11100000)) size=3;
    else if(!((sym.arr[0]&0b11111000)^0b11110000)) size=4;

    for(int i=0; i<size; ++i) {
      try {
        stream << sym.arr[i];
      }
      catch(...) {
        throw SymbolUTF8::Error("bod uotput stream");
      }
    }

    return stream;
  }

  std::istream& operator>>(std::istream& stream, SymbolUTF8& sym) {
    SymbolUTF8 ret; //UTF-8 space symbol, 1 byte
    try {
      stream.get(ret.arr[0]);
    }
    catch(...) {
      throw SymbolUTF8::Error("bad input stream");
    }

    int size;
    if(!(ret.arr[0]&0b10000000)) size=1;
    else if(!((ret.arr[0]&0b11100000)^0b11000000)) size=2;
    else if(!((ret.arr[0]&0b11110000)^0b11100000)) size=3;
    else if(!((ret.arr[0]&0b11111000)^0b11110000)) size=4;
    else throw SymbolUTF8::Error("attempt to input non-UTF-8 symbol");

    for(int i=1; i<size; ++i) {
      try {
        stream.get(ret.arr[i]);
      }
      catch(...) {
        throw SymbolUTF8::Error("bad input stream");
      }
    }

    switch(size) {
    case 4:
      if(((ret.arr[3]&0b11000000)^0b10000000)) throw SymbolUTF8::Error("non-UTF-8 symbol in input");
    case 3:
      if(((ret.arr[2]&0b11000000)^0b10000000)) throw SymbolUTF8::Error("non-UTF-8 symbol in input");
    case 2:
      if(((ret.arr[1]&0b11000000)^0b10000000)) throw SymbolUTF8::Error("non-UTF-8 symbol in input");
    }

    sym=ret;

    return stream;
  }

  std::string SymbolUTF8::getString() const {
    return std::string(arr, getSize());
  }

  void SymbolUTF8::writeToFile(FILE* file) const {
    int size=getSize();
    for(int i=0; i<size; ++i) {
      int temp=fputc(static_cast<int>(arr[i]), file);
      if(temp==EOF) throw Error("Bad file given in writeToFile");
    }
  }

  template <>
  SymbolUTF8 getSym<SymbolUTF8>(FILE* file) {
    int temp=getc(file);
    if(temp==EOF) throw SymbolUTF8::Error("bad input file given");

    SymbolUTF8 ret; //UTF-8 space symbol, 1 byte
    ret.arr[0]=static_cast<char>(temp);

    int size;
    if(!(ret.arr[0]&0b10000000)) size=1;
    else if(!((ret.arr[0]&0b11100000)^0b11000000)) size=2;
    else if(!((ret.arr[0]&0b11110000)^0b11100000)) size=3;
    else if(!((ret.arr[0]&0b11111000)^0b11110000)) size=4;
    else throw SymbolUTF8::Error("non-UTF-8 symbol in input");

    for(int i=1; i<size; ++i) {
      temp=getc(file);
      if(temp==EOF) throw SymbolUTF8::Error("input state unexpectly changed to bad");

      ret.arr[i]=static_cast<char>(temp);

      if((ret.arr[i]&0b11000000)^0b10000000) throw SymbolUTF8::Error("non-UTF-8 symbol in input");
    }

    return ret;
  }
}