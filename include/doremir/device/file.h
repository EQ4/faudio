
#ifndef _DOREMIR_DEVICE_FILE
#define _DOREMIR_DEVICE_FILE

#include <doremir/string.h>
#include <doremir/device.h>
#include <doremir/processor.h>

/** @defgroup Doremir Doremir
    @{
    @defgroup DoremirDevice Device
    @{
    @defgroup DoremirDeviceFile File
    @{
    */

typedef struct _doremir_device_file_result_t * doremir_device_file_result_t;
doremir_device_file_t doremir_device_file_open(doremir_string_file_path_t);
void doremir_device_file_close(doremir_device_file_t);
doremir_device_file_result_t doremir_device_file_run(doremir_device_file_t,
                                                     doremir_processor_t,
                                                     doremir_device_file_t);

/** @}
    @}
    @}
    */

#endif // _DOREMIR_DEVICE_FILE

