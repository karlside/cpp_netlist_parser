#ifndef LINE_H
#define LINE_H

#include "word.h"
#include <string>
#include <vector>

class Line {
public:
  void add(Word word);
  Word at(int index) const;
  int length() const;

  friend std::ostream &operator<<(std::ostream &os, const Line &rhs);

private:
  std::vector<Word> entries{};
};

#endif
