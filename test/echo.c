/* echo.c
 * Program that read user input then print it from/to IO device
 * Program will repeat above task until user input nothing (leave empty)
*/

#include "syscall.h"

#define ECHO_INS "***Echo program | Max length allowed: 500 | Leave empty to quit***\n"
#define ECHO_LEN 67


int main()
{
	// Init
	char inp[500];
	int len;
	PrintS(ECHO_INS, ECHO_LEN);	
	
	// Init len that suppose user (still) want to input (for first run)
	len = 1;

	// Repeatedly read and print user input
	while (len > 0)
	{
		len = ReadS(inp, 500); 
		PrintS(inp, len);

		// Line break
		PrintS("\n", 1);
	}
	
	return 0;
}
