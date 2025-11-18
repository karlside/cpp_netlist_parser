#include "word.h"
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

void Word::add_char(char ch) {
  if (has_been_parsed())
    throw std::runtime_error(
        "Cannot add more characters when a Word has been parsed");
  if (' ' == ch) {
    _is_done = true;
    return;
  } else if ('\n' == ch) {
    _is_done = true;
    _newline = true;
    return;
  } else if (std::isspace(ch)) {
    // 'catch all' for other whitespace characters
    return;
  }
  text += ch;
};

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

bool Word::has_been_parsed() const { return _has_been_parsed; }
bool Word::is_done() const { return _is_done; }
bool Word::newline() const { return _newline; }

void Word::activate() { _is_active = true; }
void Word::deactivate() { _is_active = false; }
bool Word::is_active() const { return _is_active; }
bool Word::has_value() const { return _has_value; }

void Word::set_key(std::string input) {}
void Word::set_value(std::string input) {}
std::string Word::get_key() const { return key; }
std::string Word::get_value() const { return value; }

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
