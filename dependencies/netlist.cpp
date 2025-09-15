#include "line.h"
#include "netlist.h"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <memory>

void Netlist::add_line(std::unique_ptr<Line> line) {
  list.push_back(std::move(line));
}

std::unique_ptr<std::fstream> Netlist::load_file(std::string file_path) {
  std::unique_ptr<std::fstream> file = std::make_unique<std::fstream>(file_path);
  //if (!*file || file->is_open()) {
  if (!*file ) {
    // TODO: Throw exception 
    throw std::runtime_error("Failed to open file: " + file_path);
  }
  return file; 
}

void Netlist::load_netlist_from_file(std::unique_ptr<std::fstream> &file) {
  char ch;
  std::unique_ptr<Line> line;
  std::string word;

  enum State {
    INIT,
    READ_CHAR,
    ADD_LINE
  };
  State state{INIT};
  State next_state{INIT};

  while (true) {
    if (INIT == state) {
      std::cout << "INIT" << std::endl;
      line = std::make_unique<Line>();
      next_state = READ_CHAR;
    }

    if (READ_CHAR == state) {
      std::cout << "READ_CHAR" << std::endl;
      ch = file->get(); 
      if (!*file) break;
      if (!std::isspace(ch)) {
        word += ch;
      } else {
        if ("" != word) line->add(word);
        if ("\\" == word) {
          line->add(word);
          ch = file->get(); // Read the new_line char, but do nothing with it.
          // TODO: read_until_new_line()
        }
        word = "";
        if ('\n' == ch) next_state == ADD_LINE;
      }
    }

    if (ADD_LINE == state) {
      std::cout << "ADD_LINE" << std::endl;
      add_line(std::move(line));
      next_state = INIT;
    }
  state = next_state;
  } 
}

std::ostream& operator<<(std::ostream& os, const Netlist& rhs) {
  for (const std::unique_ptr<Line>& line: rhs.list) {
    std::cout << *line << std::endl;   
  }
  return os;
}
