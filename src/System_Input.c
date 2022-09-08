#include "Common.h"
#if __C64__
#include <joystick.h>
#endif

byte joyState = 0;
byte joyStateLast = 0;
bool ChangedState = false;
byte joyTemp;

void InitializeInput()
{
  #if __C64__
  joy_install(joy_static_stddrv);
  #endif
}
void UpdateInput()
{
  ++randseed;
  #if __C64__
  joyTemp = joy_read(0);
  #endif

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
  #if __C64__
  if (joy_read(0) == 0)
    return true;
  #endif
  return false;
}

bool InputChanged()
{
  #if __C64__
  if (!NoInput())
    return ChangedState;
  else
  #endif
    return false;
}

bool InputUp()
{
  #if __C64__
  if (JOY_UP(joyState))
    return true;
  else
  #endif
    return false;
}

bool InputDown()
{
  #if __C64__
  if (JOY_DOWN(joyState))
    return true;
  else
  #endif
    return false;
}

bool InputLeft()
{
  #if __C64__
  if (JOY_LEFT(joyState))
    return true;
  else
  #endif
    return false;
}

bool InputRight()
{
  #if __C64__
  if (JOY_RIGHT(joyState))
    return true;
  else
  #endif
    return false;
}

bool InputFire()
{
  #if __C64__
  if (JOY_FIRE(joyState))
    return true;
  else
  #endif
    return false;
}