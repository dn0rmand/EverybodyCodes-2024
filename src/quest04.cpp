#include "tools/readData.h"
#include <stdio.h>

namespace quest04_namespace {
class Data {
private:
  int *data;
  int size;

public:
  int length;
  int min = -1;
  int max = 0;

public:
  Data(void *data) {
    const char *ptr = (const char *)data;

    this->size = 500;
    this->length = 0;
    this->data = (int *)calloc(this->size, sizeof(int));

    int value = 0;

    while (*ptr) {
      if (*ptr == 0x0A) {
        if (value == 0) {
          throw;
        }
        this->addValue(value);
        value = 0;
      } else if (*ptr < '0' || *ptr > '9') {
        throw;
      } else {
        value = value * 10 + (*ptr - '0');
      }
      ptr++;
    }

    if (value != 0) {
      this->addValue(value);
    }

    free(data);
  }

  ~Data() { free(this->data); }

private:
  void addValue(int value) {
    if (this->length == this->size) {
      throw;
    }
    this->data[this->length] = value;
    this->length++;
    if (this->min < 0 || this->min > value) {
      this->min = value;
    }
    if (this->max < value) {
      this->max = value;
    }
  }

public:
  unsigned int calculate() {
    unsigned int total = 0;

    for (int i = 0; i < this->length; i++) {
      total += (this->data[i] - this->min);
    }

    return total;
  }

  unsigned int calculate2() {
    unsigned long total = 0;

    for (int j = 0; j < this->length; j++) {
      total += this->data[j];
    }

    int median = (total / this->length);
    int best = -1;
    while (true) {
      total = 0;
      for (int i = 0; i < this->length; i++) {
        int v = this->data[i] - median;
        if (v < 0) {
          total -= v;
        } else {
          total += v;
        }
      }
      if (best == -1 || total <= (unsigned long)best) {
        best = (int)total;
      }
      if (total > (unsigned long)best) {
        break;
      }
      median++;
    }
    return best;
  }
};

static unsigned int part1() {
  Data data(tools::readData("04", 1));

  unsigned int total = data.calculate();

  return total;
}

static unsigned int part2() {
  Data data(tools::readData("04", 2));

  unsigned int total = data.calculate();

  return total;
}

static unsigned int part3() {
  Data data(tools::readData("04", 3));

  unsigned int total = data.calculate2();

  return total;
}
} // namespace quest04_namespace

void quest04() {
  printf("QUEST 4\n");

  unsigned int d1 = quest04_namespace::part1();
  printf("\tPART 1 = %u\n", d1);

  unsigned int d2 = quest04_namespace::part2();
  printf("\tPART 2 = %u\n", d2);

  unsigned int d3 = quest04_namespace::part3();
  printf("\tPART 3 = %u\n", d3);
}
