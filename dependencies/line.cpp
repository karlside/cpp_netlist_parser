#include "line.h"
#include <iostream>
#include <memory>
#include <string>
#include <vector>

Line::Line()
    : entries{std::make_unique<std::vector<std::unique_ptr<Word>>>()} {}
Line::Line(std::unique_ptr<std::vector<std::unique_ptr<Word>>> input) {
  entries = std::move(input);
}

void Line::add(std::unique_ptr<Word> word) {
  if (word->is_end_of_line())
    _is_done = true;
  if (R"(\\)" == word->get_text())
    _is_done = false;
  entries->push_back(std::move(word));
}

std::unique_ptr<Word> Line::pop_word() {
  std::unique_ptr<Word> temp_word = std::move(entries->back());
  entries->pop_back();
  temp_word->set_done(false);
  return temp_word;
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

bool Line::is_done() const { return _is_done; }

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
