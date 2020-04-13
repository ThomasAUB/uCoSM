#pragma once

// provides a simple status container


enum eStatus
{
	eSuspended			= 0b00000100,
	eLocked				= 0b00001000,
	eStatusMask			= 0b00001111
};
/*
enum eNotifyStatus
{
	eNotifyFirstExe		= 0b00010000,
	eNotifyExeStart		= 0b00100000,
	eNotifySuspension	= 0b01000000,
	eNotifyDeletion		= 0b10000000,
	eNotifyMask			= 0b11110000
};*/



//template<typename callee_t>
struct Status_M
{
 	
	bool isStatus(uint8_t s){ return ((mStatus&s) == s);}
	
	void setStatus(eStatus s, bool state){
		// task is locked : cancel operation
		if(isStatus(eLocked) && s!=eLocked){ return;}
		state ? mStatus|=s : mStatus&=~s;

		/*if( (s == eSuspended) && state){
			if(mStatus&eNotifySuspension){
				callee_t::notifyStatusChange (this, eNotifySuspension);
			}
		}*/
	}

	/*void setNotifierStatus(eNotifyStatus s, bool state){
		state ? mStatus|=s : mStatus&=~s;
	}*/

	bool isRunning(){
		return (mStatus&eRunning);
	}

	bool isStarted(){
		return (mStatus&eStarted);
	}

	bool isPending(){
		return !(mStatus&eStarted);
	}
	
	void init()	{ mStatus = 0; }

	bool isExeReady() const { return !(mStatus&eSuspended) ;}

	bool isDelReady() const { return !(mStatus&eLocked);}

	void makePreExe(){
		
		mStatus |= eRunning;

		/*if(!(mStatus&eStarted) && (mStatus&eNotifyFirstExe)){
			callee_t::notifyStatusChange (this, eNotifyFirstExe);
		}
		
		if(mStatus&eNotifyExeStart){
			callee_t::notifyStatusChange (this, eNotifyExeStart);
		}*/
	}

	void makePreDel(){
		
		/*if(mStatus&eNotifyDeletion){
			callee_t::notifyStatusChange (this, eNotifyDeletion);
		}*/
		
	}

	void makePostExe(){		
		mStatus &= ~eRunning;
		mStatus |= eStarted;
	}

private:
	
	enum eSystemStatus:uint8_t
	{
		eRunning			= 0b00000001,
		eStarted			= 0b00000010
	};
	
	uint8_t mStatus;
	
};
