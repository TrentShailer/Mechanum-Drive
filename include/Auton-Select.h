#ifndef AutonSelect_H
#define AutonSelect_H

#include "util.h"
#include "vex.h"
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using std::vector;

using namespace vex;
int timeForSelection = 15;

vector<vector<std::string>> options = {
    {"Left", "None", "Right"}, {"Blue", "Red"}, {"Win Point", "No Win Point"}};
vector<std::string> results = {};

vector<std::string> Initialize() {
  for (int i = 0; i < options.size(); i++) {
    int numOfOptions = options[i].size();

    Brain.Screen.clearScreen();

    for (int j = 1; j <= numOfOptions; j++) {
      int xPos = (480 / numOfOptions) * j;

      Brain.Screen.drawLine(xPos, 0, xPos, 280);
      Brain.Screen.printAt(((480 / numOfOptions) / 2) +
                               (480 / numOfOptions) * (j - 1) -
                               (options[i][j - 1].length() * 6),
                           130, options[i][j - 1].c_str());
    }

    for (int k = 0; k < (timeForSelection * 1000) / 20; k++) {
      int pressPos = Brain.Screen.xPosition();

      if (Brain.Screen.pressing()) {
        for (int l = 1; l <= numOfOptions; l++) {

          int xPos = (480 / numOfOptions) * l;

          if (pressPos < xPos) {
            results.push_back(options[i][l - 1]);
            Brain.Screen.clearScreen();
            wait(500, msec);
            break;
          }
        }
        break;
      }

      wait(20, msec);
    }
  }
  Brain.Screen.clearScreen();
  return results;
}

#endif