#include "tools/macros.h"
#include "tools/map.h"
#include "tools/readData.h"
#include <map>
#include <stdio.h>
#include <vector>

#define DAY "20"
#define DAY_NUM 20
#define QUEST_NAME_HELPER(x) quest##x
#define QUEST_NAME_HELPER2(x) QUEST_NAME_HELPER(x)
#define QUEST_NAME QUEST_NAME_HELPER2(DAY_NUM)

namespace quest20_namespace {
typedef enum {
  left = 0,
  right = 1,
  down = 2,
  up = 3,
} TDirection;

typedef enum {
  A = 1,
  B = 2,
  C = 3,
} TLetter;

typedef struct {
  TDirection direction;
  int x;
  int y;
  int height;
  TLetter letter;
} TState;

static TState move(TState &state, TDirection direction) {
  TState s = state;

  s.direction = direction;
  switch (direction) {
  case left:
    s.x--;
    break;
  case right:
    s.x++;
    break;
  case up:
    s.y--;
    break;
  case down:
    s.y++;
    break;
  default:
    throw;
  }
  return s;
}

static TState turnRight(TState &s) {
  TDirection direction;
  switch (s.direction) {
  case left:
    direction = up;
    break;
  case right:
    direction = down;
    break;
  case down:
    direction = left;
    break;
  case up:
    direction = right;
    break;
  default:
    throw;
  }

  return move(s, direction);
}

static TState turnLeft(TState &s) {
  TDirection direction;
  switch (s.direction) {
  case left:
    direction = down;
    break;
  case right:
    direction = up;
    break;
  case down:
    direction = right;
    break;
  case up:
    direction = left;
    break;
  default:
    throw;
  }

  return move(s, direction);
}

static u_long getKey(TState &state) {
  u_long key = 0;

  key = key * 200 + state.y;
  key = key * 200 + state.x;
  key = key * 10 + state.letter;
  key = key * 10 + state.direction;

  return key;
}

static void updateHeight(tools::Map &map, TState &state) {
  if (state.height == 0) // On the ground => can't move anymore
  {
    state.height = -1;
    return;
  }

  const char c = map.get(state.x, state.y % map.height);

  switch (c) {
  case '#':
    return;

  case 'S':
  case '.':
  case 'A':
  case 'B':
  case 'C':
    state.height--;
    break;

  case '-':
    state.height -= 2;
    break;
  case '+':
    state.height += 1;
    break;

  default:
    throw;
  }
}

static bool addState(tools::Map &map, std::map<u_long, TState> &newStates,
                     TState &state, int startHeight) {
  updateHeight(map, state);

  const char c = map.get(state.x, state.y % map.height);

  switch (c) {
  case '#':
    return false;
  case 'S':
    if (state.height >= startHeight && state.letter == C) {
      return true;
    }
    return false;
  case 'A':
    if (state.letter != 0) {
      return false;
    }
    state.letter = A;
    break;
  case 'B':
    if (state.letter != A) {
      return false;
    }
    state.letter = B;
    break;
  case 'C':
    if (state.letter != B) {
      return false;
    }
    state.letter = C;
    break;
  }

  if (state.height <= 0) {
    return false;
  }

  u_long key = getKey(state);
  if (newStates.find(key) != newStates.end()) {
    TState old = newStates[key];
    if (state.height > old.height) {
      newStates[key] = state;
    }
  } else {
    newStates[key] = state;
  }
  return false;
}

static int getMaxHeight(tools::Map &map, int time, int startHeight) {
  std::map<u_long, TState> info[2];
  std::map<u_long, TState> *states = &(info[0]);
  std::map<u_long, TState> *newStates = &(info[1]);

  for (int x = 0; x < map.width; x++) {
    if (map.get(x, 0) == 'S') {
      (*states)[0] = {.x = x, .y = 0, .direction = down, .height = startHeight};
      (*states)[1] = {.x = x, .y = 0, .direction = left, .height = startHeight};
      (*states)[2] = {
          .x = x, .y = 0, .direction = right, .height = startHeight};
      break;
    }
  }

  while (time--) {
    newStates->clear();
    for (auto e : (*states)) {
      TState state = e.second;

      for (auto newState :
           {move(state, state.direction), turnLeft(state), turnRight(state)}) {
        addState(map, *newStates, newState, 0);
      }
    }

    SWAP(states, newStates);
  }

  int maxHeight = -1;
  for (auto e : (*states)) {
    maxHeight = MAX(maxHeight, e.second.height);
  }
  return maxHeight;
}

static int shortestPath(tools::Map &map, int startHeight) {
  std::map<u_long, TState> info[2];
  std::map<u_long, TState> *states = &(info[0]);
  std::map<u_long, TState> *newStates = &(info[1]);

  for (int x = 0; x < map.width; x++) {
    if (map.get(x, 0) == 'S') {
      (*states)[0] = {.x = x, .y = 0, .direction = down, .height = startHeight};
      (*states)[1] = {.x = x, .y = 0, .direction = left, .height = startHeight};
      (*states)[2] = {
          .x = x, .y = 0, .direction = right, .height = startHeight};
      break;
    }
  }

  int time = 0;
  while (states->size() > 0) {
    time++;
    newStates->clear();
    for (auto e : (*states)) {
      TState state = e.second;

      for (auto newState :
           {move(state, state.direction), turnLeft(state), turnRight(state)}) {
        if (addState(map, *newStates, newState, startHeight)) {
          return time;
        }
      }
    }

    SWAP(states, newStates);
  }

  throw;
}

static int furthestPath(tools::Map &map, int startX, int startHeight,
                        int column) {
  std::vector<int> sums(map.height);
  int s = 0;
  int min = 0;
  for (int y = 0; y < map.height; y++) {
    const char c = map.get(column, y);
    if (c == '#') {
      return 0;
    }
    s = s + (c == '.' ? -1 : c == '-' ? -2 : 1);
    sums[y] = s;
    if (s < min) {
      min = s;
    }
  }

  int height = startHeight - ABS(startX - column) + 1;
  int y = 0;

  while (height + min >= 0) {
    height += sums[map.height - 1];
    y += map.height;
  }
  for (int i = 0; height > 0 && i < map.height; i++) {
    int h = height + sums[i];
    if (h <= 0) {
      y += i;
      break;
    }
  }
  return y;
}

static unsigned int part1() {
  tools::Map map(DAY, 1);

  return getMaxHeight(map, 100, 1000);
}

static unsigned int part2() {
  tools::Map map(DAY, 2);

  return shortestPath(map, 10000);
}

static unsigned int part3() {
  tools::Map map(DAY, 3);

  int startX = 0;

  for (int x = 0; x < map.width; x++) {
    if (map.get(x, 0) == 'S') {
      startX = x;
      map.set(x, 0, '.');
      break;
    }
  }

  int furthest = 0;
  for (int x = 1; x < map.width - 1; x++) {
    furthest = MAX(furthest, furthestPath(map, startX, 384400, x));
  }

  return furthest;
}
} // namespace quest20_namespace

void QUEST_NAME() {
  printf("QUEST %s\n", DAY);

  unsigned int d1 = quest20_namespace::part1();
  printf("\tPART 1 = %u\n", d1);

  unsigned int d2 = quest20_namespace::part2();
  printf("\tPART 2 = %u\n", d2);

  unsigned int d3 = quest20_namespace::part3();
  printf("\tPART 3 = %u\n", d3);
}
