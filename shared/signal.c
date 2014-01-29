
/*
    faudio

    Copyright (c) DoReMIR Music Research 2012-2013
    All rights reserved.

 */

#include <fa/signal.h>
#include <fa/pair/left.h>
#include <fa/priority_queue.h>
#include <fa/action.h>
#include <fa/buffer.h>
#include <fa/clock.h>
#include <fa/util.h>
#include <fa/midi/message.h>

#include "signal.h"
#include "signal_internal.h"
#include "action_internal.h"

typedef fa_signal_custom_processor_t   *custom_proc_t;
typedef fa_signal_unary_signal_t        fixpoint_t;
typedef fa_signal_unary_double_t        dunary_t;
typedef fa_signal_binary_double_t       dbinary_t;
typedef fa_action_t                     action_t;

struct _fa_signal_t {

    impl_t                  impl;

    enum {
        time_signal,
        random_signal,
        constant_signal,
        lift_signal,
        lift2_signal,
        loop_signal,
        delay_signal,
        custom_signal,
        input_signal,
        output_signal
    }                       tag;

    union {
        struct {}           time;

        struct {}           random;

        struct {
            double          value;
        }                   constant;

        struct {
            string_t        name;
            dunary_t        function;
            ptr_t           data;
            signal_t        a;
        }                   lift;

        struct {
            string_t        name;
            dbinary_t       function;
            ptr_t           data;
            signal_t        a;
            signal_t        b;
        }                   lift2;

        struct {
            fixpoint_t      function;
            ptr_t           data;
        } loop;

        struct {
            int             n;
            signal_t        a;
        } delay;

        struct {
            custom_proc_t   proc;
            signal_t        a;
        }                   custom;

        struct {
            int             c;
        }                   input;

        struct {
            int             n;
            int             c;
            signal_t        a;
        }                   output;
    }                       fields;

};

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

#define is_time(v)          (v->tag == time_signal)
#define is_random(v)        (v->tag == random_signal)
#define is_constant(v)      (v->tag == constant_signal)
#define is_lift(v)          (v->tag == lift_signal)
#define is_lift2(v)         (v->tag == lift2_signal)

#define is_delay(v)         (v->tag == delay_signal)
#define is_loop(v)          (v->tag == loop_signal)
#define is_custom(v)        (v->tag == custom_signal)
#define is_input(v)         (v->tag == input_signal)
#define is_output(v)        (v->tag == output_signal)

#define time_get(v,f)       v->fields.time.f
#define random_get(v,f)     v->fields.random.f
#define constant_get(v,f)   v->fields.constant.f
#define lift_get(v,f)       v->fields.lift.f
#define lift2_get(v,f)      v->fields.lift2.f
#define loop_get(v,f)       v->fields.loop.f
#define delay_get(v,f)      v->fields.delay.f
#define custom_get(v,f)     v->fields.custom.f
#define input_get(v,f)      v->fields.input.f
#define output_get(v,f)     v->fields.output.f

#define neg_bus(x) ((x + 1)*(-1))

// --------------------------------------------------------------------------------

signal_t fa_signal_time()
{
    signal_t signal = new_signal(time_signal);
    return signal;
}

signal_t fa_signal_random()
{
    signal_t signal = new_signal(random_signal);
    return signal;
}

fa_signal_t fa_signal_constant(double x)
{
    signal_t signal = new_signal(constant_signal);
    constant_get(signal, value) = x;
    return signal;
}


fa_signal_t fa_signal_lift(fa_string_t n,
                           fa_signal_unary_double_t function,
                           fa_ptr_t data,
                           fa_signal_t a)
{
    signal_t signal = new_signal(lift_signal);
    lift_get(signal, name)      = n;
    lift_get(signal, function)  = function;
    lift_get(signal, data)      = data;
    lift_get(signal, a)         = a;
    return signal;
}


fa_signal_t fa_signal_lift2(fa_string_t n,
                            fa_signal_binary_double_t function,
                            fa_ptr_t data,
                            fa_signal_t a,
                            fa_signal_t b)
{
    signal_t signal = new_signal(lift2_signal);
    lift2_get(signal, name)     = n;
    lift2_get(signal, function) = function;
    lift2_get(signal, data)     = data;
    lift2_get(signal, a)        = a;
    lift2_get(signal, b)        = b;
    return signal;
}

fa_signal_t fa_signal_loop(fa_signal_unary_signal_t function, fa_ptr_t data)
{
    if (kVectorMode) {
        warn(string("Loop not supported in vector mode"));
        assert(false && "Loop not supported in vector mode");
    }

    signal_t signal = new_signal(loop_signal);
    loop_get(signal, function)  = function;
    loop_get(signal, data)      = data;
    return signal;
}


fa_signal_t fa_signal_delay(int n, fa_signal_t a)
{
    if (kVectorMode) {
        warn(string("Delay not supported in vector mode"));
        assert(false && "Delay not supported in vector mode");
    }

    signal_t signal = new_signal(delay_signal);
    delay_get(signal, n)  = n;
    delay_get(signal, a)  = a;
    return signal;
}

