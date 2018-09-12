//#include "stdafx.h"
#include "sipCall.hpp"
#include <unistd.h>

//#include <Windows.h>

void sipCall::onCallState(OnCallStateParam &prm)
{
	PJ_UNUSED_ARG(prm);

	CallInfo ci = getInfo();
	std::cout << "*** Call: " << ci.remoteUri << " [" << ci.stateText 
		<< "]" << std::endl;

	if (ci.state == PJSIP_INV_STATE_EARLY) { //early media   [wrong !]
		AudioMedia *remoteAudioMed = NULL;
		// Find out which media index is the audio
		for (unsigned i = 0; i < ci.media.size(); ++i) {
			if (ci.media[i].type == PJMEDIA_TYPE_AUDIO) {
				remoteAudioMed = (AudioMedia *)this->getMedia(i);
				break;
			}
		}

		if (remoteAudioMed) {
			AudioMediaPlayer player;

			try {
				player.createPlayer("input.44.wav",
					PJMEDIA_FILE_NO_LOOP);
				player.startTransmit(*remoteAudioMed);
				sleep(4 );
			}
			catch (Error& err) {
				cout << err.reason << endl;
			}
		}
	}


	if (ci.state == PJSIP_INV_STATE_CONFIRMED) { // session established
		
		AudioMedia &play_med = Endpoint::instance().audDevManager().getPlaybackDevMedia();
		AudioMedia &cap_med = Endpoint::instance().audDevManager().getCaptureDevMedia();	
		AudioMedia *remoteAudioMed = NULL;
		// Find out which media index is the audio
		for (unsigned i = 0; i < ci.media.size(); ++i) {
			if (ci.media[i].type == PJMEDIA_TYPE_AUDIO) {
				remoteAudioMed = (AudioMedia *)this->getMedia(i);
				break;
			}
		}

		if (remoteAudioMed) {
			// This will connect the sound device/mic to the call audio media
			cap_med.startTransmit(*remoteAudioMed);

			// And this will connect the call audio media to the sound device/speaker
			remoteAudioMed->startTransmit(play_med);
		}


	}

	if (ci.state == PJSIP_INV_STATE_DISCONNECTED) {
		myAcc->removeCall(this);
		/* Delete the call */
		delete this;
	}


}


void sipCall::onInstantMessage(OnInstantMessageParam &prm) {
	cout << prm.fromUri << " send[in sipCall] : " << prm.msgBody << endl;
}

void sipCall::onInstantMessageStatus(OnInstantMessageStatusParam &prm) {
	cout << "im status [in sipCall] : " << prm.msgBody << endl;
}


