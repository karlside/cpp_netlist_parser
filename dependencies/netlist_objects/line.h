#ifndef LINE_H
#define LINE_H

#include "misc/containers.h"
#include "misc/exceptions.h"
#include "netlist_objects/word.h"
#include <map>
#include <memory>
#include <string>
#include <vector>

struct ListOfWords : ListOfTypes<Word> {
  std::shared_ptr<Word> get_word(std::string key) {
    return ListOfTypes::get_item(key);
  }
};

class Line {
public:
  Line() {};
  Line(std::shared_ptr<ListOfWords> input);

  void activate() { _is_active = true; }
  void deactivate() { _is_active = false; }
  bool is_active() const { return _is_active; }
  std::shared_ptr<Word> get_word(std::string name) {
    return list->get_word(name);
  }

  void add_param(std::string input_string); // TODO: Parse the input and turn it
                                            // into a word
  std::string get_text() const;
  virtual std::string produce_id() { return list->at(0)->produce_id(); }
  virtual std::string print_list() const;

  friend std::ostream &operator<<(std::ostream &os, Line &rhs);

  ~Line() = default;

protected:
  bool _is_active{true};
  std::shared_ptr<ListOfWords> list;
};

// ------------------------
// --- ControlStatement ---
// ------------------------

class ControlStatement : public Line {
public:
  ControlStatement(std::shared_ptr<ListOfWords> input) : Line(input) {}
};

// -------------------------
// --- InstanceStatement ---
// -------------------------

class InstanceStatement : public Line {
public:
  InstanceStatement(std::shared_ptr<ListOfWords> input) : Line(input) {}
};

// -----------------------
// --- DeviceStatement ---
// -----------------------

class DeviceStatement : public Line {
public:
  DeviceStatement(std::shared_ptr<ListOfWords> input) : Line(input) {}
};

// ---------------------------
// --- SimulationStatement ---
// ---------------------------

class SimulationStatement : public Line {
public:
  SimulationStatement(std::shared_ptr<ListOfWords> input) : Line(input) {}
};

#endif
