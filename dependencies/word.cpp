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
      _attach_to_prev = true;
    _skip_whitespace = true;
    // TODO: add some keyword
  } else if (std::isspace(ch))
    return;
  text += ch;
};

bool Word::check_keyword(char ch) {
  if (!_wait_for_keyword || ch == keyword) {
    _wait_for_keyword = false;
    return true;
  }
  return false;
}

void Word::add_string(std::string input) {
  if (has_been_parsed())
    throw std::runtime_error("Cannot add strings when a Word has been parsed");
  text += input;
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
