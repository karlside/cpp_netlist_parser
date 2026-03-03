#include "parsing_objects.h"
#include <cassert>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

// ------------
// --- Word ---
// ------------
void Word::set_end_of_line() {
  _is_done = true;
  _is_end_of_line = true;
}

void Word::set_append_to_prev_word(char ch) { _append_to_prev_word = true; }

void Word::set_ignore_whitespace(char ch) {
  charKeyword = char_keyword_map.at(ch);
  _ignore_whitespace = true;
  _add_whitespace = false;
}

void Word::set_add_whitespace(char ch) {
  charKeyword = char_keyword_map.at(ch);
  _ignore_whitespace = false;
  _add_whitespace = true;
}

void Word::clear_whitespace_flag(char ch) {
  switch (charKeyword) {
  case CharKeyword::NONE:
    return;
  case CharKeyword::ANY:
    if (' ' == ch)
      return;
    break;
  case CharKeyword::OPENING_PARENTHESIS:
    if (' ' == ch)
      return;
    else {
      set_add_whitespace(')');
      return;
    }
    break;
  case CharKeyword::CLOSING_PARENTHESIS:
    if (')' != ch)
      return;
    remove_previous_whitespace();
    break;
  }
  _ignore_whitespace = false;
  _add_whitespace = false;
  charKeyword = CharKeyword::NONE;
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
  if (is_done())
    throw WordIsDoneError("Cannot add more characters when a Word is done");

  check_char_for_type(ch);
  clear_whitespace_flag(ch);

  switch (ch) {
  case '\n':
    set_end_of_line();
    return;

  case ' ':
    assert(!(_ignore_whitespace && _add_whitespace));
    if (!_add_whitespace && !_ignore_whitespace) {
      // This makes sure double whitespaces are not added as empty words..
      if (0 != text->size()) {
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

void Word::check_char_for_type(char input_char) {
  if (type_flag_is_set())
    return;
  if (charKeywordMap.find(input_char) != charKeywordMap.end())
    set_type_flag(charKeywordMap.at(input_char));
}

void Word::set_type_flag(ObjectType input_type) {
  if (type_flag_is_set())
    return;
  _type_flag = true;
  object_type = input_type;
}

// TODO: Mark word as done after objectify
// This is important to avoid segfault if objectify is called twice
std::shared_ptr<StatementWord> Word::objectify() {
  if (keywordSet.find(get_text()) != keywordSet.end())
    return std::make_shared<KeywordWord>(std::move(text));

  if (mathConstantSet.find(get_text()) != mathConstantSet.end())
    return std::make_shared<MathConstantWord>(std::move(text));

  // TODO:
  if (simulationSet.find(get_text()) != simulationSet.end())
    return std::make_shared<SimulationWord>(std::move(text));

  if (type_flag_is_set()) {
    switch (object_type) {
    case ObjectType::KEYVALUE:
      return std::make_shared<KeyValueWord>(std::move(text));
    case ObjectType::PORT:
      return std::make_shared<PortWord>(std::move(text));
      // case ObjectType::COMMENT:
      // TODO:
      // case ObjectType::STRING:
      // TODO:
    }
  }

  return std::make_shared<StatementWord>(std::move(text));
}

void Word::append_word(const Word &input_word) {
  _is_done = false;
  try {
    add_string(input_word.get_text());
  } catch (const std::exception &e) {
    throw std::runtime_error("Error while appending to word");
  }
  _append_to_prev_word = false; // The _append_to_prev_word flag is
                                // cleared during the addition operation
  _is_done = input_word._is_done;
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

// ------------
// --- Line ---
// ------------

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

void Line::set_ignore_end_of_line(std::string word) {
  wordKeyword = word_keyword_map.at(word);
  _ignore_newline = true;
}

void Line::clear_newline_flag(std::string word) {
  switch (wordKeyword) {
  case WordKeyword::ENDS:
    if ("ends" != word)
      return;
    break;
  }
  _ignore_newline = false;
}

void Line::add_word(std::unique_ptr<Word> word) {
  clear_newline_flag(word->get_text());

  if (word->is_end_of_line() && !(_ignore_newline)) {
    _is_done = true;
  }
  // TODO: Change this to some falg instead of using get_text()
  if (R"(\\)" == word->get_text())
    _is_done = false;
  if ("subckt" == word->get_text())
    set_ignore_end_of_line(word->get_text());
  list->push_back(word->objectify());
}

std::string Line::get_text() const {
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
