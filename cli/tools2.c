#include "cli.h"

/**
 * is_number - check if a string is a number
 * @string: string to check
 * Return: 1 if true, 0 if false
 */
bool is_number(char *string)
{
	int length = 0, i;

	length = strlen(string);
	for (i = 0; i < length; i++)
		if (!isdigit(string[i]))
			return (false);

	return (true);
}
