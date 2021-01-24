#include <common.h>
#include <joystick.h>

char joyState = 0;
char joyStateLast = 0;
bool ChangedState = false;

void InitializeInput()
{
  joy_install (joy_static_stddrv);
  
}
void UpdateInput()
{
  joyState = joy_read(0);
  
  if (joyState == joyStateLast)
  {
    ChangedState = false;
  }
  else
  {
    ChangedState = true;
    joyStateLast = joyState;
  }
}

bool InputChanged()
{
  return ChangedState;
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