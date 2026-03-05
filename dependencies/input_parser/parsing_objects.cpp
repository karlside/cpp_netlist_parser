#include "parsing_objects.h"
#include <cassert>
#include <fstream>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

// ------------------
// --- WordParser ---
// ------------------

void WordParser::set_end_of_line() {
  _is_done = true;
  _is_end_of_line = true;
}

void WordParser::set_append_to_prev_word(char ch) {
  _append_to_prev_word = true;
}

void WordParser::set_ignore_whitespace(char ch) {
  charKeyword = char_keyword_map.at(ch);
  _ignore_whitespace = true;
  _add_whitespace = false;
}

void WordParser::set_add_whitespace(char ch) {
  charKeyword = char_keyword_map.at(ch);
  _ignore_whitespace = false;
  _add_whitespace = true;
}

void WordParser::clear_whitespace_flag(char ch) {
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

bool WordParser::is_double_whitespace(char ch) const {
  // check if the current character and the previous one is both whitespacees
  int last_index = text->size();
  if (0 == last_index)
    return false;
  if (' ' == ch && ' ' == text->at(last_index - 1))
    return true;
  return false;
}

void WordParser::remove_previous_whitespace() {
  if (' ' != text->at(text->size() - 1))
    return;
  text->pop_back();
}

void WordParser::add_char(char ch) {
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

void WordParser::add_string(std::string input) {
  try {
    for (char ch : input)
      add_char(ch);
  } catch (const WordIsParsedError &e) {
    throw WordIsParsedError("Cannot add string when word is parsed.");
  } catch (const WordIsDoneError &e) {
    throw WordIsDoneError("Cannot add string when word is done.");
  }
}

void WordParser::check_char_for_type(char input_char) {
  if (type_flag_is_set())
    return;
  if (charKeywordMap.find(input_char) != charKeywordMap.end())
    set_type_flag(charKeywordMap.at(input_char));
}

void WordParser::set_type_flag(ObjectType input_type) {
  if (type_flag_is_set())
    return;
  _type_flag = true;
  object_type = input_type;
}

// TODO: Mark word as done after objectify
// This is important to avoid segfault if objectify is called twice
std::shared_ptr<Word> WordParser::objectify() {
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

  return std::make_shared<Word>(std::move(text));
}

void WordParser::append_word(const WordParser &input_word) {
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

void WordParser::merge_word_in_front(const std::string input_text) {
  *text = input_text + *text;
  _append_to_prev_word = false;
}

std::ostream &operator<<(std::ostream &os, const WordParser &rhs) {
  os << rhs.get_text();
  return os;
}

// ------------------
// --- LineParser ---
// ------------------

LineParser::LineParser(std::string input)
    : list{std::make_shared<ListOfWords>()} {
  // TODO: Input formatting
  // This is hacked together and is not a good solution.
  std::unique_ptr<WordParser> word = std::make_unique<WordParser>();
  for (char ch : input) {
    if (' ' == ch) {
      add_word(std::move(word));
      word = std::make_unique<WordParser>();
      continue;
    }
    word->add_char(ch);
  }
  add_word(std::move(word));
}

void LineParser::set_ignore_end_of_line(std::string word) {
  wordKeyword = word_keyword_map.at(word);
  _ignore_newline = true;
}

void LineParser::clear_newline_flag(std::string word) {
  switch (wordKeyword) {
  case WordKeyword::ENDS:
    if ("ends" != word)
      return;
    break;
  }
  _ignore_newline = false;
}

void LineParser::add_word(std::unique_ptr<WordParser> word) {
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

std::string LineParser::get_text() const {
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

std::shared_ptr<Line> LineParser::objectify() {
  if (first_word_is_keyword())
    return std::make_shared<ControlStatement>(list);
  if (second_word_is_keyword())
    return std::make_shared<InstanceStatement>(list);
  if (second_word_is_simulation())
    return std::make_shared<SimulationStatement>(list);
  return std::make_shared<Line>(list);
}

bool LineParser::first_word_is_keyword() const {
  if (1 > list->size())
    return false;
  if (ObjectType::NONE == list->at(0)->get_keyword())
    return true;
  else
    return false;
}

bool LineParser::second_word_is_keyword() const {
  if (2 > list->size())
    return false;
  if (ObjectType::NONE == list->at(1)->get_keyword())
    return true;
  else
    return false;
}

bool LineParser::second_word_is_port() const {
  if (2 > list->size())
    return false;
  if (ObjectType::PORT == list->at(1)->get_keyword())
    return true;
  else
    return false;
}

bool LineParser::second_word_is_simulation() const {
  if (2 > list->size())
    return false;
  if (ObjectType::NONE == list->at(1)->get_keyword())
    return true;
  else
    return false;
}

std::ostream &operator<<(std::ostream &os, LineParser &rhs) {
  os << rhs.get_text();
  return os;
}

// ---------------------
// --- NetlistParser ---
// ---------------------

std::unique_ptr<WordParser>
NetlistParser::merge_to_prev_word(std::shared_ptr<LineParser> &line,
                                  std::unique_ptr<WordParser> word) {
  if (0 == line->length()) {
    return word;
  }
  std::shared_ptr<Word> statementWord = line->pop_word();
  // Because the word retrived from the line is already objectified it needs to
  // be prepended to the unobjectified word
  word->merge_word_in_front(statementWord->get_text());
  return word;
}

void print_state(State state) {
  switch (state) {
  case START:
    std::cout << "START";
    break;
  case READ_CHAR:
    std::cout << "READ_CHAR";
    break;
  case ADD_WORD:
    std::cout << "ADD_WORD";
    break;
  case ADD_LINE:
    std::cout << "ADD_LINE";
    break;
  case DONE:
    std::cout << "DONE";
    break;
  default:
    std::cout << "UNKNOWN_STATE";
    break;
  }
  std::cout << std::endl;
}

std::unique_ptr<ListOfLines>
NetlistParser::parse_netlist(const std::unique_ptr<std::fstream> &file) {
  list = std::make_unique<ListOfLines>();
  char ch;
  std::unique_ptr<WordParser> word_parser;
  std::shared_ptr<LineParser> line_parser;
  State state{START};
  State next_state{START};
  bool ignore_newline{false};
  std::string keyword;

  while (State::DONE != state) {
    // print_state(state);

    switch (state) {

    case State::START:
      word_parser = std::make_unique<WordParser>();
      line_parser = std::make_shared<LineParser>();
      next_state = READ_CHAR;
      break;

    case State::READ_CHAR:
      ch = file->get();
      if (!*file) {
        next_state = DONE;
        break;
      }
      word_parser->add_char(ch);
      if (word_parser->is_append_to_prev_word()) {
        word_parser = merge_to_prev_word(line_parser, std::move(word_parser));
        next_state = READ_CHAR;
      } else if (word_parser->is_done()) {
        next_state = ADD_WORD;
      } else {
        next_state = READ_CHAR;
      }
      break;

    case State::ADD_WORD:
      line_parser->add_word(std::move(word_parser));
      word_parser = std::make_unique<WordParser>();
      if (line_parser->is_done()) {
        next_state = ADD_LINE;
      } else {
        next_state = READ_CHAR;
      }
      break;

    case State::ADD_LINE:
      add_line(std::move(line_parser->objectify()));
      line_parser = std::make_shared<LineParser>();
      next_state = READ_CHAR;
      break;

    case State::DONE:
      break;
    }

    state = next_state;
  }
  return std::move(list);
}
