//#include "stdafx.h"
#include "sipBuddy.hpp"


sipBuddy::sipBuddy()
{
}


sipBuddy::~sipBuddy()
{
}

void sipBuddy::onBuddyState() {
	BuddyInfo bi = getInfo();
    cout <<"[sipBuddy::onBuddyState]" << "Buddy " << bi.uri << " is "
        << bi.presStatus.statusText << endl;
}
