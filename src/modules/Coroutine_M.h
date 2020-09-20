#pragma once


// macro based coroutine inspired by protothread
//
// !!! INFO !!! 
// the current implementation needs to be implemented in ModuleHub
//

// Example

/*	

 * 
 * 
 * ///////		Coroutine_M		///////////
 * 
 *  // ModuleHub<Coroutine_M>
 * 
 *	bool isReady(){
 *		// ...
 *	}
 *
 *	void myTask(){
 *
 * 
 * 		__CR_START__
 * 
 * 		// do stuff
 * 
 * 		__CR_YIELD__
 * 
 * 		// do stuff
 * 
 * 		__WAIT_UNTIL(isReady())
 * 
 * 		// do stuff
 * 
 *		__CR_END__
 * 	}
 *
 *
 *
 *
 *
 */
 


//#define CR_GET_HANDLE	thisTask()->get<Coroutine_M>()

// mandatory statement
#define __CR_START(inHandle)															\
	Coroutine_M *handle = inHandle;														\
	switch(handle->mLine){												\
	case 0:{


// stores the current line and return, will restart at this point
#define __CR_YIELD__															\
	}handle->mLine = __LINE__;											\
	return;																		\
	case __LINE__:{


// yields until the condition is true, then stores the new line
#define __CR_WAIT_UNTIL(condition)												\
	}handle->mLine = __LINE__;											\
	case __LINE__:																\
	if(!(condition)){															\
		return;																	\
	}																			\
	CR_GET_HANDLE->mLine = __LINE__+1;case __LINE__+1:{


	

// restarts the coroutine
#define __CR_RESET__															\
	handle->mLine = 0;													\
	return;


// mandatory statement
#define __CR_END__         														\
	break;}																		\
	default:/* error case : should not happen */								\
	break;}																		\
	deleteTask(thisTaskHandle());


// TODO : 
//	CR_WHILE(condition) : loops and yield on every iterations



struct Coroutine_M
{
	
	uint16_t mLine;

	void init() { mLine = 0; }

	bool isExeReady() const { return true; }

	bool isDelReady() const { return true; } 

	void makePreExe(){}

	void makePreDel(){}

	void makePostExe(){}

};

