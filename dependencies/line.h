#ifndef LINE_H
#define LINE_H

#include "word.h"
#include <map>
#include <memory>
#include <string>
#include <vector>

// ------------
// --- List ---
// ------------

struct ListOfWords {
public:
  void add_word(std::shared_ptr<StatementWord> input);
  std::shared_ptr<StatementWord> get_word(std::string key);
  std::shared_ptr<StatementWord> pop_word();

  friend std::ostream &operator<<(std::ostream &os, const ListOfWords &rhs);

private:
  std::vector<std::shared_ptr<StatementWord>> words;
  std::unordered_map<std::string, int> index;
  std::string create_key(std::string key, int iterator = 0);
};

// -----------------
// --- Statement ---
// -----------------

class Statement {
public:
  Statement() {};
  Statement(std::shared_ptr<std::vector<std::shared_ptr<StatementWord>>> input);

  void activate() { _is_active = true; }
  void deactivate() { _is_active = false; }
  bool is_active() const { return _is_active; }

  void add_param(
      std::string input_string); // Parse the input and turn it into a word
  const std::string &get_text();
  virtual std::string get_list() const;

  friend std::ostream &operator<<(std::ostream &os, const Statement &rhs);

  ~Statement() = default;

protected:
  bool _is_active;
  std::shared_ptr<std::vector<std::shared_ptr<StatementWord>>> list;
  std::string text;
  void build_text();
  bool _text_is_built{false};
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
    return &list->at(index);
  }
  const std::string &get_text();
  int length() const { return list->size(); }
  bool is_done() const { return _is_done; }

  std::shared_ptr<Statement> objectify();

  friend std::ostream &operator<<(std::ostream &os, const Line &rhs);

  virtual ~Line() = default;

private:
protected:
  std::shared_ptr<Statement> get_obj_from_keyword(ObjectType obj_keyword);
  bool _is_done{false};
  bool _text_is_built{false};
  std::shared_ptr<std::vector<std::shared_ptr<StatementWord>>> list{};
  void build_text();
  std::string text;
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
