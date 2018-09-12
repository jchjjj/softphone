#pragma once
#include <pjsua2.hpp>
#include <iostream>
#include <memory>
#include <QObject>
#include <global.h>
#include <QString>

using namespace pj;
class sipAccount : public QObject,
    public Account
{
    Q_OBJECT
public:
	sipAccount();
	~sipAccount();

signals:
    void incommingcall(QString remoteUri);
    void recvmsg(const OnInstantMessageParam & prm);

private slots:
    void answer(QString remoteUri, bool yesorno);
    void hang(QString number);
public:
	std::vector<Call *> calls;

	void removeCall(Call *call)
	{
		for (std::vector<Call *>::iterator it = calls.begin();
		it != calls.end(); ++it)
		{
			if (*it == call) {
				calls.erase(it);
				break;
			}
		}
	}

	virtual void onRegState(OnRegStateParam &prm);
	virtual void onIncomingCall(OnIncomingCallParam &iprm);

	/**
	* Notify application on incoming instant message or pager (i.e. MESSAGE
	* request) that was received outside call context.
	*
	* @param prm	    Callback parameter.
	*/
	virtual void onInstantMessage(OnInstantMessageParam &prm);
	
	/**
	* Notify application about the delivery status of outgoing pager/instant
	* message (i.e. MESSAGE) request.
	*
	* @param prm	    Callback parameter.
	*/
	virtual void onInstantMessageStatus(OnInstantMessageStatusParam &prm);

	/**
	* Notification when incoming SUBSCRIBE request is received. Application
	* may use this callback to authorize the incoming subscribe request
	* (e.g. ask user permission if the request should be granted).
	*
	* If this callback is not implemented, all incoming presence subscription
	* requests will be accepted.
	*
	* If this callback is implemented, application has several choices on
	* what to do with the incoming request:
	*	- it may reject the request immediately by specifying non-200 class
	*    final response in the IncomingSubscribeParam.code parameter.
	*	- it may immediately accept the request by specifying 200 as the
	*	  IncomingSubscribeParam.code parameter. This is the default value if
	*	  application doesn't set any value to the IncomingSubscribeParam.code
	*	  parameter. In this case, the library will automatically send NOTIFY
	*	  request upon returning from this callback.
	*	- it may delay the processing of the request, for example to request
	*    user permission whether to accept or reject the request. In this
	*	  case, the application MUST set the IncomingSubscribeParam.code
	*	  argument to 202, then IMMEDIATELY calls presNotify() with
	*	  state PJSIP_EVSUB_STATE_PENDING and later calls presNotify()
	*    again to accept or reject the subscription request.
	*
	* Any IncomingSubscribeParam.code other than 200 and 202 will be treated
	* as 200.
	*
	* Application MUST return from this callback immediately (e.g. it must
	* not block in this callback while waiting for user confirmation).
	*
	* @param prm	    Callback parameter.
	*/
	virtual void onIncomingSubscribe(OnIncomingSubscribeParam &prm);
};

