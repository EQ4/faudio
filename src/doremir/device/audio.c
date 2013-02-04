
/*
    DoReMIR Audio Engine
    Copyright (c) DoReMIR Music Research 2012-2013
    All rights reserved.
 */

#include <doremir/device/audio.h>

#include <doremir/atomic.h> // TODO improving
#include <doremir/thread.h>
#include <doremir/util.h>

#include <portaudio.h>

typedef doremir_device_audio_t                  device_t;
typedef doremir_device_audio_stream_t           stream_t;
typedef doremir_device_audio_session_t          session_t;
typedef doremir_device_audio_stream_callback_t  stream_callback_t;
typedef doremir_device_audio_session_callback_t session_callback_t;

typedef PaDeviceIndex native_index_t;
typedef PaStream     *native_stream_t;

struct _doremir_device_audio_session_t {
    impl_t              impl;               // Dispatcher
    system_time_t       acquired;           // Time of acquisition (not used at the moment)

    list_t              devices;            // Cached device list

    device_t            def_input;          // Default devices
    device_t            def_output;         //      Both possibly null
                                            //      If present, these are also in the above list
};

struct _doremir_device_audio_t {
    impl_t              impl;               // Dispatcher
    native_index_t      index;              // Index
    
    string_t            name;               // Cached names
    string_t            host_name;

    bool                muted;
    double              volume;
};

struct _doremir_device_audio_stream_t {
    impl_t              impl;               // Dispatcher

    device_t            input, output;
    processor_t         proc;
    proc_interface_t   *proc_impl;

    atomic_t            time;
    dispatcher_t        in_disp;
    dispatcher_t        out_disp;

    native_stream_t     native;
};

static mutex_t pa_mutex;
static bool    pa_status;

error_t audio_device_error(string_t msg);
ptr_t audio_session_impl(doremir_id_t interface);
ptr_t audio_device_impl(doremir_id_t interface);
ptr_t audio_stream_impl(doremir_id_t interface);
inline static session_t new_session();
inline static void session_init_devices(session_t session);
inline static void delete_session(session_t session);
inline static device_t new_device(native_index_t index);
inline static void delete_device(device_t device);
inline static stream_t new_stream(device_t input, device_t output, processor_t proc);
inline static void delete_stream(stream_t stream);

// --------------------------------------------------------------------------------

inline static session_t new_session()
{
    session_t session = doremir_new(device_audio_session);
    session->impl = &audio_session_impl;
    // TODO
    return session;
}
inline static void session_init_devices(session_t session)
{
    native_index_t count;
    list_t         devices;
    pair_t         def_devices;

    count   = Pa_GetDeviceCount();
    devices = doremir_list_empty();

    for(size_t i = 0; i < count; ++i)
    {
        device_t device = new_device(i);
        if (device)
            devices = doremir_list_dcons(device, devices);
    }

    session->devices      = devices;
    session->def_input    = new_device(Pa_GetDefaultInputDevice());
    session->def_output   = new_device(Pa_GetDefaultOutputDevice());
}

inline static void delete_session(session_t session)
{
    // TODO free device list
    doremir_delete(session);
}

inline static device_t new_device(native_index_t index)
{
    if (index == paNoDevice)
        return NULL;

    device_t device = doremir_new(device_audio);
    device->impl    = &audio_device_impl;

    PaDeviceInfo  *info      = Pa_GetDeviceInfo(index);
    PaHostApiInfo *host_info = Pa_GetHostApiInfo(info->hostApi);

    device->index       = index;
    device->name        = string(info->name);
    device->host_name   = string(host_info->name);
    device->muted       = false;
    device->volume      = 1.0;

    return device;
}

inline static void delete_device(device_t device)
{
    doremir_delete(device);
}

inline static stream_t new_stream(device_t input, device_t output, processor_t proc)
{
    stream_t stream     = doremir_new(device_audio_stream);

    stream->impl        = &audio_stream_impl;
    stream->input       = input;
    stream->output      = output;
    stream->proc        = proc;

    return stream;
}

