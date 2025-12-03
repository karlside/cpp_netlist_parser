#include "word.h"
#include "exceptions.h"
#include <cassert>
// #include <exception>
#include "keywords.h"
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

void Word::set_ignore_whitespace(char ch) {
  keyword = keyword_map.at(ch);
  _ignore_whitespace = true;
  _add_whitespace = false;
}

void Word::set_add_whitespace(char ch) {
  keyword = keyword_map.at(ch);
  _ignore_whitespace = false;
  _add_whitespace = true;
}

void Word::clear_whitespace_flag(char ch) {
  // std::cout << keyword << " " << ch << std::endl;
  switch (keyword) {
  case NONE:
    return;
  case ANY:
    if (' ' == ch)
      return;
    break;
  case OPENING_PARENTHESIS:
    if (' ' == ch)
      return;
    else {
      set_add_whitespace(')');
      return;
    }
    break;
  case CLOSING_PARENTHESIS:
    if (')' != ch)
      return;
    remove_previous_whitespace();
    break;
  }
  _ignore_whitespace = false;
  _add_whitespace = false;
  keyword = NONE;
}

bool Word::is_double_whitespace(char ch) const {
  // check if the current character and the previous one is both whitespacees
  int last_index = text->size();
  if (0 == last_index)
    return false;
  if (' ' == ch && ' ' == text->at(last_index - 1))
    return true;
  return false;
}

void Word::remove_previous_whitespace() {
  if (' ' != text->at(text->size() - 1))
    return;
  text->pop_back();
}

void Word::add_char(char ch) {
  if (has_been_parsed())
    throw WordIsParsedError(
        "Cannot add more characters when a Word has been parsed");
  if (is_done())
    throw WordIsDoneError("Cannot add more characters when a Word is done");

  clear_whitespace_flag(ch);

  switch (ch) {
  case '\n':
    set_end_of_line();
    return;

  case ' ':
    assert(!(_ignore_whitespace && _add_whitespace));
    if (!_add_whitespace && !_ignore_whitespace) {
      if (0 != text->size()) {
        // This makes sure double whitespaces are not added as empty words..
        set_done();
      }
      return;
    }
    if (_add_whitespace) {
      if (is_double_whitespace(ch))
        return;
      break;
    }
    if (_ignore_whitespace)
      return;

  case '=':
    if ("" == *text)
      set_append_to_prev_word(ch);
    set_ignore_whitespace(ch);
    break;

  case '(':
    set_ignore_whitespace(ch);
    // case std::isspace():
    //   return;
  }

  *text += ch;
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

std::shared_ptr<StatementWord> Word::objectify() {
  ObjectType word_obj_type{ObjectType::NONE};
  if (keywordMap.find(get_text()) != keywordMap.end()) {
    word_obj_type = keywordMap.at(get_text());
  } else if (simKeywordMap.find(get_text()) != simKeywordMap.end()) {
    word_obj_type = simKeywordMap.at(get_text());
  } else if (otherKeywordMap.find(get_text()) != otherKeywordMap.end()) {
    word_obj_type = otherKeywordMap.at(get_text());
  }
  for (char ch : get_text()) {
    if ('=' == ch) {
      word_obj_type = ObjectType::KEYVALUE;
      break;
    }
    if ('(' == ch) {
      word_obj_type = ObjectType::PORT;
      break;
    }
  }
  if (ObjectType::NONE == word_obj_type) {
    // check get_text() vs. list of keywords
  }
  switch (word_obj_type) {
  case ObjectType::NONE:
    return std::make_shared<StatementWord>(std::move(text));
  case ObjectType::KEYVALUE:
    return std::make_shared<KeyValueWord>(std::move(text));
  case ObjectType::PORT:
    return std::make_shared<PortWord>(std::move(text));
  }
  return std::make_shared<StatementWord>(std::move(text));
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
  _ignore_whitespace = input_word._ignore_whitespace;
  _add_whitespace = input_word._add_whitespace;
}

void Word::merge_word_in_front(const std::string input_text) {
  *text = input_text + *text;
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
  *text = input;
  parse();
}

// --------------------
// --- KeyValueWord ---
// --------------------

void KeyValueWord::parse() {
  std::istringstream iss(*text);
  std::getline(iss, key, '=');
  std::getline(iss, value);
  if (!value.empty())
    _has_value = true;
  activate();
  _has_been_parsed = true;
}

std::string KeyValueWord::get_text() const {
  if (!_has_been_parsed) {
    return *text;
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
  // TODO: parse don't handle input formating in a good way.
  std::string dump;
  std::istringstream iss(*text);
  std::getline(iss, dump, '(');
  // std::getline(iss, dump, ' ');
  std::getline(iss, *text, ')');
  activate();
  _has_been_parsed = true;
}
