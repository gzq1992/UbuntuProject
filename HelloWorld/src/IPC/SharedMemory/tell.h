#ifndef _TELL_H_
#define _TELL_H_

//initial pipe
extern void init();

//use pipe to wait
extern void wait_pipe();

//use pipe to notify
extern void notify_pipe();

//destroy pipe
extern void destroy_pipe();

#endif
