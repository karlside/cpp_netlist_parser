#ifndef NETLIST_H
#define NETLIST_H

#include "line.h"
#include <fstream>
#include <memory>

class Netlist {
public:
  void add_line(std::unique_ptr<Line> line);
  std::unique_ptr<Line> pop_line();
  void load_netlist_from_file(const std::unique_ptr<std::fstream> &file);
  std::unique_ptr<std::fstream> load_file(std::string file_path);

  friend std::ostream &operator<<(std::ostream &os, const Netlist &rhs);

private:
  std::unique_ptr<Word> append_to_prev_word(std::unique_ptr<Line> &line,
                                            std::unique_ptr<Word> word);
  std::vector<std::unique_ptr<Line>> list{};
};

#endif
