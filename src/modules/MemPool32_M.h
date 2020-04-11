#pragma once





// Fixed size dynamic memory allocation
// features :
//  - Allows to allocate and release a buffer of sizeof(elem_t) bytes
//  - Forbids task deletion if a buffer is allocated to avoid memory leakage
//  - The number of buffer per task types has a maximum value of 32
//	- If auto_release is "true", the module will automatically release allocated memory on deletion 

template<typename elem_t, uint16_t elem_count, bool auto_release = true>
struct MemPool32_M
{
	static_assert(elem_count <= 32, "size of pool must not exceed 32");
	
	static_assert( (sizeof(elem_t) * sizeof(elem_count) ) > 4, 
	"Suboptimal implementation : Pool's size inferior to overhead's");

	template<typename T>
	T *allocate()
	{
		static_assert(sizeof(T) <= sizeof(elem_t), "Allocation error");
		elem_t *e = allocate();
		if(e == nullptr){ return nullptr; }
		return reinterpret_cast<T *>(e);
	}
		
	// could use "placement new" here
	elem_t *allocate()
	{
		// pool is full
		if(mMemoryMap == (1<<elem_count)-1){ return nullptr; }

		// task already has allocated memory
		if(mAllocIndex){ return nullptr; }

		
		uint8_t i=0;
		
		do{
			if(!mMemoryMap&(1<<i)) // slot free
			{
				mMemoryMap |= (1<<i); // take slot

				mAllocIndex = i; // stores the index for fast deletion
				
				// set task alloc active with a boolean,
				// allows to check allocation in case index is 0
				mAllocIndex |= kAllocBoolMask;
				
				return &mElems[i];
			}
		}while(++i<elem_count);
		
		// allocation error : should not happen
		return nullptr;
	}
		
	bool release()
	{

		// pool is empty, nothing to free
		if(!mMemoryMap){ return false; }

		// task has no allocated memory
		if(!mAllocIndex){ return false; }

		// remove boolean alloc state
		mAllocIndex &= ~kAllocBoolMask;

		// security check : verify that the memory has been allocated
		// critical error : map and index does not coincide
		if(!(mMemoryMap&(1<<mAllocIndex))){ return false; }

		// release memory
		mMemoryMap &= ~(1<<mAllocIndex);

		// delete index
		mAllocIndex = 0;
		return true;

	}

	template<typename T>
	T *getMemory()
	{
		static_assert(sizeof(T) <= sizeof(elem_t), "Allocation error");
		elem_t *e = getMemory();
		if(e == nullptr){ return nullptr; }
		return reinterpret_cast<T *>(e);
	}

	elem_t *getMemory()
	{
		if(!mAllocIndex){ return nullptr; }
		return &mElems[mAllocIndex&(~kAllocBoolMask)];
	}

	template<typename T>
	void init(T *t) { mAllocIndex = 0; }
	template<typename T>
	bool isExeReady(T *t) const { return true; }
	
	// decide if deletion forbidden if allocated memory or auto release?
	template<typename T>
	bool isDelReady(T *t) 
	{ 
		if(mAllocIndex) // memory is allocated
		{
			if constexpr (auto_release)
				return release();
			else
				return false;
		}
		return true;
	}
	template<typename T>
	void makePreExe(T *t){}
	template<typename T>
	void makePreDel(T *t){}
	template<typename T>
	void makePostExe(T *t){}
	
private:

	uint8_t mAllocIndex;
	
	static elem_t mElems[elem_count];
	static uint32_t mMemoryMap;
	static const uint8_t kAllocBoolMask = 0b10000000;

};

template <typename elem_t, uint16_t elem_count, bool auto_release>
elem_t MemPool32_M<elem_t, elem_count, auto_release>::mElems[elem_count];

template <typename elem_t, uint16_t elem_count, bool auto_release>
uint32_t MemPool32_M<elem_t, elem_count, auto_release>::mMemoryMap = 0;

