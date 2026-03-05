#ifndef LINE_PARSER_H
#define LINE_PARSER_H

#include "input_parser/word_parser.h"
#include "netlist_objects/line.h"
#include "netlist_objects/word.h"
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>

// ------------
// --- Line ---
// ------------

class LineParser {
public:
  LineParser() : list{std::make_shared<ListOfWords>()} {};
  LineParser(std::shared_ptr<ListOfWords> input) : list{input} {};
  LineParser(std::string input);
  void add_word(std::unique_ptr<WordParser> word);
  std::shared_ptr<Word> pop_word() { return list->pop_back(); }
  std::shared_ptr<Word> at(int index) const { return list->at(index); }
  std::string get_text() const;
  int length() const { return list->size(); }
  bool is_done() const { return _is_done; }

  std::shared_ptr<Line> objectify();

  friend std::ostream &operator<<(std::ostream &os, const LineParser &rhs);

private:
  bool _is_done{false};
  bool _ignore_newline{false};
  std::shared_ptr<ListOfWords> list;

  void set_ignore_end_of_line(std::string word);
  void clear_newline_flag(std::string word);
  enum WordKeyword { NONE, ANY, ENDS };
  WordKeyword wordKeyword;
  const std::unordered_map<std::string, WordKeyword> word_keyword_map = {
      {"subckt", ENDS}};

  bool first_word_is_keyword() const;
  bool first_word_is_comment() const;
  bool second_word_is_keyword() const;
  bool second_word_is_port() const;
  bool second_word_is_simulation() const;
};

#endif
