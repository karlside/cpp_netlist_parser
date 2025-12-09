#include "line.h"
#include <cassert>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

Line::Line() : list{std::make_shared<ListOfWords>()} {}
Line::Line(std::shared_ptr<ListOfWords> input) : list{input} {}
Line::Line(std::string input) : list{std::make_shared<ListOfWords>()} {
  // TODO: Input formatting
  std::unique_ptr<Word> word = std::make_unique<Word>();
  for (char ch : input) {
    if (' ' == ch) {
      add_word(std::move(word));
      word = std::make_unique<Word>();
      continue;
    }
    word->add_char(ch);
  }
  add_word(std::move(word));
}

void Line::add_word(std::unique_ptr<Word> word) {
  if (word->is_end_of_line()) {
    _is_done = true;
  }
  // TODO: Change this to some falg instead of using get_text()
  if (R"(\\)" == word->get_text())
    _is_done = false;
  std::shared_ptr statementWord = word->objectify();
  list->push_back(statementWord);
}

std::shared_ptr<StatementWord> Line::pop_word() { return list->pop_back(); }

const std::string &Line::get_text() {
  if (!_text_is_built)
    build_text();
  return text;
}

void Line::build_text() {
  std::string new_text;
  for (auto wordItem : *list) {
    std::string word_text = wordItem.item->get_text();
    new_text += word_text;
    if (R"(\\)" == text)
      new_text += "\n";
    else
      new_text += " ";
  }
  new_text.pop_back(); // Remove last whitespace
  text = new_text;
  _text_is_built = true;
}

std::shared_ptr<Statement> Line::get_obj_from_keyword(ObjectType obj_keyword) {
  switch (obj_keyword) {
  case ObjectType::SIMULATOR:
    return std::make_shared<SimulatorStatement>(list);
  case ObjectType::PORT:
    return std::make_shared<PortStatement>(list);
  }
  return nullptr;
}

std::shared_ptr<Statement> Line::objectify() {
  for (auto &wordItem : *list) {
    // TODO: Some function that returns the correct object based on keyword
    if (ObjectType::NONE == wordItem.item->get_keyword())
      continue;
    std::shared_ptr<Statement> ret_obj =
        get_obj_from_keyword(wordItem.item->get_keyword());
    if (nullptr == ret_obj)
      continue;
    return ret_obj;
  }
  return std::make_shared<Statement>(std::move(list));
}

std::ostream &operator<<(std::ostream &os, Line &rhs) {
  os << rhs.get_text();
  return os;
}

// -----------------
// --- Statement ---
// -----------------

Statement::Statement(std::shared_ptr<ListOfWords> input) : list{input} {}

std::string Statement::get_text() { return build_text(); }

std::string Statement::build_text() {
  std::string ret_text;
  for (auto &wordItem : *list) {
    if (!wordItem.item->is_active())
      continue;
    std::string text = wordItem.item->get_text();
    ret_text += text;
    if (R"(\\)" == text)
      ret_text += "\n";
    else
      ret_text += " ";
  }
  if (!ret_text.empty()) // Undefiend behavior if the text is empty
    ret_text.pop_back(); // Remove last whitespace
  return ret_text;
}

std::ostream &operator<<(std::ostream &os, Statement &rhs) {
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

std::string Statement::print_line() {
  std::string ret_str;
  for (auto wordItem : *list) {
    ret_str += wordItem.print_list() + "\n";
  }
  return ret_str;
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
