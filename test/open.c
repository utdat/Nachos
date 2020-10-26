/* open.c
 * Simple program to test open file syscall
 * Just open file and do nothing
 */

#include "syscall.h"

#define PROGRAM_INS "***Open file | Max file length 64***\n"
#define PROGRAM_INS_LEN 37
#define SUCCESS_MSG "File was successfully opened\n"
#define SUCCESS_MSG_LEN 29
#define ERROR_MSG "Error\n"
#define ERROR_MSG_LEN 6

#define NAME_BUFFER_SIZE 65

int main()
{
	char fileName[NAME_BUFFER_SIZE];
	OpenFileId id;
	char txtId[4];
	txtId[0] = '0';
	txtId[1] = '0';
	txtId[2] = '\n';
	txtId[3] = '\0';

	PrintS(PROGRAM_INS, PROGRAM_INS_LEN);
	ReadS(fileName, NAME_BUFFER_SIZE);
	id = Open(fileName, 0);
	if (id >= 0)
	{
		PrintS(SUCCESS_MSG, SUCCESS_MSG_LEN);
		if (id == 10)
		{
			txtId[0] = '1';
		}
		else
		{
			txtId[1] = '0' + id;
		}
		PrintS(txtId, 3);
	}
	else
	{
		PrintS(ERROR_MSG, ERROR_MSG_LEN);
	}
	
	return 0;
}
