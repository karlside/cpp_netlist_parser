#include "netlist.h"
#include "line.h"
#include "word.h"
#include <fstream>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <unordered_map>

std::unordered_map<std::string, std::string> ignore_newline_keywords = {
    {R"(\\)", "\n"}, {"subckt", "ends"}};

void Netlist::add_line(std::shared_ptr<Statement> line) {
  list.push_back(std::move(line));
}

std::shared_ptr<Statement> Netlist::pop_line() {
  std::shared_ptr<Statement> temp = std::move(list.back());
  list.pop_back();
  return std::move(temp);
}

std::unique_ptr<std::fstream> Netlist::load_file(std::string file_path) {
  std::unique_ptr<std::fstream> file =
      std::make_unique<std::fstream>(file_path);
  // if (!*file || file->is_open()) {
  if (!*file) {
    // TODO: Throw exception
  }
  return file;
}

std::shared_ptr<Word> Netlist::append_to_prev_word(std::shared_ptr<Line> &line,
                                                   std::shared_ptr<Word> word) {
  if (0 == line->length()) {
    return word;
  }
  std::shared_ptr<Word> ret_word = std::move(line->pop_word());
  ret_word->append_word(*word);
  return ret_word;
}

enum State { START, READ_CHAR, ADD_WORD, ADD_LINE, DONE };

void print_state(State state) {
  switch (state) {
  case START:
    std::cout << "START";
    break;
  case READ_CHAR:
    std::cout << "READ_CHAR";
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
  std::shared_ptr<Line> line;
  std::shared_ptr<Word> word;
  State state{START};
  State next_state{START};
  bool ignore_newline{false};
  std::string keyword;

  while (State::DONE != state) {
    // print_state(state);

    switch (state) {

    case State::START:
      word = std::make_shared<Word>();
      line = std::make_shared<Line>();
      next_state = READ_CHAR;
      break;

    case State::READ_CHAR:
      ch = file->get();
      if (!*file) {
        next_state = DONE;
        break;
      }
      word->add_char(ch);
      if (word->is_append_to_prev_word()) {
        word = append_to_prev_word(line, std::move(word));

      } else if (word->is_done()) {
        next_state = ADD_WORD;
      } else {
        next_state = READ_CHAR;
      }
      break;

    case State::ADD_WORD:
      word = word->objectify();
      word->parse();
      line->add_word(std::move(word));
      word = std::make_shared<Word>();
      if (line->is_done()) {
        next_state = ADD_LINE;
      } else {
        next_state = READ_CHAR;
      }
      break;

    case State::ADD_LINE:
      add_line(std::move(line->objectify()));
      line = std::make_shared<Line>();
      next_state = READ_CHAR;
      break;

    case State::DONE:
      break;
    }

    state = next_state;
  }
}

std::ostream &operator<<(std::ostream &os, const Netlist &rhs) {
  for (const std::shared_ptr<Statement> &statement : rhs.list) {
    // std::cout << line->get_text() << std::endl;
    os << statement->get_text() << std::endl;
  }
  return os;
}
