#ifndef NETLIST_H
#define NETLIST_H

#include "input_parser/parsing_objects.h"
#include "misc/containers.h"
#include "misc/exceptions.h"
#include "netlist_objects/line.h"
#include "netlist_objects/list_of_lines.h"
#include "netlist_objects/word.h"
#include <fstream>
#include <memory>

class Netlist {
public:
  Netlist() : list{std::make_unique<ListOfLines>()} {}
  void add_line(std::shared_ptr<Line> line) { list->push_back(line); }
  std::shared_ptr<Line> pop_line() { return list->pop_back(); }
  std::shared_ptr<Line> get_line(std::string name) {
    return list->get_line(name);
  }

  void load_netlist_from_file(const std::unique_ptr<std::fstream> &file);
  std::unique_ptr<std::fstream> load_file(std::string file_path);
  virtual std::string print_list() const;

  friend std::ostream &operator<<(std::ostream &os, const Netlist &rhs);

private:
  std::unique_ptr<WordParser>
  merge_to_prev_word(std::shared_ptr<LineParser> &line,
                     std::unique_ptr<WordParser> word);
  std::unique_ptr<ListOfLines> list{};
};

#endif
