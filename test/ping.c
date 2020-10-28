/* ping.c
 * Simple program that repeatedly print letter A 1000 times
*/

#include "syscall.h"

int main()
{
	int i;
	for (i = 0; i < 1000; i++)
	{
		PrintS("A", 1);
	}
	return 0;
}
