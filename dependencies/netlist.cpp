#include "netlist.h"
#include "line.h"
#include <fstream>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <unordered_map>

std::unordered_map<std::string, std::string> ignore_newline_keywords = {
    {R"(\\)", "\n"}, {"subckt", "ends"}};

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

enum State {
  START,
  NEW_LINE,
  READ_CHAR,
  ADD_CHAR,
  ADD_WORD,
  ADD_LINE,
  READ_CHAR_IGNORE_NEWLINE,
  ADD_CHAR_IGNORE_NEWLINE,
  ADD_WORD_IGNORE_NEWLINE,
  DONE
};

void print_state(State state) {
  switch (state) {
  case START:
    std::cout << "START";
    break;
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
  case READ_CHAR_IGNORE_NEWLINE:
    std::cout << "READ_CHAR_IGNORE_NEWLINE";
    break;
  case ADD_CHAR_IGNORE_NEWLINE:
    std::cout << "ADD_CHAR_IGNORE_NEWLINE";
    break;
  case ADD_WORD_IGNORE_NEWLINE:
    std::cout << "ADD_WORD_IGNORE_NEWLINE";
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
  std::shared_ptr<Word> word = std::make_shared<Word>();
  State state{START};
  State next_state{START};
  bool ignore_newline{false};
  std::string keyword;

  while (State::DONE != state) {
    // print_state(state);

    switch (state) {

    case State::START:
      next_state = NEW_LINE;
      break;

    case State::NEW_LINE:
      line = std::make_unique<Line>();
      next_state = READ_CHAR;
      break;

    case State::READ_CHAR:
      ch = file->get();
      if (!*file) {
        next_state = DONE;
      } else if (std::isspace(ch)) {
        next_state = ADD_WORD;
      } else {
        next_state = ADD_CHAR;
      }
      break;

    case State::ADD_CHAR:
      word->add_char(ch);
      next_state = READ_CHAR;
      break;

    case State::ADD_WORD:
      if (ignore_newline_keywords.find(word->get_text()) !=
          ignore_newline_keywords.end()) {
        keyword = ignore_newline_keywords.at(word->get_text());
        next_state = READ_CHAR_IGNORE_NEWLINE;
      } else if ('\n' == ch) {
        next_state = ADD_LINE;
      } else {
        next_state = READ_CHAR;
      }
      line->add(word);
      word = std::make_shared<Word>();
      break;

    case State::ADD_LINE:
      // std::unique_ptr<Line> obj_line = line->objectify();
      add_line(std::move(line->objectify()));
      next_state = NEW_LINE;
      break;

    case State::READ_CHAR_IGNORE_NEWLINE:
      ch = file->get();
      if (!*file) {
        // TODO: I might need some exception to make sure the line is added
        next_state = DONE;
      } else if (std::isspace(ch)) {
        next_state = ADD_WORD_IGNORE_NEWLINE;
      } else {
        next_state = ADD_CHAR_IGNORE_NEWLINE;
      }
      break;

    case State::ADD_CHAR_IGNORE_NEWLINE:
      word->add_char(ch);
      next_state = READ_CHAR_IGNORE_NEWLINE;
      break;

    case State::ADD_WORD_IGNORE_NEWLINE:
      if (keyword == word->get_text())
        if ('\n' == ch) {
          next_state = ADD_LINE;
        } else {
          next_state = READ_CHAR;
        }
      else
        next_state = READ_CHAR_IGNORE_NEWLINE;
      line->add(std::move(word));
      word = std::make_unique<Word>();
      break;

    case State::DONE:
      break;
    }

    state = next_state;
  }
}

std::ostream &operator<<(std::ostream &os, const Netlist &rhs) {
  for (const std::unique_ptr<Line> &line : rhs.list) {
    std::cout << line->get_text() << std::endl;
  }
  return os;
}
