// exception.cc 
//	Entry point into the Nachos kernel from user programs.
//	There are two kinds of things that can cause control to
//	transfer back to here from user code:
//
//	syscall -- The user code explicitly requests to call a procedure
//	in the Nachos kernel.  Right now, the only function we support is
//	"Halt".
//
//	exceptions -- The user code does something that the CPU can't handle.
//	For instance, accessing memory that doesn't exist, arithmetic errors,
//	etc.  
//
//	Interrupts (which can also cause control to transfer from user
//	code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "syscall.h"

//----------------------------------------------------------------------
// ExceptionHandler
// 	Entry point into the Nachos kernel.  Called when a user program
//	is executing, and either does a syscall, or generates an addressing
//	or arithmetic exception.
//
// 	For system calls, the following is the calling convention:
//
// 	system call code -- r2
//		arg1 -- r4
//		arg2 -- r5
//		arg3 -- r6
//		arg4 -- r7
//
//	The result of the system call, if any, must be put back into r2. 
//
// And don't forget to increment the pc before returning. (Or else you'll
// loop making the same system call forever!
//
//	"which" is the kind of exception.  The list of possible exceptions 
//	are in machine.h.
//----------------------------------------------------------------------


// Max length of filename allow on system
#define MAX_LENGTH 64

// Copy memory from user memory to system memory
// param virtAddr: Address in user memory
// param limit: Max length of memory space
// return: Data as array of byte. Must be freed manually
char* 
User2System(int virtAddr,int limit)
{
	if (limit <= 0) return NULL;

	// Buffer to hold memory
	char* buffer = new char[limit + 1];
	if (buffer == NULL) return NULL;
	memset(buffer, '\0', limit + 1);

	// Read memory from user memory and parse it to buffer
	int c;
	for (int i = 0; i < limit; i++)
	{
		machine->ReadMem(virtAddr + i, 1, &c);
		buffer[i] = (char)c;
		if (c == 0) break;
	}

	return buffer;
}


// Copy memory from system memory to user memory
// param virtAddr: Address in user memory to parse data to
// param len: Size of data
// param buffer: data to coppy
// Return: Number of bytes copied
int
System2User(int virtAddr, int len, char* buffer)
{
	if (len < 0) return -1;
	if (len == 0) return 0;

	// Copy data from buffer to user memory
	int i = 0;
	while (i < len)
	{
		int c = (int) buffer[i];
		machine->WriteMem(virtAddr + i, 1, c);

		if (c == 0) break;
		i++;
	}
}


// Handle syscall Halt
// Print system information and halt the os
void
HandleSyscallHalt()
{
	DEBUG('a', "\nSystem call: Halt");
    interrupt->Halt();
}


// Handle syscall Exit
// TODO: Describe this function
void
HandleSyscallExit()
{
	// Program exit
	// Do nothing other than log debug info
	DEBUG('a', "\nException Syscall Exit:");
}


// Handle syscall Exec
// TODO: Describe this function
void
HandleSyscallExec()
{
	DEBUG('a', "\nUnexpected exception Syscall Exec: Not impelemted");
	printf("\nUnexpected exception Syscall Exec: Not impelemted");
	interrupt->Halt();
}


// Handle syscall Join
// TODO: Describe this function
void
HandleSyscallJoin()
{
	DEBUG('a', "\nUnexpected exception Syscall Join: Not impelemted");
	printf("\nUnexpected exception Syscall Join: Not impelemted");
	interrupt->Halt();
}


// Handle syscall Create
// Create file with specified name in Nachos file system
// Set result 0 if success, -1 otherwise
void
HandleSyscallCreate()
{
	// Get file name
	int filenameAddr = machine->ReadRegister(4);
	char* filename = User2System(filenameAddr, MAX_LENGTH);

	if (filename == NULL)
	{
		DEBUG('a', "\nUnexpected error: System could not allocate memory for file name");
		printf("\nUnexpected error: System could not allocate memory for file name");
		machine->WriteRegister(2, -1);
		return;
	}

	// Create empty file using built-in file system
	// Catch the error from built-in file system (if any)
	if (!fileSystem->Create(filename, 0))
	{
		DEBUG('a', "\nUnexpected error: System could not create file");
		printf("\nUnexpected error: System could not create file");
		machine->WriteRegister(2, -1);
		return;
	}

	// File created successfully
	machine->WriteRegister(2, 0);
	delete[] filename;
}


