#ifndef PARSING_OBJECTS_H
#define PARSING_OBJECTS_H

#include "exceptions.h"
#include "keywords.h"
#include "line.h"
#include "word.h"
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>

// ------------
// --- Word ---
// ------------

class Word {
public:
  Word() : text(std::make_unique<std::string>()) {}
  Word(std::string input) : text(std::make_unique<std::string>()) {
    add_string(input);
  }
  Word(const Word *input_word) { append_word(*input_word); }

  void add_char(char input);
  void add_string(std::string input);
  void append_word(const Word &input_word);
  void merge_word_in_front(const std::string text);
  std::shared_ptr<StatementWord> objectify();

  bool is_done() const { return _is_done; }
  bool is_end_of_line() const { return _is_end_of_line; }
  bool is_append_to_prev_word() const { return _append_to_prev_word; }
  void clear(); // TODO: clear the text and flag

  virtual const std::string &get_text() const { return *text; }

  friend std::ostream &operator<<(std::ostream &os, const Word &rhs);

  virtual ~Word() = default;

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

class Line {
public:
  Line() : list{std::make_shared<ListOfWords>()} {};
  Line(std::shared_ptr<ListOfWords> input) : list{input} {};
  Line(std::string input);
  void add_word(std::unique_ptr<Word> word);
  std::shared_ptr<StatementWord> pop_word() { return list->pop_back(); }
  std::shared_ptr<StatementWord> at(int index) const { return list->at(index); }
  std::string get_text() const;
  int length() const { return list->size(); }
  bool is_done() const { return _is_done; }

  std::shared_ptr<Statement> objectify();

  friend std::ostream &operator<<(std::ostream &os, const Line &rhs);

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

#endif
