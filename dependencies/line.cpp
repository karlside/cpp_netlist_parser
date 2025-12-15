#include "line.h"
#include <cassert>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

// ------------
// --- Line ---
// ------------

Line::Line() : list{std::make_shared<ListOfWords>()} {}
Line::Line(std::shared_ptr<ListOfWords> input) : list{input} {}
Line::Line(std::string input) : list{std::make_shared<ListOfWords>()} {
  // TODO: Input formatting
  // This is hacked together and is not a good solution.
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
  list->push_back(word->objectify());
}

std::string const Line::get_text() {
  std::string ret_text;
  for (auto wordItem : *list) {
    std::string word_text = wordItem.item->get_text();
    ret_text += word_text;
    if (R"(\\)" == word_text)
      ret_text += "\n";
    else
      ret_text += " ";
  }
  if (!ret_text.empty()) // Undefiend behavior if the text is empty
    ret_text.pop_back(); // Remove last whitespace
  return ret_text;
}

std::shared_ptr<Statement> Line::objectify() {
  if (first_word_is_keyword())
    return std::make_shared<ControlStatement>(list);
  if (second_word_is_keyword())
    return std::make_shared<InstanceStatement>(list);
  if (second_word_is_simulation())
    return std::make_shared<SimulationStatement>(list);
  return std::make_shared<Statement>(list);
}

bool Line::first_word_is_keyword() const {
  if (1 > list->size())
    return false;
  if (ObjectType::NONE == list->at(0)->get_keyword())
    return true;
  else
    return false;
}

bool Line::second_word_is_keyword() const {
  if (2 > list->size())
    return false;
  if (ObjectType::NONE == list->at(1)->get_keyword())
    return true;
  else
    return false;
}

bool Line::second_word_is_port() const {
  if (2 > list->size())
    return false;
  if (ObjectType::PORT == list->at(1)->get_keyword())
    return true;
  else
    return false;
}

bool Line::second_word_is_simulation() const {
  if (2 > list->size())
    return false;
  if (ObjectType::NONE == list->at(1)->get_keyword())
    return true;
  else
    return false;
}

std::ostream &operator<<(std::ostream &os, Line &rhs) {
  os << rhs.get_text();
  return os;
}

// -----------------
// --- Statement ---
// -----------------

Statement::Statement(std::shared_ptr<ListOfWords> input) : list{input} {}

std::string const Statement::get_text() {
  std::string ret_text;
  for (auto &wordItem : *list) {
    if (!wordItem.item->is_active())
      continue;
    std::string word_text = wordItem.item->get_text();
    ret_text += word_text;
    if (R"(\\)" == word_text)
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
