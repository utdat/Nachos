#include "syscall.h"

#define ECHO_INS "***Echo program | Max length allowed: 500 | Leave empty to quit***\n"
#define ECHO_LEN 67

#define ENDL "\n"
#define ENDL_LEN 1

int main()
{
	char inp[500];
	int len;
	PrintS(ECHO_INS, ECHO_LEN);	
	
	// Init l for first run
	len = 1;
	while (len > 0)
	{
		len = ReadS(inp, 500); 
		PrintS(inp, len);
		PrintS(ENDL, ENDL_LEN);
	}
	
	return 0;
}
