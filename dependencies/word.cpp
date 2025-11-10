#include "word.h"
#include <iostream>
#include <sstream>

// ----------------
// --- BaswWord ---
// ----------------

void BaseWord::activate() { _is_active = true; }
void BaseWord::deactivate() { _is_active = false; }
bool BaseWord::is_active() const { return _is_active; }
bool BaseWord::has_value() const { return _has_value; }

void BaseWord::set_key(std::string input) {}
void BaseWord::set_value(std::string input) {}
std::string BaseWord::get_key() const { return key; }
std::string BaseWord::get_value() const { return value; }
std::string BaseWord::get_text() const {
  if (has_value())
    return get_key() + "=" + get_value();
  else
    return get_key();
}

std::ostream &operator<<(std::ostream &os, const BaseWord &rhs) {
  os << rhs.get_text();
  return os;
}

// ------------
// --- Word ---
// ------------

Word::Word(std::string input) {
  std::istringstream iss(input);
  std::getline(iss, key, '=');
  std::getline(iss, value);
  if (value.empty())
    _has_value = false;
}

void Word::set_key(std::string input) {}
void Word::set_value(std::string input) {}
std::string Word::get_key() const { return key; }
std::string Word::get_value() const { return value; }
std::string Word::get_text() const { return key + "=" + value; }