fa_signal_t fa_signal_custom(custom_proc_t proc, signal_t a)
{
    signal_t signal = new_signal(custom_signal);
    custom_get(signal, proc)        = proc;
    custom_get(signal, a)           = a;
    return signal;
}

fa_signal_t fa_signal_input(int c)
{
    signal_t signal = new_signal(input_signal);
    input_get(signal, c)  = c;
    return signal;
}

fa_signal_t fa_signal_output(int n, int c, fa_signal_t a)
{
    signal_t signal = new_signal(output_signal);
    output_get(signal, n)  = n;
    output_get(signal, c)  = c;
    output_get(signal, a)  = a;
    return signal;
}

signal_t copy_constant(signal_t signal2)
{
    signal_t signal = new_signal(constant_signal);
    constant_get(signal, value) = constant_get(signal2, value);
    return signal;
}

signal_t copy_lift(signal_t signal2)
{
    signal_t signal = new_signal(lift_signal);
    lift_get(signal, name)      = lift_get(signal2, name);
    lift_get(signal, function)  = lift_get(signal2, function);
    lift_get(signal, data)      = lift_get(signal2, data);
    lift_get(signal, a)         = lift_get(signal2, a);
    return signal;
}

signal_t copy_lift2(signal_t signal2)
{
    signal_t signal = new_signal(lift2_signal);
    lift2_get(signal, name)     = lift2_get(signal2, name);
    lift2_get(signal, function) = lift2_get(signal2, function);
    lift2_get(signal, data)     = lift2_get(signal2, data);
    lift2_get(signal, a)        = lift2_get(signal2, a);
    lift2_get(signal, b)        = lift2_get(signal2, b);
    return signal;
}

signal_t copy_loop(signal_t signal2)
{
    signal_t signal = new_signal(loop_signal);
    loop_get(signal, function)  = loop_get(signal2, function);
    loop_get(signal, data)      = loop_get(signal2, data);
    return signal;
}

signal_t copy_delay(signal_t signal2)
{
    signal_t signal = new_signal(delay_signal);
    delay_get(signal, n)  = delay_get(signal2, n);
    delay_get(signal, a)  = delay_get(signal2, a);
    return signal;
}


fa_signal_t copy_custom(signal_t signal2)
{
    signal_t signal = new_signal(custom_signal);
    custom_get(signal, proc)        = custom_get(signal2, proc);
    custom_get(signal, a)           = custom_get(signal2, a);
    return signal;
}


signal_t copy_input(signal_t signal2)
{
    signal_t signal = new_signal(input_signal);
    input_get(signal, c)  = input_get(signal2, c);
    return signal;
}

signal_t copy_output(signal_t signal2)
{
    signal_t signal = new_signal(output_signal);
    output_get(signal, n)  = output_get(signal2, n);
    output_get(signal, c)  = output_get(signal2, c);
    output_get(signal, a)  = output_get(signal2, a);
    return signal;
}

fa_signal_t fa_signal_copy(fa_signal_t signal)
{
    switch (signal->tag) {
    case time_signal:
        return fa_signal_time();

    case random_signal:
        return fa_signal_random();

    case constant_signal:
        return copy_constant(signal);

    case lift_signal:
        return copy_lift(signal);

    case lift2_signal:
        return copy_lift2(signal);

    case loop_signal:
        return copy_loop(signal);

    case delay_signal:
        return copy_delay(signal);

    case custom_signal:
        return copy_custom(signal);

    case input_signal:
        return copy_input(signal);

    case output_signal:
        return copy_output(signal);

    default:
        assert(false);
    }
}

void fa_signal_destroy(fa_signal_t signal)
{
    // TODO
    delete_signal(signal);
}



// --------------------------------------------------------------------------------
// Introspection etc

bool fa_signal_is_variable(fa_signal_t a)
{
    assert(false && "Not implemented");
}


bool fa_signal_is_constant(fa_signal_t a)
{
    return !fa_signal_is_variable(a);
}


bool fa_signal_are_constant(fa_list_t a)
{
    assert(false && "Not implemented");
}


int fa_signal_signal_node_count(fa_signal_t a)
{
    assert(false && "Not implemented");
}


int fa_signal_required_inputs(fa_signal_t a)
{
    assert(false && "Not implemented");
}


int fa_signal_required_buses(fa_signal_t a)
{
    assert(false && "Not implemented");
}


int fa_signal_required_delay(fa_signal_t a)
{
    assert(false && "Not implemented");
}

