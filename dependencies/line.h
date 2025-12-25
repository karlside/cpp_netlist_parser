#ifndef LINE_H
#define LINE_H

#include "containers.h"
#include "word.h"
#include <map>
#include <memory>
#include <string>
#include <vector>

struct ListOfWords : ListOfTypes<StatementWord> {
  std::shared_ptr<StatementWord> get_word(std::string key) {
    return ListOfTypes::get_item(key);
  }
};

class Statement {
public:
  Statement() {};
  Statement(std::shared_ptr<ListOfWords> input);

  void activate() { _is_active = true; }
  void deactivate() { _is_active = false; }
  bool is_active() const { return _is_active; }
  std::shared_ptr<StatementWord> get_word(std::string name) {
    return list->get_word(name);
  }

  void add_param(std::string input_string); // TODO: Parse the input and turn it
                                            // into a word
  std::string get_text() const;
  virtual std::string produce_id() { return list->at(0)->produce_id(); }
  virtual std::string print_list() const;

  friend std::ostream &operator<<(std::ostream &os, Statement &rhs);

  ~Statement() = default;

protected:
  bool _is_active{true};
  std::shared_ptr<ListOfWords> list;
};

// ------------
// --- Line ---
// ------------

class Line {
public:
  Line();
  Line(std::string input);
  Line(std::shared_ptr<ListOfWords> input);
  void add_word(std::unique_ptr<Word> word);
  std::shared_ptr<StatementWord> pop_word() { return list->pop_back(); }
  std::shared_ptr<StatementWord> at(int index) const { return list->at(index); }
  std::string get_text() const;
  int length() const { return list->size(); }
  bool is_done() const { return _is_done; }

  std::shared_ptr<Statement> objectify();

  friend std::ostream &operator<<(std::ostream &os, const Line &rhs);

private:
  bool _is_done{false};
  std::shared_ptr<ListOfWords> list;
  bool first_word_is_keyword() const;
  bool first_word_is_comment() const;
  bool second_word_is_keyword() const;
  bool second_word_is_port() const;
  bool second_word_is_simulation() const;
};

// ------------------------
// --- ControlStatement ---
// ------------------------

class ControlStatement : public Statement {
public:
  ControlStatement(std::shared_ptr<ListOfWords> input) : Statement(input) {}
};

// -------------------------
// --- InstanceStatement ---
// -------------------------

class InstanceStatement : public Statement {
public:
  InstanceStatement(std::shared_ptr<ListOfWords> input) : Statement(input) {}
};

// -----------------------
// --- DeviceStatement ---
// -----------------------

class DeviceStatement : public Statement {
public:
  DeviceStatement(std::shared_ptr<ListOfWords> input) : Statement(input) {}
};

// ---------------------------
// --- SimulationStatement ---
// ---------------------------

class SimulationStatement : public Statement {
public:
  SimulationStatement(std::shared_ptr<ListOfWords> input) : Statement(input) {}
};

#endif
