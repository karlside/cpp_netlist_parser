#include "line.h"
#include <iostream>
#include <memory>
#include <string>
#include <vector>

Line::Line() : entries{std::make_unique<std::vector<Word>>()} {}
Line::Line(std::unique_ptr<std::vector<Word>> input) {
  entries = std::move(input);
}

void Line::add(Word word) { entries->push_back(word); }

Word Line::at(int index) const { return entries->at(index); }

int Line::length() const { return entries->size(); }

std::unique_ptr<Line> Line::objectify() {
  for (Word word : *entries) {
    if ("simulator" == word.get_text()) {
      std::unique_ptr<SimulatorStatement> ret_obj =
          std::make_unique<SimulatorStatement>(std::move(entries));
      return ret_obj;
    }
  }
  // return std::make_unique<SimulatorStatement>(std::move(entries));
  return std::make_unique<Line>(std::move(entries));
  // Read through the entires, and identify what type of object the line is
}

std::ostream &operator<<(std::ostream &os, const Line &rhs) {
  std::string whitespace{
      " "}; // I don't know why, but I need this for it to work..
  for (int i = 0; i < rhs.length(); ++i) {
    os << rhs.at(i) << whitespace;
  }
  return os;
}

SimulatorStatement::SimulatorStatement(
    std::unique_ptr<std::vector<Word>> input) {
  entries = std::move(input);
};
// SimulatorStatement::SimulatorStatement(
// std::unique_ptr<std::vector<Word>> entires) {}
