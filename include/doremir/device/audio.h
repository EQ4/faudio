
#ifndef _DOREMIR_DEVICE_AUDIO
#define _DOREMIR_DEVICE_AUDIO

#include <doremir/list.h>
#include <doremir/pair.h>
#include <doremir/message.h>
#include <doremir/processor.h>

/** @defgroup Doremir Doremir
    @{
    @defgroup DoremirDevice Device
    @{
    @defgroup DoremirDeviceAudio Audio
    @{
    */

typedef struct _doremir_device_audio_t * doremir_device_audio_t;
typedef struct _doremir_device_audio_session_t * doremir_device_audio_session_t;
typedef struct _doremir_device_audio_stream_t * doremir_device_audio_stream_t;
doremir_list_t doremir_device_audio_all(doremir_device_audio_session_t);
doremir_pair_t doremir_device_audio_default(doremir_device_audio_session_t);
doremir_string_t doremir_device_audio_name(doremir_device_audio_t);
doremir_string_t doremir_device_audio_host_name(doremir_device_audio_t);
bool doremir_device_audio_has_input(doremir_device_audio_t);
bool doremir_device_audio_has_output(doremir_device_audio_t);
doremir_pair_t doremir_device_audio_channels(doremir_device_audio_t);
doremir_device_audio_session_t doremir_device_audio_begin_session();
doremir_device_audio_session_t doremir_device_audio_restart_session(doremir_device_audio_session_t);
void doremir_device_audio_end_session(doremir_device_audio_session_t);
doremir_device_audio_stream_t doremir_device_audio_start_stream(doremir_device_audio_t,
                                                                doremir_processor_t,
                                                                doremir_device_audio_t);
doremir_device_audio_stream_t doremir_device_audio_restart_stream(doremir_device_audio_stream_t);
void doremir_device_audio_stop_stream(doremir_device_audio_stream_t);
typedef doremir_device_audio_session_t (* doremir_device_audio_session_callback_t)(doremir_device_audio_session_t);
typedef doremir_device_audio_stream_t (* doremir_device_audio_stream_callback_t)(doremir_device_audio_stream_t);
void doremir_device_audio_with_session(doremir_device_audio_session_callback_t);
void doremir_device_audio_with_stream(doremir_device_audio_t,
                                      doremir_processor_t,
                                      doremir_device_audio_t,
                                      doremir_device_audio_stream_callback_t);

/** @}
    @}
    @}
    */

#endif // _DOREMIR_DEVICE_AUDIO

