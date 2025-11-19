#include "word.h"
#include <cassert>
#include <exception>
#include <iostream>
#include <sstream>

// ----------------
// --- BaseWord ---
// ----------------
Word::Word() {}

Word::Word(std::string text) {
  std::istringstream iss(text);
  std::getline(iss, key, '=');
  std::getline(iss, value);
  if (value.empty())
    _has_value = false;
}

void Word::set_done(bool input) { _is_done = input; }

void Word::set_end_of_line() {
  _is_done = true;
  _is_end_of_line = true;
}

void Word::add_char(char ch) {
  if (has_been_parsed())
    throw std::runtime_error(
        "Cannot add more characters when a Word has been parsed");
  if ('\n' == ch) {
    set_end_of_line();
    return;
  } else if (' ' == ch) {
    assert(!(_skip_whitespace && _add_whitespace));
    if (_skip_whitespace)
      return;
    if (!_add_whitespace) {
      set_done();
      return;
    }
  } else if ('=' == ch) {
    if ("" == text)
      _append_to_prev_word = true;
    _skip_whitespace = true;
    // TODO: add some keyword
  } else if (std::isspace(ch))
    return;
  text += ch;
};

void Word::add_string(std::string input) {
  if (has_been_parsed())
    throw std::runtime_error("Cannot add strings when a Word has been parsed");
  for (char ch : input)
    add_char(ch);
}

void Word::parse() {
  if (has_been_parsed())
    throw std::runtime_error(
        "Word has already been parsed. Cannot re-parse a word");
  std::istringstream iss(text);
  std::getline(iss, key, '=');
  std::getline(iss, value);
  if (value.empty())
    _has_value = false;
  activate();
  _has_been_parsed = true;
}

void Word::set_key(std::string input) {}
void Word::set_value(std::string input) {}

std::string Word::get_text() const {
  if (!has_been_parsed())
    return text;
  else if (!is_active())
    return "";
  else if (has_value())
    return get_key() + "=" + get_value();
  else
    return get_key();
}

std::ostream &operator<<(std::ostream &os, const Word &rhs) {
  os << rhs.get_text();
  return os;
}

std::unique_ptr<Word> operator+(std::unique_ptr<Word> lhs,
                                const std::unique_ptr<Word> &rhs) {
  // for (char ch : rhs->get_text())
  //   lhs->add_char(ch);
  lhs->add_string(rhs->get_text());
  lhs->_is_active = rhs->_is_active;
  lhs->_has_value = rhs->_has_value;
  lhs->_has_been_parsed = rhs->_has_been_parsed;
  lhs->_append_to_prev_word = false; // This function is the operation of
                                     // attaching to the previous word.
  lhs->_is_done = rhs->_is_done;
  lhs->_is_end_of_line = rhs->_is_end_of_line;
  lhs->_skip_whitespace = rhs->_skip_whitespace;
  lhs->_add_whitespace = rhs->_add_whitespace;
  return lhs;
}
