#include "cedis/util.h"
#include <ctype.h>

char *toLower(char *str)
{
	for (char *p = str; *p; *p++)
		*p = tolower(*p);
	return str;
}