fa_pair_t fa_signal_to_tree(fa_signal_t signal)
{
    switch (signal->tag) {

    case time_signal:
        return pair(string("time"), empty());

    case random_signal:
        return pair(string("random"), empty());

    case constant_signal: {
        double value = constant_get(signal, value);
        return pair(fa_string_to_string(fa_from_double(value)), empty());
    }

    case lift_signal: {
        pair_t tree = fa_signal_to_tree(lift_get(signal, a));
        return pair(lift_get(signal, name), list(tree));
    }

    case lift2_signal: {
        pair_t tree1 = fa_signal_to_tree(lift2_get(signal, a));
        pair_t tree2 = fa_signal_to_tree(lift2_get(signal, b));
        return pair(lift_get(signal, name), list(tree1, tree2));
    }

    case input_signal:
        return pair(
                   concat(
                       string("input "),
                       fa_string_format_integral("%d", input_get(signal, c))),
                   empty());

    case output_signal:
        return pair(
                   concat(
                       string("output "),
                       fa_string_show(fa_from_int32(output_get(signal, c))),
                       string("[-"),
                       fa_string_show(fa_from_int32(output_get(signal, n))),
                       string("]")
                   ),

                   list(fa_signal_to_tree(output_get(signal, a))));

    default:
        assert(false);
    }
}

string_t draw_tree(pair_t value, string_t indent, bool is_last, string_t result)
{
    ptr_t  label    = fa_pair_first(value);
    list_t children = fa_pair_second(value);

    fa_write_string(result, indent);

    if (is_last) {
        fa_write_string(result, string("`- "));
        fa_write_string(indent, string("   "));
    } else {
        fa_write_string(result, string("+- "));
        fa_write_string(indent, string("|  "));
    }

    fa_write_string(result, fa_string_to_string(label));
    fa_write_string(result, string("\n"));

    fa_for_each_last(x, children, last) {
        result = draw_tree(x, indent, last, result);
    }
    return result;
}

string_t fa_signal_draw_tree(fa_pair_t p)
{
    return draw_tree(p, string(""), true, string(""));
}


// --------------------------------------------------------------------------------
// Simplification

struct part {
    int o, d;
};
typedef struct part part_t;
void init_part(struct part *p)
{
    p->o = 0;
    p->d = 1;
}
void run_part(struct part *p, int *r, struct part *p2)
{
    *r = p->o;
    p2->o = p->o + p->d;
    p2->d = p->d;
}
void split_part(struct part *p, struct part *p2, struct part *p3)
{
    p2->o = p->o;
    p2->d = p->d * 2;
    p3->o = p->d;
    p3->d = p->d * 2;
}

void run_part_neg(struct part *p, int *r, struct part *p2)
{
    run_part(p, r, p2);
    *r = neg_bus(*r);
}



// TODO
int get_buffer_range(part_t *part,
                     part_t *newPart,
                     string_t name,
                     int is,
                     int os)
{
    newPart->o = part->o;
    newPart->d = part->d;
    return 16;
}

// inline static
fa_signal_t simplify(part_t *part, list_t *procs, fa_signal_t signal2)
{
    switch (signal2->tag) {

    case loop_signal: {
        fixpoint_t fix      = loop_get(signal2, function);
        ptr_t      fix_data = loop_get(signal2, data);

        int channel;
        part_t part1;
        // Allocate one bus channel for the delay
        run_part_neg(part, &channel, &part1);

        signal_t input          = fa_signal_input(channel);
        signal_t res            = simplify(&part1, procs, fix(fix_data, input));

        return fa_signal_output(1, channel, res);
    }

    case delay_signal: {
        signal_t a              = delay_get(signal2, a);
        int samples             = delay_get(signal2, n);

        int channel;
        part_t part1;
        // Allocate one bus channel for the delay
        run_part_neg(part, &channel, &part1);

        signal_t input          = fa_signal_input(channel);
        signal_t output         = fa_signal_output(samples, channel, simplify(&part1, procs, a));

        return fa_signal_former(input, output);
    }

    case custom_signal: {
        signal_t a              = simplify(part, procs, custom_get(signal2, a));

        custom_proc_t proc = custom_get(signal2, proc);
        // Push to processor list
        *procs = fa_list_dcons(proc, *procs);
        return a;
    }

    case lift_signal: {
        string_t    name        = lift_get(signal2, name);
        dunary_t    func        = lift_get(signal2, function);
        ptr_t       func_data   = lift_get(signal2, data);

        signal_t a              = simplify(part, procs, lift_get(signal2, a));
        return fa_signal_lift(name, func, func_data, a);
    }

    case lift2_signal: {
        string_t    name        = lift2_get(signal2, name);
        dbinary_t   func        = lift2_get(signal2, function);
        ptr_t       func_data   = lift2_get(signal2, data);

        part_t part1;
        part_t part2;
        // Split the channel partition
        split_part(part, &part1, &part2);

        signal_t a              = simplify(&part1, procs, lift2_get(signal2, a));
        signal_t b              = simplify(&part2, procs, lift2_get(signal2, b));

        return fa_signal_lift2(name, func, func_data, a, b);
    }

    case output_signal: {
        int samples             = output_get(signal2, n);
        int channel             = output_get(signal2, c);

        signal_t a              = simplify(part, procs, output_get(signal2, a));

        return fa_signal_output(samples, channel, a);
    }

    default:
        return fa_copy(signal2);
    }
}
fa_signal_t fa_signal_simplify(fa_signal_t signal2)
{
    part_t part;
    init_part(&part);
    list_t procs = empty();
    return simplify(&part, &procs, signal2);
}

list_t fa_signal_get_procs(fa_signal_t signal2)
{
    part_t part;
    init_part(&part);
    list_t procs = empty();
    simplify(&part, &procs, signal2);
    return procs;
}

