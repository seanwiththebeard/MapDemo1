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
  char temp = joy_read(0);
  
  if (joyState == temp)
  {
    ChangedState = false;
  }
  else
  {
    joyState = temp;
    ChangedState = true;
    joyStateLast = joyState;
  }
}

bool InputChanged()
{
  return ChangedState;
}

bool NoInput()
{
  if (joy_read(0) == 0)
    return true;
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