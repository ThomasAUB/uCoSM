#pragma once


// contains a buffer of the specified type and size

template<typename buffer_t, size_t size> 
struct Buffer_M
{

	void setData(buffer_t *inData, size_t inByteSize) {
		if(inByteSize > size*sizeof(buffer_t)) { return; }
		memcpy(mBuffer, inData, inByteSize);
	}

	void setDataAt(size_t inIdx, buffer_t inData) {
		if(inIdx >= size) { return; }
		mBuffer[inIdx] = inData;
	}
	
	buffer_t& getDataAt(size_t inIdx) {
		if(inIdx >= size) { return 0; }
		return mBuffer[inIdx];
	}
	
	buffer_t *getBuffer() {
		return mBuffer;
	}

	void init() {}

	bool isExeReady() const { return true; }

	bool isDelReady() const { return true; } 

	void makePreExe(){}

	void makePreDel(){}

	void makePostExe(){}

private:

	buffer_t mBuffer[size];
};