// --------------------------------------------------------------------------------
// Running

#define max_delay(state) ((long) (state->rate * kMaxDelaySeconds))

/*  Internal DSP state.
    Local to a real-time stream or non-real-time "run" of a signal.

    Contains all "input" buffers (actually both input and output) as well as "buses", used
    internally for loop and delay.

    The input buffer contains up to kMaxVectorSize values for each channel and are non-interleaved.
    The bus channel contains up to max_delay(state) values for each channel and are non-interleaved.

    This layout is used regardless of whether vector processing is enabled or not, so allocation and indexing
    is unaffected by this change. In non-vector mode, only the first index of an I/O bus is used. In vector
    mode up to kMaxVectorSize indices may be used, depending on current vector size settings and number of
    samples being delivered by the underlying system API.

 */
struct _state_t {
    double     *inputs;                 // Current input values (TODO should not be called inputs as they are also outputs...)
    double     *buses;                  // Current and future bus values

    int         count;                  // Number of processed samples
    double      rate;                   // Sample rate

    int           custom_proc_count;
    custom_proc_t custom_procs[kMaxCustomProcs];      // Array of custom processors

};

state_t new_state(int sample_rate)
{
    // TODO use a single random generator for whole signal, see below
    srand(time(NULL));
    state_t state = fa_new_struct(_state_t);

    state->count              = 0;
    state->rate               = sample_rate;
    state->custom_proc_count  = 0;

    state->inputs   = fa_malloc(kMaxInputs * kMaxVectorSize * sizeof(double));
    state->buses    = fa_malloc(kMaxBuses * max_delay(state)   * sizeof(double));
    memset(state->inputs,   0, kMaxInputs * kMaxVectorSize * sizeof(double));
    memset(state->buses,    0, kMaxBuses * max_delay(state)    * sizeof(double));

    return state;
}

void add_custom_proc(custom_proc_t proc, state_t state)
{
    assert(state->custom_proc_count < kMaxCustomProcs && "Too many custom processors");
    state->custom_procs[state->custom_proc_count] = proc;
    state->custom_proc_count++;
}

void delete_state(state_t state)
{
    fa_free(state->inputs);
    fa_free(state->buses);
    fa_free(state);
}

inline static
double state_random(state_t state)
{
    // TODO use a single random generator for whole signal, see above
    return ((double)rand() / (double)RAND_MAX) * 2 - 1;
}

inline static
double state_time(state_t state)
{
    return state->count / state->rate;
}

inline static
double state_time_plus(state_t state, int n)
{
    return (state->count + n) / state->rate;
}

inline static double  read_input1(int c, state_t state);
inline static double  read_bus1(int c, state_t state);
inline static void    write_input1(int c, double x, state_t state);
inline static void    write_bus1(int n, int c, double x, state_t state);

inline static double  *read_input(int c, state_t state);
inline static double  *read_bus(int c, state_t state);
inline static double  *write_input(int c, state_t state);
inline static double  *write_bus(int n, int c, state_t state);

inline static
double read_samp1(int c, state_t state)
{
    return (c >= 0) ? read_input1(c, state) : read_bus1(neg_bus(c), state);
}

inline static
void write_samp1(int n, int c, double x, state_t state)
{
    if (c >= 0) {
        write_input1(c, x, state);
    } else {
        write_bus1(n, neg_bus(c), x, state);
    }
}

inline static
double *read_samp(int c, state_t state)
{
    return (c >= 0) ? read_input(c, state) : read_bus(neg_bus(c), state);
}

inline static
double *write_samp(int n, int c, state_t state)
{
    if (c >= 0) {
        return write_input(c, state);
    } else {
        return write_bus(n, neg_bus(c), state);
    }
}


// inline static
void inc_state1(state_t state)
{
    state->count++;
}

void inc_state(int n, state_t state)
{
    state->count += n;
}


//----------
// Internal state stuff

/* */
int buffer_pointer(state_t state)
{
    return state->count % max_delay(state);
}

int index_bus(int n, int c, state_t state)
{
    return c * max_delay(state) + n;
}

// a[n]  = *(a + n)
// &a[n] = a + n

double read_input1(int c, state_t state)
{
    return *read_input(c, state);
}

void write_input1(int c, double x, state_t state)
{
    *write_input(c, state) = x;
}

double read_bus1(int c, state_t state)
{
    return *read_bus(c, state);
}

void write_bus1(int n, int c, double x, state_t state)
{
    *write_bus(n, c, state) = x;
}


double *read_input(int c, state_t state)
{
    return state->inputs + (c * kMaxVectorSize);
}

double *write_input(int c, state_t state)
{
    return state->inputs + (c * kMaxVectorSize);
}

double *read_bus(int c, state_t state)
{
    int bp = buffer_pointer(state);
    return state->buses + (index_bus(bp, c, state));
}

double *write_bus(int n, int c, state_t state)
{
    int bp = buffer_pointer(state);
    return state->buses + (index_bus((bp + n) % max_delay(state), c, state));
}

