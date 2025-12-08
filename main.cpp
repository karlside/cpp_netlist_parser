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

  // std::shared_ptr<StatementWord> word0 = std::make_shared<StatementWord>(
  //     std::make_unique<std::string>("lang=spectre"));
  // std::shared_ptr<StatementWord> word1 =
  //     std::make_shared<StatementWord>(std::make_unique<std::string>("(1 2
  //     3)"));
  // myWords->push_back(word0);
  // myWords->push_back(word1);
  // Statement myLine = Statement(myWords);
  //
  // std::cout << myLine << std::endl;
  // std::cout << myLine.print_line() << std::endl;
  // myLine.get_word("(1 2 3)")->deactivate();
  // std::cout << myLine << std::endl;
  // return 0;

  // TODO:
  // Calling print_line need to parse the objects.
}
