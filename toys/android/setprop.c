/* setprop.c - Set an Android system property
 *
 * Copyright 2015 The Android Open Source Project

USE_SETPROP(NEWTOY(setprop, "<2>2", TOYFLAG_USR|TOYFLAG_SBIN))

config SETPROP
  bool "setprop"
  default y
  help
    usage: setprop NAME VALUE

    Sets an Android system property.
*/

#define FOR_setprop
#include "toys.h"

#if defined(__ANDROID__)
#include <cutils/properties.h>
#endif

void setprop_main(void)
{
#if defined(__ANDROID__)
  char *name = toys.optargs[0], *value = toys.optargs[1];
  char *p;
  size_t name_len = strlen(name), value_len = strlen(value);

  // property_set doesn't tell us why it failed, and actually can't
  // recognize most failures (because it doesn't wait for init), so
  // we duplicate all of init's checks here to help the user.

  if (name_len >= PROP_NAME_MAX)
    error_exit("name '%s' too long; try '%.*s'",
               name, PROP_NAME_MAX - 1, name);
  if (value_len >= PROP_VALUE_MAX)
    error_exit("value '%s' too long; try '%.*s'",
               value, PROP_VALUE_MAX - 1, value);

  if (*name == '.' || name[name_len - 1] == '.')
    error_exit("property names must not start or end with '.'");
  if (strstr(name, ".."))
    error_exit("'..' is not allowed in a property name");
  for (p = name; *p; ++p)
    if (!isalnum(*p) && !strchr("_.-", *p))
      error_exit("invalid character '%c' in name '%s'", *p, name);

  if (property_set(name, value))
    error_msg("failed to set property '%s' to '%s'", name, value);
#endif
}
