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


// Handle syscall Close
// TODO: Describe this function
void
HandleSyscallCreate()
{
	DEBUG('a', "\nUnexpected exception Syscall Create: Not impelemted");
	printf("\nUnexpected exception Syscall Create: Not impelemted");
	interrupt->Halt();
}


// Handle syscall Open
// TODO: Describe this function
void
HandleSyscallOpen()
{
	DEBUG('a', "\nUnexpected exception Syscall Open: Not impelemted");
	printf("\nUnexpected exception Syscall Open: Not impelemted");
	interrupt->Halt();
}


// Handle syscall Read
// TODO: Describe this function
void
HandleSyscallRead()
{
	DEBUG('a', "\nUnexpected exception Syscall Read: Not impelemted");
	printf("\nUnexpected exception Syscall Write: Not impelemted");
	interrupt->Halt();
}


// Handle syscall Write
// TODO: Describe this function
void
HandleSyscallWrite()
{
	DEBUG('a', "\nUnexpected exception Syscall Write: Not impelemted");
	printf("\nUnexpected exception Syscall Write: Not impelemted");
	interrupt->Halt();
}


// Handle syscall Close
// TODO: Describe this function
void
HandleSyscallClose()
{
	DEBUG('a', "\nUnexpected exception Syscall Close: Not impelemted");
	printf("\nUnexpected exception Syscall Close: Not impelemted");
	interrupt->Halt();
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
		printf("\nUnexpected Error: System runned out of memory");
		return;
	}
	int len = 0;
	
	// Read data from console. Make sure this string is null-terminated (cstring)
	len = gSynchConsole->Read(tempBuffer, buffSize - 1);
	tempBuffer[len] = '\0';
	
	// Result
	System2User(buffAddr, len, tempBuffer);
	machine->WriteRegister(2, 0);
	machine->WriteRegister(2, len);	
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
