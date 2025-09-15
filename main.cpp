#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <memory>
#include "netlistObjects.h"
#include "line.h"
#include "netlist.h"

std::string FILE_PATH = "files/netlist.cir";

int main() {
  Netlist netlist = Netlist();
  std::unique_ptr<std::fstream> file = netlist.load_file(FILE_PATH); 
  netlist.load_netlist_from_file(file);

  return 0;
  }
