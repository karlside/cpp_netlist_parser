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

  // Word my_word("(1 2 3)");
  // std::shared_ptr<StatementWord> stWord = my_word.objectify();
  // std::cout << my_word.get_text() << std::endl;
  // std::cout << stWord->get_text() << std::endl;
  return 0;
}