//----------

/**
    Loops through the custom processors in a state_t or state2_t, running
    the respective rendering action.

    0 -> prepare
    1 -> run
    2 -> cleanup

 */
void run_custom_procs(custom_proc_when_t when, int count, state_t state)
{
    for (int i = 0; i < state->custom_proc_count; ++i) {

        custom_proc_t proc = state->custom_procs[i];
        // printf("Running custom proc %p!\n", proc);

        switch (when) {
        case custom_proc_before:
            proc->before(proc->data, kMaxVectorSize, (fa_signal_state_t *) state);
            break;

        case custom_proc_render:
            proc->render(proc->data, count, (fa_signal_state_t *) state);
            break;

        case custom_proc_after:
            proc->after(proc->data, kMaxVectorSize, (fa_signal_state_t *) state);
            break;

        default:
            assert(false);
        }
    }
}

/**
    Run a simple action.

    @param
        action  Action to run.
        state   State to run action on (for control updates and custom processor messages).
 */
ptr_t run_simple_action(state_t state, action_t action)
{
    if (fa_action_is_compound(action)) {
        warn(string_dappend(string("Compound action passed to Signal.runSimpleAction: "), fa_string_show(action)));
        return NULL;
    }

    if (fa_action_is_set(action)) {
        int ch = fa_action_set_channel(action);
        double v = fa_action_set_value(action);
        write_samp1(0, ch, v, state);
        return NULL;
    }

    // TODO accum

    if (fa_action_is_send(action)) {
        for (int i = 0; i < state->custom_proc_count; ++i) {
            custom_proc_t proc = state->custom_procs[i];
            mark_used(proc);

            string_t name = fa_action_send_name(action);
            ptr_t value = fa_action_send_value(action);
            proc->receive(proc->data, name, value);

            // fa_print("  Sending to processor: %s\n", fa_string_show(value));
        }

        return NULL;
    }

    warn(string_dappend(string("Unknown simple action passed to Signal.runSimpleAction: "), fa_string_show(action)));
    return NULL;
}


/**
    Step over a sample.
 */
double step(signal_t signal, state_t state)
{
    switch (signal->tag) {

    case time_signal: {
        return state_time(state);
    }

    case random_signal: {
        return state_random(state);
    }

    case constant_signal: {
        return constant_get(signal, value);
    }

    case lift_signal: {
        dunary_t    function  = lift_get(signal, function);
        ptr_t       data      = lift_get(signal, data);
        signal_t    a         = lift_get(signal, a);
        double      xa        = step(a, state);
        return function(data, xa);
    }

    case lift2_signal: {
        dbinary_t   function  = lift2_get(signal, function);
        ptr_t       data      = lift2_get(signal, data);
        signal_t    a         = lift2_get(signal, a);
        signal_t    b         = lift2_get(signal, b);
        double      xa        = step(a, state);
        double      xb        = step(b, state);
        return function(data, xa, xb);
    }

    case input_signal: {
        int         c         = input_get(signal, c);
        return read_samp1(c, state);
    }

    case output_signal: {
        int         n         = output_get(signal, n);
        int         c         = output_get(signal, c);
        signal_t    a         = output_get(signal, a);

        double      xa        = step(a, state);
        write_samp1(n, c, xa, state);
        return xa;
    }

    default:
        assert(false && "step: Strange signal");
    }

    assert(false);
}

/**
    Step over a vector of samples.
 */
void step_vector(signal_t signal, state_t state, int count, double *out)
{
    switch (signal->tag) {

    case time_signal: {
        for (int i = 0; i < count; ++i) {
            out[i] = state_time_plus(state, i);
        }

        return;
    }

    case random_signal: {
        for (int i = 0; i < count; ++i) {
            out[i] = state_random(state);
        }

        return;
    }

    case constant_signal: {
        for (int i = 0; i < count; ++i) {
            out[i] = constant_get(signal, value);
        }

        return;
    }

    case lift_signal: {
        dunary_t    function  = lift_get(signal, function);
        ptr_t       data      = lift_get(signal, data);
        signal_t    a         = lift_get(signal, a);
        step_vector(a, state, count, out);

        for (int i = 0; i < count; ++i) {
            out[i] = function(data, out[i]);
        }

        return;
    }

    case lift2_signal: {
        dbinary_t   function  = lift2_get(signal, function);
        ptr_t       data      = lift2_get(signal, data);
        signal_t    a         = lift2_get(signal, a);
        signal_t    b         = lift2_get(signal, b);

        // TODO is stack allocation efficient enough?
        double temp[count];
        step_vector(a, state, count, out);
        step_vector(b, state, count, temp);

        for (int i = 0; i < count; ++i) {
            out[i] = function(data, out[i], temp[i]);
        }

        return;
    }

    case input_signal: {
        int         c         = input_get(signal, c);
        double *xs = read_samp(c, state);

        for (int i = 0; i < count; ++i) {
            out[i] = xs[i];
        }

        return;
    }

    case output_signal: {
        int         n         = output_get(signal, n);
        int         c         = output_get(signal, c);
        signal_t    a         = output_get(signal, a);

        step_vector(a, state, count, out);
        double *xs = write_samp(n, c, state);

        for (int i = 0; i < count; ++i) {
            xs[i] = out[i];
        }

        return;
    }

    default:
        assert(false && "step: Strange signal");
    }

    assert(false);
}

