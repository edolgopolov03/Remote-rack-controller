#ifndef _ECHO_H
#define _ECHO_H

#ifdef __cplusplus
extern "C" {
#endif

extern bool dosync;
void send_test(void);
err_t echo_init(void);

#ifdef __cplusplus
}
#endif

#endif // _ECHO_H
