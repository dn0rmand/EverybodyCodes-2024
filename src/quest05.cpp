#include "tools/macros.h"
#include "tools/readData.h"
#include <map>
#include <stdio.h>

#define DAY "05"
#define COLUMNS 4

namespace quest05_namespace {
struct Node {
  int digit;
  struct Node *next;
  struct Node *previous;

  Node(int digit) { this->digit = digit; }
};

class Column {
public:
  struct Node *first;

  Column() { this->first = NULL; }

  ~Column() {
    while (this->first) {
      struct Node *ptr = this->first;
      this->first = ptr->next;
      delete ptr;
    }
  }

  void push(int digit) {
    if (!digit) {
      throw;
    }
    struct Node *ptr = this->first;
    struct Node *n = new Node(digit);
    if (ptr == NULL) {
      this->first = n;
    } else {
      while (ptr->next != NULL) {
        ptr = ptr->next;
      }
      ptr->next = n;
      n->previous = ptr;
    }
  }

  struct Node *pop() {
    struct Node *v = this->first;
    if (!v) {
      throw;
    }
    this->first = this->first->next;
    if (this->first) {
      this->first->previous = NULL;
    }
    v->previous = NULL;
    v->next = NULL;
    return v;
  }

  void insert(struct Node *node) {
    int value = node->digit;
    struct Node *position = this->first;
    if (position == NULL) {
      throw;
    }
    while (value) {
      // Going down
      struct Node *last = position;
      while (value && position) {
        value--;
        last = position;
        position = position->next;
      }
      if (!value) {
        node->next = last;
        node->previous = last->previous;
        last->previous = node;
        if (node->previous == NULL) {
          this->first = node;
        } else {
          node->previous->next = node;
        }
        return;
      }
      // Going up
      position = last;
      while (value && position) {
        value--;
        last = position;
        position = position->previous;
      }
      if (!value) {
        node->next = last->next;
        node->previous = last;
        last->next = node;
        if (node->next) {
          node->next->previous = node;
        }
        return;
      }
      position = this->first;
    }
  }
};

class Columns {
private:
  int base = 10;
  Column columns[COLUMNS];

public:
  Columns(void *data) {
    const char *ptr = (const char *)data;

    int column = 0;
    while (*ptr) {
      if (*ptr == ' ') {
        column++;
        if (column >= COLUMNS) {
          throw;
        }
        while (*ptr == ' ') {
          ptr++;
        }
      } else if (*ptr == 0x0A) {
        ptr++;
        column = 0;
      } else if (*ptr >= '0' && *ptr <= '9') {
        int v = 0;
        int b = 1;
        while (*ptr >= '0' && *ptr <= '9') {
          b *= 10;
          v = v * 10 + *ptr - '0';
          ptr++;
        }
        this->base = MAX(this->base, b);
        columns[column].push(v);
      }
    }
    free(data);
  }

  int height() {
    int h = 0;
    struct Node *n = this->columns[0].first;
    while (n) {
      h++;
      n = n->next;
    }
    return h;
  }

  void dance(int round) {
    int col = round % COLUMNS;
    struct Node *n = this->columns[col].pop();
    this->columns[(col + 1) % COLUMNS].insert(n);
  }

  unsigned long callIt() {
    unsigned long value = 0;
    for (int col = 0; col < COLUMNS; col++) {
      struct Node *n = this->columns[col].first;
      value = value * this->base + n->digit;
    }

    return value;
  }
};

static unsigned long part1() {
  Columns columns(tools::readData(DAY, 1));

  for (int round = 0; round < 10; round++) {
    columns.dance(round);
  }

  return columns.callIt();
}

static unsigned long part2() {
  Columns columns(tools::readData(DAY, 2));

  std::map<unsigned long, int> map;

  unsigned long total = 0;

  for (int round = 0;; round++) {
    columns.dance(round);
    unsigned long value = columns.callIt();
    if (map.find(value) == map.end()) {
      map[value] = 1;
    } else {
      int old = map[value];
      map[value] = old + 1;
      if (map[value] == 2024) {
        total = (long)value * (long)(round + 1);
        break;
      }
    }
  }

  return total;
}

static unsigned long part3() {
  Columns columns(tools::readData(DAY, 3));

  unsigned long max = 0;
  int round = 0;

  for (round = 0; round < (columns.height() * COLUMNS); round++) {
    columns.dance(round);
    unsigned long v = columns.callIt();
    if (v > max) {
      max = v;
    }
  }

  return max;
}
} // namespace quest05_namespace

void quest05() {
  printf("QUEST %s\n", DAY);

  unsigned long d1 = quest05_namespace::part1();
  printf("\tPART 1 = %lu\n", d1);

  unsigned long d2 = quest05_namespace::part2();
  printf("\tPART 2 = %lu\n", d2);

  unsigned long d3 = quest05_namespace::part3();
  printf("\tPART 3 = %lu\n", d3);
}
