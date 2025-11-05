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

enum State { NEW_LINE, READ_CHAR, ADD_CHAR, ADD_WORD, ADD_LINE, DONE };
void print_state(State state) {
  // return;
  switch (state) {
  case NEW_LINE:
    std::cout << "NEW_LINE";
    break;
  case READ_CHAR:
    std::cout << "READ_CHAR";
    break;
  case ADD_CHAR:
    std::cout << "ADD_CHAR";
    break;
  case ADD_WORD:
    std::cout << "ADD_WORD";
    break;
  case ADD_LINE:
    std::cout << "ADD_LINE";
    break;
  case DONE:
    std::cout << "DONE";
    break;
  default:
    std::cout << "UNKNOWN_STATE";
    break;
  }
  std::cout << std::endl;
}

void Netlist::load_netlist_from_file(
    const std::unique_ptr<std::fstream> &file) {
  char ch;
  std::unique_ptr<Line> line;
  std::string word;
  State state{NEW_LINE};
  State next_state{NEW_LINE};

  while (State::DONE != state) {
    // print_state(state);

    switch (state) {

    case State::NEW_LINE:
      line = std::make_unique<Line>();
      next_state = READ_CHAR;
      break;

    case State::READ_CHAR:
      ch = file->get();
      if (!*file) {
        next_state = DONE;
      } else if (!std::isspace(ch)) {
        next_state = ADD_CHAR;
      } else {
        next_state = ADD_WORD;
      }
      break;

    case State::ADD_CHAR:
      word += ch;
      next_state = READ_CHAR;
      break;

    case State::ADD_WORD:
      line->add(word);
      word = "";
      if ('\n' == ch) {
        next_state = ADD_LINE;
      } else {
        next_state = READ_CHAR;
      }
      break;

    case State::ADD_LINE:
      add_line(std::move(line));
      next_state = NEW_LINE;
      break;

    case State::DONE:
      break;
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