// Handle syscall Open
// Open file with specified file name and type
// Return file ID in file system
void
HandleSyscallOpen()
{
	// Fetch params
	int fileNameAddr = machine->ReadRegister(4);
	int type = machine->ReadRegister(5);
	
	// Process stdin and stdout open
	if (type == 2)
	{
		machine->WriteRegister(2, 0);
		return;
	}
	if (type == 3)
	{
		machine->WriteRegister(3, 0);
		return;
	}

	// Bound check file name
	char* fileName = User2System(fileNameAddr, MAX_LENGTH);
	if (fileName == NULL) // Cant get file name
	{
		DEBUG('a', "\nUnexpected Error: System runned out of memory");
		printf("\nUnexpected Error: System runned out of memory");
		machine->WriteRegister(2, -1);
		return;
	}
	if (fileName[0] == '\0') // Empty name
	{
		DEBUG('a', "\nUnexpected error when trying to open file: Invalid file name");
		printf("\nUnexpected error when trying to open file: Invalid file name");
		machine->WriteRegister(2, -1);
		delete[] fileName;
		return;
	}
	
	// Try to open file and get id
	int fileId = 0; 
	if (fileSystem->Open(fileName, type, fileId) != NULL)
	{
		machine->WriteRegister(2, fileId);
	}
	else if (fileId < 0)
	{
		
		DEBUG('a', "\nUnexpected error: Not enough slot for opening file");
		printf("\nUnexpected error: Not enough slot for opening file");
		machine->WriteRegister(2, -1);	
	}
	else
	{
		DEBUG('a', "\nUnexpected error: Could not open file. Maybe file do not exist");
		printf("\nUnexpected error: Could not open file. Maybe file do not exist");
		machine->WriteRegister(2, -1);
	}
	
	// Free space used for file name
	delete[] fileName;
}


// Handle syscall Read
// Read data from file with file id given
// If file is stdin and stdout, use console IO instead of file system
// Return number of bytes read/written
// Return -1 on error, -2 on eof reached
void
HandleSyscallRead()
{
	// Fetch param
	int bufferAddr = machine->ReadRegister(4);
	int byteCount = machine->ReadRegister(5);
	int fileId = machine->ReadRegister(6);
	
	// Get file from file system
	OpenFile* file = fileSystem->FileAt(fileId);

	// Check for file with given id not found
	if (file == NULL)
	{
		DEBUG('a', "\nUnexpected error: File system could not provide file with given id");
		printf("\nUnexpected error: File system could not provide file with given id");
		machine->WriteRegister(2, -1);
		return;
	}

	// Check for reading stdout
	if (file->Type() == 3)
	{
		DEBUG('a', "\nIllegal action: Could not read from stdout");
		printf("\nIllegal action: Could not read from stdout");
		machine->WriteRegister(2, -1);
		return;
	}

	// Allocate space for data read. 1 more byte for null terminated charecter
	char* buffer = new char[byteCount];

	// Read data
	int byteRead;
	if (file->Type() == 2)
	{	
		// Read from console. Reserved final byte from null character
		byteRead = gSynchConsole->Read(buffer, byteCount - 1);
		if (byteRead > 0)
		{
			// Read successfully
			buffer[byteRead] = '\0';
			System2User(bufferAddr, byteRead + 1, buffer);
			machine->WriteRegister(2, byteRead);
		}
		else
		{
			// End of file
			machine->WriteRegister(2, -2);
		}
	}
	else
	{
		// Read from file
		byteRead = file->Read(buffer, byteCount);
		if (byteRead > 0)
		{
			// Read successfully
			System2User(bufferAddr, byteRead, buffer);
			machine->WriteRegister(2, byteRead);
		}
		else
		{
			// End of file
			machine->WriteRegister(2, -2);
		}
	}
	
	// Free buffer space
	delete[] buffer;
}


