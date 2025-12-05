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

  // ListOfWords myWords = ListOfWords();
  // std::shared_ptr<StatementWord> word0 =
  //     std::make_shared<StatementWord>(std::make_unique<std::string>("word0"));
  // std::shared_ptr<StatementWord> word1 =
  //     std::make_shared<StatementWord>(std::make_unique<std::string>("word0"));
  // myWords.add_word(word0);
  // myWords.add_word(word1);
  // std::cout << myWords << std::endl;
  return 0;
}
