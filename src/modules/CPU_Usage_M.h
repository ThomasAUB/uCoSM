#pragma once

#include "ucosm-sys-data.h"

#include <limits>


struct CPU_Usage_M
{

	/*static void setTimeBase(tick_t (*inGetTick)(), tick_t inMaxValue){
		sCPUGetTick = inGetTick;
		sMaxTickValue = inMaxValue;
	}*/
	
	static void setTimerOverFlow(){	
		// to check ?
		sTimerOverFlow++;
	}

	tick_t getCPU_UsagePercent(){
		return ( ( mExeTime * 100 )/ mLoopTime );
	}

	tick_t getExecutionTime(){
		return mExeTime;
	}

	tick_t getMaxExecutionTime(){
		return mMaxExeTime;
	}

	tick_t getCallPeriod(){
		return mLoopTime;
	}

	template<typename T>
	void init(T *t) {
		mStartExeTS = 0;
		mExeTime = 0;
		mMaxExeTime = 0;
		mStartLoopTS = 0;
		mLoopTime = 1;
	}

	template<typename T>
	bool isExeReady(T *t) const { return true; }
	template<typename T>
	bool isDelReady(T *t) const { return true; } 

	template<typename T>
	void makePreExe(T *t){
		mStartExeTS = sGetTick();
	}

	template<typename T>
	void makePostExe(T *t){
		tick_t curTS = sGetTick();

		mExeTime = curTS - mStartExeTS;

		if(mExeTime > mMaxExeTime){
			mMaxExeTime = mExeTime;
		}

		mLoopTime = curTS - mStartLoopTS;
		mStartLoopTS = curTS;
	}

	template<typename T>
	void makePreDel(T *t){}
	
private:

	static tick_t (*sGetTick)();
	static tick_t sMaxTickValue;
	static uint8_t sTimerOverFlow;

	tick_t mStartExeTS;
	tick_t mExeTime;
	tick_t mMaxExeTime;

	tick_t mStartLoopTS;
	tick_t mLoopTime;

};

tick_t (*CPU_Usage_M::sGetTick)() = SysKernelData::sGetTick;
tick_t CPU_Usage_M::sMaxTickValue = std::numeric_limits<tick_t>::max();
uint8_t CPU_Usage_M::sTimerOverFlow = 0;