inline static void delete_stream(stream_t stream)
{
    doremir_delete(stream);
}


// --------------------------------------------------------------------------------

void doremir_device_audio_initialize()
{
    pa_mutex  = doremir_thread_create_mutex();
    pa_status = false;
}

void doremir_device_audio_terminate()
{
    doremir_thread_destroy_mutex(pa_mutex);
}

// --------------------------------------------------------------------------------

session_t doremir_device_audio_begin_session()
{
    if (!pa_mutex) {
        assert(false && "Not initalized");
    }

    inform(string("Beginning audio session"));

    doremir_thread_lock(pa_mutex);
    {
        if (pa_status) {
            doremir_thread_unlock(pa_mutex);
            return (session_t) audio_device_error(string("Overlapping audio sessions"));
        } else {
            Pa_Initialize();
            pa_status = true;
            doremir_thread_unlock(pa_mutex);

            session_t session = new_session();
            session_init_devices(session);
            // session->acquired = time(NULL);
            return session;
        }
    }
}

void doremir_device_audio_end_session(session_t session)
{
    if (!pa_mutex) {
        assert(false && "Not initalized");
    }

    inform(string("Ending audio session"));

    doremir_thread_lock(pa_mutex);
    {
        if (pa_status) {
            Pa_Terminate();
            pa_status = false;
        }
    }
    doremir_thread_unlock(pa_mutex);
    delete_session(session);
}

void doremir_device_audio_with_session(
    doremir_device_audio_session_callback_t session_callback,
    doremir_ptr_t                           session_data,
    doremir_error_callback_t                error_callback,
    doremir_ptr_t                           error_data
)
{
    session_t session = doremir_device_audio_begin_session();

    if (doremir_check(session)) {
        error_callback(error_data, (error_t) session);
    } else {
        session_callback(session_data, session);
    }

    doremir_device_audio_end_session(session);
}

doremir_list_t doremir_device_audio_all(session_t session)
{
    return doremir_copy(session->devices);
}

doremir_pair_t doremir_device_audio_default(session_t session)
{
    return pair(session->def_input, session->def_output);
}

device_t doremir_device_audio_default_input(session_t session)
{
    return session->def_input;
}

device_t doremir_device_audio_default_output(session_t session)
{
    return session->def_output;
}

doremir_string_t doremir_device_audio_name(device_t device)
{
    return doremir_copy(device->name);
}

doremir_string_t doremir_device_audio_host_name(device_t device)
{
    return doremir_copy(device->host_name);
}

bool doremir_device_audio_has_input(device_t device)
{
    assert(false && "Not implemented");
}

bool doremir_device_audio_has_output(device_t device)
{
    assert(false && "Not implemented");
}

doremir_pair_t doremir_device_audio_channels(device_t device)
{
    assert(false && "Not implemented");
}


// --------------------------------------------------------------------------------

// FIXME return Optional?
stream_t doremir_device_audio_open_stream(device_t    input,
        processor_t processor,
        device_t    output)
{
    assert(false && "Not implemented");
    // call Pa_OpenStream
    // call before_processing
    // call Pa_StartStream
}

stream_t doremir_device_audio_restart_stream(stream_t stream)
{
    assert(false && "Not implemented");
}

void doremir_device_audio_close_stream(stream_t stream)
{
    assert(false && "Not implemented");
    // call Pa_StopStream
    // (after_processing is called from the finished callback)
    // call Pa_CloseStream
}

void doremir_device_audio_with_stream(
    doremir_device_audio_t                 input,
    doremir_processor_t                    processor,
    doremir_device_audio_t                 output,
    doremir_device_audio_stream_callback_t stream_callback,
    doremir_ptr_t                          stream_data,
    doremir_error_callback_t         error_callback,
    doremir_ptr_t                          error_data
)
{
    stream_t stream = doremir_device_audio_open_stream(input, processor, output);
    // doremir_check(stream) ? callback(stream) : error_callback(stream);
    doremir_device_audio_close_stream(stream);
}