ptr_t run_simple_action_(ptr_t x, ptr_t a)
{
    return run_simple_action(x, a);
}
void fa_signal_run(int count, list_t controls, signal_t a, double *output)
{
    priority_queue_t controls2 = priority_queue();
    fa_for_each(x, controls) {
        fa_priority_queue_insert(fa_pair_left_from_pair(x), controls2);
    }

    state_t state = new_state(kDefSampleRate); // TODO other sample rates
    fa_for_each(x, fa_signal_get_procs(a)) {
        add_custom_proc(x, state);
    }
    signal_t a2 = fa_signal_simplify(a);

    // TODO optimize
    // TODO verify

    run_custom_procs(custom_proc_before, 0, state);

    for (int i = 0; i < count; ++ i) {
        time_t now = fa_milliseconds((double) state->count / (double) state->rate * 1000.0);
        run_actions(controls2, now, run_simple_action_, state);

        run_custom_procs(custom_proc_render, 1, state);

        output[i] = step(a2, state);

        inc_state1(state);
    }

    run_custom_procs(custom_proc_after, 0, state);

    delete_state(state);
}

buffer_t fa_signal_run_buffer(int n, list_t controls, signal_t a)
{
    buffer_t b = fa_buffer_create(n * sizeof(double));
    fa_signal_run(n, controls, a, fa_buffer_unsafe_address(b));
    return b;
}

void fa_signal_print(int n, list_t controls, signal_t a)
{
    buffer_t b = fa_signal_run_buffer(n, controls, a);

    for (size_t i = 0; i < fa_buffer_size(b); ++i) {
        double x = fa_buffer_get_double(b, i);
        printf("%3ld: %4f\n", (long) i, x);
    }

    fa_destroy(b);
}

ptr_t fa_signal_run_file(int n, list_t controls, signal_t a, string_t path)
{
    buffer_t b = fa_signal_run_buffer(n, controls, a);
    ptr_t res = fa_buffer_write_audio(path, 1, b); // TODO number of channels
    fa_destroy(b);
    return res;
}




// --------------------------------------------------------------------------------
// Derived signals


inline static double _former(ptr_t _, double x, double y)
{
    return x;
}
fa_signal_t fa_signal_former(fa_signal_t a, fa_signal_t b)
{
    return fa_signal_lift2(string("former"), _former, NULL, a, b);
}

inline static double _latter(ptr_t _, double x, double y)
{
    return y;
}
fa_signal_t fa_signal_latter(fa_signal_t a, fa_signal_t b)
{
    return fa_signal_lift2(string("latter"), _latter, NULL, a, b);
}



inline static double _impulse(ptr_t _, double x)
{
    return (x == 0) ? 1 : 0;
}
fa_signal_t fa_signal_impulse()
{
    return fa_signal_lift(string("mkImp"), _impulse, NULL, fa_signal_time());
}

/*
    (defun signal-counter ()
      (- (signal-loop* (lambda (x) (+ x 1))) 1))

*/
inline static signal_t _fix_counter(ptr_t _, signal_t x)
{
    return fa_signal_add(x, fa_signal_constant(1));
}
fa_signal_t fa_signal_counter()
{
    return fa_signal_add(fa_signal_loop(_fix_counter, NULL), fa_signal_constant(-1));
}


inline static double _impulses(ptr_t n, double x)
{
    int n2 = (int) n;
    int x2 = (int) x;
    return (x2 % n2) == 0 ? 1 : 0;
}
fa_signal_t fa_signal_impulses(int n)
{
    return fa_signal_lift(string("mkImps"), _impulses, (ptr_t) n, fa_signal_counter());
}


fa_signal_t fa_signal_line(double x)
{
    double tau = 2 * 3.141592653589793;
    return fa_signal_multiply(fa_signal_time(), fa_signal_constant(x * tau));
}


inline static double _play(ptr_t buffer, double i)
{
    size_t size = fa_buffer_size(buffer);
    uint64_t j = ((uint64_t) i) % (size / sizeof(double));
    return fa_buffer_get_double(buffer, j);
}
fa_signal_t fa_signal_play(fa_buffer_t buffer, fa_signal_t i)
{
    return fa_signal_lift(string("play"), _play, buffer, i);
}

inline static double _record(ptr_t buffer, double i, double x)
{
    size_t size = fa_buffer_size(buffer);
    uint64_t j = ((uint64_t) i) % (size / sizeof(double));
    fa_buffer_set_double(buffer, j, x);
    return x;
}
fa_signal_t fa_signal_record(fa_buffer_t buffer, fa_signal_t i, fa_signal_t x)
{
    return fa_signal_lift2(string("record"), _record, buffer, i, x);
}

