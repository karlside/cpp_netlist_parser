#ifndef NETLIST_H
#define NETLIST_H

#include "containers.h"
#include "line.h"
#include <fstream>
#include <memory>

// -------------------
// --- ListOfLines ---
// -------------------

struct ListOfLines : ListOfTypes<Statement> {
  std::shared_ptr<Statement> get_line(std::string key) {
    return ListOfTypes::get_item(key);
  }
};

class Netlist {
public:
  void add_line(std::shared_ptr<Statement> line);
  std::shared_ptr<Statement> pop_line();
  void load_netlist_from_file(const std::unique_ptr<std::fstream> &file);
  std::unique_ptr<std::fstream> load_file(std::string file_path);

  friend std::ostream &operator<<(std::ostream &os, const Netlist &rhs);

private:
  std::unique_ptr<Word> merge_to_prev_word(std::shared_ptr<Line> &line,
                                           std::unique_ptr<Word> word);
  std::vector<std::shared_ptr<Statement>> list{};
};

#endif
