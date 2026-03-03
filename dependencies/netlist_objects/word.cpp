#include "netlist_objects/word.h"
#include <cassert>
#include <iostream>
#include <sstream>

// ---------------------
// --- StatementWord ---
// ---------------------

std::string StatementWord::print_list() {
  std::string ret_text = "text:'" + get_text() + "'";
  if (!is_active())
    ret_text += " - DEACTIVATED";
  return ret_text;
}

std::ostream &operator<<(std::ostream &os, const StatementWord &rhs) {
  os << rhs.get_text();
  return os;
}

// ---------------------
// --- SimulatorWord ---
// ---------------------

// --------------------
// --- KeyValueWord ---
// --------------------

void KeyValueWord::parse() {
  std::istringstream iss(*text);
  std::getline(iss, key, '=');
  std::getline(iss, value);
  if (!value.empty())
    _has_value = true;
  _has_been_parsed = true;
}

void KeyValueWord::build_text() {
  if (has_value())
    *text = key + "=" + value;
  else
    *text = key;
}

// void KeyValueWord::set_key(std::string input) {
//   if (!has_been_parsed())
//     parse();
//   key = input;
//   build_text();
// }

void KeyValueWord::set_value(std::string input) {
  if (!has_been_parsed())
    parse();
  value = input;
  build_text();
}

const std::string &KeyValueWord::get_key() {
  if (!has_been_parsed())
    parse();
  return key;
}

const std::string &KeyValueWord::get_value() {
  if (!has_been_parsed())
    parse();
  return value;
}

std::string KeyValueWord::print_list() {
  std::string ret_text = "text:'" + get_text() + "' - key:'" + get_key() +
                         "' - value:'" + get_value() + "'";
  if (!is_active())
    ret_text += " - DEACTIVATED";
  return ret_text;
}

// ----------------
// --- PortWord ---
// ----------------

void PortWord::parse() {
  // TODO: parse don't handle input formating in a good way.
  std::string dump;
  std::istringstream iss(*text);
  std::getline(iss, dump, '(');
  // std::getline(iss, dump, ' ');
  std::getline(iss, port, ')');
  _has_been_parsed = true;
}

void PortWord::set_port(std::string input) {
  // TODO: input formatting?
  port = input;
  build_text();
}

const std::string &PortWord::get_port() {
  if (!has_been_parsed())
    parse();
  return port;
}

std::string PortWord::print_list() {
  std::string ret_text =
      "text:'" + get_text() + "' - port string:'" + get_port() + "'";
  if (!is_active())
    ret_text += " - DEACTIVATED";
  return ret_text;
}