static bool play_started = false; // TODO debug
inline static double _play_stream(ptr_t buffer, double _)
{
    double fa_atomic_ring_buffer_filled(fa_atomic_ring_buffer_t buffer);

    // Unmodified if underflowing
    // TODO report
    double x = 0;

    // if (play_started || (fa_atomic_ring_buffer_filled(buffer) > 1)) {
    {
        play_started = true;
        bool res = fa_atomic_ring_buffer_read_double(buffer, &x);

        if (!res) {
            // warn(string("U"));
        }

        mark_used(res);
    }

    return x;
}

fa_signal_t fa_signal_play_stream(fa_atomic_ring_buffer_t buffer)
{
    return fa_signal_lift(string("playStream"), _play_stream, buffer, fa_signal_constant(0));
}

fa_signal_t fa_signal_record_stream(fa_atomic_ring_buffer_t buffer, fa_signal_t x)
{
    assert(false);
}


#define kRecExternalOffset 40

struct rec_external {              
    string_t name;
    fa_atomic_ring_buffer_t buffer;
    
    // debug
    size_t bytes_written;
};
ptr_t record_extrenal_before_(ptr_t x, int count, fa_signal_state_t *state)
{
    return x;
}
ptr_t record_extrenal_after_(ptr_t x, int count, fa_signal_state_t *state)
{
    return x;
}
ptr_t record_extrenal_render_(ptr_t x, int count, fa_signal_state_t *state)
{
    struct rec_external* ext = (struct rec_external*) x;

    if (!kVectorMode) {
        double x = state->buffer[(kRecExternalOffset + 0)*kMaxVectorSize];
        if (ext->buffer) {
            fa_atomic_ring_buffer_write_double(ext->buffer, x);
            ext->bytes_written += sizeof(double);
        }
    } else {
        for (int i = 0; i < count; ++i) {
            double x = state->buffer[(kRecExternalOffset + 0)*kMaxVectorSize + i];
            if (ext->buffer) {
                fa_atomic_ring_buffer_write_double(ext->buffer, x);
                ext->bytes_written += sizeof(double);
            }
        }
    }
    return x;
}

ptr_t record_extrenal_receive_(ptr_t x, fa_signal_name_t n, fa_signal_message_t msg)
{
    struct rec_external* ext = (struct rec_external*) x;

    if (fa_equal(ext->name, n)) {
        if (ext->buffer) {
            warn(fa_string_format_integral("Bytes written: %zu", ext->bytes_written));
            fa_atomic_ring_buffer_close(ext->buffer);
        }
        
        // TODO assert it is actually a ring buffer
        ext->buffer = msg;
        ext->bytes_written = 0;
    }
    return x;
}


fa_signal_t fa_signal_record_external(fa_string_t name,
                                      fa_signal_t signal)
{
    struct rec_external* ext = fa_new_struct(rec_external);
    ext->name = name;
    ext->buffer = NULL;
    
    fa_signal_custom_processor_t *proc = fa_malloc(sizeof(fa_signal_custom_processor_t));
    proc->before  = record_extrenal_before_;
    proc->after   = record_extrenal_after_;
    proc->render  = record_extrenal_render_;
    proc->receive = record_extrenal_receive_;
    proc->data    = ext;

    return fa_signal_custom(proc, fa_signal_output(0, kRecExternalOffset, signal));
}






inline static double _add(ptr_t _, double x, double y)
{
    return x + y;
}
fa_signal_t fa_signal_add(fa_signal_t a, fa_signal_t b)
{
    return fa_signal_lift2(string("(+)"), _add, NULL, a, b);
}

inline static double _mul(ptr_t _, double x, double y)
{
    return x * y;
}
fa_signal_t fa_signal_multiply(fa_signal_t a, fa_signal_t b)
{
    return fa_signal_lift2(string("(*)"), _mul, NULL, a, b);
}

inline static double _subtract(ptr_t _, double x, double y)
{
    return x - y;
}
fa_signal_t fa_signal_subtract(fa_signal_t a, fa_signal_t b)
{
    return fa_signal_lift2(string("(-)"), _subtract, NULL, a, b);
}

inline static double _divide(ptr_t _, double x, double y)
{
    return x / y;
}
fa_signal_t fa_signal_divide(fa_signal_t a, fa_signal_t b)
{
    return fa_signal_lift2(string("(/)"), _divide, NULL, a, b);
}


inline static double _absolute(ptr_t _, double x)
{
    return abs(x);
}
fa_signal_t fa_signal_absolute(fa_signal_t a)
{
    return fa_signal_lift(string("absolute"), _absolute, NULL, a);
}

inline static double _sin(ptr_t _, double x)
{
    return sin(x);
}
fa_signal_t fa_signal_sin(fa_signal_t a)
{
    return fa_signal_lift(string("sin"), _sin, NULL, a);
}

inline static double _cos(ptr_t _, double x)
{
    return cos(x);
}
fa_signal_t fa_signal_cos(fa_signal_t a)
{
    return fa_signal_lift(string("cos"), _cos, NULL, a);
}






fa_signal_t fa_signal_power(fa_signal_t x, fa_signal_t y)
{
    assert(false && "Not implemented");
}

