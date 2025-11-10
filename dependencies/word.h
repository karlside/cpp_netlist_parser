#ifndef WORD_H
#define WORD_H

#include <string>

class Word {
public:
  Word();
  Word(std::string input);
  void add_char(char input);
  void add_string(std::string input);
  void parse();
  bool has_been_parsed() const;

  void activate();
  void deactivate();
  bool is_active() const;
  bool has_value() const;

  virtual void set_key(std::string input);
  virtual void set_value(std::string input);
  virtual std::string get_key() const;
  virtual std::string get_value() const;
  virtual std::string get_text() const;

  friend std::ostream &operator<<(std::ostream &os, const Word &rhs);

  virtual ~Word() = default;

private:
protected:
  std::string text;
  std::string key;
  std::string value;
  bool _is_active{false}; // is set when being parsed
  bool _has_value{false}; // is set when being parsed
  bool _has_been_parsed{false};
};

#endif
