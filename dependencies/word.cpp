#include "word.h"
#include "exceptions.h"
#include <cassert>
// #include <exception>
#include <iostream>
#include <sstream>

// ---------------------
// --- StatementWord ---
// ---------------------

// ------------
// --- Word ---
// ------------
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
    // TODO: Should I use std::isspace() to catch tab charcter and such?
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

bool Word::is_double_whitespace(char ch) const {
  // check if the current character and the previous one is both whitespacees
  int last_index = text.size();
  if (0 == last_index)
    return false;
  if (' ' == ch && ' ' == text.at(last_index - 1))
    return true;
  return false;
}

void Word::add_char(char ch) {
  if (has_been_parsed())
    throw WordIsParsedError(
        "Cannot add more characters when a Word has been parsed");
  if (is_done())
    throw WordIsDoneError("Cannot add more characters when a Word is done");
  clear_whitespace_flag(ch);
  if ('\n' == ch) {
    set_end_of_line();
    return;
  } else if (' ' == ch) {
    assert(!(_skip_whitespace && _add_whitespace));
    if (_skip_whitespace)
      return;
    if (!_add_whitespace) {
      if (0 != text.size()) // This makes sure double whitespaces are not added
                            // as empty words..
        set_done();
      return;
    }
    if (is_double_whitespace(ch))
      return;
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
  try {
    for (char ch : input)
      add_char(ch);
  } catch (const WordIsParsedError &e) {
    throw WordIsParsedError("Cannot add string when word is parsed.");
  } catch (const WordIsDoneError &e) {
    throw WordIsDoneError("Cannot add string when word is done.");
  }
}

enum WordType { BASEWORD, KEYVALUE, PORT };
std::shared_ptr<StatementWord> Word::objectify() const {
  WordType word_type{BASEWORD};
  for (char ch : get_text()) {
    if ('=' == ch) {
      word_type = KEYVALUE;
      break;
    }
    if ('(' == ch) {
      word_type = PORT;
      break;
    }
  }
  switch (word_type) {
  case BASEWORD:
    return std::make_shared<StatementWord>(get_text());
  case KEYVALUE:
    return std::make_shared<KeyValueWord>(get_text());
  case PORT:
    return std::make_shared<PortWord>(get_text());
  }
  return std::make_shared<StatementWord>(get_text());
}

void Word::parse() {
  // TODO: Do I need this parse function?
  // Should _is_active default to true?
  if (has_been_parsed())
    throw std::runtime_error("Cannot reparse Word that has been parsed");
  activate();
  _has_been_parsed = true;
}

void Word::append_word(const Word &input_word) {
  _is_done = false;
  _has_been_parsed = false;
  try {
    add_string(input_word.get_text());
  } catch (const std::exception &e) {
    throw std::runtime_error("Error while appending to word");
  }
  _is_active = input_word._is_active;
  _has_been_parsed = input_word._has_been_parsed;
  _append_to_prev_word = false; // The _append_to_prev_word flag is cleared
                                // during the addition operation
  _is_done = input_word._is_done;
  _has_been_parsed = input_word._has_been_parsed;
  _is_end_of_line = input_word._is_end_of_line;
  _skip_whitespace = input_word._skip_whitespace;
  _add_whitespace = input_word._add_whitespace;
}

void Word::merge_word_in_front(const std::string input_text) {
  text = input_text + text;
  _append_to_prev_word = false;
}

std::ostream &operator<<(std::ostream &os, const Word &rhs) {
  os << rhs.get_text();
  return os;
}

// ---------------------
// --- StatementWord ---
// ---------------------

void StatementWord::add_string(std::string input) {
  text = input;
  parse();
}

// --------------------
// --- KeyValueWord ---
// --------------------

void KeyValueWord::parse() {
  std::istringstream iss(text);
  std::getline(iss, key, '=');
  std::getline(iss, value);
  if (!value.empty())
    _has_value = true;
  activate();
  _has_been_parsed = true;
}

std::string KeyValueWord::get_text() const {
  if (!_has_been_parsed) {
    return text;
  } else if (has_value()) {
    return get_key() + "=" + get_value();
  } else {
    return get_key();
  }
}

// ----------------
// --- PortWord ---
// ----------------

void PortWord::parse() {
  std::string dump;
  std::istringstream iss(text);
  std::getline(iss, dump, '(');
  std::getline(iss, dump, ' ');
  std::getline(iss, text, ')');
  activate();
  _has_been_parsed = true;
}
