#include "dependencies/line.h"
#include "dependencies/netlist.h"
#include "dependencies/netlistObjects.h"
#include "dependencies/word.h"
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

std::string FILE_PATH = "files/netlist.cir";

int main() {
  Netlist netlist = Netlist();
  std::unique_ptr<std::fstream> file = netlist.load_file(FILE_PATH);
  netlist.load_netlist_from_file(file);

  std::cout << "PRINTING NETLIST:" << std::endl;
  std::cout << netlist << std::endl;

  // Word word0("");
  // Word word1(R"(\\)");
  // Line my_line;
  // my_line.add(word0);
  // my_line.add(word1);
  // std::cout << my_line << std::endl;
  return 0;
}
