#pragma once

#include <pjsua2.hpp>
#include <iostream>
#include <memory>
#include "sipAccount.hpp"
using namespace pj;
using namespace std;
//class MyAccount;
//

class sipCall : public  Call
{
private:	
sipAccount * myAcc;
//
//
public:

	sipCall(sipAccount &acc, int call_id = PJSUA_INVALID_ID)
		: Call(acc, call_id)
	{
		myAcc =  &acc;
	}
	virtual ~sipCall() {

	}
//
	virtual void onCallState(OnCallStateParam &prm);

	/**
	* Notify application on incoming MESSAGE request.
	*
	* @param prm	Callback parameter.
	*/
	virtual void onInstantMessage(OnInstantMessageParam &prm);

	/**
	* Notify application about the delivery status of outgoing MESSAGE
	* request.
	*
	* @param prm	Callback parameter.
	*/
	virtual void onInstantMessageStatus(OnInstantMessageStatusParam &prm);
};



