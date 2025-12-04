#ifndef WORD_H
#define WORD_H

#include "keywords.h"
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>

// enum WordKeyword { NONE, SIMULATOR, KEYVALUE, PORT };

class StatementWord {
public:
  StatementWord() {}
  StatementWord(std::unique_ptr<std::string> input) { text = std::move(input); }

  void activate() { _is_active = true; }
  void deactivate() { _is_active = false; }
  bool is_active() const { return _is_active; }
  ObjectType get_keyword() const { return _keyword; }

  virtual std::string get_text() const { return *text; }

  virtual ~StatementWord() = default;

protected:
  ObjectType _keyword{ObjectType::NONE};
  bool _is_active{true};
  bool _has_been_parsed{false};
  bool has_been_parsed() const { return _has_been_parsed; }

  std::shared_ptr<std::string> text;
  void add_string(std::string input);
  virtual void parse() { _has_been_parsed = true; }
};

class Word {

public:
  Word() : text(std::make_unique<std::string>()) {}
  Word(std::string input) { add_string(input); }
  Word(const Word *input_word) { append_word(*input_word); }

  void add_char(char input);
  void add_string(std::string input);
  void append_word(const Word &input_word);
  void merge_word_in_front(const std::string text);
  std::shared_ptr<StatementWord> objectify();

  bool is_done() const { return _is_done; }
  bool is_end_of_line() const { return _is_end_of_line; }
  bool is_append_to_prev_word() const { return _append_to_prev_word; }

  virtual std::string get_text() const { return *text; }

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

  enum CharKeyword { NONE, ANY, OPENING_PARENTHESIS, CLOSING_PARENTHESIS };
  CharKeyword charKeyword;
  const std::unordered_map<char, CharKeyword> char_keyword_map = {
      {'=', ANY}, {'(', OPENING_PARENTHESIS}, {')', CLOSING_PARENTHESIS}};
};

class SimulatorWord : public StatementWord {
protected:
  ObjectType _keyword{ObjectType::SIMULATOR};
};

class KeyValueWord : public StatementWord {
public:
  KeyValueWord();
  KeyValueWord(std::unique_ptr<std::string> input) { text = std::move(input); }
  // KeyValueWord(const Word *input_word) { append_word(*input_word); }

  void set_key(std::string input) { *key = input; }
  void set_value(std::string input) { *value = input; }
  std::string get_key();
  std::string get_value();
  std::string get_text();
  bool has_value() const { return _has_value; }
  void parse();

private:
  ObjectType _keyword{ObjectType::KEYVALUE};
  const bool _is_done{true};
  std::shared_ptr<std::string> key;
  std::shared_ptr<std::string> value;
  bool _has_value{false};
};

class PortWord : public StatementWord {
public:
  PortWord();
  PortWord(std::unique_ptr<std::string> input) { text = std::move(input); }

  // PortWord(const Word *input_word) { append_word(*input_word); }

  std::string get_text() const;
  void parse();
  std::string get_port();

private:
  ObjectType _keyword{ObjectType::PORT};
  const bool _is_done{true};
};

#endif
