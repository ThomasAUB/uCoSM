#pragma once


// macro based coroutine inspired by protothread
//
// !!! INFO !!! 
// the current implementation needs to be implemented in ModuleKit


// Example

/*	

 * 
 * 
 * ///////		Coroutine_M	without context		///////////
 * 
 *  // ModuleKit<Coroutine_M<>>
 * 
 *	bool isReady(){
 *		// ...
 *	}
 *
 *	void myTask(){
 *
 * 		COROUTINE_MAX_CONTEXT_SIZE(0)
 * 
 * 		CR_START
 * 
 * 		// do stuff
 * 
 * 		CR_YIELD
 * 
 * 		// do stuff
 * 
 * 		WAIT_UNTIL(isReady())
 * 
 * 		// do stuff
 * 
 *		CR_END
 * 	}
 *
 *
 * ///////		Coroutine_M with context saving		///////////
 * 
 * 
 *  // ModuleKit<Coroutine_M<myContextSize>>
 * 
 *	bool isReady(){
 *		// ...
 *	}
 *
 *	void myTask(){
 *
 * 		COROUTINE_MAX_CONTEXT_SIZE(myContextSize)
 * 		
 *	 	CR_CTX_START // definition of the context //
 *
 *		uint8_t k = 2;
 * 		uint32_t j = 10
 * 
 *		CR_CTX_END(myContext) 
 *
 * 
 * 		CR_START
 * 
 * 		// do stuff
 * 		myContext.k++;
 * 
 * 		CR_YIELD
 * 
 * 		// do stuff
 * 		myContext.j++;
 * 
 * 		WAIT_UNTIL(isReady())
 * 
 * 		// do stuff
 * 
 *		CR_END
 * 	}
 *
 *
 *
 */
 

#include <limits>

using cr_line_t = uint16_t;
const uint16_t kFirst_CR_Execution = std::numeric_limits<cr_line_t>::max();



#define COROUTINE_MAX_CONTEXT_SIZE(size)										\
	using cr_internal_t = Coroutine_M<size>;


#define CR_GET_HANDLE	thisTaskHandle()->get<cr_internal_t>()


// context
#define CR_CTX_START															\
	struct Ctx_def{

#define CR_CTX_END(label)														\
	};																			\
	Ctx_def label = CR_GET_HANDLE->getContext<Ctx_def>();




// mandatory statement
#define CR_START																\
	switch(CR_GET_HANDLE->mLine){												\
	case kFirst_CR_Execution:													\
		CR_GET_HANDLE->mLine = 0;												\
	case 0:{


// stores the current line and return, will restart at this point
#define CR_YIELD																\
	}CR_GET_HANDLE->mLine = __LINE__;											\
	return;																		\
	case __LINE__:{


// yields until the condition is true, then stores the new line
#define CR_WAIT_UNTIL(condition)												\
	}CR_GET_HANDLE->mLine = __LINE__;											\
	case __LINE__:																\
	if(!(condition)){															\
		return;																	\
	}																			\
	CR_GET_HANDLE->mLine = __LINE__+1;case __LINE__+1:{


// set delay and YIELD
#define CR_WAIT_FOR(delay)														\
	thisTaskHandle()->get<Interval_M>()->setDelay(delay);						\
	CR_YIELD
	

// restarts the coroutine
#define CR_RESET																\
	CR_GET_HANDLE->mLine = 0;													\
	return;


// mandatory statement
#define CR_END         															\
	break;}																		\
	default:/* error case : should not happen */								\
	break;}																		\
	deleteTask(thisTaskHandle());


// TODO : 
//	CR_WHILE(condition) : loops and yield on every iterations
//	CR_DO and CR_WHILE(condition) : loops and yield on every iterations







template<size_t max_context_size = 0>
struct Coroutine_M
{
	
	cr_line_t mLine;

	void init() { mLine = kFirst_CR_Execution; }

	bool isExeReady() const { return true; }

	bool isDelReady() const { return true; } 

	void makePreExe(){}

	void makePreDel(){}

	void makePostExe(){}
	
	template<typename T>
	T& getContext()
	{
		static_assert(sizeof(T) <= sizeof(mContext), "Coroutine context size error");
		if(mLine == kFirst_CR_Execution){
			// instantiate T inside context buffer
			T temp;
			uint8_t *dest = mContext;
			uint8_t *src = reinterpret_cast<uint8_t *>(&temp);
			const uint8_t *end = src+sizeof(T);
			while(src != end){
				*dest++ = *src++;
			}
		}
		return *reinterpret_cast<T *>(mContext);
	}

private : 

	uint8_t mContext[max_context_size];
	
	
};



/*
struct Coroutine_M
{

	uint16_t mLine;

	void init() { mLine = kFirst_CR_Execution;}

	bool isExeReady() const { return true; }

	bool isDelReady() const { return true; } 

	void makePreExe(){}

	void makePreDel(){}

	void makePostExe(){}
	
};*/

