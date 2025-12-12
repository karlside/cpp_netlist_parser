#ifndef WORD_H
#define WORD_H

#include "keywords.h"
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>

// ---------------------
// --- StatementWord ---
// ---------------------

class StatementWord {
public:
  StatementWord() {}
  StatementWord(std::unique_ptr<std::string> input) { text = std::move(input); }

  void activate() { _is_active = true; }
  void deactivate() { _is_active = false; }
  bool is_active() const { return _is_active; }

  virtual ObjectType get_keyword() const { return _keyword; }
  const std::string &get_text() const { return *text; }
  virtual std::string produce_id() { return *text; }
  virtual std::string print_list();

  virtual ~StatementWord() = default;

protected:
  const ObjectType _keyword{ObjectType::NONE};
  bool _is_active{true};
  bool _has_been_parsed{false};
  bool has_been_parsed() const { return _has_been_parsed; }

  std::shared_ptr<std::string> text;
  void set_text(std::string input) { *text = input; }
  virtual void parse() { _has_been_parsed = true; }
};

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

  enum CharKeyword { NONE, ANY, OPENING_PARENTHESIS, CLOSING_PARENTHESIS };
  CharKeyword charKeyword;
  const std::unordered_map<char, CharKeyword> char_keyword_map = {
      {'=', ANY}, {'(', OPENING_PARENTHESIS}, {')', CLOSING_PARENTHESIS}};
};

// --------------------
// --- KeywordWord---
// --------------------

class KeywordWord : public StatementWord {
public:
  KeywordWord(std::unique_ptr<std::string> input, ObjectType keyword)
      : StatementWord(std::move(input)), _keyword{keyword} {}
  ObjectType get_keyword() const override { return _keyword; }

private:
  const ObjectType _keyword;
};

// -----------------------
// --- MathConstantWord---
// -----------------------

class MathConstantWord : public StatementWord {
public:
  MathConstantWord(std::unique_ptr<std::string> input, ObjectType keyword)
      : StatementWord(std::move(input)), _keyword{keyword} {}
  ObjectType get_keyword() const override { return _keyword; }

private:
  const ObjectType _keyword;
};

// ---------------------
// --- SimulationWord---
// ---------------------

class SimulationWord : public StatementWord {
public:
  SimulationWord(std::unique_ptr<std::string> input, ObjectType keyword)
      : StatementWord(std::move(input)), _keyword{keyword} {}
  ObjectType get_keyword() const override { return _keyword; }

private:
  const ObjectType _keyword;
};

// -------------------
// --- keyValueWord---
// -------------------

class KeyValueWord : public StatementWord {
public:
  KeyValueWord();
  KeyValueWord(std::unique_ptr<std::string> input)
      : StatementWord(std::move(input)) {}

  ObjectType get_keyword() const { return _keyword; }
  std::string produce_id() { return get_key(); }
  virtual std::string print_list();

  // void set_key(std::string input);
  void set_value(std::string input);
  const std::string &get_key();
  const std::string &get_value();
  // const std::string &get_text();
  bool has_value() const { return _has_value; }
  void parse();

private:
  ObjectType _keyword{ObjectType::KEYVALUE};
  std::string key;
  std::string value;
  bool _has_value{false};
  void build_text();
};

// ---------------
// --- PortWord---
// ---------------

class PortWord : public StatementWord {
public:
  PortWord();
  PortWord(std::unique_ptr<std::string> input)
      : StatementWord(std::move(input)) {}

  ObjectType get_keyword() const { return _keyword; }
  std::string produce_id() { return "port"; }
  std::string print_list();

  void parse();
  const std::string &get_port();
  void set_port(std::string input);

private:
  const ObjectType _keyword{ObjectType::PORT};
  std::string port;
  void build_text() { *text = "(" + port + ")"; }
};

#endif
