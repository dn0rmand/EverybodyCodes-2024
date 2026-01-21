#include "tools/timings.h"
#include <cstdlib>
#include <stdio.h>

// Forward declarations
void quest01();
void quest02();
void quest03();
void quest04();
void quest05();
void quest06();
void quest07();
void quest08();
void quest09();
void quest10();
void quest11();
void quest12();
void quest13();
void quest14();
void quest15();
void quest16();
void quest17();
void quest18();
void quest19();
void quest20();

int main(int count, char **args) {
  Timing _t;

  if (count > 1) {
    for (int i = 1; i < count; i++) {
      int quest = atoi(args[i]);
      printf("Running quest %d\n", quest);
      switch (quest) {
      case 1:
        quest01();
        break;
      case 2:
        quest02();
        break;
      case 3:
        quest03();
        break;
      case 4:
        quest04();
        break;
      case 5:
        quest05();
        break;
      case 6:
        quest06();
        break;
      case 7:
        quest07();
        break;
      case 8:
        quest08();
        break;
      case 9:
        quest09();
        break;
      case 10:
        quest10();
        break;
      case 11:
        quest11();
        break;
      case 12:
        quest12();
        break;
      case 13:
        quest13();
        break;
      case 14:
        quest14();
        break;
      case 15:
        quest15();
        break;
      case 16:
        quest16();
        break;
      case 17:
        quest17();
        break;
      case 18:
        quest18();
        break;
      case 19:
        quest19();
        break;
      case 20:
        quest20();
        break;
      default:
        printf("Invalid day: %d\n", quest);
        return 1;
      }
    }
    return 0;
  }

  quest01();
  _t.log("quest01");
  quest02();
  _t.log("quest02");
  quest03();
  _t.log("quest03");
  quest04();
  _t.log("quest04");
  quest05();
  _t.log("quest05");
  quest06();
  _t.log("quest06");
  quest07();
  _t.log("quest07");
  quest08();
  _t.log("quest08");
  quest09();
  _t.log("quest09");
  quest10();
  _t.log("quest10");
  quest11();
  _t.log("quest11");
  quest12();
  _t.log("quest12");
  quest13();
  _t.log("quest13");
  quest14();
  _t.log("quest14");
  quest15();
  _t.log("quest15");
  quest16();
  _t.log("quest16");
  quest17();
  _t.log("quest17");
  quest18();
  _t.log("quest18");
  quest19();
  _t.log("quest19");
  quest20();
  _t.log("quest20");

  return 0;
}
