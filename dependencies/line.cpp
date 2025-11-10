#include "line.h"
#include <iostream>
#include <memory>
#include <string>
#include <vector>

Line::Line() : entries{std::make_unique<std::vector<BaseWord>>()} {}
Line::Line(std::unique_ptr<std::vector<BaseWord>> input) {
  entries = std::move(input);
}

void Line::add(BaseWord word) { entries->push_back(word); }
BaseWord Line::at(int index) const { return entries->at(index); }
std::string Line::get_text() const {
  std::string ret_text;
  for (BaseWord word : *entries) {
    std::string text = word.get_text();
    ret_text += text;
    if (R"(\\)" == text)
      ret_text += "\n";
    else
      ret_text += " ";
  }
  return ret_text;
}
int Line::length() const { return entries->size(); }

std::unique_ptr<Line> Line::objectify() {
  for (BaseWord &word : *entries) {
    if ("simulator" == word.get_text()) {
      std::unique_ptr<SimulatorStatement> ret_obj =
          std::make_unique<SimulatorStatement>(std::move(entries));
      return ret_obj;
    }
  }
  return std::make_unique<Line>(std::move(entries));
  // Read through the entires, and identify what type of object the line is
}

std::ostream &operator<<(std::ostream &os, const Line &rhs) {
  os << rhs.get_text();
  return os;
}

SimulatorStatement::SimulatorStatement(
    std::unique_ptr<std::vector<BaseWord>> input) {
  entries = std::move(input);
};

void SimulatorStatement::analyse_entries() {}
