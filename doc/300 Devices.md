
# Devices {#Devices}

> *All for one, one for all, that is our device.*

@anchor Devices
@tableofcontents

Devices are the entities that allow the Audio Engine to communicate with the outside world. Any client will need
to connect at least two devices to each other to form an *audio stream*. Usually, these connections will involve
a processor or signal function.

While signals and processors denote functions, devices denote external entities provided by the system.
Typically, each *physical* audio or midi interface is represented by a single device in the Audio Engine. The
operating system may also provide *non-physical* devices, which may represent network connections, software
mixers and the like.

TODO

# Real time devices {#RealTime}

The Audio Engine supports real-time audio and midi computations. The interface for these are similar but differs
in that audio streams does not require a [processor](@ref Processors). 

See the \ref DoremirDeviceAudio and \ref DoremirDeviceMidi modules for details.

## Sessions and streams {#SessionDeviceStream}

The Audio Engine provides access to devices through the concepts of *sessions*, and *streams*. While a *device*
provides access to an external audio interface, a *session* provides access to the entire audio system, and a *stream*
to a specific audio computation. These concepts are hierarchical, each stream is associated with a device and each
device with a session.

The Audio Engine places certain restrictions on the order or acquisition of sessions, devices and streams. Any
client that wants to obtain a device must first initiate a session. The initialization of a session may fail, for
example if the underlying audio system is already being used by an exclusive process. If it succeeds, a handle
to the underlying session is provided. This handle allow the user to inspect the set of devices available in the
sesssion. 

Sessions are *immutable* and *transient*, meaning that the set of available devices in a session will not
change, but represents a snapshot of the setup at the time it was initiated. If a change in the underlying 
audio system is detected while a session is still active, and a new session has to be started to observe the
new setup.

TODO register notification callback

@image html  device_states.png "State transactions of the audio system"
@image latex device_states.pdf "State transactions of the audio system" width=0.8\textwidth

@note
    The semantics of *streams* have been changed from earlier versions of the Audio Engine, in which a *stream*
    could be repeatedly stopped and started. In the new implementation, streams are immutable and transient just
    like sessions.

## Acquire-release vs. callback {#DevicesStyles}

## Audio streams {#AudioStreams}

### Acquire-release style {#RealTimeAcquire-release}

To use a real-time device in imperative fashion, the typical paired method pattern should be used. You call 
a creation method to get a session or stream, and a destruction method to release it after use. 

~~~~
#include <doremir/time.h>
#include <doremir/thread.h>
#include <doremir/device/audio.h>

typedef doremir_device_audio_t         device_t;
typedef doremir_device_audio_session_t session_t;
typedef doremir_device_audio_stream_t  stream_t_;
typedef doremir_processor_any_t        processor_t;

void test()
{       
    session_t session;
    session = doremir_device_audio_begin_session();

    if (doremir_check(session))
        doremir_print_error(session);

    {
        device_t    input, output;
        processor_t proc;
        stream_t    stream
        
        proc    = doremir_processor_identity();
        input   = doremir_device_audio_default(session)->first;
        output  = doremir_device_audio_default(session)->second;        
        stream  = doremir_device_audio_start_stream(input, proc, output);

        if (doremir_check(stream))
            doremir_print_error(stream);

        doremir_thread_sleep(doremir_seconds(10));
        
        doremir_device_audio_stop_stream(stream);
        doremir_destroy(proc);
    }

    doremir_device_audio_end_session(session);
}
~~~~


### Callback style {#RealTimeCallback}

The callback style API use inversion of control to hide acquire-release pattern. You provide a
callback to be invoked when the session or stream is valid, and the destruction is handled automatically
after this method has returned. Errors are handled by a special callback, to which you can pass
doremir_print_error, or any user defined function.

~~~~
#include <doremir/time.h>
#include <doremir/thread.h>
#include <doremir/device/audio.h>

typedef doremir_device_audio_t         device_t;
typedef doremir_device_audio_session_t session_t;
typedef doremir_device_audio_stream_t  stream_t_;
typedef doremir_processor_any_t        processor_t;

stream_t run_callback(stream_t stream)
{
    doremir_thread_sleep(doremir_seconds(10));
    return stream;
}

session_t session_callback(session_t session, doremir_ptr_t data)
{
    device_t    input, output;
    processor_t proc;

    proc    = doremir_processor_identity();
    input   = doremir_device_audio_default(session)->first;
    output  = doremir_device_audio_default(session)->second;        

    doremir_device_audio_with_stream(devices.first, processor, devices.second,
        run_callback, doremir_print_error, NULL);

    doremir_destroy(proc);
    return session;
}

void test()
{
    doremir_device_audio_with_session(session_callback, doremir_print_error, NULL);
}
~~~~


# Non-realtime devices {#NonRealTime}

## File devices {#File}

### Acquire-release style {#AcquireReleaseFile}

~~~~
#include <doremir/time.h>
#include <doremir/thread.h>
#include <doremir/device/file.h>

typedef doremir_device_file_t    device_t;
typedef doremir_processor_any_t  processor_t;

void test()
{
    device_t    input, output;
    processor_t proc;
    result_t    result;

    proc    = doremir_processor_identity();
    input   = doremir_device_file_open(doremir_str("test/in.wav"));
    output  = doremir_device_file_open(doremir_str("test/out.wav"));

    result = doremir_device_file_run(in, out);

    if (doremir_check(result))
        doremir_print_error(result);

    doremir_device_file_wait(stream);
}
~~~~

### Callback style {#CallbackFile}



## Buffer devices {#BufferStream}

### Acquire-release style {#AcquireReleaseBuffer}

~~~~
#include <doremir/time.h>
#include <doremir/thread.h>
#include <doremir/device/file.h>

typedef doremir_device_file_t    device_t;
typedef doremir_processor_any_t  processor_t;

void test()
{
    device_t    input, output;
    processor_t proc;
    result_t    result;

    proc    = doremir_processor_identity();
    input   = doremir_device_buffer_open(doremir_buffer_create(1024));
    output  = doremir_device_buffer_open(doremir_buffer_create(1024));

    result = doremir_device_buffer_run(in, out);

    if (doremir_check(result))
        doremir_print_error(result);

    doremir_device_buffer_wait(stream);
}
~~~~


### Callback style {#CallbackBuffer}




