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
  std::string const get_text();
  std::string produce_id() { return get_text(); } // TODO: Fix this
  virtual std::string get_list() const;
  std::string print_line();

  friend std::ostream &operator<<(std::ostream &os, Statement &rhs);

  ~Statement() = default;

protected:
  bool _is_active{true};
  std::shared_ptr<ListOfWords> list;
  void parse(); // Analyse the entries in ListOfWords and 'label'
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
  std::string const get_text();
  int length() const { return list->size(); }
  bool is_done() const { return _is_done; }

  std::shared_ptr<Statement> objectify();

  friend std::ostream &operator<<(std::ostream &os, const Line &rhs);

private:
  std::shared_ptr<Statement> get_obj_from_keyword(ObjectType obj_keyword);
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
  ControlStatement(std::shared_ptr<ListOfWords> input) : Statement(input) {
    std::cout << "Creating ControlStatement" << std::endl;
  }
};

// -------------------------
// --- InstanceStatement ---
// -------------------------

class InstanceStatement : public Statement {
public:
  InstanceStatement(std::shared_ptr<ListOfWords> input) : Statement(input) {
    std::cout << "Creating InstanceStatement" << std::endl;
  }
};

// -----------------------
// --- DeviceStatement ---
// -----------------------

class DeviceStatement : public Statement {
public:
  DeviceStatement(std::shared_ptr<ListOfWords> input) : Statement(input) {
    std::cout << "Creating DeviceStatement" << std::endl;
  }
};

// ---------------------------
// --- SimulationStatement ---
// ---------------------------

class SimulationStatement : public Statement {
public:
  SimulationStatement(std::shared_ptr<ListOfWords> input) : Statement(input) {
    std::cout << "Creating SimulationStatemen" << std::endl;
  }
};

// -------------------------------
// --- SkipSecondWordStatement ---
// -------------------------------

class SkipFirstWordStatement : public Statement {
public:
  SkipFirstWordStatement() {};
  SkipFirstWordStatement(std::shared_ptr<ListOfWords> input)
      : Statement(input) {}

  ~SkipFirstWordStatement() = default;
};

class SimulatorStatement : public SkipFirstWordStatement {
public:
  SimulatorStatement(std::shared_ptr<ListOfWords> input)
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
  SkipSecondWordStatement(std::shared_ptr<ListOfWords> input)
      : Statement(input) {}

  ~SkipSecondWordStatement() = default;
};

class PortStatement : public SkipSecondWordStatement {
public:
  PortStatement(std::shared_ptr<ListOfWords> input)
      : SkipSecondWordStatement(input) {
    // std::cout << "Creating PortStatement" << std::endl;
  }
};

#endif
