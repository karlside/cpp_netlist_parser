#ifndef LINE_H
#define LINE_H

#include "word.h"
#include <memory>
#include <string>
#include <vector>

class Statement {
public:
  Statement(std::shared_ptr<std::vector<std::shared_ptr<StatementWord>>> input);

  void activate() { _is_active = true; }
  void deactivate() { _is_active = false; }
  bool is_active() { return _is_active; }

  void add_param(
      std::string input_string); // Parse the input and turn it into a word
  std::string get_text() const;
  virtual std::string get_list() const;

  friend std::ostream &operator<<(std::ostream &os, const Statement &rhs);

  ~Statement() = default;

protected:
  bool _is_active;
  std::shared_ptr<std::vector<std::shared_ptr<StatementWord>>> list;
};

class Line {
public:
  Line();
  Line(std::shared_ptr<std::vector<std::shared_ptr<StatementWord>>> input);
  void add_word(std::unique_ptr<Word> word);
  std::shared_ptr<StatementWord> pop_word();
  std::shared_ptr<StatementWord> *at(int index) const;
  std::string get_text() const;
  int length() const;
  bool is_done() const;
  std::shared_ptr<Statement>
  objectify(); // Return a 'dynamicly dispateched' line
  // object based in line content

  friend std::ostream &operator<<(std::ostream &os, const Line &rhs);

  virtual ~Line() = default;

private:
protected:
  bool _is_done{false};
  std::shared_ptr<std::vector<std::shared_ptr<StatementWord>>> entries{};
};

// class FirstWordDefinedStatement : public Statement {
// public:
//   std::string get_list(); // Return the list of param, but skip the first
//   entire
// };
#endif
