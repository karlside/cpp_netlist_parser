#ifndef WORD_H
#define WORD_H

#include <memory>
#include <string>
#include <unordered_map>

class Word {
public:
  Word();
  Word(std::string input);

  void add_char(char input);
  void add_string(std::string input);
  void parse();
  bool has_been_parsed() const { return _has_been_parsed; }

  void activate() { _is_active = true; }
  void deactivate() { _is_active = false; }
  bool is_active() const { return _is_active; }

  bool is_done() const { return _is_done; }
  bool is_end_of_line() const { return _is_end_of_line; }
  bool is_append_to_prev_word() const { return _append_to_prev_word; }

  virtual std::string get_text() const;

  friend std::ostream &operator<<(std::ostream &os, const Word &rhs);
  friend std::unique_ptr<Word> operator+(std::unique_ptr<Word> lhs,
                                         const std::unique_ptr<Word> &rhs);

  virtual ~Word() = default;

private:
  bool _has_been_parsed{false};
  bool _is_done{false};
  void set_done() { _is_done = true; }

protected:
  std::string text;

  const void check_is_parsed_or_done();
  const void check_has_been_parsed();
  const void check_is_done();

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

class KeyValueWord : Word {
public:
  KeyValueWord(std::string input);
  void set_key(std::string input) { key = input; }
  void set_value(std::string input) { value = input; }
  std::string get_key() const { return key; }
  std::string get_value() const { return value; }
  std::string get_text() const;
  bool has_value() const { return _has_value; }

private:
  const bool _is_done{true};
  const bool _has_been_parsed{true};
  std::string key;
  std::string value;
  bool _has_value{false};
};

#endif
