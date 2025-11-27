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

void Line::add_word(std::unique_ptr<Word> word) {
  if (word->is_end_of_line()) {
    _is_done = true;
  }
  if (R"(\\)" == word->get_text())
    _is_done = false;
  entries->push_back(std::move(word));
}

std::unique_ptr<Word> Line::pop_word() {
  std::unique_ptr<Word> temp_word = std::move(entries->back());
  entries->pop_back();
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

std::unique_ptr<Statement> Line::objectify() {
  for (std::unique_ptr<Word> &word : *entries) {
    if ("simulator" == word->get_text()) {
      std::unique_ptr<Statement> ret_obj =
          std::make_unique<Statement>(std::move(entries));
      return ret_obj;
    }
  }
  return std::make_unique<Statement>(std::move(entries));
  // Read through the entires, and identify what type of object the line is
}

std::ostream &operator<<(std::ostream &os, const Line &rhs) {
  os << rhs.get_text();
  return os;
}

// -----------------
// --- Statement ---
// -----------------

Statement::Statement(
    std::unique_ptr<std::vector<std::unique_ptr<Word>>> input) {
  list = std::move(input);
}

std::string Statement::get_text() const {
  std::string ret_text;
  // TODO: Test is_active() here
  for (std::unique_ptr<Word> &word : *list) {
    if (!word->is_active())
      continue;
    std::string text = word->get_text();
    ret_text += text;
    if (R"(\\)" == text)
      ret_text += "\n";
    else
      ret_text += " ";
  }
  return ret_text;
}

std::ostream &operator<<(std::ostream &os, const Statement &rhs) {
  os << rhs.get_text();
  return os;
}

std::string Statement::get_list() const {
  // I guess the list should be a shared pointer,
  //  with a vector of shared pointers to the words...
  //  That way I can pass the shared pointers around and make changes
  //  to the word anywhere, without worring about
  //  memory saftey..
}
