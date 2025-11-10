#include "line.h"
#include <iostream>
#include <memory>
#include <string>
#include <vector>

// TODO: The line has to use unqie_ptr<Word> instead of just Word objects.
//       This inlcudes the entires vector as well
Line::Line()
    : entries{std::make_unique<std::vector<std::unique_ptr<Word>>>()} {}
Line::Line(std::unique_ptr<std::vector<std::unique_ptr<Word>>> input) {
  entries = std::move(input);
}

void Line::add(std::unique_ptr<Word> word) {
  entries->push_back(std::move(word));
}
std::unique_ptr<Word> *Line::at(int index) const { return &entries->at(index); }

std::string Line::get_text() const {
  std::string ret_text;
  for (std::unique_ptr<Word> &word : *entries) {
    std::string text = word->get_text();
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
  for (std::unique_ptr<Word> &word : *entries) {
    if ("simulator" == word->get_text()) {
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
    std::unique_ptr<std::vector<std::unique_ptr<Word>>> input) {
  entries = std::move(input);
};

void SimulatorStatement::analyse_entries() {}
