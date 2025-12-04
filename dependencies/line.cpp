#include "line.h"
#include <iostream>
#include <memory>
#include <string>
#include <vector>

Line::Line()
    : entries{std::make_shared<std::vector<std::shared_ptr<StatementWord>>>()} {
}
Line::Line(std::shared_ptr<std::vector<std::shared_ptr<StatementWord>>> input) {
  entries = std::move(input);
}

void Line::add_word(std::unique_ptr<Word> word) {
  if (word->is_end_of_line()) {
    _is_done = true;
  }
  // TODO: Change this to some falg instead of using get_text()
  if (R"(\\)" == word->get_text())
    _is_done = false;
  std::shared_ptr statementWord = word->objectify();
  entries->push_back(statementWord);
}

std::shared_ptr<StatementWord> Line::pop_word() {
  std::shared_ptr<StatementWord> temp_word = std::move(entries->back());
  entries->pop_back();
  return temp_word;
}

std::string Line::get_text() const {
  std::string ret_text;
  for (std::shared_ptr<StatementWord> word : *entries) {
    std::string text = word->get_text();
    ret_text += text;
    if (R"(\\)" == text)
      ret_text += "\n";
    else
      ret_text += " ";
  }
  return ret_text;
}

std::shared_ptr<Statement> Line::get_obj_from_keyword(ObjectType obj_keyword) {
  switch (obj_keyword) {
  case ObjectType::SIMULATOR:
    return std::make_shared<SimulatorStatement>(entries);
  case ObjectType::PORT:
    return std::make_shared<PortStatement>(entries);
  }
  return nullptr;
}

std::shared_ptr<Statement> Line::objectify() {
  for (std::shared_ptr<StatementWord> &word : *entries) {
    // TODO: Some function that returns the correct object based on keyword
    if (ObjectType::NONE == word->get_keyword())
      continue;
    std::shared_ptr<Statement> ret_obj =
        get_obj_from_keyword(word->get_keyword());
    if (nullptr == ret_obj)
      continue;
    return ret_obj;
  }
  return std::make_shared<Statement>(std::move(entries));
}

std::ostream &operator<<(std::ostream &os, const Line &rhs) {
  os << rhs.get_text();
  return os;
}

// -----------------
// --- Statement ---
// -----------------

Statement::Statement(
    std::shared_ptr<std::vector<std::shared_ptr<StatementWord>>> input) {
  list = std::move(input);
}

std::string Statement::get_text() const {
  std::string ret_text;
  // TODO: Test is_active() here
  for (std::shared_ptr<StatementWord> &word : *list) {
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
  return "";
}

// --------------------------
// --- SimulatorStatement ---
// --------------------------

// ---------------------
// --- PortStatement ---
// ---------------------
//
// PortStatement::PortStatement(
//     std::shared_ptr<std::vector<std::shared_ptr<StatementWord>>> input) {
//   // list = std::move(input);
//   Statement(input);
// }
