#include "tools/macros.h"
#include "tools/readData.h"
#include <algorithm>
#include <stdio.h>
#include <vector>

#define DAY "17"
#define DAY_NUM 17
#define QUEST_NAME_HELPER(x) quest##x
#define QUEST_NAME_HELPER2(x) QUEST_NAME_HELPER(x)
#define QUEST_NAME QUEST_NAME_HELPER2(DAY_NUM)

#define DISTANCE(p1, p2) (ABS((p1).x - (p2).x) + ABS((p1).y - (p2).y))

namespace quest17_namespace {
typedef struct {
  int x;
  int y;
} TPoint;

static std::vector<TPoint> parseGalaxy(int part) {
  void *data = tools::readData(DAY, part);
  char *ptr = (char *)data;

  std::vector<TPoint> galaxy;

  int x = 0;
  int y = 0;
  while (*ptr) {
    switch (*ptr) {
    case '*':
      galaxy.push_back({.x = x, .y = y});
      break;
    case '\n':
      x = -1;
      y++;
      break;
    }
    ptr++;
    x++;
  }
  free(data);

  return galaxy;
}

static int getGalaxySize(std::vector<TPoint> &galaxy, int maxDistance) {
  std::vector<TPoint> connected;

  int size = 1;

  // make first point connected
  connected.push_back(galaxy.back());
  galaxy.pop_back();

  while (galaxy.size() > 0) {
    int min = 1000000;
    int best = -1;
    for (size_t i = 0; i < galaxy.size(); i++) {
      auto pt1 = galaxy.at(i);

      for (auto pt2 : connected) {
        int d = DISTANCE(pt1, pt2);
        if (d < min && d < maxDistance) {
          min = d;
          best = i;
        }
      }
    }
    if (best < 0) {
      break;
    }
    connected.push_back(galaxy.at(best));
    galaxy.erase(galaxy.begin() + best);
    size += min + 1;
  }

  return size;
}

static unsigned int part1() {
  std::vector<TPoint> galaxy = parseGalaxy(1);

  int size = getGalaxySize(galaxy, 1000000);

  return size;
}

static unsigned int part2() {
  auto galaxy = parseGalaxy(2);

  return getGalaxySize(galaxy, 1000000);
}

static u_long part3() {
  auto galaxy = parseGalaxy(3);

  std::vector<u_long> sizes;

  while (galaxy.size() > 0) {
    u_long size = getGalaxySize(galaxy, 6);
    sizes.push_back(size);
  }

  std::sort(sizes.begin(), sizes.end(), std::greater<u_long>());
  u_long total = sizes.at(0) * sizes.at(1) * sizes.at(2);
  return total;
}
} // namespace quest17_namespace

void QUEST_NAME() {
  printf("QUEST %s\n", DAY);

  unsigned int d1 = quest17_namespace::part1();
  printf("\tPART 1 = %u\n", d1);

  unsigned int d2 = quest17_namespace::part2();
  printf("\tPART 2 = %u\n", d2);

  u_long d3 = quest17_namespace::part3();
  printf("\tPART 3 = %lu\n", d3);
}
