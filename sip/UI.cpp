#include <iostream>
#include <pjsua2.hpp>
#include "Manager.hpp"

#include <pthread.h>

void *startUI(void* m){
    Manager * manager = (Manager * ) m;

    const int CMD_LEN = 128;
    char * cmd = new char[CMD_LEN];
    const string THREADNAME = "UI thread";
    try
	{
		manager->ep.libRegisterThread(THREADNAME);
	}
	catch (Error & err) {
		std::cout << "Exception in ep.libRegisterThread(THREADNAME) : " << 
			err.info() << std::endl;
		exit(-1);
	}

    for(;;){
        pj_bzero(cmd,CMD_LEN);
        //std::cin >> cmd ;
        cin.getline(cmd,CMD_LEN);
        std::cout<< "You input: "<<cmd<<std::endl;
        cin.clear();
        cin.sync(); //clear buffer , or it will enter dead loop .-- jch 
        manager->ProcessCMD(cmd);
    }
    
}
