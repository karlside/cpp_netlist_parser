#include "netlist.h"
// #include "line.h"
// #include "word.h"
#include <fstream>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <unordered_map>

Netlist::Netlist(const std::string &file_path) {
  std::fstream file = load_file(file_path);
  NetlistParser netlistParser;
  list = netlistParser.parse_netlist(file);
}

Netlist::Netlist(std::fstream &file) {
  NetlistParser netlistParser;
  list = netlistParser.parse_netlist(file);
}

std::fstream Netlist::load_file(std::string file_path) {
  std::fstream file = std::fstream(file_path);
  if (!file) {
    // TODO: Throw exception
  }
  return file;
}

std::string Netlist::print_list() const {
  std::string ret_str;
  for (auto wordItem : *list) {
    ret_str += "ID: '" + wordItem.key + "' - Text: '" +
               wordItem.item->get_text() + "'\n";
  }
  return ret_str;
}

std::ostream &operator<<(std::ostream &os, const Netlist &rhs) {
  for (const auto &statementLine : *rhs.list) {
    // std::cout << line->get_text() << std::endl;
    os << statementLine.item->get_text() << std::endl;
  }
  return os;
}
