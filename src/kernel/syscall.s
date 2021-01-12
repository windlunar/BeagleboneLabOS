
/**
 * 當呼叫SVC時，SVC會觸發exception 
 * 跳進 svc_handler
 */
.global svc_call
svc_call:
	svc #0
	nop
	bx lr




