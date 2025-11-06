#ifndef LINE_H
#define LINE_H

#include "word.h"
#include <memory>
#include <string>
#include <vector>

class Line {
public:
  Line();
  void add(Word word);
  Word at(int index) const;
  int length() const;
  // std::unique_ptr<Line> objectify(); // Return a 'dynamicly dispateched' line
  // object based in line content

  friend std::ostream &operator<<(std::ostream &os, const Line &rhs);

  virtual ~Line() = default;

private:
  std::unique_ptr<std::vector<Word>> entries{};
};

class SimulatorStatement : Line {
public:
  SimulatorStatement(std::string text);

private:
};

#endif
