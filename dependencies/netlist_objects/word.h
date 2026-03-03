#ifndef WORD_H
#define WORD_H

#include "misc/containers.h"
#include "misc/exceptions.h"
#include "misc/keywords.h"
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>

// ---------------------
// --- StatementWord ---
// ---------------------

class StatementWord {
public:
  StatementWord(std::string input)
      : text{std::make_shared<std::string>(input)} {}
  StatementWord(std::unique_ptr<std::string> input) : text{std::move(input)} {}

  void activate() { _is_active = true; }
  void deactivate() { _is_active = false; }
  bool is_active() const { return _is_active; }

  virtual ObjectType get_keyword() const { return _keyword; }
  const std::string &get_text() const { return *text; }
  virtual std::string produce_id() { return get_text(); }
  virtual std::string print_list();

  friend std::ostream &operator<<(std::ostream &os, const StatementWord &rhs);

  virtual ~StatementWord() = default;

protected:
  const ObjectType _keyword{ObjectType::NONE};
  bool _is_active{true};
  bool _has_been_parsed{false};
  bool has_been_parsed() const { return _has_been_parsed; }

  std::shared_ptr<std::string> text;
  void set_text(std::string input) { *text = input; }
  virtual void parse() { _has_been_parsed = true; }
};

// --------------------
// --- KeywordWord---
// --------------------

class KeywordWord : public StatementWord {
public:
  KeywordWord(std::string input) : StatementWord(input) {}
  KeywordWord(std::unique_ptr<std::string> input)
      : StatementWord(std::move(input)) {}
  ObjectType get_keyword() const override { return _keyword; }

private:
  const ObjectType _keyword{ObjectType::KEYWORD};
};

// -----------------------
// --- MathConstantWord---
// -----------------------

class MathConstantWord : public StatementWord {
public:
  MathConstantWord(std::string input) : StatementWord(input) {}
  MathConstantWord(std::unique_ptr<std::string> input)
      : StatementWord(std::move(input)) {}
  ObjectType get_keyword() const override { return _keyword; }

private:
  const ObjectType _keyword{ObjectType::MATH_CONSTANT};
};

// ---------------------
// --- SimulationWord---
// ---------------------

class SimulationWord : public StatementWord {
public:
  SimulationWord(std::string input) : StatementWord(input) {}
  SimulationWord(std::unique_ptr<std::string> input)
      : StatementWord(std::move(input)) {}
  ObjectType get_keyword() const override { return _keyword; }

private:
  const ObjectType _keyword{ObjectType::SIMULATION};
};

// -------------------
// --- keyValueWord---
// -------------------

class KeyValueWord : public StatementWord {
public:
  KeyValueWord(std::string input) : StatementWord(input) {}
  KeyValueWord(std::unique_ptr<std::string> input)
      : StatementWord(std::move(input)) {}

  ObjectType get_keyword() const override { return _keyword; }
  std::string produce_id() override { return get_key(); }
  virtual std::string print_list() override;

  // void set_key(std::string input);
  void set_value(std::string input);
  const std::string &get_key();
  const std::string &get_value();
  // const std::string &get_text();
  bool has_value() const { return _has_value; }
  void parse() override;

private:
  ObjectType _keyword{ObjectType::KEYVALUE};
  std::string key;
  std::string value;
  bool _has_value{false};
  void build_text();
};

// ---------------
// --- PortWord---
// ---------------

class PortWord : public StatementWord {
public:
  PortWord(std::string input) : StatementWord(input) {}
  PortWord(std::unique_ptr<std::string> input)
      : StatementWord(std::move(input)) {}

  ObjectType get_keyword() const override { return _keyword; }
  std::string produce_id() override { return "port"; }
  std::string print_list() override;

  void parse() override;
  const std::string &get_port();
  void set_port(std::string input);

private:
  const ObjectType _keyword{ObjectType::PORT};
  std::string port;
  void build_text() { *text = "(" + port + ")"; }
};

#endif
