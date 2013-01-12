
#ifndef _DOREMIR_DISPATCHER
#define _DOREMIR_DISPATCHER

#include <doremir.h>
#include <doremir/std.h>
#include <doremir/pair.h>

/** @defgroup Doremir Doremir
    @{
    @defgroup DoremirDispatcher Dispatcher
    @{
    */

typedef doremir_ptr_t doremir_dispatcher_address_t;
typedef doremir_ptr_t doremir_dispatcher_message_t;
typedef struct {
            void (* receive)(doremir_ptr_t,
                             doremir_dispatcher_address_t,
                             doremir_dispatcher_message_t);
        } doremir_dispatcher_receiver_t;
typedef struct {
            void (* add_receiver)(doremir_ptr_t,
                                  doremir_dispatcher_receiver_t);
            void (* remove_receiver)(doremir_ptr_t,
                                     doremir_dispatcher_receiver_t);
            void (* dispatch)(doremir_ptr_t);
        } doremir_dispatcher_sender_t;
typedef struct _doremir_dispatcher_t * doremir_dispatcher_t;
doremir_dispatcher_t doremir_dispatcher_simple();
void doremir_dispatcher_destroy(doremir_dispatcher_t);
doremir_pair_t doremir_dispatcher_buffered();
doremir_pair_t doremir_dispatcher_non_blocking();

/** @}
    @}
    */

#endif // _DOREMIR_DISPATCHER

