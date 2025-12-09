#include "netlist.h"
#include "line.h"
#include "word.h"
#include <fstream>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <unordered_map>

// -------------------
// --- ListOfLines ---
// -------------------

void ListOfLines::push_back(std::shared_ptr<Statement> input_line) {
  //  std::string key = input_word->get_text();
  // TODO: Create new keys if needed
  std::string key = create_key(input_line->produce_id());
  lines.push_back(LineEntry(std::make_pair(key, input_line)));
  index[key] = index.size();
};

std::shared_ptr<Statement> ListOfLines::get_word(std::string key) {
  if (index.find(key) == index.end())
    throw std::runtime_error("Key not in List");
  // TODO: Create new errors
  return lines.at(index.at(key)).line;
}

std::shared_ptr<Statement> ListOfLines::pop_back() {
  LineEntry temp_word = lines.back();
  lines.pop_back();
  index.erase(temp_word.line->get_text());
  return temp_word.line;
}

std::string ListOfLines::create_key(std::string key, int iterator) {
  if (index.find(key) == index.end())
    return key;
  iterator += 1;
  key = key + "_" + std::to_string(iterator);
  return create_key(key, iterator);
}

std::ostream &operator<<(std::ostream &os, const ListOfLines &rhs) {
  for (LineEntry wordPair : rhs.lines)
    os << wordPair.line->get_text() << " ";
  return os;
}

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

std::unique_ptr<Word> Netlist::merge_to_prev_word(std::shared_ptr<Line> &line,
                                                  std::unique_ptr<Word> word) {
  if (0 == line->length()) {
    return word;
  }
  std::shared_ptr<StatementWord> statementWord = line->pop_word();
  // Because the word retrived from the line is already objectified it needs to
  // be prepended to the unobjectified word
  word->merge_word_in_front(statementWord->get_text());
  return word;
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
  std::unique_ptr<Word> word;
  std::shared_ptr<Line> line;
  State state{START};
  State next_state{START};
  bool ignore_newline{false};
  std::string keyword;

  while (State::DONE != state) {
    // print_state(state);

    switch (state) {

    case State::START:
      word = std::make_unique<Word>();
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
        word = merge_to_prev_word(line, std::move(word));
        next_state = READ_CHAR;
      } else if (word->is_done()) {
        next_state = ADD_WORD;
      } else {
        next_state = READ_CHAR;
      }
      break;

    case State::ADD_WORD:
      line->add_word(std::move(word));
      word = std::make_unique<Word>();
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
