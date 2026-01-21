#include "tools/macros.h"
#include "tools/map.h"
#include "tools/readData.h"
#include <set>
#include <stdio.h>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#define DAY "15"
#define DAY_NUM 15
#define QUEST_NAME_HELPER(x) quest##x
#define QUEST_NAME_HELPER2(x) QUEST_NAME_HELPER(x)
#define QUEST_NAME QUEST_NAME_HELPER2(DAY_NUM)

#define MAX_WIDTH 300L

namespace quest15_namespace {
typedef struct {
  int x;
  int y;
  unsigned int herbs;
} TState;

static unsigned int getHerbValue(const char c) {
  if (c >= 'A' && c <= 'Z') {
    int i = (int)(c - 'A');
    return 1 << i;
  } else {
    return 0;
  }
}

static unsigned int getTargetValue(tools::Map *map) {
  unsigned int target = 0;

  for (int x = 0; x < map->width; x++)
    for (int y = 0; y < map->height; y++) {
      auto c = map->get(x, y);
      target |= getHerbValue(c);
    }

  return target;
}

static u_long getKey(TState pt) {
  u_long key = pt.herbs;
  key = key * MAX_WIDTH + pt.y;
  key = key * MAX_WIDTH + pt.x;
  return key;
}

static u_long getPositionKey(int x, int y) { return (u_long)y * MAX_WIDTH + x; }

// Check if herbs1 is a subset of herbs2 (all bits in herbs1 are also in herbs2)
static bool isSubset(unsigned int herbs1, unsigned int herbs2) {
  return (herbs1 & herbs2) == herbs1;
}

static std::vector<TState> moves(
    {{.x = 0, .y = -1}, {.x = 0, .y = 1}, {.x = -1, .y = 0}, {.x = 1, .y = 0}});

static int getEntrance(tools::Map *map) {
  for (int x = 0; x < map->width; x++) {
    if (map->get(x, 0) != '#') {
      return x;
    }
  }

  return -1;
}

static int shortestPath(tools::Map *map, TState start, unsigned int target) {
  std::vector<TState> info[2];
  std::vector<TState> *states = &(info[0]);
  std::vector<TState> *newStates = &(info[1]);
  std::unordered_set<u_long> visited;
  // Track all non-dominated herb sets at each position for aggressive pruning
  // Key: position, Value: unordered_set of herb combinations seen at that
  // position
  std::unordered_map<u_long, std::unordered_set<unsigned int>> herbsAtPosition;

  // Precompute herb values at each position to avoid repeated getHerbValue
  // calls
  std::vector<std::vector<unsigned int>> herbCache(
      map->height, std::vector<unsigned int>(map->width));
  for (int y = 0; y < map->height; y++) {
    for (int x = 0; x < map->width; x++) {
      char c = map->get(x, y);
      herbCache[y][x] = getHerbValue(c);
    }
  }

  // Reserve capacity to reduce reallocations
  visited.reserve(map->width * map->height *
                  64); // Estimate: up to 64 herb combinations
  herbsAtPosition.reserve(map->width * map->height);
  states->reserve(map->width * map->height);
  newStates->reserve(map->width * map->height);

  start.herbs = 0;
  states->push_back(start);
  visited.insert(getKey(start));
  herbsAtPosition[getPositionKey(start.x, start.y)].insert(start.herbs);

  int steps = 0;
  while (states->size() > 0) {
    steps++;
    newStates->clear();

    for (const TState &pt : *states) {
      for (const auto &move : moves) {
        TState newPt = {
            .x = pt.x + move.x, .y = pt.y + move.y, .herbs = pt.herbs};

        // Bounds check
        if (newPt.x < 0 || newPt.x >= map->width || newPt.y < 0 ||
            newPt.y >= map->height) {
          continue;
        }

        // Use cached herb value (already computed, includes wall/water check)
        unsigned int herbValue = herbCache[newPt.y][newPt.x];
        // herbValue will be 0 for walls, water, and empty spaces
        // We need to check if it's actually a wall or water
        if (herbValue == 0) {
          char c = map->get(newPt.x, newPt.y);
          if (c == '#' || c == '~') {
            continue;
          }
        }
        newPt.herbs |= herbValue;

        // Early termination check before expensive operations
        if (newPt.herbs == target && newPt.y == 0) {
          return steps;
        }

        // Aggressive state pruning: check if this state is dominated
        u_long posKey = getPositionKey(newPt.x, newPt.y);
        auto &herbSets = herbsAtPosition[posKey];

        // Fast dominance check using unordered_set
        bool isDominated = false;
        // Check if any existing herb set at this position dominates this one
        for (unsigned int existingHerbs : herbSets) {
          // If existing herbs is a superset of new herbs, new state is
          // dominated
          if (isSubset(newPt.herbs, existingHerbs)) {
            isDominated = true;
            break;
          }
        }

        if (isDominated) {
          continue;
        }

        // Remove any existing herb sets that are dominated by this new one
        // Use erase-remove idiom for better performance with unordered_set
        std::vector<unsigned int> toRemove;
        for (unsigned int existingHerbs : herbSets) {
          if (isSubset(existingHerbs, newPt.herbs)) {
            toRemove.push_back(existingHerbs);
          }
        }
        for (unsigned int herbs : toRemove) {
          herbSets.erase(herbs);
        }

        // Add this new herb set
        herbSets.insert(newPt.herbs);

        u_long key = getKey(newPt);
        if (visited.find(key) != visited.end()) {
          continue;
        }

        visited.insert(key);
        newStates->push_back(newPt);
      }
    }

    SWAP(states, newStates);
  }

  return -1;
}

static unsigned int part1() {
  tools::Map map(DAY, 1);

  int entrance = getEntrance(&map);
  int total = shortestPath(&map, {.x = entrance, .y = 0, .herbs = 0},
                           getHerbValue('H'));

  return total;
}

static unsigned int part2() {
  tools::Map map(DAY, 2);

  int entrance = getEntrance(&map);
  int target = getTargetValue(&map);
  int total = shortestPath(&map, {.x = entrance, .y = 0, .herbs = 0}, target);

  return total;
}

static unsigned int part3() {
  tools::Map map(DAY, 3);

  int entrance = getEntrance(&map);
  auto target = getTargetValue(&map);
  int total = shortestPath(&map, {.x = entrance, .y = 0, .herbs = 0}, target);

  return total;
}
} // namespace quest15_namespace

void QUEST_NAME() {
  printf("QUEST %s\n", DAY);

  unsigned int d1 = quest15_namespace::part1();
  printf("\tPART 1 = %u\n", d1);

  unsigned int d2 = quest15_namespace::part2();
  printf("\tPART 2 = %u\n", d2);

  unsigned int d3 = quest15_namespace::part3();
  printf("\tPART 3 = %u\n", d3);
}
