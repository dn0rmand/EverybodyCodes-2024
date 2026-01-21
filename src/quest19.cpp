#include "tools/map.h"
#include "tools/readData.h"
#include <map>
#include <stdio.h>
#include <string.h>
#include <vector>

#define DAY "19"
#define DAY_NUM 19
#define QUEST_NAME_HELPER(x) quest##x
#define QUEST_NAME_HELPER2(x) QUEST_NAME_HELPER(x)
#define QUEST_NAME QUEST_NAME_HELPER2(DAY_NUM)

namespace quest19_namespace {
class Decoder {
private:
  int width;
  int height;
  int size;
  const char *instructions;

  int toIndex(int x, int y) { return x + y * width; }

  template <typename T> void rotateLeft(T *data, int x, int y) {
    std::vector<int> points = {
        toIndex(x - 1, y),     toIndex(x - 1, y + 1), toIndex(x, y + 1),
        toIndex(x + 1, y + 1), toIndex(x + 1, y),     toIndex(x + 1, y - 1),
        toIndex(x, y - 1),     toIndex(x - 1, y - 1),
    };

    T previous = data[toIndex(x - 1, y - 1)];
    for (int idx : points) {
      T old = data[idx];
      data[idx] = previous;
      previous = old;
    }
  }

  template <typename T> void rotateRight(T *data, int x, int y) {
    std::vector<int> points = {
        toIndex(x, y - 1),     toIndex(x + 1, y - 1), toIndex(x + 1, y),
        toIndex(x + 1, y + 1), toIndex(x, y + 1),     toIndex(x - 1, y + 1),
        toIndex(x - 1, y),     toIndex(x - 1, y - 1),
    };

    T previous = data[toIndex(x - 1, y - 1)];

    for (int idx : points) {
      T old = data[idx];
      data[idx] = previous;
      previous = old;
    }
  }

public:
  Decoder(const char *instructions, int width, int height) {
    this->width = width;
    this->height = height;
    this->instructions = instructions;
    size = width * height;
  }

  template <typename T> void decode(T *data) {
    const char *ptr = instructions;

    for (int y = 1; y < height - 1; y++) {
      for (int x = 1; x < width - 1; x++, ptr++) {
        if (!*ptr) {
          ptr = instructions;
        }

        if (*ptr == 'L') {
          rotateLeft(data, x, y);
        } else if (*ptr == 'R') {
          rotateRight(data, x, y);
        }
      }
    }
  }
};

class Transformer {
private:
  int width;
  int height;
  const char *instructions;
  int *transform;
  Transformer *next;

  Transformer(int width, int height) : size(width * height) {
    this->width = width;
    this->height = height;
    next = NULL;
    transform = new int[size];
  }

public:
  const int size;

  Transformer(const char *instructions, int width, int height)
      : size(width * height) {
    this->instructions = instructions;
    this->width = width;
    this->height = height;

    int *indexes = new int[size];

    for (int index = 0; index < size; index++) {
      indexes[index] = index;
    }

    Decoder decoder(instructions, width, height);

    decoder.decode(indexes);

    transform = new int[size];

    for (int i = 0; i < size; i++) {
      int value = indexes[i];
      transform[value] = i;
    }

    delete[] indexes;
  }

  ~Transformer() {
    delete[] transform;
    if (next != NULL) {
      delete next;
    }
  }

  template <typename T> void applyTransform(T *data) {
    std::vector<T> oldData(size);

    for (int i = 0; i < size; i++) {
      oldData[i] = data[i];
    }

    for (int i = 0; i < size; i++) {
      int newIndex = transform[i];
      data[newIndex] = oldData[i];
    }
  }

  Transformer *doubleIt() {
    auto t = new Transformer(width, height);

    for (int i = 0; i < size; i++) {
      t->transform[i] = transform[transform[i]];
    }
    next = t;
    return t;
  }
};

class TInput {
private:
  int width;
  int height;
  char *data;
  const char *instructions;

public:
  TInput(int part) {
    void *inputData = tools::readData(DAY, part);
    char *ptr = (char *)inputData;
    int l = tools::stringLength(ptr);

    char *directions = (char *)malloc(l + 1);
    for (int i = 0; i < l; i++) {
      directions[i] = *ptr++;
    }
    directions[l] = 0;
    instructions = directions;

    ptr += 2;

    tools::Map map((void *)ptr, false);
    width = map.width;
    height = map.height;

    data = new char[width * height];

    ptr = data;

    for (int y = 0; y < height; y++) {
      for (int x = 0; x < width; x++) {
        *ptr++ = map.get(x, y);
      }
    }

    free(inputData);
  }

  ~TInput() {
    delete[] data;
    free((void *)instructions);
  }

  Decoder getDecoder() { return Decoder(instructions, width, height); }

  void dump() {
    char *ptr = data;

    for (int y = 0; y < height; y++) {
      for (int x = 0; x < width; x++) {
        printf("%c", *ptr++);
      }

      printf("\n");
    }
  }

  void decode(int time = 1) {
    std::map<int, Transformer *> transformers;

    Transformer *t = new Transformer(instructions, width, height);
    transformers[1] = t;

    int c = 1;
    while (c * 2 <= time) {
      c *= 2;
      t = t->doubleIt();
      transformers[c] = t;
    }

    while (time > 0) {
      c = 1;
      while (c * 2 <= time) {
        c *= 2;
      }
      t = transformers[c];
      t->applyTransform(data);
      time -= c;
    }
  }

  const char *getMessage() {
    static char buffer[500];

    char *ptr = buffer;
    bool started = false;
    for (int idx = 0; idx < width * height; idx++) {
      if (data[idx] == '>') {
        started = true;
      } else if (data[idx] == '<') {
        break;
      } else if (started) {
        *ptr++ = data[idx];
      }
    }
    *ptr = 0;
    return buffer;
  }
};

static const char *part1() {
  TInput input(1);

  input.decode(1);

  const char *msg1 = input.getMessage();

  return msg1;
}

static const char *part2() {
  TInput input(2);

  input.decode(100);

  const char *msg1 = input.getMessage();

  return msg1;
}

static const char *part3() {
  TInput input(3);

  input.decode(1048576000);

  return input.getMessage();
}
} // namespace quest19_namespace

void QUEST_NAME() {
  printf("QUEST %s\n", DAY);

  const char *d1 = quest19_namespace::part1();
  printf("\tPART 1 = %s\n", d1);

  const char *d2 = quest19_namespace::part2();
  printf("\tPART 2 = %s\n", d2);

  const char *d3 = quest19_namespace::part3();
  printf("\tPART 3 = %s\n", d3);
}
