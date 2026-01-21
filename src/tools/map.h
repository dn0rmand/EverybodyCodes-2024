#ifndef TOOLS_MAP_H
#define TOOLS_MAP_H

#include "./readData.h"
#include <string.h>

namespace tools {
class Map {
private:
  char *data;
  bool autoDelete;

  void initialize(const void *input);

protected:
  int getIndex(int x, int y);

public:
  int width;
  int height;

public:
  Map(const char *day, int part);

  Map(const void *input, bool autoDelete = true);

  ~Map();

  virtual char get(int x, int y);

  virtual char set(int x, int y, const char c);
};
} // namespace tools
#endif
