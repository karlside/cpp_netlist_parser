#include "word.h"
#include <iostream>
#include <string>

Word::Word(std::string text) : text{text} {
  if ("" == text) {
    empty = true;
    return;
  }
  if (R"(\\)" == text) {
    // Set word as a keyword
    return;
  }
}

bool Word::is_empty() const { return empty; }

std::string Word::get_text() const { return text; }

std::ostream &operator<<(std::ostream &os, const Word &rhs) {
  os << rhs.get_text();
  return os;
}
