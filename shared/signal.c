
/*
    faudio
    
    Copyright (c) DoReMIR Music Research 2012-2013
    All rights reserved.

 */

#include <fa/signal.h>
#include <fa/util.h>

typedef fa_signal_t signal_t;

struct _fa_signal_t {

    impl_t          impl;       //  Interface dispatcher

    enum {
        constant_signal,
        identity_signal,
        lifted_signal,
        time_signal,
        delay_signal,
        read_signal,
        write_signal
    }                       tag;

    union {

        struct {
            ptr_t           value;
        }                   constant;

        struct {
            uint8_t         arity;
            uint8_t         saturation;
            signal_t        arguments[256];
        }                   identity;

        struct {
            uint8_t         arity;
            uint8_t         saturation;
            signal_t        arguments[256];
            ptr_t           function;
            ptr_t           data;
        }                   lifted;

        struct {
        }                   time;

        struct {
            uint8_t         arity;
            uint8_t         saturation;
            signal_t        arguments[256];
            time_t          time;
        }                   delay;

        struct {
            int             address;
        }                   read;

        struct {
            uint8_t         arity;
            uint8_t         saturation;
            signal_t        arguments[256];
            int             address;
        }                   write;

    }                       fields;

};

// --------------------------------------------------------------------------------

inline static signal_t new_signal(int tag)
{
    fa_ptr_t signal_impl(fa_id_t interface);

    signal_t s = fa_new(signal);
    s->impl = &signal_impl;
    s->tag  = tag;
    return s;
}

inline static void delete_signal(signal_t signal)
{
    fa_delete(signal);
}

#define is_constant(v)    (v->tag == constant_signal)
#define is_identity(v)    (v->tag == identity_signal)
#define is_lifted(v)      (v->tag == lifted_signal)
#define is_time(v)        (v->tag == time_signal)
#define is_delay(v)       (v->tag == delay_signal)
#define is_read(v)        (v->tag == read_signal)
#define is_write(v)       (v->tag == write_signal)

#define constant_get(v,f) v->fields.constant.f
#define identity_get(v,f) v->fields.identity.f
#define lifted_get(v,f)   v->fields.lifted.f
#define time_get(v,f)     v->fields.time.f
#define delay_get(v,f)    v->fields.delay.f
#define read_get(v,f)     v->fields.read.f
#define write_get(v,f)    v->fields.write.f

// --------------------------------------------------------------------------------

fa_type_t fa_signal_type_of(fa_signal_t signal)
{
    assert(false);
}


signal_t copy_constant(signal_t signal)
{
    assert(false && "Not implemented");
}

signal_t copy_identity(signal_t signal)
{
    signal_t signal2 = new_signal(identity_signal);
    identity_get(signal2, arity)        = identity_get(signal, arity);
    identity_get(signal2, saturation)   = identity_get(signal, saturation);

    for (int i = 0; i < identity_get(signal, saturation); ++i) {
        identity_get(signal2, arguments)[i]    = identity_get(signal, arguments)[i];
    }

    return signal2;
}

signal_t copy_lifted(signal_t signal)
{
    signal_t signal2 = new_signal(lifted_signal);
    lifted_get(signal2, arity)        = lifted_get(signal, arity);
    lifted_get(signal2, saturation)   = lifted_get(signal, saturation);

    for (int i = 0; i < lifted_get(signal, saturation); ++i) {
        lifted_get(signal2, arguments)[i]    = lifted_get(signal, arguments)[i];
    }

    lifted_get(signal2, function)   = lifted_get(signal, function);
    lifted_get(signal2, data)   = lifted_get(signal, data);
    return signal2;
}

signal_t copy_time(signal_t signal)
{
    assert(false && "Not implemented");
}

signal_t copy_delay(signal_t signal)
{
    signal_t signal2 = new_signal(delay_signal);
    delay_get(signal2, arity)        = delay_get(signal, arity);
    delay_get(signal2, saturation)   = delay_get(signal, saturation);

    for (int i = 0; i < delay_get(signal, saturation); ++i) {
        delay_get(signal2, arguments)[i]    = delay_get(signal, arguments)[i];
    }

    delay_get(signal2, time)   = delay_get(signal, time);
    return signal2;
}

