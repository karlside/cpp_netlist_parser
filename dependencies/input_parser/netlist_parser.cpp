#include "input_parser/netlist_parser.h"
#include <cassert>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>

// ---------------------
// --- NetlistParser ---
// ---------------------

std::unique_ptr<WordParser>
NetlistParser::merge_to_prev_word(std::shared_ptr<LineParser> &line,
                                  std::unique_ptr<WordParser> word) {
  if (0 == line->length()) {
    return word;
  }
  std::shared_ptr<Word> statementWord = line->pop_word();
  // Because the word retrived from the line is already objectified it needs to
  // be prepended to the unobjectified word
  word->merge_word_in_front(statementWord->get_text());
  return word;
}

void NetlistParser::print_state(State state) {
  switch (state) {
  case NetlistParser::START:
    std::cout << "START";
    break;
  case NetlistParser::READ_CHAR:
    std::cout << "READ_CHAR";
    break;
  case NetlistParser::ADD_WORD:
    std::cout << "ADD_WORD";
    break;
  case NetlistParser::ADD_LINE:
    std::cout << "ADD_LINE";
    break;
  case NetlistParser::DONE:
    std::cout << "DONE";
    break;
  default:
    std::cout << "UNKNOWN_STATE";
    break;
  }
  std::cout << std::endl;
}

std::unique_ptr<ListOfLines> NetlistParser::parse_netlist(std::fstream &file) {
  list = std::make_unique<ListOfLines>();
  char ch;
  std::unique_ptr<WordParser> word_parser;
  std::shared_ptr<LineParser> line_parser;
  State state{START};
  State next_state{START};
  bool ignore_newline{false};
  std::string keyword;

  while (State::DONE != state) {
    // print_state(state);

    switch (state) {

    case State::START:
      word_parser = std::make_unique<WordParser>();
      line_parser = std::make_shared<LineParser>();
      next_state = READ_CHAR;
      break;

    case State::READ_CHAR:
      ch = file.get();
      if (!file) {
        next_state = DONE;
        break;
      }
      word_parser->add_char(ch);
      if (word_parser->is_append_to_prev_word()) {
        word_parser = merge_to_prev_word(line_parser, std::move(word_parser));
        next_state = READ_CHAR;
      } else if (word_parser->is_done()) {
        next_state = ADD_WORD;
      } else {
        next_state = READ_CHAR;
      }
      break;

    case State::ADD_WORD:
      line_parser->add_word(std::move(word_parser));
      word_parser = std::make_unique<WordParser>();
      if (line_parser->is_done()) {
        next_state = ADD_LINE;
      } else {
        next_state = READ_CHAR;
      }
      break;

    case State::ADD_LINE:
      add_line(std::move(line_parser->objectify()));
      line_parser = std::make_shared<LineParser>();
      next_state = READ_CHAR;
      break;

    case State::DONE:
      break;
    }

    state = next_state;
  }
  return std::move(list);
}
