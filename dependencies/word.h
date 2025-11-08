#ifndef WORD_H
#define WORD_H

#include <string>

class BaseWord {
public:
  void activate();
  void deactivate();
  bool is_active() const;
  bool has_value() const;

  virtual void set_key(std::string input);
  virtual void set_value(std::string input);
  virtual std::string get_key() const;
  virtual std::string get_value() const;
  virtual std::string get_text() const;

  friend std::ostream &operator<<(std::ostream &os, const BaseWord &rhs);

  virtual ~BaseWord() = default;

private:
protected:
  std::string key;
  std::string value{};
  bool _is_active{true};
  bool _has_value{true};
};

class Word : public BaseWord {
public:
  Word(std::string input);

  virtual void set_key(std::string input);
  virtual void set_value(std::string input);
  virtual std::string get_key() const;
  virtual std::string get_value() const;
  virtual std::string get_text() const;

private:
};

#endif
