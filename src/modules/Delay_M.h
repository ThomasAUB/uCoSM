#pragma once

#include "ucosm-sys-data.h"

struct Delay_M
{

	void setDelay(tick_t inDelay)
	{
		mExecution_time_stamp = SysKernelData::sGetTick()+inDelay;
	}
	
	tick_t getDelay()
	{	
		if(mExecution_time_stamp > SysKernelData::sGetTick()){
			return mExecution_time_stamp - SysKernelData::sGetTick(); 
		}else{
			return 0;
		}
	}


	template<typename T>
	void init(T *t)
	{
		mExecution_time_stamp = SysKernelData::sGetTick();
	}
	template<typename T>
    bool isExeReady(T *t) const 
	{
		return (SysKernelData::sGetTick() >= mExecution_time_stamp);
	}
	template<typename T>
	bool isDelReady(T *t) const { return true; }
	template<typename T>
	void makePreExe(T *t){}
	template<typename T>
	void makePreDel(T *t){}
	template<typename T>
	void makePostExe(T *t){}	
	
private:
	
	tick_t mExecution_time_stamp;
	
};



