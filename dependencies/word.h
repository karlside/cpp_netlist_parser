#ifndef WORD_H
#define WORD_H

#include <memory>
#include <string>

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
  bool has_value() const { return _has_value; }

  void set_done(bool input = true);
  bool is_done() const { return _is_done; }
  bool is_end_of_line() const { return _is_end_of_line; }
  bool is_attach_to_prev() const { return _attach_to_prev; }

  virtual void set_key(std::string input);
  virtual void set_value(std::string input);
  virtual std::string get_key() const { return key; }
  virtual std::string get_value() const { return value; }
  virtual std::string get_text() const;

  friend std::ostream &operator<<(std::ostream &os, const Word &rhs);
  friend std::unique_ptr<Word> operator+(std::unique_ptr<Word> lhs,
                                         const std::unique_ptr<Word> &rhs);

  virtual ~Word() = default;

private:
protected:
  std::string text;
  std::string key;
  std::string value;

  bool _is_active{false};
  bool _has_value{false};
  bool _has_been_parsed{false};
  bool _attach_to_prev{false};
  bool _is_done{false};
  bool _is_end_of_line{false};
  bool _skip_whitespace{false};
  bool _add_whitespace{false};

  void set_end_of_line();
};

#endif
