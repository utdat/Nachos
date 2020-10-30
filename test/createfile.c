/* createfile.c
 * Program help users create an empty file with name input from IO device
*/

#include "syscall.h"


#define CREDIT "***Create file program | Max filename length allowed: 500***\n"
#define CREDIT_LEN 61

#define REQUEST_MSG "File name: "
#define REQUEST_MSG_LEN 11

#define SUCCESS 0
#define SUCCESS_MSG "File created successfully!\n"
#define SUCCESS_MSG_LEN 27
#define ERROR_MSG "Error(s) occured when creating file\n"
#define ERROR_MSG_LEN 36


int main()
{
	char filename[500];
	PrintS(CREDIT, CREDIT_LEN);
	
	// Request file name
	PrintS(REQUEST_MSG, REQUEST_MSG_LEN);
	ReadS(filename, 500);
	
	// Create file and catching error
	if (Create(filename) == SUCCESS)
	{
		PrintS(SUCCESS_MSG, SUCCESS_MSG_LEN);
	}
	else
	{
		PrintS(ERROR_MSG, ERROR_MSG_LEN);
	}

	return 0;
}