// Handle syscall Write
// Write certain specified bytes to file with id given
// Return number of bytes actually written. -1 on error. -2 on eof
void
HandleSyscallWrite()
{
	// Fetch params
	int bufferAddr = machine->ReadRegister(4);
	int byteCount = machine->ReadRegister(5);
	int fileId = machine->ReadRegister(6);
	
	// Get file from file system
	OpenFile* file = fileSystem->FileAt(fileId);
	if (file == NULL)
	{
		DEBUG('a', "\nUnexpected error: File system could not provide file with given id");
		printf("\nUnexpected error: File system could not provide file with given id");
		machine->WriteRegister(2, -1);
		return;
	}

	// Prevent writing to stdin
	if (file->Type() == 2)
	{
		DEBUG('a', "\nIllegal action: Could not write to stdin");
		printf("\nIllegal action: Could not write to stdin");
		machine->WriteRegister(2, -1);
		return;
	}

	if (file->Type() == 1)
	{
		DEBUG('a', "\nIllegal action: Could not write to read-only file");
		printf("\nIllegal action: Could not write to read-only file");
		machine->WriteRegister(2, -1);
		return;
	}

	// Get buffer from user space
	char* buffer = User2System(bufferAddr, byteCount);
	if (buffer == NULL)
	{
		DEBUG('a', "\nUnexpected error: System could not allocate memory for buffer");
		printf("\nUnexpected error: System could not allocate memory for buffer");
		machine->WriteRegister(2, -1);
		return;
	}

	// Write
	int byteWritten = 0;
	if (file->Type() == 3)
	{
		// Write to console byte by byte
		while (buffer[byteWritten] != '\0' && byteWritten < byteCount)
		{
			gSynchConsole->Write(buffer + byteWritten, 1);
			++byteWritten;
		}
	}
	else
	{
		// Write to file
		byteWritten = file->Write(buffer, byteCount);
	}

	// Check for error and return result
	if (byteWritten < 0)
	{
		DEBUG('a', "\nUnexpected error: Write to file failed");
		printf("\nUnexpected error: Write to file failed");
		machine->WriteRegister(2, -1);
	}
	else
	{
		machine->WriteRegister(2, byteWritten);
	}

	// Deallocate buffer
	delete[] buffer;
}


// Handle syscall Close
// Close a file with given OpenFileId
void
HandleSyscallClose()
{
	int fileId = machine->ReadRegister(4);
	if (fileSystem->Close(fileId) < 0)
	{
		DEBUG('a', "\nUnexpected error when close a file: Invalid File ID");
		printf("\nUnexpected error when close a file: Invalid File ID");
	}
	
	// File was closed successfully
}


// Handle syscall Fork
// TODO: Describe this function
void
HandleSyscallFork()
{
	DEBUG('a', "\nUnexpected exception Syscall Fork: Not impelemted");
	printf("\nUnexpected exception Syscall Fork: Not impelemted");
	interrupt->Halt();
}


// Handle syscall Yield
// TODO: Describe this function
void
HandleSyscallYield()
{
	DEBUG('a', "\nUnexpected exception Syscall Yield: Not impelemted");
	printf("\nUnexpected exception Syscall Yield: Not impelemted");
	interrupt->Halt();
}


// Handle syscall PrintS
// Print a string to console
void
HandleSyscallPrintS()
{
	// Fetch params	
	int len = machine->ReadRegister(5);
	if (len <= 0) return;

	int strAddr = machine->ReadRegister(4);
	char* str = User2System(strAddr, len);
	if (str == NULL) return;

	// Write to console. Null character included
	gSynchConsole->Write(str, len + 1);

	delete[] str;
}


// Handle syscall ReadS
// Read a string from console
void
HandleSyscallReadS()
{
	// Fetch params
	int buffAddr = machine->ReadRegister(4);
	int buffSize = machine->ReadRegister(5);

	// Allocate temporary buffer for reading data
	char* tempBuffer = new char[buffSize];
	if (tempBuffer == NULL)
	{
		machine->WriteRegister(2, 0);
		DEBUG('a', "\nUnexpected error: System could not allocate memory for buffer");
		printf("\nUnexpected error: System could not allocate memory for buffer");
		return;
	}
	int len = 0;
	
	// Read data from console. Make sure this string is null-terminated (cstring)
	len = gSynchConsole->Read(tempBuffer, buffSize - 1);
	tempBuffer[len] = '\0';
	
	// Result
	System2User(buffAddr, len + 1, tempBuffer);
	machine->WriteRegister(2, len);	

	// Deallocate buffer
	delete[] tempBuffer;
}


