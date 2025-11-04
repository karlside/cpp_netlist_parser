#include "netlist.h"
#include "line.h"
#include <fstream>
#include <iostream>
#include <memory>
#include <stdexcept>

void Netlist::add_line(std::unique_ptr<Line> line) {
  list.push_back(std::move(line));
}

std::unique_ptr<std::fstream> Netlist::load_file(std::string file_path) {
  std::unique_ptr<std::fstream> file =
      std::make_unique<std::fstream>(file_path);
  // if (!*file || file->is_open()) {
  if (!*file) {
    // TODO: Throw exception
    throw std::runtime_error("Failed to open file: " + file_path);
  }
  return file;
}

void Netlist::load_netlist_from_file(
    const std::unique_ptr<std::fstream> &file) {
  char ch;
  std::unique_ptr<Line> line;
  std::string word;
  enum State { INIT, READ_CHAR, ADD_LINE };
  State state{INIT};
  State next_state{INIT};

  while (true) {
    if (INIT == state) {
      line = std::make_unique<Line>();
      next_state = READ_CHAR;
    }

    if (READ_CHAR == state) {
      ch = file->get();
      if (!*file)
        break;
      if (!std::isspace(ch)) {
        // std::cout << "CHAR: " << ch << std::endl;
        word += ch;
      } else {
        line->add(word);
        word = "";
        if ('\n' == ch) {
          next_state = ADD_LINE;
        }
      }
    }

    if (ADD_LINE == state) {
      add_line(std::move(line));
      next_state = INIT;
    }

    state = next_state;
  }
}

std::ostream &operator<<(std::ostream &os, const Netlist &rhs) {
  for (const std::unique_ptr<Line> &line : rhs.list) {
    std::cout << *line << std::endl;
  }
  return os;
}