signal_t copy_read(signal_t signal)
{
    assert(false && "Not implemented");
}

signal_t copy_write(signal_t signal)
{
    signal_t signal2 = new_signal(write_signal);
    write_get(signal2, arity)        = write_get(signal, arity);
    write_get(signal2, saturation)   = write_get(signal, saturation);

    for (int i = 0; i < write_get(signal, saturation); ++i) {
        write_get(signal2, arguments)[i]    = write_get(signal, arguments)[i];
    }

    write_get(signal2, address)   = write_get(signal, address);
    return signal2;
}

fa_signal_t fa_signal_copy(fa_signal_t signal)
{
    match(signal->tag) {
        against(constant_signal)   copy_constant(signal);
        against(identity_signal)   copy_identity(signal);
        against(lifted_signal)     copy_lifted(signal);
        against(time_signal)       copy_time(signal);
        against(delay_signal)      copy_delay(signal);
        against(read_signal)       copy_read(signal);
        against(write_signal)      copy_write(signal);
        no_default();
    }
}






/*
    This is were the magic happens. The compute(context, signal) computes
    the value of the signal in a context.
 */

struct context_ {
    int64_t     count;           // invocation count (monotonically increasing with time)
    double      rate;            // samples per second (if applicable)
    int64_t     size;            // number of samples
    time_t      time;            // current time (if applicable)
    time_t      diff;            // difference in seconds (size/rate)

    ptr_t   buses[0xffff];   // buses
};
typedef struct context_ *context_t;

static ptr_t compute(context_t context, fa_signal_t signal);
// static void before(context_t context, fa_signal_t signal);
// static void after(context_t context, fa_signal_t signal);


// void before(context_t context, fa_signal_t signal) {}
// void after(context_t context, fa_signal_t signal) {}

ptr_t compute_constant(context_t context, signal_t signal)
{
    return constant_get(signal, value);
}

ptr_t compute_identity(context_t context, signal_t signal)
{
    assert(identity_get(signal, saturation)
           ==
           identity_get(signal, arity));

    signal_t arg1 = identity_get(signal, arguments)[0];
    return compute(context, arg1);
}

ptr_t compute_lifted(context_t context, signal_t signal)
{
    assert(lifted_get(signal, saturation)
           ==
           lifted_get(signal, arity));

    switch (lifted_get(signal, arity)) {
    case 1: {
        signal_t arg1 = lifted_get(signal, arguments)[0];
        unary_t  func = lifted_get(signal, function);
        ptr_t    data = lifted_get(signal, data);
        return func(data,
                    compute(context, arg1));
    }

    case 2: {
        signal_t  arg1 = lifted_get(signal, arguments)[0];
        signal_t  arg2 = lifted_get(signal, arguments)[1];
        binary_t  func = lifted_get(signal, function);
        ptr_t     data = lifted_get(signal, data);
        return func(data,
                    compute(context, arg1),
                    compute(context, arg2)
                   );
    }

    case 3: {
        signal_t  arg1 = lifted_get(signal, arguments)[0];
        signal_t  arg2 = lifted_get(signal, arguments)[1];
        signal_t  arg3 = lifted_get(signal, arguments)[2];
        ternary_t func = lifted_get(signal, function);
        ptr_t     data = lifted_get(signal, data);
        return func(data,
                    compute(context, arg1),
                    compute(context, arg2),
                    compute(context, arg3)
                   );
    }

    default: {
        assert(false && "Strange arity");
    }
    }
}

ptr_t compute_time(context_t context, signal_t signal)
{
    return context->time;
}

ptr_t compute_delay(context_t context, signal_t signal)
{
    assert(false && "Not implemented");
}

ptr_t compute_read(context_t context, signal_t signal)
{
    assert(false && "Not implemented");
}

ptr_t compute_write(context_t context, signal_t signal)
{
    assert(false && "Not implemented");
}

