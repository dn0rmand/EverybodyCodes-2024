#include "tools/readData.h"
#include <functional>
#include <list>
#include <map>
#include <stdio.h>
#include <string.h>
#include <vector>

#define DAY "07"

namespace quest07_namespace {
class TrackLoader {
private:
  std::vector<int> path;
  char *data;
  int width;
  int height;
  int ox = 1;
  int oy = 0;
  int x = 0;
  int y = 0;

public:
  TrackLoader(char *input) {
    if (*input != 'S') {
      path.push_back(0);
    } else {
      this->data = input;
      int size = strlen(this->data) + 1;

      this->width = tools::stringLength(this->data);
      this->height = size / (this->width + 1);

      while (true) {
        this->move();
        char c = this->get(this->x, this->y);
        this->set(this->x, this->y, ' ');
        if (c == 'S') {
          this->path.push_back(0);
          break;
        }
        switch (c) {
        case '+':
          this->path.push_back(1);
          break;
        case '-':
          this->path.push_back(-1);
          break;
        case '=':
          this->path.push_back(0);
          break;
        default:
          throw;
        }
      }

      this->data = NULL;
    }
  }

  std::vector<int> getPath() { return this->path; }

private:
  char get(int x, int y) {
    int idx = x + y * (this->width + 1);
    return this->data[idx];
  }

  void set(int x, int y, char c) {
    int idx = x + y * (this->width + 1);
    this->data[idx] = c;
  }

  bool valid(int x, int y) {
    return !(x < 0 || x >= this->width || y < 0 || y >= this->height ||
             this->get(x, y) == ' ');
  }

  void move() {
    int x = this->x + this->ox;
    int y = this->y + this->oy;
    if (!this->valid(x, y)) {
      // need to turn
      if (this->valid(this->x + 1, this->y)) {
        this->ox = 1;
        this->oy = 0;
      } else if (this->valid(this->x - 1, this->y)) {
        this->ox = -1;
        this->oy = 0;
      } else if (this->valid(this->x, this->y + 1)) {
        this->ox = 0;
        this->oy = 1;
      } else if (this->valid(this->x, this->y - 1)) {
        this->ox = 0;
        this->oy = -1;
      } else {
        throw; // Stuck
      }

      this->x += this->ox;
      this->y += this->oy;
    } else {
      this->x = x;
      this->y = y;
    }
  }
};

static std::vector<int> parse(std::map<char, std::vector<int>> &data,
                              int part) {
  void *input = tools::readData(DAY, part);
  char *ptr = (char *)input;

  while (*ptr) {
    if (*ptr == 'S') {
      break;
    }
    if (*ptr < 'A' || *ptr > 'Z') {
      throw;
    }
    char key = *ptr++;
    if (*ptr != ':') {
      throw;
    }
    data[key].clear();
    while (*ptr == ':' || *ptr == ',') {
      ptr++;
      switch (*ptr++) {
      case '+':
        data[key].push_back(1);
        break;
      case '-':
        data[key].push_back(-1);
        break;
      case '=':
        data[key].push_back(0);
        break;
      default:
        throw;
      }
    }
    if (*ptr == 0x0A) {
      ptr++;
    }
  }

  TrackLoader track(ptr);
  std::vector<int> path = track.getPath();

  free(input);

  return path;
}

static void
strategies(std::function<void(std::vector<int> &strategy)> callback) {
  std::vector<int> strategy;

  std::function<void(int, int, int)> inner{[&](int equal, int plus, int minus) {
    if (strategy.size() == 11) {
      callback(strategy);
      return;
    }

    if (equal > 0) {
      strategy.push_back(0);
      inner(equal - 1, plus, minus);
      strategy.pop_back();
    }
    if (plus > 0) {
      strategy.push_back(1);
      inner(equal, plus - 1, minus);
      strategy.pop_back();
    }
    if (minus > 0) {
      strategy.push_back(-1);
      inner(equal, plus, minus - 1);
      strategy.pop_back();
    }
  }};

  inner(3, 5, 3);
}

static long getScore(std::vector<int> &track, std::vector<int> &actions,
                     int rounds) {
  long score = 0;
  long current = 10;
  int count = actions.size();
  int step = 0;

  long subTotal = 0;
  long value = 0;
  int times = 0;

  for (int round = 0; round < rounds; round++) {
    if (step == 0 && round > 0) {
      if (rounds % round != 0) {
        throw;
      }

      int factor = rounds / round;

      score += subTotal * factor;
      score += current * times * factor;
      score += times * value * (factor * (factor - 1)) / 2;

      return score;
    }

    for (int t : track) {
      if (t == 0) {
        t = actions.at(step);
      }
      times++;
      value += t;
      subTotal += value;
      step = (step + 1) % count;
    }
  }

  score += subTotal + current * times;

  return score;
}

static char *process(int part, int rounds) {
  static char sorted[10];

  std::map<char, int> scores;
  std::map<char, std::vector<int>> input;
  std::list<char> players;

  std::vector<int> track = parse(input, part);

  for (const auto &entry : input) {
    auto actions = entry.second;
    int score = getScore(track, actions, rounds);
    scores[entry.first] = score;
    players.push_back(entry.first);
  }

  players.sort(
      [&](const char p1, const char p2) { return scores[p1] > scores[p2]; });

  char *ptr = &sorted[0];
  for (char c : players) {
    *ptr++ = c;
  }
  *ptr = 0;

  return sorted;
}

static unsigned int part3() {
  std::map<char, std::vector<int>> input;
  std::vector<int> track = parse(input, 3);

  long target = getScore(track, input['A'], 2024);
  long total = 0;

  auto callback = [&](std::vector<int> &strategy) {
    if (getScore(track, strategy, 2024) > target) {
      total++;
    }
  };

  strategies(callback);

  return total;
}

static char *part2() { return process(2, 10); }

static char *part1() { return process(1, 10); }
} // namespace quest07_namespace

void quest07() {
  printf("QUEST %s\n", DAY);

  char *d1 = quest07_namespace::part1();
  printf("\tPART 1 = %s\n", d1);

  char *d2 = quest07_namespace::part2();
  printf("\tPART 2 = %s\n", d2);

  unsigned int d3 = quest07_namespace::part3();
  printf("\tPART 3 = %u\n", d3);
}