// Handle exception from machine
// For SyscallException, each kind of syscall will be handled 
// separately in another function (for readability and maintainability)
// param which: Type of exception to handle
void
ExceptionHandler(ExceptionType which)
{
	// Get syscall type. Used in case of SyscallException
	int syscallType = machine->ReadRegister(2);

    switch (which)
    {
    	case NoException: // Do nothing due to no exception raised
    		break;
    	case PageFaultException: // No valid translation found
    		DEBUG('a', "\nUnexpected exception PageFaultException: No valid translation found");
    		printf("\nUnexpected exception PageFaultException: No valid translation found");
            interrupt->Halt();
    		break;
    	case ReadOnlyException: // Write attempted to page marked "read-only"
    		DEBUG('a', "\nUnexpected exception ReadOnlyException: Write attempted to page marked \"read-only\"");
    		printf("\nUnexpected exception ReadOnlyException: Write attempted to page marked \"read-only\"");
    		interrupt->Halt();
    		break;
    	case BusErrorException: // Translation resulted in an invalid physical address
    		DEBUG('a', "\nUnexpected exception BusErrorException: Translation resulted in an invalid physical address");
    		printf("\nUnexpected BusErrorException: exception Translation resulted in an invalid physical address");
    		interrupt->Halt();
    		break;
    	case AddressErrorException: // Unaligned reference or one that was beyond the end of the address space
    		DEBUG('a', "\nUnexpected exception AddressErrorException: Unaligned reference or one that was beyond the end of the address space");
    		printf("\nUnexpected exception AddressErrorException: Unaligned reference or one that was beyond the end of the address space");
    		interrupt->Halt();
    		break;
    	case OverflowException: // Integer overflow in add or sub.
    		DEBUG('a', "\nUnexpected exception OverflowException: Integer overflow in add or sub.");
    		printf("\nUnexpected exception OverflowException: Integer overflow in add or sub.");
    		interrupt->Halt();
    		break;
		case IllegalInstrException:// Unimplemented or reserved instr.
		    DEBUG('a', "\nUnexpected exception IllegalInstrException: Unimplemented or reserved instr.");
    		printf("\nUnexpected exception IllegalInstrException: Unimplemented or reserved instr.");
    		interrupt->Halt();
    		break;
		case NumExceptionTypes:
			DEBUG('a', "\nUnexpected exception NumExceptionTypes:");
    		printf("\nUnexpected exception NumExceptionTypes:");
    		interrupt->Halt();
    		break;
    	case SyscallException:
    		switch (syscallType)
    		{
    			case SC_Halt: // Print system information and halt the os
    				HandleSyscallHalt();
    				break;
				case SC_Exit: // TODO: Describe syscall here
					HandleSyscallExit();
					break;
				case SC_Exec: // TODO: Describe syscall here
					HandleSyscallExec();
					break;
				case SC_Join: // TODO: Describe syscall here		
					HandleSyscallJoin();
					break;
				case SC_Create:	// TODO: Describe syscall here 
					HandleSyscallCreate();
					break;
				case SC_Open: // TODO: Describe syscall here	
					HandleSyscallOpen();
					break;
				case SC_Read: // TODO: Describe syscall here
					HandleSyscallRead();
					break;
				case SC_Write: // TODO: Describe syscall here
					HandleSyscallWrite();
					break;
				case SC_Close: // TODO: Describe syscall here	
					HandleSyscallClose();
					break;
				case SC_Fork: // TODO: Describe syscall	here
					HandleSyscallFork();
					break;
				case SC_Yield: // TODO: Describe syscall here
					HandleSyscallYield();
					break;

				case SC_PrintS: // Print a string to console
					HandleSyscallPrintS();
					break;
				case SC_ReadS: // Read a string from console
					HandleSyscallReadS();
					break;
				default:
					break;
    		}

    		// Increase program counter
    		int nextPcValue, currPcValue;
    		currPcValue = machine->ReadRegister(PCReg);
    		nextPcValue = machine->ReadRegister(NextPCReg);
    		machine->WriteRegister(PrevPCReg, currPcValue);
    		machine->WriteRegister(PCReg, nextPcValue);
    		machine->WriteRegister(NextPCReg, nextPcValue + 4);
    		break;
    	default:
    		break;
    }
}
