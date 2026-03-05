#ifndef PARSING_OBJECTS_H
#define PARSING_OBJECTS_H

#include "input_parser/line_parser.h"
#include "input_parser/word_parser.h"
#include "netlist_objects/line.h"
#include "netlist_objects/list_of_lines.h"
#include <memory>

// ---------------
// --- Netlist ---
// ---------------

class NetlistParser {
public:
  NetlistParser() {};
  std::unique_ptr<ListOfLines> parse_netlist(std::fstream &file);

  enum State { START, READ_CHAR, ADD_WORD, ADD_LINE, DONE };
  void print_state(State state);

private:
  void add_line(std::shared_ptr<Line> line) { list->push_back(line); }
  std::unique_ptr<WordParser>
  merge_to_prev_word(std::shared_ptr<LineParser> &line,
                     std::unique_ptr<WordParser> word);

  std::unique_ptr<ListOfLines> list{};
};

#endif
