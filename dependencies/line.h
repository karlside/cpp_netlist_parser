#ifndef LINE_H
#define LINE_H

#include "word.h"
#include <memory>
#include <string>
#include <vector>

// -----------------
// --- Statement ---
// -----------------

class Statement {
public:
  Statement() {};
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

// ------------
// --- Line ---
// ------------

class Line {
public:
  Line();
  Line(std::shared_ptr<std::vector<std::shared_ptr<StatementWord>>> input);
  void add_word(std::unique_ptr<Word> word);
  std::shared_ptr<StatementWord> pop_word();
  std::shared_ptr<StatementWord> *at(int index) const {
    return &entries->at(index);
  }
  std::string get_text() const;
  int length() const { return entries->size(); }
  bool is_done() const { return _is_done; }

  std::shared_ptr<Statement>
  objectify(); // Return a 'dynamicly dispateched' line
  // object based in line content
  std::shared_ptr<Statement> get_obj_from_keyword(ObjectType obj_keyword);

  friend std::ostream &operator<<(std::ostream &os, const Line &rhs);

  virtual ~Line() = default;

private:
protected:
  bool _is_done{false};
  std::shared_ptr<std::vector<std::shared_ptr<StatementWord>>> entries{};
};

// -------------------------------
// --- SkipSecondWordStatement ---
// -------------------------------

class SkipFirstWordStatement : public Statement {
public:
  SkipFirstWordStatement() {};
  SkipFirstWordStatement(
      std::shared_ptr<std::vector<std::shared_ptr<StatementWord>>> input)
      : Statement(input) {}

  ~SkipFirstWordStatement() = default;
};

class SimulatorStatement : public SkipFirstWordStatement {
  // class SimulatorStatement : public Statement {
public:
  SimulatorStatement(
      std::shared_ptr<std::vector<std::shared_ptr<StatementWord>>> input)
      : SkipFirstWordStatement(input) {
    // std::cout << "Creating SimulatorStatement" << std::endl;
  }
};

// -------------------------------
// --- SkipSecondWordStatement ---
// -------------------------------

class SkipSecondWordStatement : public Statement {
public:
  SkipSecondWordStatement() {};
  SkipSecondWordStatement(
      std::shared_ptr<std::vector<std::shared_ptr<StatementWord>>> input)
      : Statement(input) {}

  ~SkipSecondWordStatement() = default;
};

class PortStatement : public SkipSecondWordStatement {
public:
  PortStatement(
      std::shared_ptr<std::vector<std::shared_ptr<StatementWord>>> input)
      : SkipSecondWordStatement(input) {
    // std::cout << "Creating PortStatement" << std::endl;
  }
};

#endif
