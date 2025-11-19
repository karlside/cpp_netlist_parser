#include "word.h"
#include <cassert>
#include <exception>
#include <iostream>
#include <sstream>

// ----------------
// --- BaseWord ---
// ----------------
Word::Word() {}

void Word::set_end_of_line() {
  _is_done = true;
  _is_end_of_line = true;
}

void Word::set_append_to_prev_word(char ch) { _append_to_prev_word = true; }

void Word::set_skip_whitespace(char ch) {
  keyword = keyword_map.at(ch);
  _skip_whitespace = true;
}

void Word::set_add_whitespace(char ch) {
  keyword = keyword_map.at(ch);
  _add_whitespace = true;
}

void Word::clear_whitespace_flag(char ch) {
  switch (keyword) {
  case NONE:
    return;
  case ANY:
    if (' ' == ch)
      return;
    break;
  case CLOSING_PARENTHESIS:
    if (')' != ch)
      return;
    break;
  }
  _skip_whitespace = false;
  _add_whitespace = false;
  keyword = NONE;
}

const void Word::check_is_parsed_or_done() {
  check_has_been_parsed();
  check_is_done();
}

const void Word::check_has_been_parsed() {
  if (has_been_parsed())
    throw std::runtime_error(
        "Cannot add more characters when a Word has been parsed");
}

const void Word::check_is_done() {
  if (is_done())
    throw std::runtime_error("Cannot add more characters when a Word is done");
}

void Word::add_char(char ch) {
  check_is_parsed_or_done();
  clear_whitespace_flag(ch);
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
      set_append_to_prev_word(ch);
    set_skip_whitespace(ch);
  } else if ('(' == ch) {
    set_add_whitespace(ch);
  } else if (std::isspace(ch))
    return;
  text += ch;
}

void Word::add_string(std::string input) {
  check_is_parsed_or_done();
  for (char ch : input)
    add_char(ch);
}

void Word::parse() {
  check_has_been_parsed();
  // std::istringstream iss(text);
  // std::getline(iss, key, '=');
  // std::getline(iss, value);
  // if (value.empty())
  //   _has_value = false;
  activate();
  _has_been_parsed = true;
}

std::string Word::get_text() const { return text; }

std::ostream &operator<<(std::ostream &os, const Word &rhs) {
  os << rhs.get_text();
  return os;
}

std::unique_ptr<Word> operator+(std::unique_ptr<Word> lhs,
                                const std::unique_ptr<Word> &rhs) {
  // for (char ch : rhs->get_text())
  //   lhs->add_char(ch);
  lhs->_is_active = rhs->_is_active;
  lhs->_has_been_parsed = rhs->_has_been_parsed;
  lhs->_append_to_prev_word =
      false; // The _append_to_prev_word flag is cleared during this operation
  lhs->_is_done = rhs->_is_done;
  lhs->_is_end_of_line = rhs->_is_end_of_line;
  lhs->_skip_whitespace = rhs->_skip_whitespace;
  lhs->_add_whitespace = rhs->_add_whitespace;
  try {
    lhs->add_string(rhs->get_text()); // This operation has to be done after the
                                      // _is_done flag is cleared
  } catch (const std::exception &e) {
    throw std::runtime_error("Error while adding two Word objects");
  }
  return lhs;
}

// --------------------
// --- KeyValueWord ---
// --------------------

KeyValueWord::KeyValueWord(std::string text) {
  std::istringstream iss(text);
  std::getline(iss, key, '=');
  std::getline(iss, value);
  if (value.empty())
    _has_value = false;
  else
    _has_value = true;
}

std::string KeyValueWord::get_text() const {
  if (!is_active())
    return "";
  else if (has_value())
    return get_key() + "=" + get_value();
  else
    return get_key();
}
