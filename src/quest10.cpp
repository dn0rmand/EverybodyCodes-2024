#include "tools/readData.h"
#include <map>
#include <stdio.h>
#include <string.h>

#define DAY "10"

#define HEIGHT 6
#define WIDTH 6

namespace quest10_namespace {
static const std::vector<int> outerIndexes = {0, 1, 6, 7};
static const std::vector<int> innerIndexes = {2, 3, 4, 5};
static const std::vector<int> allIndexes = {0, 1, 2, 3, 4, 5, 6, 7};

class LetterInfo {
public:
  int letter;
  int count;

  LetterInfo() { count = 0; }
};

class MapCollectionBase {
protected:
  char *data;
  int size;
  int lineWidth;
  int width;
  int height;

  virtual void init() = 0;
  virtual int getIndex(int index, int x, int y) = 0;

  int getOffset(int x, int y) {
    int index = (x + y * lineWidth);
    return index;
  }

public:
  int count;

  MapCollectionBase(int part) {
    data = (char *)tools::readData(DAY, part);
    size = strlen(data) + 1;
    lineWidth = tools::stringLength(data) + 1;
  }

  ~MapCollectionBase() { free(data); }

  char get(int index, int x, int y) {
    int idx = getIndex(index, x, y);
    return data[idx];
  }

  void set(int index, int x, int y, char c) {
    int idx = getIndex(index, x, y);
    data[idx] = c;
  }
};

class Map {
private:
  MapCollectionBase *maps;
  int index;

public:
  Map() {
    maps = NULL;
    index = 0;
  }

  void setup(MapCollectionBase *maps, int index) {
    this->maps = maps;
    this->index = index;
  }

  char get(int x, int y) { return maps->get(index, x, y); }

  void set(int x, int y, char c) { maps->set(index, x, y, c); }

  int getPower() {
    int power = 0;
    int pos = 1;
    for (int y : innerIndexes) {
      for (int x : innerIndexes) {
        char c = get(x, y);
        if (c != '.') {
          power += pos * (c - 'A' + 1);
        } else {
          return 0;
        }
        pos++;
      }
    }

    return power;
  }

private:
  int populate(bool questions[8][8]) {
    int missing = 0;

    for (int y : allIndexes) {
      for (int x : allIndexes) {
        char c = get(x, y);

        questions[y][x] = c == '?';

        if (c != '.') {
          continue;
        }

        std::vector<char> row;
        std::vector<char> column;
        std::vector<char> intersection;

        for (int z : outerIndexes) {
          char cc = get(z, y);
          if (cc != '?' && cc != '.') {
            row.push_back(cc);
          }
          cc = get(x, z);
          if (cc != '?' && cc != '.') {
            column.push_back(cc);
          }
        }

        for (char c1 : row) {
          for (char c2 : column) {
            if (c1 == c2) {
              intersection.push_back(c1);
            }
          }
        }

        if (intersection.size() > 0) {
          if (intersection.size() > 1) {
            // Multiple intersections found
          }
          char cc = *intersection.begin();

          set(x, y, cc);
        } else {
          missing++;
        }
      }
    }

    return missing;
  }

  int resolve(bool questions[8][8]) {
    std::map<char, int> columns[8];
    std::map<char, int> rows[8];

    for (int a : allIndexes) {
      for (int b : allIndexes) {
        char c = get(a, b);
        if (c >= 'A' && c <= 'Z') {
          int old = columns[a][c];
          columns[a][c] = old + 1;
        }
        c = get(b, a);
        if (c >= 'A' && c <= 'Z') {
          int old = rows[a][c];
          rows[a][c] = old + 1;
        }
      }
    }

    int found = 0;

    for (int y : innerIndexes) {
      for (int x : innerIndexes) {
        if (get(x, y) != '.') {
          continue;
        }

        int xq, yq, count = 0;

        for (int q : outerIndexes) {
          if (questions[y][q]) {
            xq = q;
            yq = y;
            count++;
          }
          if (questions[q][x]) {
            xq = x;
            yq = q;
            count++;
          }
        }

        if (count != 1) {
          continue;
        }

        std::vector<char> letters;
        char c = 0;

        for (auto q : columns[x]) {
          if (q.second == 1) {
            letters.push_back(q.first);
            c = q.first;
          }
        }
        for (auto q : rows[y]) {
          if (q.second == 1) {
            letters.push_back(q.first);
            c = q.first;
          }
        }

        if (letters.size() != 1) {
          continue;
        }

        set(x, y, c);
        set(xq, yq, c);
        found++;
      }
    }

    return found;
  }

public:
  bool fillRunes() {
    bool questions[8][8];

    int missing = populate(questions);

    if (missing == 0) {
      return false;
    }

    return resolve(questions) > 0;
  }

