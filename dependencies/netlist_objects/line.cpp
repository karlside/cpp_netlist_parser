#include "line.h"
#include <cassert>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

// -----------------
// --- Statement ---
// -----------------

Statement::Statement(std::shared_ptr<ListOfWords> input) : list{input} {}

std::string Statement::get_text() const {
  std::string ret_text;
  for (auto &wordItem : *list) {
    if (!wordItem.item->is_active())
      continue;
    std::string word_text = wordItem.item->get_text();
    ret_text += word_text;
    if (R"(\\)" == word_text)
      ret_text += "\n";
    else
      ret_text += " ";
  }
  if (!ret_text.empty()) // Undefiend behavior if the text is empty
    ret_text.pop_back(); // Remove last whitespace
  return ret_text;
}

std::ostream &operator<<(std::ostream &os, Statement &rhs) {
  os << rhs.get_text();
  return os;
}

std::string Statement::print_list() const {
  std::string ret_str;
  for (auto wordItem : *list) {
    ret_str += "ID: '" + wordItem.key + "' - Text: '" +
               wordItem.item->get_text() + "'\n";
  }
  return ret_str;
}

// --------------------------
// --- SimulatorStatement ---
// --------------------------

// ---------------------
// --- PortStatement ---
// ---------------------
//
// PortStatement::PortStatement(
//     std::shared_ptr<std::vector<std::shared_ptr<StatementWord>>> input) {
//   // list = std::move(input);
//   Statement(input);
// }
