#pragma once

#include "utils/Fifo.h"

// Allow to send data to a specific task

template<typename data_t, uint16_t fifo_size>
struct Signal_M
{
	
	bool send(Signal_M<data_t, fifo_size> *inReceiver, data_t inData){
		if(!inReceiver){return false;}
		return (inReceiver->mRxData.push(inData));
	}

	data_t receive(){
		return mRxData.pop();
	}

	bool hasData(){
		return !mRxData.isEmpty();
	}

	void init() {}

	bool isExeReady() { return true; }

	bool isDelReady() { return mRxData.isEmpty(); }

	void makePreExe() {}

	void makePreDel() {}

	void makePostExe(){}

private:
	
	Fifo<data_t, fifo_size> mRxData;
};

