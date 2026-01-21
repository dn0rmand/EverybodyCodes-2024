#include "tools/macros.h"
#include "tools/readData.h"
#include <map>
#include <stdio.h>
#include <string.h>
#include <vector>

#define DAY "16"
#define DAY_NUM 16
#define QUEST_NAME_HELPER(x) quest##x
#define QUEST_NAME_HELPER2(x) QUEST_NAME_HELPER(x)
#define QUEST_NAME QUEST_NAME_HELPER2(DAY_NUM)

#define MAX_WHEEL_ROWS 50

namespace quest16_namespace {
static char buffer[100];

typedef char str[4];

int gcd(int a, int b) {
  while (b != 0) {
    int c = a % b;
    a = b;
    b = c;
  }
  return a;
}

int lcm(int a, int b) {
  int g = gcd(a, b);
  int l = (a / g) * b;

  return l;
}

class TWheel {
private:
  int spinCount;
  str rows[MAX_WHEEL_ROWS];

public:
  int current;
  int size;

  TWheel(int count) {
    spinCount = count;
    current = 0;
    size = 0;
  }

  void addRow(str row) {
    rows[size][0] = row[0];
    rows[size][1] = row[1];
    rows[size][2] = row[2];
    rows[size][3] = 0;

    size++;
  }

  void spin(int times) { current = (current + spinCount * times) % size; }

  char *getValue() { return rows[current]; }

  void reset() { current = 0; }

  void cheat(int direction) {
    current = (current + direction + size + spinCount) % size;
  }
};

static int getWinnings(std::vector<TWheel> &wheels) {
  std::map<char, int> counts;

  for (auto wheel : wheels) {
    auto row = wheel.getValue();
    counts[row[0]] = counts[row[0]] + 1;
    counts[row[2]] = counts[row[2]] + 1;
  }

  int total = 0;
  for (auto x : counts) {
    if (x.second > 2) {
      total += (x.second - 2);
    }
  }
  return total;
}

class State {
private:
  std::vector<int> indexes;

public:
  int minCoins;
  int maxCoins;

public:
  State(std::vector<TWheel> &wheels, int minCoins, int maxCoins) {
    for (size_t i = 0; i < wheels.size(); i++) {
      indexes.push_back(wheels.at(i).current);
    }
    this->minCoins = minCoins;
    this->maxCoins = maxCoins;
  }

  u_long getKey() {
    u_long key = 0;
    for (size_t i = 0; i < indexes.size(); i++) {
      key = key * MAX_WHEEL_ROWS + indexes.at(i);
    }
    return key;
  }

  State makeMove(std::vector<TWheel> &wheels, int direction) {
    // Restore the position and apply cheat + roll
    for (size_t i = 0; i < wheels.size(); i++) {
      auto &w = wheels.at(i);
      w.current = indexes.at(i);
      w.cheat(direction);
    }

    int coins = getWinnings(wheels);

    return State(wheels, coins + minCoins, coins + maxCoins);
  }
};

static std::vector<TWheel> loadMachine(int part) {
  std::vector<TWheel> wheels;

  wheels.clear();

  void *data = tools::readData(DAY, part);
  char *ptr = (char *)data;

  while (true) {
    int value = tools::readNumber(&ptr);

    wheels.push_back(TWheel(value));
    if (*ptr == ',') {
      ptr++;
    } else if (*ptr == '\n') {
      ptr++;
      break;
    } else {
      throw;
    }
  }

  if (*ptr == '\n') {
    ptr++;
  }

  while (*ptr) {
    str row;

    int index = 0;
    while (true) {
      row[0] = *ptr++;
      row[1] = *ptr++;
      row[2] = *ptr++;
      row[3] = 0;
      if (row[0] != ' ') {
        wheels.at(index).addRow(row);
      }
      if (*ptr == ' ') {
        ptr++;
        index++;
      }
      if (*ptr == '\n') {
        ptr++;
        break;
      } else if (!*ptr) {
        break;
      }
    }
  }
  free(data);
  return wheels;
}

static void spin(std::vector<TWheel> &wheels, int times) {
  for (size_t i = 0; i < wheels.size(); i++) {
    wheels.at(i).spin(times);
  }
}

static char *calculate(std::vector<TWheel> &wheels) {
  std::vector<State> info[2];
  std::vector<State> *states = &(info[0]);
  std::vector<State> *newStates = &(info[1]);

  states->push_back(State(wheels, 0, 0));

  for (int i = 0; i < 256; i++) {
    newStates->clear();
    std::map<u_long, int> visited;

    for (auto state : *states) {
      for (int direction = -1; direction <= 1; direction++) {
        auto newState = state.makeMove(wheels, direction);
        u_long key = newState.getKey();
        if (visited.find(key) != visited.end()) {
          State &s = newStates->at(visited[key]);
          s.maxCoins = MAX(s.maxCoins, newState.maxCoins);
          s.minCoins = MIN(s.minCoins, newState.minCoins);
        } else {
          visited[key] = newStates->size();
          newStates->push_back(newState);
        }
      }
    }
    SWAP(states, newStates);
  }

  int max = 0;
  int min = 10000000;

  for (auto s : *states) {
    max = MAX(max, s.maxCoins);
    min = MIN(min, s.minCoins);
  }
  snprintf(buffer, 100, "%i %i", max, min);

  return buffer;
}

static char *part1() {
  auto machine = loadMachine(1);

  spin(machine, 100);

  buffer[0] = 0;
  bool first = true;
  for (auto w : machine) {
    if (!first) {
      strcat(buffer, " ");
    } else {
      first = false;
    }
    strcat(buffer, w.getValue());
  }
  return buffer;
}

static long part2() {
  auto wheels = loadMachine(2);

  int max = 1;

  for (auto wheel : wheels) {
    max = lcm(max, wheel.size);
  }

  long totalSpins = 202420242024L;
  long remaining = totalSpins % max;
  long factor = (totalSpins - remaining) / max;
  long coins = 0;
  long extra = 0;

  for (int i = 0; i < max; i++) {
    if (i == remaining) {
      extra = coins;
    }
    spin(wheels, 1);
    auto win = getWinnings(wheels);
    coins += win;
  }

  return coins * factor + extra;
}

static char *part3() {
  auto machine = loadMachine(3);

  return calculate(machine);
}
} // namespace quest16_namespace

void QUEST_NAME() {
  printf("QUEST %s\n", DAY);

  char *d1 = quest16_namespace::part1();
  printf("\tPART 1 = %s\n", d1);

  long d2 = quest16_namespace::part2();
  printf("\tPART 2 = %li\n", d2);

  char *d3 = quest16_namespace::part3();
  printf("\tPART 3 = %s\n", d3);
}
