#ifndef WORD_H
#define WORD_H

#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>

class Word {
public:
  Word() {}
  Word(std::string input) { add_string(input); }
  Word(const Word *input_word) { append_word(*input_word); }

  void add_char(char input);
  void add_string(std::string input);
  void append_word(const Word &input_word);
  std::unique_ptr<Word> objectify() const;
  virtual void parse();
  bool has_been_parsed() const { return _has_been_parsed; }

  void activate() { _is_active = true; }
  void deactivate() { _is_active = false; }
  bool is_active() const { return _is_active; }

  bool is_done() const { return _is_done; }
  bool is_end_of_line() const { return _is_end_of_line; }
  bool is_append_to_prev_word() const { return _append_to_prev_word; }

  virtual std::string get_text() const { return text; }

  friend std::ostream &operator<<(std::ostream &os, const Word &rhs);

  virtual ~Word() = default;

private:
  bool _has_been_parsed{false};
  bool _is_done{false};
  void set_done() { _is_done = true; }
  bool is_double_whitespace(char ch) const;

protected:
  std::string text;

  bool _is_active{false};
  bool _append_to_prev_word{false};
  bool _is_end_of_line{false};
  bool _skip_whitespace{false};
  bool _add_whitespace{false};

  void set_end_of_line();
  void set_append_to_prev_word(char ch);
  void set_skip_whitespace(char ch);
  void set_add_whitespace(char ch);
  void clear_whitespace_flag(char ch);

  enum Keyword { NONE, ANY, CLOSING_PARENTHESIS };
  Keyword keyword;
  const std::unordered_map<char, Keyword> keyword_map = {
      {'=', ANY}, {'(', CLOSING_PARENTHESIS}};
};

class KeyValueWord : public Word {
public:
  KeyValueWord();
  KeyValueWord(std::string input) { add_string(input); }
  KeyValueWord(const Word *input_word) { append_word(*input_word); }

  void set_key(std::string input) { key = input; }
  void set_value(std::string input) { value = input; }
  std::string get_key() const { return key; }
  std::string get_value() const { return value; }
  std::string get_text() const;
  bool has_value() const { return _has_value; }
  virtual void parse();

private:
  const bool _is_done{true};
  bool _has_been_parsed{false};
  std::string key;
  std::string value;
  bool _has_value{false};
};

class PortWord : public Word {
public:
  PortWord();
  PortWord(std::string input) { add_string(input); };
  PortWord(const Word *input_word) { append_word(*input_word); }

  std::string get_text() const { return "(" + text + ")"; }
  void parse();

private:
  const bool _is_done{true};
  bool _has_been_parsed{false};
};

#endif
