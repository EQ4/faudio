
# Devices {#Devices}

@anchor Devices
@tableofcontents

Devices are the entities that allow *faudio* to communicate with the
outside world. Any client will need to connect at least two devices to each other
to form a audio stream. While signals and processors denote functions, devices
denote sources and sinks of audio data, such as files, memory buffers or audio
hardware.

Devices are grouped into *real-time devices*, *non-real-time devices*. Audio and
midi information are handled by different devices. Note that *faudio* does not
provide non-real-time midi at the moment: if you need to parse and process a midi
file you must use some other method.


# Real time devices {#RealTime}

## Sessions and streams {#SessionsAndStreams}

*faudio* provides access to real-time devices through of *sessions*, and
*streams*. While a *device* provides access to an external audio interface, a
*session* provides access to the entire audio system, and a *stream* to a specific
audio computation. These concepts are hierarchical, each stream is associated with
a device and each device with a session.

Typically, each physical audio or midi interface is represented by a
single device in *faudio*. The operating system may also provide abstract
devices, representing network connections, software mixers and the like.

*faudio* places certain restrictions on the order or acquisition of
sessions, devices and streams. Any client that wants to obtain a device must first
initiate a session. The initialization of a session may fail, for example if the
underlying audio system is already being used by an exclusive process. If it
succeeds, a handle to the underlying session is provided. This handle allow the
user to inspect the set of devices available in the sesssion. The client may then
open a stream.

TODO

@image html  device_states.png "State transactions of the audio system (simplified)"
@image latex device_states.pdf "State transactions of the audio system (simplified)" width=0.8\textwidth

@note
    The semantics of *streams* have been changed from earlier versions of *faudio*, in which a *stream*
    could be repeatedly stopped and started.


## Session notifications {#Notifications}

Sessions represent a snapshot of the setup at the time it was initiated; the set of
available devices in a specific session will never change. If a change in the
underlying audio system is detected while a session is still active, a new session
has to be started to observe the new setup.

You can register a callback to be invoked when the a possible change in hardware
setup is detected, see @ref fa__audio_set_status_callback. Note that
this callback may be invoked in an interrupt handler thread and that the task of
ending a session should generally be handled in the same thread that created the
session. Use an atomic reference or a condition variable to communicate the
notification to the appropriate thread.


## Audio streams {#AudioStreams}

### Imperative style {#AudioAR}

The acquire-release style use a paired method pattern. You call a creation method
to get a value, and a destruction method to release. Note that devices, sessions and
streams have single-ownership semantics.

~~~~
#include <fa/fa.h>

int main (int argc, char const *argv[])
{
    fa_audio_session_t     session;
    fa_audio_device_t      input, output;
    fa_audio_stream_t      stream
    fa_signal_t            id;
    
    // Processor to use
    id = fa_signal_identity();
    
    // Begin session
    session = fa_audio_begin_session();
    if (fa_check(stream)) {
        fa_error_log(stream);
        goto cleanup;
    
    } else {
        // Session obtained, we can now access devices
        input  = fa_audio_default_input(session);
        input  = fa_audio_default_output(session);

        // Start stream
        stream = fa_audio_open_stream(input, id, output);
        if (fa_check(stream)) {
            fa_error_log(stream);
            goto cleanup;
        
        } else {
            // Stream active, let it run for 5 seconds
            fa_thread_sleep(5000);
        }
    }

    // Cleanup
cleanup:
    fa_destroy(stream);
    fa_destroy(session);
    fa_destroy(id);
}
~~~~


### Callback style {#AudioCB}

The callback style require that you provide a callback to be invoked when the
session or stream is valid. Destruction is handled automatically after this method
has returned. Errors are handled by a special callback, to which you can pass
[fa_error_log](@ref fa_error_log), or a user defined function.

~~~~
#include <fa/fa.h>
#include <fa/util.h>

stream_t run_callback(stream_t stream)
{
    // Stream active, let it run for 5 seconds
    fa_thread_sleep(seconds(10));
    return stream;
}

session_t session_callback(void* data, session_t session)
{
    fa_audio_device_t    input, output;
    fa_signal_t          id;

    // Session obtained, we can now access devices
    input   = fa_audio_default_input(session);
    output  = fa_audio_default_input(session);
    id      = (signal_t*) data;

    // Start stream
    fa_audio_with_stream(input, id, output,
                          run_callback, fa_error_log, NULL);

    fa_destroy(id);
    return session;
}

int main (int argc, char const *argv[])
{                  
    // Processor to use
    fa_signal_t signal = fa_signal_identity();
    
    // Begin session
    fa_audio_with_session(session_callback, id,
                           fa_error_log, NULL);
}
~~~~


# Non-realtime devices {#id8127832}

## The run method {#id98281723}

TODO

## File devices {#id9192746}

~~~~
#include <fa/fa.h>

typedef fa_file_t      device_t;
typedef fa_signal_t    signal_t;

int main (int argc, char const *argv[])
{
    fa_device_t    input, output;
    fa_signal_t    id;
    fa_result_t    result;

    // Processor to use
    id = fa_signal_identity();

    // Open streams
    input   = fa__file_open(string("test/in.wav"));
    output  = fa__file_open(string("test/out.wav"));

    // Handle possible errors
    if (fa_check(input)) {
        fa_error_log(result);
    }                                    
    
    if (fa_check(output)) {
        fa_error_log(result);
    }                                    

    result  = fa__file_run(in, id, out);

    // Handle possible error
    if (fa_check(result)) {
        fa_error_log(result);
    }                                    
    
    fa_destroy(input);
    fa_destroy(output);;
}
~~~~


## Buffer devices {#id11127283}

~~~~
#include <fa/fa.h>

typedef fa_file_t   device_t;
typedef fa_signal_t signal_t;

int main (int argc, char const *argv[])
{
    device_t    input, output;
    signal_t    id;
    result_t    result;

    // Processor to use
    id = fa_signal_identity();

    // Open streams
    input   = fa_buffer_open(fa_buffer_create(1024));
    output  = fa_buffer_open(fa_buffer_create(1024));

    // Handle possible errors
    if (fa_check(input)) {
        fa_error_log(result);
    }                                    
    
    if (fa_check(output)) {
        fa_error_log(result);
    }                                    

    result  = fa_buffer_run(in, id, out);

    // Handle possible error
    if (fa_check(result)) {
        fa_error_log(result);
    }                                    
    
    fa_destroy(input);
    fa_destroy(output);
}
~~~~





