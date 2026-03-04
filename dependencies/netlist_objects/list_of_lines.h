#ifndef LIST_OF_LINES_H
#define LIST_OF_LINES_H

#include "netlist_objects/line.h"

// -------------------
// --- ListOfLines ---
// -------------------

struct ListOfLines : ListOfTypes<Line> {
  std::shared_ptr<Line> get_line(std::string key) {
    return ListOfTypes::get_item(key);
  }
};

#endif
