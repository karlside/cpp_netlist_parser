#include <stdexcept>

class WordIsDoneError : public std::runtime_error {
public:
  using std::runtime_error::runtime_error; // inherit constructor
};

class WordIsParsedError : public std::runtime_error {
public:
  using std::runtime_error::runtime_error; // inherit constructor
};