ptr_t compute(context_t context, fa_signal_t signal)
{
    match(signal->tag) {
        against(constant_signal)   compute_constant(context, signal);
        against(identity_signal)   compute_identity(context, signal);
        against(lifted_signal)     compute_lifted(context, signal);
        against(time_signal)       compute_time(context, signal);
        against(delay_signal)      compute_delay(context, signal);
        against(read_signal)       compute_read(context, signal);
        against(write_signal)      compute_write(context, signal);
        no_default();
    }
}


/* Run the given signal (for debug).
 */
void fa_signal_run(signal_t signal, fa_unary_t function, fa_ptr_t data)
{

    struct context_ context;
    context.count = 0;
    context.rate  = 44100;
    context.size  = 1024;

    context.time  = seconds(0);
    context.diff  = divisions(context.size, context.rate);
    // context.buses[0];

    for (int i = 0; i < 400; ++i) {
        // TODO fix time etc
        context.count += 1;
        context.time = fa_dadd(context.time, fa_copy(context.diff)); // TODO rate
        function(data, compute(&context, signal));
    }
}





















// TODO optimized version for non-ptr types

fa_signal_t fa_signal_add()
{
    assert(false);
}

fa_signal_t fa_signal_subtract()
{
    assert(false);
}

fa_signal_t fa_signal_multiply()
{
    assert(false);
}

fa_signal_t fa_signal_power()
{
    assert(false);
}

fa_signal_t fa_signal_divide()
{
    assert(false);
}

fa_signal_t fa_signal_modulo()
{
    assert(false);
}

fa_signal_t fa_signal_absolute()
{
    assert(false);
}

fa_signal_t fa_signal_not()
{
    assert(false);
}

fa_signal_t fa_signal_and()
{
    assert(false);
}

fa_signal_t fa_signal_or()
{
    assert(false);
}

fa_signal_t fa_signal_xor()
{
    assert(false);
}

fa_signal_t fa_signal_bit_not()
{
    assert(false);
}

fa_signal_t fa_signal_bit_and()
{
    assert(false);
}

fa_signal_t fa_signal_bit_or()
{
    assert(false);
}

fa_signal_t fa_signal_bit_xor()
{
    assert(false);
}

fa_signal_t fa_signal_shift_left()
{
    assert(false);
}

fa_signal_t fa_signal_shift_right()
{
    assert(false);
}

fa_signal_t fa_signal_equal()
{
    assert(false);
}

fa_signal_t fa_signal_less_than()
{
    assert(false);
}

fa_signal_t fa_signal_greater_than()
{
    assert(false);
}

fa_signal_t fa_signal_less_than_equal()
{
    assert(false);
}

fa_signal_t fa_signal_greater_than_equal()
{
    assert(false);
}

fa_signal_t fa_signal_acos()
{
    assert(false);
}

fa_signal_t fa_signal_asin()
{
    assert(false);
}

fa_signal_t fa_signal_atan()
{
    assert(false);
}

fa_signal_t fa_signal_cos()
{
    assert(false);
}

fa_signal_t fa_signal_sin()
{
    assert(false);
}

fa_signal_t fa_signal_tan()
{
    assert(false);
}

fa_signal_t fa_signal_exp()
{
    assert(false);
}

fa_signal_t fa_signal_log()
{
    assert(false);
}

fa_signal_t fa_signal_log10()
{
    assert(false);
}

fa_signal_t fa_signal_pow()
{
    assert(false);
}

fa_signal_t fa_signal_sqrt()
{
    assert(false);
}

fa_signal_t fa_signal_abs()
{
    assert(false);
}

fa_signal_t fa_signal_min()
{
    assert(false);
}

fa_signal_t fa_signal_max()
{
    assert(false);
}

fa_signal_t fa_signal_fmod()
{
    assert(false);
}

fa_signal_t fa_signal_remainder()
{
    assert(false);
}

fa_signal_t fa_signal_floor()
{
    assert(false);
}

fa_signal_t fa_signal_ceil()
{
    assert(false);
}

fa_signal_t fa_signal_rint()
{
    assert(false);
}



// --------------------------------------------------------------------------------

fa_ptr_t signal_impl(fa_id_t interface)
{

    switch (interface) {
    default:
        return NULL;
    }
}
