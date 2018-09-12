#ifndef UTIL_H
#define UTIL_H

enum class CallState{
    //NULL,	    /**< Before INVITE is sent or received  */
    CALLING,	    /**< After INVITE is sent		    */
    INCOMING,	    /**< After INVITE is received.	    */
    EARLY,	    /**< After response with To tag.	    */
    CONNECTING,	    /**< After 2xx is sent/received.	    */
    CONFIRMED,	    /**< After ACK is sent/received.	    */
    DISCONNECTED,   /**< Session is terminated.		    */
    IDLE,
};

#endif // UTIL_H
