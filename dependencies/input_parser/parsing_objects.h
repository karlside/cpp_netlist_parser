#ifndef PARSING_OBJECTS_H
#define PARSING_OBJECTS_H

#include "misc/exceptions.h"
#include "misc/keywords.h"
#include "netlist_objects/line.h"
// #include "netlist.h"
#include "netlist_objects/list_of_lines.h"
#include "netlist_objects/word.h"
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>

// ------------
// --- Word ---
// ------------

class WordParser {
public:
  WordParser() : text(std::make_unique<std::string>()) {}
  WordParser(std::string input) : text(std::make_unique<std::string>()) {
    add_string(input);
  }
  WordParser(const WordParser *input_word) { append_word(*input_word); }

  void add_char(char input);
  void add_string(std::string input);
  void append_word(const WordParser &input_word);
  void merge_word_in_front(const std::string text);
  std::shared_ptr<Word> objectify();

  bool is_done() const { return _is_done; }
  bool is_end_of_line() const { return _is_end_of_line; }
  bool is_append_to_prev_word() const { return _append_to_prev_word; }
  void clear(); // TODO: clear the text and flag

  virtual const std::string &get_text() const { return *text; }

  friend std::ostream &operator<<(std::ostream &os, const WordParser &rhs);

  virtual ~WordParser() = default;

private:
  bool _is_done{false};
  void set_done() { _is_done = true; }
  bool is_double_whitespace(char ch) const;
  void remove_previous_whitespace();

  std::unique_ptr<std::string> text;

  bool _append_to_prev_word{false};
  bool _is_end_of_line{false};
  bool _ignore_whitespace{false};
  bool _add_whitespace{false};

  void set_end_of_line();
  void set_append_to_prev_word(char ch);
  void set_ignore_whitespace(char ch);
  void set_add_whitespace(char ch);
  void clear_whitespace_flag(char ch);

  bool _type_flag{false};
  bool type_flag_is_set() const { return _type_flag; }
  void set_type_flag(ObjectType input_type);
  void check_char_for_type(char input_char);
  ObjectType object_type{ObjectType::NONE};

  enum CharKeyword { NONE, ANY, OPENING_PARENTHESIS, CLOSING_PARENTHESIS };
  CharKeyword charKeyword;
  const std::unordered_map<char, CharKeyword> char_keyword_map = {
      {'=', ANY}, {'(', OPENING_PARENTHESIS}, {')', CLOSING_PARENTHESIS}};
};

// ------------
// --- Line ---
// ------------

class LineParser {
public:
  LineParser() : list{std::make_shared<ListOfWords>()} {};
  LineParser(std::shared_ptr<ListOfWords> input) : list{input} {};
  LineParser(std::string input);
  void add_word(std::unique_ptr<WordParser> word);
  std::shared_ptr<Word> pop_word() { return list->pop_back(); }
  std::shared_ptr<Word> at(int index) const { return list->at(index); }
  std::string get_text() const;
  int length() const { return list->size(); }
  bool is_done() const { return _is_done; }

  std::shared_ptr<Line> objectify();

  friend std::ostream &operator<<(std::ostream &os, const LineParser &rhs);

private:
  bool _is_done{false};
  bool _ignore_newline{false};
  std::shared_ptr<ListOfWords> list;

  void set_ignore_end_of_line(std::string word);
  void clear_newline_flag(std::string word);
  enum WordKeyword { NONE, ANY, ENDS };
  WordKeyword wordKeyword;
  const std::unordered_map<std::string, WordKeyword> word_keyword_map = {
      {"subckt", ENDS}};

  bool first_word_is_keyword() const;
  bool first_word_is_comment() const;
  bool second_word_is_keyword() const;
  bool second_word_is_port() const;
  bool second_word_is_simulation() const;
};

// ---------------
// --- Netlist ---
// ---------------

enum State { START, READ_CHAR, ADD_WORD, ADD_LINE, DONE };

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

class NetlistParser {
public:
  NetlistParser() : list{std::make_unique<ListOfLines>()} {};

private:
  std::unique_ptr<ListOfLines> list{};
};

// char ch;
// std::unique_ptr<WordParser> word;
// std::shared_ptr<LineParser> line;
// State state{START};
// State next_state{START};
// bool ignore_newline{false};
// std::string keyword;
//
// while (State::DONE != state) {
//   // print_state(state);
//
//   switch (state) {
//
//   case State::START:
//     word = std::make_unique<WordParser>();
//     line = std::make_shared<LineParser>();
//     next_state = READ_CHAR;
//     break;
//
//   case State::READ_CHAR:
//     ch = file->get();
//     if (!*file) {
//       next_state = DONE;
//       break;
//     }
//     word->add_char(ch);
//     if (word->is_append_to_prev_word()) {
//       word = merge_to_prev_word(line, std::move(word));
//       next_state = READ_CHAR;
//     } else if (word->is_done()) {
//       next_state = ADD_WORD;
//     } else {
//       next_state = READ_CHAR;
//     }
//     break;
//
//   case State::ADD_WORD:
//     line->add_word(std::move(word));
//     word = std::make_unique<WordParser>();
//     if (line->is_done()) {
//       next_state = ADD_LINE;
//     } else {
//       next_state = READ_CHAR;
//     }
//     break;
//
//   case State::ADD_LINE:
//     add_line(std::move(line->objectify()));
//     line = std::make_shared<LineParser>();
//     next_state = READ_CHAR;
//     break;
//
//   case State::DONE:
//     break;
//   }
//
//   state = next_state;
// }
//
#endif