// --------------------------------------------------------------------------------

void before_processing(stream_t stream)
{
    // extract top-level audio type
    // allocate buffers
    // create message allocator
    // register all processors as receivers on the incoming message dispatcher
    // call setup() on top processor (passing outgoing message receiver)
}

void after_processing(stream_t stream)
{
    // call cleanup() on top processor
    // unregister processors from incoming message dispatcher
    // destroy message allocator
    // free buffers
}

int during_processing(stream_t stream)
{
    // update stream time
    // call dispatch() on the innner dispatcher

    // deliver inputs
    // call process() on top processor
    // deliver outputs

    // decide whether to continue
    return 0; // TODO
}


/* The callbacks */

int pa_main_callback(const void                       *input_ptr,
                     void                             *output_ptr,
                     unsigned long                     frame_count,
                     const PaStreamCallbackTimeInfo   *time_info,
                     PaStreamCallbackFlags             flags,
                     void                             *data)
{
    // call during_processing
    return 0;
}

void pa_finished_callback(void *userData)
{
    // call after_processing
}




// --------------------------------------------------------------------------------

doremir_string_t audio_session_show(ptr_t a)
{
    string_t str = string("<AudioSession ");
    str = string_dappend(str, doremir_string_format_integer(" %p", (long) a));
    str = string_dappend(str, string(">"));
    return str;
}

void audio_session_destroy(ptr_t a)
{
    doremir_device_audio_end_session(a);
}

ptr_t audio_session_impl(doremir_id_t interface)
{
    static doremir_string_show_t audio_session_show_impl
        = { audio_session_show };
    static doremir_destroy_t audio_session_destroy_impl
        = { audio_session_destroy };

    switch (interface) {
        case doremir_string_show_i:
            return &audio_session_show_impl;

        case doremir_destroy_i:
            return &audio_session_destroy_impl;

        default:
            return NULL;
    }
}


// --------------------------------------------------------------------------------

doremir_string_t audio_device_show(ptr_t a)
{                                                       
    device_t device = (device_t*) a;
    
    string_t str = string("<AudioDevice ");
    str = string_dappend(str, doremir_device_audio_host_name(device));
    str = string_dappend(str, string(" "));
    str = string_dappend(str, doremir_device_audio_name(device));
    str = string_dappend(str, string(">"));
    return str;
}

ptr_t audio_device_impl(doremir_id_t interface)
{
    static doremir_string_show_t audio_device_show_impl
        = { audio_device_show };

    switch (interface) {
        case doremir_string_show_i:
            return &audio_device_show_impl;

        default:
            return NULL;
    }
}


// --------------------------------------------------------------------------------

doremir_string_t audio_stream_show(ptr_t a)
{
    string_t str = string("<AudioStream ");
    str = string_dappend(str, doremir_string_format_integer(" %p", (long) a));
    str = string_dappend(str, string(">"));
    return str;
}

void audio_stream_destroy(ptr_t a)
{
    doremir_device_audio_close_stream(a);
}

ptr_t audio_stream_impl(doremir_id_t interface)
{
    static doremir_string_show_t audio_stream_show_impl
        = { audio_stream_show };
    static doremir_destroy_t audio_stream_destroy_impl
        = { audio_stream_destroy };

    switch (interface) {
        case doremir_string_show_i:
            return &audio_stream_show_impl;

        case doremir_destroy_i:
            return &audio_stream_destroy_impl;

        default:
            return NULL;
    }
}


// --------------------------------------------------------------------------------

void doremir_audio_engine_log_error_from(doremir_string_t msg, doremir_string_t origin);

error_t audio_device_error(string_t msg)
{
    return doremir_error_create_simple(error, msg, string("Doremir.Device.Audio"));
}

void audio_device_fatal(char *msg, int error)
{
    doremir_audio_engine_log_error_from(
        string_dappend(string(msg), format_integer(" (error code %d)", error)),
        string("Doremir.Device.Audio"));
    doremir_audio_engine_log_error(string("Terminating Audio Engine"));
    exit(error);
}

