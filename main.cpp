#include "dependencies/line.h"
#include "dependencies/netlist.h"
#include "dependencies/netlistObjects.h"
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

std::string FILE_PATH = "files/netlist.cir";

int main() {
  // Netlist netlist = Netlist();
  // std::unique_ptr<std::fstream> file = netlist.load_file(FILE_PATH);
  // netlist.load_netlist_from_file(file);
  std::unique_ptr<Line> my_line;
  my_line->add("test");
  my_line->add("line");
  Netlist my_netlist;
  my_netlist.add_line(std::move(my_line));
  std::cout << my_netlist << std::endl;
  return 0;
}
