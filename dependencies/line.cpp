#include "line.h"
#include <memory>
#include <string>
#include <vector>

Line::Line() : entries{std::make_unique<std::vector<Word>>()} {}

void Line::add(Word word) { entries->push_back(word); }

Word Line::at(int index) const { return entries->at(index); }

int Line::length() const { return entries->size(); }

// std::unique_ptr<Line> objectify() {
//   // Read through the entires, and identify what type of object the line is
// }

std::ostream &operator<<(std::ostream &os, const Line &rhs) {
  std::string whitespace{
      " "}; // I don't know why, but I need this for it to work..
  for (int i = 0; i < rhs.length(); ++i) {
    os << rhs.at(i) << whitespace;
  }
  return os;
}
