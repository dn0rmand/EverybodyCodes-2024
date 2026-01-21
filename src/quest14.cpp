#include "tools/readData.h"
#include <map>
#include <set>
#include <stdio.h>
#include <vector>

#define DAY "14"

#define MAX_DISTANCE 1000000

namespace quest14_namespace {
typedef struct {
  int x;
  int y;
  int z;
} TPoint;

typedef struct {
  char direction;
  int distance;
} TStep;

typedef struct {
  int distance;
  int y;
} TDistance;

static std::vector<TStep> parseInput(int part) {
  std::vector<TStep> steps;

  void *data = tools::readData(DAY, part);

  char *ptr = (char *)data;

  int distance = 0;
  char direction = '?';
  char c = *ptr;
  while (c) {
    switch (c) {
    case '\n':
      // flush
      if (direction != '?') {
        steps.push_back({.direction = direction, .distance = distance});
        direction = '?';
        distance = 0;
      }
      // new Line => Origin
      steps.push_back({.direction = 'O', .distance = 0});
      break;
    case ',':
      if (direction != '?') {
        steps.push_back({.direction = direction, .distance = distance});
        direction = '?';
        distance = 0;
      }
      break;
    case 'U':
    case 'D':
    case 'F':
    case 'B':
    case 'L':
    case 'R':
      if (direction != '?') {
        throw;
      }
      direction = c;
      distance = 0;
      break;

    default:
      if (c >= '0' && c <= '9') {
        if (direction == '?') {
          throw;
        }
        distance = distance * 10 + c - '0';
      } else {
        throw;
      }
      break;
    }

    c = *(++ptr);
  }

  if (direction != '?') {
    steps.push_back({.direction = direction, .distance = distance});
    direction = '?';
    distance = 0;
  }
  free(data);

  return steps;
}

static u_long getKey(const TPoint a) {
  u_long k = 0L;

  int x = a.x + 100;
  int y = a.y + 100;
  int z = a.z + 100;

  if (x < 0 || x >= 1000)
    throw;
  if (y < 0 || y >= 1000)
    throw;
  if (z < 0 || z >= 1000)
    throw;

  k = k * 1000L + x;
  k = k * 1000L + y;
  k = k * 1000L + z;

  return k;
}

static TPoint fromKey(u_long k) {
  int z = k % 1000L;
  k = (k - z) / 1000L;
  int y = k % 1000L;
  k = (k - y) / 1000L;
  int x = k;

  return {.x = x - 100, .y = y - 100, .z = z - 100};
}

static void addPoint(std::set<u_long> &points, const TPoint a) {
  u_long k = getKey(a);
  points.insert(k);
}

static TPoint move(std::set<u_long> &points, const TPoint a, const TStep step) {
  TPoint b = a;
  int dx = 0;
  int dy = 0;
  int dz = 0;

  switch (step.direction) {
  case 'U':
    dy = 1;
    break;
  case 'D':
    dy = -1;
    break;
  case 'L':
    dx = -1;
    break;
  case 'R':
    dx = 1;
    break;
  case 'B':
    dz = -1;
    break;
  case 'F':
    dz = 1;
    break;
  default:
    throw;
  }

  for (int i = 0; i < step.distance; i++) {
    b.x += dx;
    b.y += dy;
    b.z += dz;
    addPoint(points, b);
  }

  return b;
}

static unsigned int part1() {
  auto steps = parseInput(1);

  int y = 0;
  int max = 0;

  for (auto step : steps) {
    switch (step.direction) {
    case 'U':
      y += step.distance;
      break;
    case 'D':
      y -= step.distance;
      break;
    }

    if (y > max) {
      max = y;
    }
  }

  return max;
}

static unsigned int part2() {
  auto steps = parseInput(2);

  TPoint a = {.x = 0, .y = 0, .z = 0};
  std::set<u_long> points;

  for (auto step : steps) {
    if (step.direction == 'O') {
      a = {.x = 0, .y = 0, .z = 0};
    } else {
      a = move(points, a, step);
    }
  }

  return points.size();
}

static bool isValid(const std::set<u_long> &points,
                    const std::set<u_long> &visited, TPoint pt) {
  u_long k = getKey(pt);
  if (points.find(k) == points.end()) {
    return false;
  }
  if (visited.find(k) != visited.end()) {
    return false;
  }

  return true;
}

static void addPoint(std::map<int, int> &distances, std::set<u_long> *visited,
                     std::vector<TPoint> *newStates, int steps, TPoint p) {
  auto k = getKey(p);
  if (p.x == 0 && p.z == 0) {
    // On the trunk
    if (distances.find(p.y) != distances.end()) {
      throw;
    }

    distances[p.y] = steps;
  }
  newStates->push_back(p);
  visited->insert(k);
}

static std::map<int, int> distanceToTrunk(const std::set<u_long> &points,
                                          u_long leafKey) {
  TPoint pt = fromKey(leafKey);
  std::map<int, int> distances;
  std::vector<TPoint> info[2];
  std::set<u_long> visited;

  std::vector<TPoint> *states = &info[0];
  std::vector<TPoint> *newStates = &info[1];

  states->push_back(pt);
  visited.insert(leafKey);

  int steps = 0;

  while (states->size() > 0) {
    newStates->clear();
    steps++;
    for (auto s : *states) {
      if (isValid(points, visited, {.x = s.x, .y = s.y, .z = s.z - 1})) {
        addPoint(distances, &visited, newStates, steps,
                 {.x = s.x, .y = s.y, .z = s.z - 1});
      }
      if (isValid(points, visited, {.x = s.x, .y = s.y, .z = s.z + 1})) {
        addPoint(distances, &visited, newStates, steps,
                 {.x = s.x, .y = s.y, .z = s.z + 1});
      }
      if (isValid(points, visited, {.x = s.x, .y = s.y - 1, .z = s.z})) {
        addPoint(distances, &visited, newStates, steps,
                 {.x = s.x, .y = s.y - 1, .z = s.z});
      }
      if (isValid(points, visited, {.x = s.x, .y = s.y + 1, .z = s.z})) {
        addPoint(distances, &visited, newStates, steps,
                 {.x = s.x, .y = s.y + 1, .z = s.z});
      }
      if (isValid(points, visited, {.x = s.x - 1, .y = s.y, .z = s.z})) {
        addPoint(distances, &visited, newStates, steps,
                 {.x = s.x - 1, .y = s.y, .z = s.z});
      }
      if (isValid(points, visited, {.x = s.x + 1, .y = s.y, .z = s.z})) {
        addPoint(distances, &visited, newStates, steps,
                 {.x = s.x + 1, .y = s.y, .z = s.z});
      }
    }
    auto tmp = newStates;
    newStates = states;
    states = tmp;
  }
  if (distances.empty()) {
    throw;
  }
  return distances;
}

static unsigned int part3() {
  auto steps = parseInput(3);

  // Make the moves

  TPoint a = {.x = 0, .y = 0, .z = 0};
  std::set<u_long> points;
  std::set<u_long> leaves;

  for (auto step : steps) {
    if (step.direction == 'O') {
      leaves.insert(getKey(a));
      a = {.x = 0, .y = 0, .z = 0};
    } else {
      a = move(points, a, step);
    }
  }
  leaves.insert(getKey(a));

  // Calculate the distances

  std::vector<std::map<int, int>> distances;

  for (u_long key : leaves) {
    auto leafDistances = distanceToTrunk(points, key);
    distances.push_back(leafDistances);
  }

  // Find shortest position

  int minDistance = MAX_DISTANCE;

  for (int y0 = 1; y0 <= MAX_DISTANCE; y0++) {
    int sum = 0;
    for (auto leafDistances : distances) {
      auto v = leafDistances.find(y0);
      if (v == leafDistances.end()) {
        sum = MAX_DISTANCE;
        break;
      } else {
        sum += v->second;
      }
    }
    if (sum < minDistance) {
      minDistance = sum;
    } else {
      break;
    }
  }
  return minDistance;
}
} // namespace quest14_namespace

void quest14() {
  printf("QUEST %s\n", DAY);

  unsigned int d1 = quest14_namespace::part1();
  printf("\tPART 1 = %u\n", d1);

  unsigned int d2 = quest14_namespace::part2();
  printf("\tPART 2 = %u\n", d2);

  unsigned int d3 = quest14_namespace::part3();
  printf("\tPART 3 = %u\n", d3);
}