  const char *getRuneString() {
    static char buffer[20];
    char *ptr = buffer;

    for (int y : innerIndexes) {
      for (int x : innerIndexes) {
        char c = get(x, y);
        *ptr++ = c;
      }
    }

    *ptr = 0;
    return (const char *)buffer;
  }
};

class MapCollection1 : public MapCollectionBase {
private:
  Map *subMaps;

  void initMaps() {
    subMaps = (Map *)calloc(count, sizeof(Map));
    for (int index = 0; index < count; index++) {
      subMaps[index].setup(this, index);
    }
  }

protected:
  virtual int getIndex(int index, int x, int y) override {
    int xx = index % width;
    int yy = (index - xx) / width;
    int idx = getOffset(xx * 9 + x, yy * 9 + y);

    return idx;
  }

  virtual void init() override {
    width = lineWidth / 9;
    height = (size + lineWidth) / (lineWidth * 9);
    count = width * height;
  }

public:
  MapCollection1(int part) : MapCollectionBase(part) {
    subMaps = NULL;
    init();
  }

  ~MapCollection1() {
    if (subMaps != NULL) {
      free(subMaps);
    }
  }

  Map *getMap(int index) {
    if (subMaps == NULL) {
      initMaps();
    }
    return &subMaps[index];
  }
};

class MapCollection2 : public MapCollectionBase {
private:
  Map *subMaps;

  void initMaps() {
    subMaps = (Map *)calloc(count, sizeof(Map));
    for (int index = 0; index < count; index++) {
      subMaps[index].setup(this, index);
    }
  }

protected:
  virtual int getIndex(int index, int x, int y) override {
    int xx = index % width;
    int yy = (index - xx) / width;
    int idx = getOffset(xx * 6 + x, yy * 6 + y);

    return idx;
  }

  virtual void init() override {
    width = (lineWidth - 3) / 6;
    height = size / (lineWidth * 6);
    count = width * height;
  }

public:
  MapCollection2() : MapCollectionBase(3) {
    subMaps = NULL;
    init();
  }

  ~MapCollection2() {
    if (subMaps != NULL) {
      free(subMaps);
    }
  }

  Map *getMap(int index) {
    if (subMaps == NULL) {
      initMaps();
    }
    return &subMaps[index];
  }

  int getWidth() { return width; }

  int getPower() {
    int power = 0;
    for (int i = 0; i < count; i++) {
      Map *m = getMap(i);
      power += m->getPower();
    }

    return power;
  }

  char *getKey(int index) {
    if (index > 0 && index % width == 0) {
      char *key = (char *)malloc(lineWidth * 2);
      int idx = getIndex(index, 0, 0);
      memcpy(key, data + idx, lineWidth * 2);
      key[lineWidth * 2 - 1] = 0;
      return key;
    } else {
      return NULL;
    }
  }
};

static const char *part1() {
  MapCollection1 maps(1);
  Map *map = maps.getMap(0);

  map->fillRunes();
  return map->getRuneString();
}

static unsigned int part2() {
  MapCollection1 maps(2);

  unsigned int total = 0;

  for (int i = 0; i < maps.count; i++) {
    Map *map = maps.getMap(i);

    map->fillRunes();
    total += map->getPower();
  }

  return total;
}

static unsigned int part3() {
  MapCollection2 maps;

  bool foundSome;
  do {
    foundSome = false;
    for (int i = 0; i < maps.count; i++) {
      Map *map = maps.getMap(i);

      foundSome |= map->fillRunes();
    }
  } while (foundSome);

  int total = maps.getPower();

  return total;
}
} // namespace quest10_namespace

void quest10() {
  printf("QUEST %s\n", DAY);

  const char *d1 = quest10_namespace::part1();
  printf("\tPART 1 = %s\n", d1);

  unsigned int d2 = quest10_namespace::part2();
  printf("\tPART 2 = %u\n", d2);

  unsigned int d3 = quest10_namespace::part3();
  printf("\tPART 3 = %u\n", d3);
}