fa_signal_t fa_signal_modulo(fa_signal_t x, fa_signal_t y)
{
    assert(false && "Not implemented");
}

// fa_signal_t fa_signal_absolute(fa_signal_t x ) { assert (false && "Not implemented"); }

fa_signal_t fa_signal_not()
{
    assert(false && "Not implemented");
}

fa_signal_t fa_signal_and(fa_signal_t x, fa_signal_t y)
{
    assert(false && "Not implemented");
}

fa_signal_t fa_signal_or(fa_signal_t x, fa_signal_t y)
{
    assert(false && "Not implemented");
}

fa_signal_t fa_signal_xor(fa_signal_t x, fa_signal_t y)
{
    assert(false && "Not implemented");
}

fa_signal_t fa_signal_equal(fa_signal_t x, fa_signal_t y)
{
    assert(false && "Not implemented");
}

fa_signal_t fa_signal_less_than(fa_signal_t x, fa_signal_t y)
{
    assert(false && "Not implemented");
}

fa_signal_t fa_signal_greater_than(fa_signal_t x, fa_signal_t y)
{
    assert(false && "Not implemented");
}

fa_signal_t fa_signal_less_than_equal(fa_signal_t x, fa_signal_t y)
{
    assert(false && "Not implemented");
}

fa_signal_t fa_signal_greater_than_equal(fa_signal_t x, fa_signal_t y)
{
    assert(false && "Not implemented");
}

fa_signal_t fa_signal_acos(fa_signal_t x)
{
    assert(false && "Not implemented");
}

fa_signal_t fa_signal_asin(fa_signal_t x)
{
    assert(false && "Not implemented");
}

fa_signal_t fa_signal_atan(fa_signal_t x)
{
    assert(false && "Not implemented");
}

// fa_signal_t fa_signal_cos(fa_signal_t x ) { assert (false && "Not implemented"); }

// fa_signal_t fa_signal_sin(fa_signal_t x ) { assert (false && "Not implemented"); }

fa_signal_t fa_signal_tan(fa_signal_t x)
{
    assert(false && "Not implemented");
}

fa_signal_t fa_signal_exp(fa_signal_t x)
{
    assert(false && "Not implemented");
}

fa_signal_t fa_signal_log(fa_signal_t x)
{
    assert(false && "Not implemented");
}

fa_signal_t fa_signal_log10(fa_signal_t x)
{
    assert(false && "Not implemented");
}

fa_signal_t fa_signal_sqrt(fa_signal_t x)
{
    assert(false && "Not implemented");
}

fa_signal_t fa_signal_min(fa_signal_t x, fa_signal_t y)
{
    assert(false && "Not implemented");
}

fa_signal_t fa_signal_max(fa_signal_t x, fa_signal_t y)
{
    assert(false && "Not implemented");
}

fa_signal_t fa_signal_fmod(fa_signal_t x, fa_signal_t y)
{
    assert(false && "Not implemented");
}

fa_signal_t fa_signal_remainder(fa_signal_t x, fa_signal_t y)
{
    assert(false && "Not implemented");
}

fa_signal_t fa_signal_floor(fa_signal_t x, fa_signal_t y)
{
    assert(false && "Not implemented");
}

fa_signal_t fa_signal_ceil(fa_signal_t x, fa_signal_t y)
{
    assert(false && "Not implemented");
}

fa_list_t   fa_signal_vst(fa_string_t x, fa_string_t y, fa_list_t z)
{
    assert(false && "Not implemented");
}


// --------------------------------------------------------------------------------

ptr_t signal_copy(ptr_t a)
{
    return fa_signal_copy(a);
}

void signal_destroy(ptr_t a)
{
    return fa_signal_destroy(a);
}

ptr_t signal_add(ptr_t a, ptr_t b)
{
    return fa_signal_add(a, b);
}

ptr_t signal_subtract(ptr_t a, ptr_t b)
{
    return fa_signal_subtract(a, b);
}

ptr_t signal_multiply(ptr_t a, ptr_t b)
{
    return fa_signal_multiply(a, b);
}

ptr_t signal_divide(ptr_t a, ptr_t b)
{
    return fa_signal_divide(a, b);
}

ptr_t signal_absolute(ptr_t a)
{
    return fa_signal_absolute(a);
}

string_t signal_show(ptr_t a)
{
    return fa_signal_draw_tree(fa_signal_to_tree(fa_signal_simplify(a)));
}

fa_ptr_t signal_impl(fa_id_t interface)
{
    static fa_copy_t signal_copy_impl
        = { signal_copy };
    static fa_destroy_t signal_destroy_impl
        = { signal_destroy };
    static fa_number_t  signal_number_impl
        = { signal_add, signal_subtract, signal_multiply, signal_divide, signal_absolute };
    static fa_string_show_t signal_show_impl
        = { signal_show };

    switch (interface) {
    case fa_copy_i:
        return &signal_copy_impl;

    case fa_destroy_i:
        return &signal_destroy_impl;

    case fa_number_i:
        return &signal_number_impl;

    case fa_string_show_i:
        return &signal_show_impl;

    default:
        return NULL;
    }
}

