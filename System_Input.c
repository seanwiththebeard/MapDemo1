#include "Common.h"
#include <joystick.h>

byte joyState = 0;
byte joyStateLast = 0;
bool ChangedState = false;
byte joyTemp;

void InitializeInput()
{
  joy_install(joy_static_stddrv);
  
}
void UpdateInput()
{
  joyTemp = joy_read(0);
  
  if (joyState == joyTemp)
  {
    ChangedState = false;
  }
  else
  {
    joyState = joyTemp;
    ChangedState = true;
    joyStateLast = joyState;
  }
}

bool NoInput()
{
  if (joy_read(0) == 0)
    return true;
  return false;
}

bool InputChanged()
{
  if (!NoInput())
    return ChangedState;
  else
    return false;
}

bool InputUp()
{
  if (JOY_UP(joyState))
    return true;
  else
    return false;
}

bool InputDown()
{
  if (JOY_DOWN(joyState))
    return true;
  else
    return false;
}

bool InputLeft()
{
  if (JOY_LEFT(joyState))
    return true;
  else
    return false;
}

bool InputRight()
{
  if (JOY_RIGHT(joyState))
    return true;
  else
    return false;
}

bool InputFire()
{
  if (JOY_FIRE(joyState))
    return true;
  else
    return false;
}