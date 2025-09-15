#ifndef LINE_H
#define LINE_H

#include <vector>
#include <string>

class Line {
public:
  void add(std::string word);
  std::string at(int index) const;
  int length() const;

  friend std::ostream& operator<<(std::ostream& os, const Line& rhs);

private:
  std::vector<std::string> entries{};
};

#endif
