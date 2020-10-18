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


// TODO: Describe this function
void
HandleSyscallHalt()
{
	DEBUG('a', "\nSystem call: Halt");
    interrupt->Halt();
}


// TODO: Describe this function
void
HandleSyscallExit()
{

}


// TODO: Describe this function
void
HandleSyscallExec()
{

}


// TODO: Describe this function
void
HandleSyscallJoin()
{

}


// TODO: Describe this function
void
HandleSyscallCreate()
{

}


// TODO: Describe this function
void
HandleSyscallOpen()
{

}


// TODO: Describe this function
void
HandleSyscallRead()
{

}


// TODO: Describe this function
void
HandleSyscallWrite()
{

}


// TODO: Describe this function
void
HandleSyscallClose()
{

}


// TODO: Describe this function
void
HandleSyscallFork()
{

}


// TODO: Describe this function
void
HandleSyscallYield()
{

}


// Handle exception from machine
// For SyscallException, each kind of syscall will be handled 
// separately in another function (for readability and maintainability)
// param which: Type of exception to handle
void
ExceptionHandler(ExceptionType which)
{
    switch (which)
    {
    	case NoException: // Do nothing due to no exception raised
    		break;
    	case PageFaultException: // No valid translation found
    		DEBUG('a', "\nUnexpected exception PageFaultException: No valid translation found");
    		printf("\nUnexpected exception PageFaultException: No valid translation found")
            interrupt->halt();
    		break;
    	case ReadOnlyException: // Write attempted to page marked "read-only"
    		DEBUG('a', "\nUnexpected exception ReadOnlyException: Write attempted to page marked \"read-only\"");
    		printf("\nUnexpected exception ReadOnlyException: Write attempted to page marked \"read-only\"");
    		interrupt->halt();
    		break;
    	case BusErrorException: // Translation resulted in an invalid physical address
    		DEBUG('a', "\nUnexpected exception BusErrorException: Translation resulted in an invalid physical address");
    		printf("\nUnexpected BusErrorException: exception Translation resulted in an invalid physical address");
    		interrupt->halt();
    		break;
    	case AddressErrorException: // Unaligned reference or one that was beyond the end of the address space
    		DEBUG('a', "\nUnexpected exception AddressErrorException: Unaligned reference or one that was beyond the end of the address space");
    		printf("\nUnexpected exception AddressErrorException: Unaligned reference or one that was beyond the end of the address space");
    		interrupt->halt();
    		break;
    	case OverflowException: // Integer overflow in add or sub.
    		DEBUG('a', "\nUnexpected exception OverflowException: Integer overflow in add or sub.");
    		printf("\nUnexpected exception OverflowException: Integer overflow in add or sub.");
    		interrupt->halt();
    		break;
		case IllegalInstrException:// Unimplemented or reserved instr.
		    DEBUG('a', "\nUnexpected exception IllegalInstrException: Unimplemented or reserved instr.");
    		printf("\nUnexpected exception IllegalInstrException: Unimplemented or reserved instr.");
    		interrupt->halt();
    		break;
		case NumExceptionTypes:
			DEBUG('a', "\nUnexpected exception NumExceptionTypes:");
    		printf("\nUnexpected exception NumExceptionTypes:");
    		interrupt->halt();
    		break;
    	case SyscallException:
    		// Get the type of syscall
    		int syscallType;
    		syscallType = machine->ReadRegister(2);
    		switch (syscallType)
    		{
    			case SC_Halt: // TODO: Describe syscall here
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
					HandleSyscallCreate();
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
				default:
					break;
    		}

    		// Increase program counter
    		int nextPcValue, currPcValue;
    		currPcValue = machine->ReadRegister(PCReg);
    		nextPcValue = machine->ReadRegister(NextPCReg);
    		machine->WriteRegister(PrevPCReg, currPcValue);
    		machine->WriteRegister(PCReg, nextPcValue)
    		machine->WriteRegister(NextPCReg, nextPcValue + 4);
    		break;
    	default:
    		break;
    }
}
