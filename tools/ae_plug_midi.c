
#include <doremir/audio_engine.h>
#include <doremir/util.h>
#include <doremir/thread.h>
#include <CoreMidi/MIDIServices.h>

ptr_t status_callback(ptr_t c, ptr_t v)
{
    printf("Midi status changed!\n");
    printf("    Sources:            %d\n", (int)MIDIGetNumberOfSources());
    printf("    Destinations:       %d\n", (int)MIDIGetNumberOfDestinations());
    printf("    Devices:            %d\n", (int)MIDIGetNumberOfDevices());
    
    return 0;
}

ptr_t listen(ptr_t c)
{
    doremir_device_midi_set_status_callback(
        status_callback, 
        NULL, 
        (midi_session_t) c
        );
                          
    assert(false && "Does not return...");
}

int main (int argc, char const *argv[])
{
    midi_session_t session;

    doremir_audio_engine_initialize();
    session = doremir_device_midi_begin_session();

    if (doremir_check(session)) {
        log_error((error_t) session);
        warn(string("Aborting test due to error"));
        goto cleanup;
    }

    // This should fail (and does!)
    // thread_t listen_thread = doremir_thread_create(listen, (ptr_t) session);
    // doremir_thread_join(listen_thread);

    listen(session);

cleanup:
    doremir_device_midi_end_session(session);
    return 0; 
}