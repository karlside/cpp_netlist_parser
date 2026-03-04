#include "netlist.h"
// #include "line.h"
// #include "word.h"
#include <fstream>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <unordered_map>

std::unique_ptr<std::fstream> Netlist::load_file(std::string file_path) {
  std::unique_ptr<std::fstream> file =
      std::make_unique<std::fstream>(file_path);
  // if (!*file || file->is_open()) {
  if (!*file) {
    // TODO: Throw exception
  }
  return file;
}

std::unique_ptr<WordParser>
Netlist::merge_to_prev_word(std::shared_ptr<LineParser> &line,
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

std::string Netlist::print_list() const {
  std::string ret_str;
  for (auto wordItem : *list) {
    ret_str += "ID: '" + wordItem.key + "' - Text: '" +
               wordItem.item->get_text() + "'\n";
  }
  return ret_str;
}

void Netlist::load_netlist_from_file(
    const std::unique_ptr<std::fstream> &file) {}

std::ostream &operator<<(std::ostream &os, const Netlist &rhs) {
  for (const auto &statementLine : *rhs.list) {
    // std::cout << line->get_text() << std::endl;
    os << statementLine.item->get_text() << std::endl;
  }
  return os;
}
