#include "line.h"
#include <cassert>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

// -------------------
// --- ListOfWords ---
// -------------------

void ListOfWords::push_back(std::shared_ptr<StatementWord> input_word) {
  //  std::string key = input_word->get_text();
  // TODO: Create new keys if needed
  std::string key = create_key(input_word->get_text());
  words.push_back(WordPair(std::make_pair(key, input_word)));
  index[key] = index.size();
};

std::shared_ptr<StatementWord> ListOfWords::get_word(std::string key) {
  if (index.find(key) == index.end())
    throw std::runtime_error("Key not in List");
  // TODO: Create new errors
  return words.at(index.at(key)).word;
}

std::shared_ptr<StatementWord> ListOfWords::pop_back() {
  WordPair temp_word = words.back();
  words.pop_back();
  index.erase(temp_word.word->get_text());
  return temp_word.word;
}

std::string ListOfWords::create_key(std::string key, int iterator) {
  if (index.find(key) == index.end())
    return key;
  iterator += 1;
  key = key + "_" + std::to_string(iterator);
  return create_key(key, iterator);
}

std::ostream &operator<<(std::ostream &os, const ListOfWords &rhs) {
  for (WordPair wordPair : rhs.words)
    os << wordPair.word->get_text() << " ";
  return os;
}

// ------------
// --- Line ---
// ------------

Line::Line() : list{std::make_shared<ListOfWords>()} {}
Line::Line(std::shared_ptr<ListOfWords> input) : list{input} {}

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

std::shared_ptr<StatementWord> Line::pop_word() {
  return list->pop_back();
  // std::shared_ptr<StatementWord> temp_word = std::move(list->back());
  // list->pop_back();
  // return temp_word;
}

const std::string &Line::get_text() {
  if (!_text_is_built)
    build_text();
  return text;
}

void Line::build_text() {
  std::string new_text;
  for (WordPair wordPair : list->words) {
    std::string word_text = wordPair.word->get_text();
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
  for (WordPair &wordPair : list->words) {
    // TODO: Some function that returns the correct object based on keyword
    if (ObjectType::NONE == wordPair.word->get_keyword())
      continue;
    std::shared_ptr<Statement> ret_obj =
        get_obj_from_keyword(wordPair.word->get_keyword());
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
  for (WordPair &wordPair : list->words) {
    if (!wordPair.word->is_active())
      continue;
    std::string text = wordPair.word->get_text();
    ret_text += text;
    if (R"(\\)" == text)
      ret_text += "\n";
    else
      ret_text += " ";
  }
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
  for (WordPair word : list->words) {
    ret_str += word.print_word() + "\n";
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
