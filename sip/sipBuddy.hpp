#pragma once
#include <pjsua2.hpp>
#include <iostream>
#include <memory>
#include "sipAccount.hpp"
using namespace pj;
using namespace std;


class sipBuddy :
	public Buddy
{
public:
	sipBuddy();
	~sipBuddy();

	virtual void onBuddyState();
	
};

