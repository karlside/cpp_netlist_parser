#include "word.h"
#include <iostream>
#include <sstream>

// ----------------
// --- BaswWord ---
// ----------------
Word::Word(std::string input) {
  std::istringstream iss(input);
  std::getline(iss, key, '=');
  std::getline(iss, value);
  if (value.empty())
    _has_value = false;
}

void Word::add_char(char input) {
  // TODO: Throw exception if has_been_parsed == true;
  text += input;
};

void Word::add_string(std::string input) {
  // TODO: Throw exception if has_been_parsed == true;
  text += input;
}

void Word::parse() {
  // TODO: Throw exception if has_been_parsed == true;
  std::istringstream iss(text);
  std::getline(iss, key, '=');
  std::getline(iss, value);
  if (value.empty())
    _has_value = false;
  activate();
  _has_been_parsed = true;
}

bool Word::has_been_parsed() const { return _has_been_parsed; }

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
