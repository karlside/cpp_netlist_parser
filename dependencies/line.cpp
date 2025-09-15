#include "line.h"
#include <vector>
#include <string>

void Line::add(std::string word) {
  entries.push_back(word);
}

std::string Line::at(int index) const {
  return entries.at(index);
}

int Line::length() const {
  return entries.size();
}

std::ostream& operator<<(std::ostream& os, const Line& rhs) {
  std::string whitespace{" "}; // I don't know why, but I need this for it to work..
  for (int i = 0; i < rhs.length(); ++i) {
    os << rhs.at(i) << whitespace;
  }
  return os;
}

