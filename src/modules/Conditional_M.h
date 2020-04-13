#pragma once

// contains a free function pointer "mCondition"
// the execution will if the function attributed with "setCondition" returns true
// or if the task has no attributed condition

struct Conditional_M
{	
	void setCondition(bool (*inCondition)()){
		mCondition = inCondition;
	}
	
	void init(){
		mCondition = nullptr;
	}

	bool isExeReady(){
		if(!mCondition)	return true;
		return mCondition();
	}

	bool isDelReady() const { return true; }

	void makePreExe(){}

	void makePreDel(){}

	void makePostExe(){}	
	
private:

	bool (*mCondition)();
	
};

