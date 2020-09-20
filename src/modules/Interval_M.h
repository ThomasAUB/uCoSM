#pragma once

#include "ucosm-sys-data.h"

// allow to specify a time period of execution of the process or postpone 
// its execution
struct Interval_M
{

	using period_t = uint16_t;
	
	void setPeriod(period_t inPeriod){
		mPeriod = inPeriod;
	}

	void setDelay(tick_t inDelay){
		mExecution_time_stamp = SysKernelData::sGetTick()+inDelay; 
	}

	period_t getPeriod(){
		return mPeriod;
	}
	
	tick_t getDelay(){
		if(mExecution_time_stamp > SysKernelData::sGetTick()){
			return mExecution_time_stamp - SysKernelData::sGetTick(); 
		}else{
			return 0;
		}
	}

	void init()
	{
		mPeriod = 0;
		mExecution_time_stamp = SysKernelData::sGetTick();
	}

	bool isExeReady() const {
		return (SysKernelData::sGetTick() >= mExecution_time_stamp);
	}

	bool isDelReady() { return true; }

	void makePreExe()
	{
		mExecution_time_stamp += mPeriod;
	}

	void makePreDel(){}

	void makePostExe(){}
		
private:
	
	tick_t mExecution_time_stamp;
	period_t mPeriod;
};
