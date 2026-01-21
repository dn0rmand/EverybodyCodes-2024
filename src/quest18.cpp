#include "tools/map.h"
#include "tools/readData.h"
#include <map>
#include <set>
#include <stdio.h>
#include <vector>

#define DAY "18"

namespace quest18_namespace {
typedef struct {
  int x;
  int y;
} TPoint;

class Map : public tools::Map {
public:
  std::vector<TPoint> entrances;
  std::vector<TPoint> trees;
  std::map<u_int, char> changes;

  Map(int part) : tools::Map(DAY, part) {
    for (int x = 0; x < width; x++) {
      for (int y = 0; y < height; y++) {
        char c = get(x, y);
        if (c == 'P') {
          trees.push_back({.x = x, .y = y});
        } else if (c == '.' &&
                   (x == 0 || y == 0 || x == width - 1 || y == height - 1)) {
          entrances.push_back({.x = x, .y = y});
        }
      }
    }
  }

  virtual char get(int x, int y) {
    char c = tools::Map::get(x, y);
    if (c == '#') {
      return '#';
    }
    int index = getIndex(x, y);
    if (changes.find(index) != changes.end()) {
      return changes[index];
    }
    return c;
  }

  virtual char set(int x, int y, const char c) {
    const char old = get(x, y);

    if (old == c || old == '#') {
      return old;
    }

    int index = getIndex(x, y);
    changes[index] = c;
    return old;
  }

  int flood(TPoint *entrance = NULL, int maxTime = 0) {
    int time = 0;
    int remainingTrees = trees.size();
    int total = 0;

    int part3 = entrance != NULL;
    if (part3) {
      entrances.clear();
      entrances.push_back(*entrance);
    }
    std::vector<TPoint> states(entrances);

    while (states.size() > 0) {
      std::vector<TPoint> oldStates(states);
      states.clear();

      if (part3 && maxTime > 0 && total + remainingTrees * time > maxTime) {
        changes.clear();
        return maxTime + 1;
      }

      for (auto state : oldStates) {
        if (set(state.x, state.y, '~') == 'P') {
          remainingTrees--;
          total += time;
          if (remainingTrees == 0) {
            changes.clear();
            return part3 ? total : time;
          }
        }
      }

      time++;

      for (auto state : oldStates) {
        std::vector<TPoint> moves = {
            {.x = state.x - 1, .y = state.y},
            {.x = state.x + 1, .y = state.y},
            {.x = state.x, .y = state.y - 1},
            {.x = state.x, .y = state.y + 1},
        };

        for (auto pt : moves) {
          switch (get(pt.x, pt.y)) {
          case '.':
          case 'P':
            states.push_back(pt);
            break;
          }
        }
      }
    }

    throw;
  }
};

static int findBest(Map &map, TPoint tree, int total = -1) {
  std::vector<TPoint> states = {tree};
  std::set<u_int> visited;

  while (states.size() > 0) {
    auto state = states.back();

    states.pop_back();
    std::vector<TPoint> moves = {
        {.x = state.x - 1, .y = state.y},
        {.x = state.x + 1, .y = state.y},
        {.x = state.x, .y = state.y - 1},
        {.x = state.x, .y = state.y + 1},
    };
    for (auto newState : moves) {
      u_int key = newState.x + newState.y * 300;
      const char c = map.get(newState.x, newState.y);
      if (c == '#') {
        continue;
      } else if (c == 'P') {
        if (visited.find(key) != visited.end()) {
          continue;
        }
        visited.insert(key);
        states.push_back(newState);
      } else {
        u_int key = newState.x + newState.y * 300;
        if (visited.find(key) != visited.end()) {
          continue;
        }
        visited.insert(key);
        int value = map.flood(&newState, total);
        if (value <= total || total == -1) {
          total = value;
          states.push_back(newState);
        }
      }
    }
  }

  return total;
}

static unsigned int part1() {
  Map map(1);

  unsigned int total = map.flood();

  return total;
}

static unsigned int part2() {
  Map map(2);

  unsigned int total = map.flood();

  return total;
}

static unsigned int part3() {
  Map map(3);

  int total = -1;

  // Get best tree to start from

  TPoint bestTree;

  for (auto tree : map.trees) {
    int newTotal = map.flood(&tree, total);
    if (newTotal < total || total == -1) {
      total = newTotal;
      bestTree = tree;
    }
  }

  // find best sum
  total = findBest(map, bestTree, -1);
  return total;
}
} // namespace quest18_namespace

void quest18() {
  printf("QUEST %s\n", DAY);

  unsigned int d1 = quest18_namespace::part1();
  printf("\tPART 1 = %u\n", d1);

  unsigned int d2 = quest18_namespace::part2();
  printf("\tPART 2 = %u\n", d2);

  unsigned int d3 = quest18_namespace::part3();
  printf("\tPART 3 = %u\n", d3);
}
