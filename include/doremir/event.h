
#ifndef _DOREMIR_EVENT
#define _DOREMIR_EVENT

#include <doremir.h>
#include <doremir/std.h>
#include <doremir/time.h>
#include <doremir/atomic/queue.h>

/** @defgroup Doremir Doremir
    @{
    @defgroup DoremirEvent Event
    @{
    */

typedef struct _doremir_event_t * doremir_event_t;
doremir_event_t doremir_event_never();
doremir_event_t doremir_event_now(doremir_ptr_t);
doremir_event_t doremir_event_delay(doremir_time_t,
                                    doremir_event_t);
doremir_event_t doremir_event_merge(doremir_event_t,
                                    doremir_event_t);
doremir_event_t doremir_event_switch(doremir_event_t,
                                     doremir_event_t,
                                     doremir_event_t);
void doremir_event_destroy(doremir_event_t);
doremir_event_t doremir_event_recv(doremir_atomic_queue_t);
doremir_event_t doremir_event_send(doremir_atomic_queue_t,
                                   doremir_event_t);
bool doremir_event_has_value(doremir_event_t, doremir_time_t);
doremir_ptr_t doremir_event_value(doremir_event_t);
doremir_time_t doremir_event_offset(doremir_event_t);
doremir_event_t doremir_event_head(doremir_event_t);
doremir_event_t doremir_event_tail(doremir_event_t);
doremir_event_t doremir_event_filter(doremir_pred_t,
                                     doremir_ptr_t,
                                     doremir_event_t);
doremir_event_t doremir_event_dfilter(doremir_pred_t,
                                      doremir_ptr_t,
                                      doremir_event_t);
doremir_event_t doremir_event_map(doremir_unary_t,
                                  doremir_ptr_t,
                                  doremir_event_t);
doremir_event_t doremir_event_dmap(doremir_unary_t,
                                   doremir_ptr_t,
                                   doremir_event_t);
doremir_event_t doremir_event_join_map(doremir_unary_t,
                                       doremir_ptr_t,
                                       doremir_event_t);
doremir_event_t doremir_event_djoin_map(doremir_unary_t,
                                        doremir_ptr_t,
                                        doremir_event_t);
doremir_event_t doremir_event_join(doremir_event_t);
doremir_event_t doremir_event_djoin(doremir_event_t);

/** @}
    @}
    */

#endif // _DOREMIR_EVENT

