#ifndef LINE_H
#define LINE_H

#include "word.h"
#include <memory>
#include <string>
#include <vector>

class Line {
public:
  Line();
  Line(std::unique_ptr<std::vector<std::unique_ptr<Word>>> input);
  void add(std::unique_ptr<Word> word);
  std::unique_ptr<Word> pop_word();
  std::unique_ptr<Word> *at(int index) const;
  std::string get_text() const;
  int length() const;
  bool is_done() const;
  std::unique_ptr<Line> objectify(); // Return a 'dynamicly dispateched' line
  // object based in line content

  friend std::ostream &operator<<(std::ostream &os, const Line &rhs);

  virtual ~Line() = default;

private:
protected:
  bool _is_done{false};
  std::unique_ptr<std::vector<std::unique_ptr<Word>>> entries{};
};

class SimulatorStatement : public Line {
public:
  SimulatorStatement(std::unique_ptr<std::vector<std::unique_ptr<Word>>> input);

private:
protected:
  void analyse_entries();
};

#endif
