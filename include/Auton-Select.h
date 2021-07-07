#ifndef AutonSelect_H
#define AutonSelect_H

#include "util.h"
#include "vex.h"
#include <iostream>
#include <sstream>
#include <string>

using namespace vex;

side curSide = none;

TowerColour curTeam = TowerColour::red;

int timeForSelection = 15;

static const char *EnumStrings[] = {"Left", "Right", "None"};
static const char *EnumStringsTeam[] = {"Red", "Blue"};
static const char *EnumStringsWP[] = {"No Win Point", "Win Point"};

side InitAutonSelect()
{
	Brain.Screen.drawLine(160, 0, 160, 280);
  Brain.Screen.drawLine(320, 0, 320, 280);
	Brain.Screen.setCursor(6, 2);
	Brain.Screen.print("Left");
  Brain.Screen.setCursor(6, 22);
	Brain.Screen.print("None");
	Brain.Screen.setCursor(6, 44);
	Brain.Screen.print("Right");
	Brain.Screen.setCursor(0, 0);

	for (int i = 0; i < (timeForSelection * 1000) / 20; i++)
	{
		int xPos = Brain.Screen.xPosition();
		if (Brain.Screen.pressing())
		{
      if(xPos < 200){
        curSide = side::left;
      }
      else if(xPos >= 160 && xPos < 320){
        curSide = side::none;
      }
      else{
        curSide = side::right;
      }
			break;
		}
		wait(20, msec);
	}

	Brain.Screen.clearScreen();
  Brain.Screen.setCursor(6, 22);
	Brain.Screen.print(EnumStrings[curSide]);
	wait(1000, msec);
  Brain.Screen.setCursor(0, 0);
	Brain.Screen.clearScreen();

  return curSide;
}

TowerColour InitTeamSelect(){
  Brain.Screen.drawLine(240, 0, 240, 280);
	Brain.Screen.setCursor(6, 2);
	Brain.Screen.print("Blue");
	Brain.Screen.setCursor(6, 44);
	Brain.Screen.print("Red");
	Brain.Screen.setCursor(0, 0);

	for (int i = 0; i < (timeForSelection * 1000) / 20; i++)
	{
		int xPos = Brain.Screen.xPosition();
		if (Brain.Screen.pressing())
		{
      if(xPos < 240){
        curTeam = TowerColour::blue;
      }
      else{
        curTeam = TowerColour::red;
      }
			break;
		}
		wait(20, msec);
	}

	Brain.Screen.clearScreen();
  Brain.Screen.setCursor(6, 22);
	Brain.Screen.print(EnumStringsTeam[curTeam]);
	wait(1000, msec);
  Brain.Screen.setCursor(0, 0);
	Brain.Screen.clearScreen();

  return curTeam;
}

bool InitAutonWinPoint(){
  Brain.Screen.drawLine(240, 0, 240, 280);
	Brain.Screen.setCursor(6, 2);
	Brain.Screen.print("No Win Point");
	Brain.Screen.setCursor(6, 44);
	Brain.Screen.print("Win Point");
	Brain.Screen.setCursor(0, 0);

	for (int i = 0; i < (timeForSelection * 1000) / 20; i++)
	{
		int xPos = Brain.Screen.xPosition();
		if (Brain.Screen.pressing())
		{
      if(xPos < 240){
        return false;
      }
      else{
        return true;
      }
		}
		wait(20, msec);
	}
  return false;
}

#endif