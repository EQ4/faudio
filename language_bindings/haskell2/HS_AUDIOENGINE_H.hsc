
#if __GLASGOW_HASKELL__ && __GLASGOW_HASKELL__ < 409
#include <Rts.h>
#endif
#include <HsFFI.h>

#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <ctype.h>

#ifndef __quote__
#define __quote__(x...) x
#endif

#ifndef offsetof
#define offsetof(t, f) ((size_t) &((t *)0)->f)
#endif


#if __NHC__
#define hsc_line(line, file) \
    printf ("# %d \"%s\"\n", line, file);
#else
#define hsc_line(line, file) \
    printf ("{-# LINE %d \"%s\" #-}\n", line, file);
#endif

#define hsc_const(x)                        \
    if ((x) < 0)                            \
        printf ("%ld", (long)(x));          \
    else                                    \
        printf ("%lu", (unsigned long)(x));

#define hsc_const_str(x)                                          \
    {                                                             \
        const char *s = (x);                                      \
        printf ("\"");                                            \
        while (*s != '\0')                                        \
        {                                                         \
            if (*s == '"' || *s == '\\')                          \
                printf ("\\%c", *s);                              \
            else if (*s >= 0x20 && *s <= 0x7E)                    \
                printf ("%c", *s);                                \
            else                                                  \
                printf ("\\%d%s",                                 \
                        (unsigned char) *s,                       \
                        s[1] >= '0' && s[1] <= '9' ? "\\&" : ""); \
            ++s;                                                  \
        }                                                         \
        printf ("\"");                                            \
    }

#define hsc_type(t)                                         \
    if ((t)(int)(t)1.4 == (t)1.4)                           \
        printf ("%s%d",                                     \
                (t)(-1) < (t)0 ? "Int" : "Word",            \
                sizeof (t) * 8);                            \
    else                                                    \
        printf ("%s",                                       \
                sizeof (t) >  sizeof (double) ? "LDouble" : \
                sizeof (t) == sizeof (double) ? "Double"  : \
                "Float");

#define hsc_peek(t, f) \
    printf ("(\\hsc_ptr -> peekByteOff hsc_ptr %ld)", (long) offsetof (__quote__(t), f));

#define hsc_poke(t, f) \
    printf ("(\\hsc_ptr -> pokeByteOff hsc_ptr %ld)", (long) offsetof (__quote__(t), f));

#define hsc_ptr(t, f) \
    printf ("(\\hsc_ptr -> hsc_ptr `plusPtr` %ld)", (long) offsetof (__quote__(t), f));

#define hsc_offset(t, f) \
    printf("(%ld)", (long) offsetof (__quote__(t), f));

#define hsc_size(t) \
    printf("(%ld)", (long) sizeof(t));

#define hsc_enum(t, f, print_name, x)         \
    print_name;                               \
    printf (" :: %s\n", #t);                  \
    print_name;                               \
    printf (" = %s ", #f);                    \
    if ((x) < 0)                              \
        printf ("(%ld)\n", (long)(x));        \
    else                                      \
        printf ("%lu\n", (unsigned long)(x));

#define hsc_haskellize(x)                                          \
    {                                                              \
        const char *s = (x);                                       \
        int upper = 0;                                             \
        if (*s != '\0')                                            \
        {                                                          \
            putchar (tolower (*s));                                \
            ++s;                                                   \
            while (*s != '\0')                                     \
            {                                                      \
                if (*s == '_')                                     \
                    upper = 1;                                     \
                else                                               \
                {                                                  \
                    putchar (upper ? toupper (*s) : tolower (*s)); \
                    upper = 0;                                     \
                }                                                  \
                ++s;                                               \
            }                                                      \
        }                                                          \
    }

#def void _dummy_force_HS_AUDIOENGINE_H_hsc_c (void) { }
{-# OPTIONS -fglasgow-exts -XForeignFunctionInterface #-}

-- Split begin/HS_AUDIOENGINE_H

{-# OPTIONS -fglasgow-exts -XForeignFunctionInterface #-}
#include "hs_audioengine.h"
#ifndef __quote__
#define __quote__(x...) x
#endif

module HS_AUDIOENGINE_H(
  module HS_AUDIOENGINE_H,
{-- #SPLIT#
  module HS_AUDIOENGINE_H_C,
  module HS_AUDIOENGINE_H_S,
  module HS_AUDIOENGINE_H_F,
  module HS_AUDIOENGINE_H_E,
  module HS_AUDIOENGINE_H_S_d,
  module HS_AUDIOENGINE_H_S_t,
  module HS_AUDIOENGINE_H_S_n,
#SPLIT# --}
  module HSFFIG.FieldAccess,
  module Foreign,
  module Foreign.C.String,
  module Foreign.C.Types) where

import Foreign
import Foreign.Ptr
import Foreign.C.Types
import Foreign.C.String
import HSFFIG.FieldAccess
{-- #SPLIT#
import HS_AUDIOENGINE_H_C
import HS_AUDIOENGINE_H_S
import HS_AUDIOENGINE_H_F
import HS_AUDIOENGINE_H_E
import HS_AUDIOENGINE_H_S_d
import HS_AUDIOENGINE_H_S_t
import HS_AUDIOENGINE_H_S_n
import HSFFIG.FieldAccess
#SPLIT# --}

-- Split end


-- Split begin/HS_AUDIOENGINE_H_C_0

{-- #SPLIT#
{-# OPTIONS -fglasgow-exts -XForeignFunctionInterface #-}
module HS_AUDIOENGINE_H_C_0 (
  module HS_AUDIOENGINE_H_C_0
) where

import Foreign
import Foreign.C.Types
#SPLIT# --}


-- Split end


-- Split begin/HS_AUDIOENGINE_H_C_1

{-- #SPLIT#
{-# OPTIONS -fglasgow-exts -XForeignFunctionInterface #-}
module HS_AUDIOENGINE_H_C_1 (
  module HS_AUDIOENGINE_H_C_1
) where

import Foreign
import Foreign.C.Types
#SPLIT# --}


-- Split end


-- Split begin/HS_AUDIOENGINE_H_C_2

{-- #SPLIT#
{-# OPTIONS -fglasgow-exts -XForeignFunctionInterface #-}
module HS_AUDIOENGINE_H_C_2 (
  module HS_AUDIOENGINE_H_C_2
) where

import Foreign
import Foreign.C.Types
#SPLIT# --}


-- Split end


-- Split begin/HS_AUDIOENGINE_H_C

{-- #SPLIT#
{-# OPTIONS -fglasgow-exts -XForeignFunctionInterface #-}
module HS_AUDIOENGINE_H_C (
  module HS_AUDIOENGINE_H_C_0,
  module HS_AUDIOENGINE_H_C_1,
  module HS_AUDIOENGINE_H_C_2,
  module HS_AUDIOENGINE_H_C
) where

import Foreign
import Foreign.C.Types
import HS_AUDIOENGINE_H_C_0
import HS_AUDIOENGINE_H_C_1
import HS_AUDIOENGINE_H_C_2
#SPLIT# --}


-- Split end


-- Split begin/HS_AUDIOENGINE_H_S

{-- #SPLIT#
{-# OPTIONS -fglasgow-exts -XForeignFunctionInterface #-}
module HS_AUDIOENGINE_H_S (
  module HS_AUDIOENGINE_H_S
) where

import Foreign
import Foreign.C.Types
#SPLIT# --}


-- Split end


-- Split begin/HS_AUDIOENGINE_H_S_d

{-- #SPLIT#
{-# OPTIONS -fglasgow-exts -XForeignFunctionInterface #-}
module HS_AUDIOENGINE_H_S_d (
  module HS_AUDIOENGINE_H_S_d
) where

import Foreign
import Foreign.C.Types
#SPLIT# --}

data V_sizeof = V_sizeof
data X_sizeof = X_sizeof
data D_sizeof = D_sizeof

-- Split end



-- Split begin/HS_AUDIOENGINE_H_S_t

{-- #SPLIT#
{-# OPTIONS -fglasgow-exts -XForeignFunctionInterface #-}
module HS_AUDIOENGINE_H_S_t (
  module HS_AUDIOENGINE_H_S_t
) where

import Foreign
import Foreign.C.Types
import HS_AUDIOENGINE_H_S_n
#SPLIT# --}

type T_SclAction = FunPtr (CInt -> IO (()))
type T_SclAtom = Ptr (CChar)
type T_SclAtomType = CInt
type T_SclAudioDevice = Ptr (CChar)
type T_SclAudioHost = Ptr (CChar)
type T_SclAudioProcessor = Ptr (CChar)
type T_SclBufferedReceiver = FunPtr (CInt -> Ptr (Ptr (Ptr (CChar))) -> Ptr (CInt) -> CInt -> IO (()))
type T_SclChar = CChar
type T_SclDeviceStreamOptions = Ptr (CChar)
type T_SclDspError = Ptr (CChar)
type T_SclError = Ptr (CChar)
type T_SclErrorHandler = FunPtr (CInt -> Ptr (CChar) -> IO (()))
type T_SclFloat32 = CFloat
type T_SclFloat64 = CDouble
type T_SclFuture = Ptr (CChar)
type T_SclFutureGroup = Ptr (CChar)
type T_SclInt16 = CShort
type T_SclInt32 = CInt
type T_SclInt64 = CLLong
type T_SclInterruptable = Ptr (CChar)
type T_SclInterruptionMode = CInt
type T_SclMessageInfo = Ptr (CChar)
type T_SclMessageKind = CInt
type T_SclMidiDevice = Ptr (CChar)
type T_SclPortaudioError = Ptr (CChar)
type T_SclPortmidiError = Ptr (CChar)
type T_SclRealTime = CDouble
type T_SclReceiveOptions = Ptr (CChar)
type T_SclReceiver = FunPtr (CInt -> Ptr (Ptr (CChar)) -> CInt -> IO (()))
type T_SclScheduleOptions = Ptr (CChar)
type T_SclSendOptions = Ptr (CChar)
type T_SclStream = Ptr (CChar)
type T_SclStreamError = Ptr (CChar)
type T_SclStreamType = CInt
type T_SclString = Ptr (CChar)
type T_SclTime = CInt
type T_SclTimeUnit = CInt
type T_SclWord16 = CUShort
type T_SclWord32 = CUInt
type T_SclWord64 = CULLong
type T_int16_t = CShort
type T_int32_t = CInt
type T_int64_t = CLLong
type T_int8_t = CSChar
type T_int_fast16_t = CShort
type T_int_fast32_t = CInt
type T_int_fast64_t = CLLong
type T_int_fast8_t = CSChar
type T_int_least16_t = CShort
type T_int_least32_t = CInt
type T_int_least64_t = CLLong
type T_int_least8_t = CSChar
type T_intmax_t = CLong
type T_intptr_t = CLong
type T_uint16_t = CUShort
type T_uint32_t = CUInt
type T_uint64_t = CULLong
type T_uint8_t = CUChar
type T_uint_fast16_t = CUShort
type T_uint_fast32_t = CUInt
type T_uint_fast64_t = CULLong
type T_uint_fast8_t = CUChar
type T_uint_least16_t = CUShort
type T_uint_least32_t = CUInt
type T_uint_least64_t = CULLong
type T_uint_least8_t = CUChar
type T_uintmax_t = Unmapped_C_Type_long_unsigned_int
type T_uintptr_t = CULong

-- Split end



-- Split begin/HS_AUDIOENGINE_H_S_n

{-- #SPLIT#
{-# OPTIONS -fglasgow-exts -XForeignFunctionInterface #-}
module HS_AUDIOENGINE_H_S_n (
  module HS_AUDIOENGINE_H_S_n
) where

import Foreign
import Foreign.C.Types
#SPLIT# --}


-- Split end


-- Split begin/HS_AUDIOENGINE_H_E

{-- #SPLIT#
{-# OPTIONS -fglasgow-exts -XForeignFunctionInterface #-}
module HS_AUDIOENGINE_H_E (
  module HS_AUDIOENGINE_H_E
) where

import Foreign
import Foreign.C.Types
#SPLIT# --}


-- Split end

#include <stdlib.h>

-- Split begin/HS_AUDIOENGINE_H_F

{-- #SPLIT#
{-# OPTIONS -fglasgow-exts -XForeignFunctionInterface #-}
module HS_AUDIOENGINE_H_F (
  module HS_AUDIOENGINE_H_F
) where

import Foreign
import Foreign.C.Types
import HS_AUDIOENGINE_H_S
import HS_AUDIOENGINE_H_C
import HS_AUDIOENGINE_H_E
import HS_AUDIOENGINE_H_S_d
import HS_AUDIOENGINE_H_S_t
import HS_AUDIOENGINE_H_S_n
#SPLIT# --}

foreign import ccall "static hs_audioengine.h scl_message_type_audio"
  f_scl_message_type_audio :: IO (CInt)

--

foreign import ccall "static hs_audioengine.h scl_message_type_midi"
  f_scl_message_type_midi :: IO (CInt)

--

foreign import ccall "static hs_audioengine.h scl_time_unit_samples"
  f_scl_time_unit_samples :: IO (CInt)

--

foreign import ccall "static hs_audioengine.h scl_time_unit_milliseconds"
  f_scl_time_unit_milliseconds :: IO (CInt)

--

foreign import ccall "static hs_audioengine.h scl_atom_string"
  f_scl_atom_string :: IO (CInt)

--

foreign import ccall "static hs_audioengine.h scl_atom_int"
  f_scl_atom_int :: IO (CInt)

--

foreign import ccall "static hs_audioengine.h scl_atom_double"
  f_scl_atom_double :: IO (CInt)

--

foreign import ccall "static hs_audioengine.h scl_interruption_mode_simple"
  f_scl_interruption_mode_simple :: IO (CInt)

--

foreign import ccall "static hs_audioengine.h scl_interruption_mode_forcing"
  f_scl_interruption_mode_forcing :: IO (CInt)

--

foreign import ccall "static hs_audioengine.h scl_interruption_mode_transactional"
  f_scl_interruption_mode_transactional :: IO (CInt)

--

foreign import ccall "static hs_audioengine.h scl_schars_to_string"
  f_scl_schars_to_string :: Ptr (CSChar) -> IO (Ptr (CChar))

--

foreign import ccall "static hs_audioengine.h scl_uchars_to_string"
  f_scl_uchars_to_string :: Ptr (CUChar) -> IO (Ptr (CChar))

--

foreign import ccall "static hs_audioengine.h scl_string_to_schars"
  f_scl_string_to_schars :: Ptr (CChar) -> IO (Ptr (CSChar))

--

foreign import ccall "static hs_audioengine.h scl_string_to_uchars"
  f_scl_string_to_uchars :: Ptr (CChar) -> IO (Ptr (CUChar))

--

foreign import ccall "static hs_audioengine.h scl_atom_type"
  f_scl_atom_type :: Ptr (CChar) -> IO (CInt)

--

foreign import ccall "static hs_audioengine.h scl_atom_to_int"
  f_scl_atom_to_int :: Ptr (CChar) -> IO (CInt)

--

foreign import ccall "static hs_audioengine.h scl_atom_to_double"
  f_scl_atom_to_double :: Ptr (CChar) -> IO (CDouble)

--

foreign import ccall "static hs_audioengine.h scl_atom_to_string"
  f_scl_atom_to_string :: Ptr (CChar) -> IO (Ptr (CChar))

--

foreign import ccall "static hs_audioengine.h scl_atom_from_int"
  f_scl_atom_from_int :: CInt -> IO (Ptr (CChar))

--

foreign import ccall "static hs_audioengine.h scl_atom_from_double"
  f_scl_atom_from_double :: CDouble -> IO (Ptr (CChar))

--

foreign import ccall "static hs_audioengine.h scl_atom_from_string"
  f_scl_atom_from_string :: Ptr (CChar) -> IO (Ptr (CChar))

--

foreign import ccall "static hs_audioengine.h scl_error_message"
  f_scl_error_message :: Ptr (CChar) -> IO (Ptr (CChar))

--

foreign import ccall "static hs_audioengine.h scl_portaudio_error_code"
  f_scl_portaudio_error_code :: Ptr (CChar) -> IO (CInt)

--

foreign import ccall "static hs_audioengine.h scl_portmidi_error_code"
  f_scl_portmidi_error_code :: Ptr (CChar) -> IO (CInt)

--

foreign import ccall "static hs_audioengine.h scl_audio_host_name"
  f_scl_audio_host_name :: Ptr (CChar) -> IO (Ptr (CChar))

--

foreign import ccall "static hs_audioengine.h scl_audio_host_number_of_devices"
  f_scl_audio_host_number_of_devices :: Ptr (CChar) -> IO (CInt)

--

foreign import ccall "static hs_audioengine.h scl_audio_host_devices"
  f_scl_audio_host_devices :: Ptr (CChar) -> Ptr (CInt) -> IO (Ptr (Ptr (CChar)))

--

foreign import ccall "static hs_audioengine.h scl_audio_hosts"
  f_scl_audio_hosts :: Ptr (CInt) -> Ptr (Ptr (CChar)) -> IO (Ptr (Ptr (CChar)))

--

foreign import ccall "static hs_audioengine.h scl_default_audio_host"
  f_scl_default_audio_host :: Ptr (Ptr (CChar)) -> IO (Ptr (CChar))

--

foreign import ccall "static hs_audioengine.h scl_audio_device_name"
  f_scl_audio_device_name :: Ptr (CChar) -> IO (Ptr (CChar))

--

foreign import ccall "static hs_audioengine.h scl_audio_device_host"
  f_scl_audio_device_host :: Ptr (CChar) -> IO (Ptr (CChar))

--

foreign import ccall "static hs_audioengine.h scl_audio_device_max_input_channels"
  f_scl_audio_device_max_input_channels :: Ptr (CChar) -> IO (CInt)

--

foreign import ccall "static hs_audioengine.h scl_audio_device_max_output_channels"
  f_scl_audio_device_max_output_channels :: Ptr (CChar) -> IO (CInt)

--

foreign import ccall "static hs_audioengine.h scl_audio_device_default_low_input_latency"
  f_scl_audio_device_default_low_input_latency :: Ptr (CChar) -> IO (CInt)

--

foreign import ccall "static hs_audioengine.h scl_audio_device_default_high_input_latency"
  f_scl_audio_device_default_high_input_latency :: Ptr (CChar) -> IO (CInt)

--

foreign import ccall "static hs_audioengine.h scl_audio_device_default_low_output_latency"
  f_scl_audio_device_default_low_output_latency :: Ptr (CChar) -> IO (CInt)

--

foreign import ccall "static hs_audioengine.h scl_audio_device_default_high_output_latency"
  f_scl_audio_device_default_high_output_latency :: Ptr (CChar) -> IO (CInt)

--

foreign import ccall "static hs_audioengine.h scl_audio_device_default_sample_rate"
  f_scl_audio_device_default_sample_rate :: Ptr (CChar) -> IO (CInt)

--

foreign import ccall "static hs_audioengine.h scl_default_audio_input_device"
  f_scl_default_audio_input_device :: Ptr (Ptr (CChar)) -> IO (Ptr (CChar))

--

foreign import ccall "static hs_audioengine.h scl_default_audio_output_device"
  f_scl_default_audio_output_device :: Ptr (Ptr (CChar)) -> IO (Ptr (CChar))

--

foreign import ccall "static hs_audioengine.h scl_midi_device_name"
  f_scl_midi_device_name :: Ptr (CChar) -> IO (Ptr (CChar))

--

foreign import ccall "static hs_audioengine.h scl_midi_device_host_name"
  f_scl_midi_device_host_name :: Ptr (CChar) -> IO (Ptr (CChar))

--

foreign import ccall "static hs_audioengine.h scl_midi_device_has_input"
  f_scl_midi_device_has_input :: Ptr (CChar) -> IO (CInt)

--

foreign import ccall "static hs_audioengine.h scl_midi_device_has_output"
  f_scl_midi_device_has_output :: Ptr (CChar) -> IO (CInt)

--

foreign import ccall "static hs_audioengine.h scl_default_midi_input_device"
  f_scl_default_midi_input_device :: Ptr (Ptr (CChar)) -> IO (Ptr (CChar))

--

foreign import ccall "static hs_audioengine.h scl_default_midi_output_device"
  f_scl_default_midi_output_device :: Ptr (Ptr (CChar)) -> IO (Ptr (CChar))

--

foreign import ccall "static hs_audioengine.h scl_midi_devices"
  f_scl_midi_devices :: Ptr (CInt) -> Ptr (Ptr (CChar)) -> IO (Ptr (Ptr (CChar)))

--

foreign import ccall "static hs_audioengine.h scl_processor_controls"
  f_scl_processor_controls :: Ptr (CChar) -> Ptr (CInt) -> IO (Ptr (Ptr (CChar)))

--

foreign import ccall "static hs_audioengine.h scl_processor_num_inputs"
  f_scl_processor_num_inputs :: Ptr (CChar) -> IO (CInt)

--

foreign import ccall "static hs_audioengine.h scl_processor_num_outputs"
  f_scl_processor_num_outputs :: Ptr (CChar) -> IO (CInt)

--

foreign import ccall "static hs_audioengine.h scl_sequence"
  f_scl_sequence :: Ptr (Ptr (CChar)) -> CInt -> Ptr (Ptr (CChar)) -> IO (Ptr (CChar))

--

foreign import ccall "static hs_audioengine.h scl_parallel"
  f_scl_parallel :: Ptr (Ptr (CChar)) -> CInt -> Ptr (Ptr (CChar)) -> IO (Ptr (CChar))

--

foreign import ccall "static hs_audioengine.h scl_load_fluidsynth"
  f_scl_load_fluidsynth :: Ptr (CChar) -> Ptr (Ptr (CChar)) -> IO (Ptr (CChar))

--

foreign import ccall "static hs_audioengine.h scl_stream_sample_rate"
  f_scl_stream_sample_rate :: Ptr (CChar) -> IO (CInt)

--

foreign import ccall "static hs_audioengine.h scl_stream_audio_buffer_size"
  f_scl_stream_audio_buffer_size :: Ptr (CChar) -> IO (CInt)

--

foreign import ccall "static hs_audioengine.h scl_stream_running"
  f_scl_stream_running :: Ptr (CChar) -> IO (CInt)

--

foreign import ccall "static hs_audioengine.h scl_stream_start"
  f_scl_stream_start :: Ptr (CChar) -> Ptr (Ptr (CChar)) -> Ptr (Ptr (CChar)) -> Ptr (Ptr (CChar)) -> IO (())

--

foreign import ccall "static hs_audioengine.h scl_stream_stop"
  f_scl_stream_stop :: Ptr (CChar) -> Ptr (Ptr (CChar)) -> Ptr (Ptr (CChar)) -> Ptr (Ptr (CChar)) -> IO (())

--

foreign import ccall "static hs_audioengine.h scl_stream_abort"
  f_scl_stream_abort :: Ptr (CChar) -> Ptr (Ptr (CChar)) -> Ptr (Ptr (CChar)) -> Ptr (Ptr (CChar)) -> IO (())

--

foreign import ccall "static hs_audioengine.h scl_stream_set_error_handler"
  f_scl_stream_set_error_handler :: Ptr (CChar) -> FunPtr (CInt -> Ptr (CChar) -> IO (())) -> IO (())
foreign import ccall "wrapper"
  w_scl_stream_set_error_handler_1 :: (CInt -> Ptr (CChar) -> IO (())) -> IO (FunPtr (CInt -> Ptr (CChar) -> IO (())))

--

foreign import ccall "static hs_audioengine.h scl_default_device_stream_options"
  f_scl_default_device_stream_options :: IO (Ptr (CChar))

--

foreign import ccall "static hs_audioengine.h scl_device_stream_options_get_sample_rate"
  f_scl_device_stream_options_get_sample_rate :: Ptr (CChar) -> IO (CInt)

--

foreign import ccall "static hs_audioengine.h scl_device_stream_options_get_audio_buffer_size"
  f_scl_device_stream_options_get_audio_buffer_size :: Ptr (CChar) -> IO (CInt)

--

foreign import ccall "static hs_audioengine.h scl_device_stream_options_get_audio_latency"
  f_scl_device_stream_options_get_audio_latency :: Ptr (CChar) -> IO (CDouble)

--

foreign import ccall "static hs_audioengine.h scl_device_stream_options_get_midi_latency"
  f_scl_device_stream_options_get_midi_latency :: Ptr (CChar) -> IO (CDouble)

--

foreign import ccall "static hs_audioengine.h scl_device_stream_options_is_non_blocking"
  f_scl_device_stream_options_is_non_blocking :: Ptr (CChar) -> IO (CInt)

--

foreign import ccall "static hs_audioengine.h scl_device_stream_options_is_exclusive_mode"
  f_scl_device_stream_options_is_exclusive_mode :: Ptr (CChar) -> IO (CInt)

--

foreign import ccall "static hs_audioengine.h scl_device_stream_options_set_sample_rate"
  f_scl_device_stream_options_set_sample_rate :: Ptr (CChar) -> CInt -> IO (())

--

foreign import ccall "static hs_audioengine.h scl_device_stream_options_set_audio_buffer_size"
  f_scl_device_stream_options_set_audio_buffer_size :: Ptr (CChar) -> CInt -> IO (())

--

foreign import ccall "static hs_audioengine.h scl_device_stream_options_set_audio_latency"
  f_scl_device_stream_options_set_audio_latency :: Ptr (CChar) -> CDouble -> IO (())

--

foreign import ccall "static hs_audioengine.h scl_device_stream_options_set_midi_latency"
  f_scl_device_stream_options_set_midi_latency :: Ptr (CChar) -> CDouble -> IO (())

--

foreign import ccall "static hs_audioengine.h scl_device_stream_options_set_non_blocking"
  f_scl_device_stream_options_set_non_blocking :: Ptr (CChar) -> CInt -> IO (CDouble)

--

foreign import ccall "static hs_audioengine.h scl_device_stream_options_set_exclusive_mode"
  f_scl_device_stream_options_set_exclusive_mode :: Ptr (CChar) -> CInt -> IO (CDouble)

--

foreign import ccall "static hs_audioengine.h scl_open_device_stream"
  f_scl_open_device_stream :: Ptr (CChar) -> Ptr (CChar) -> Ptr (CChar) -> Ptr (CChar) -> Ptr (CChar) -> Ptr (CChar) -> Ptr (Ptr (CChar)) -> Ptr (Ptr (CChar)) -> Ptr (Ptr (CChar)) -> IO (Ptr (CChar))

--

foreign import ccall "static hs_audioengine.h scl_interrupt_future"
  f_scl_interrupt_future :: Ptr (CChar) -> IO (())

--

foreign import ccall "static hs_audioengine.h scl_interrupt_future_group"
  f_scl_interrupt_future_group :: Ptr (CChar) -> IO (())

--

foreign import ccall "static hs_audioengine.h scl_new_future_group"
  f_scl_new_future_group :: CInt -> IO (Ptr (CChar))

--

foreign import ccall "static hs_audioengine.h scl_future_group_interruption_mode"
  f_scl_future_group_interruption_mode :: Ptr (CChar) -> IO (CInt)

--

foreign import ccall "static hs_audioengine.h scl_default_schedule_options"
  f_scl_default_schedule_options :: IO (Ptr (CChar))

--

foreign import ccall "static hs_audioengine.h scl_schedule_options_get_unit"
  f_scl_schedule_options_get_unit :: Ptr (CChar) -> IO (CInt)

--

foreign import ccall "static hs_audioengine.h scl_schedule_options_get_groups"
  f_scl_schedule_options_get_groups :: Ptr (CChar) -> Ptr (CInt) -> IO (Ptr (Ptr (CChar)))

--

foreign import ccall "static hs_audioengine.h scl_schedule_options_get_repeats"
  f_scl_schedule_options_get_repeats :: Ptr (CChar) -> IO (CInt)

--

foreign import ccall "static hs_audioengine.h scl_schedule_options_get_interval"
  f_scl_schedule_options_get_interval :: Ptr (CChar) -> IO (CInt)

--

foreign import ccall "static hs_audioengine.h scl_schedule_options_set_unit"
  f_scl_schedule_options_set_unit :: Ptr (CChar) -> CInt -> IO (())

--

foreign import ccall "static hs_audioengine.h scl_schedule_options_set_groups"
  f_scl_schedule_options_set_groups :: Ptr (CChar) -> Ptr (Ptr (CChar)) -> CInt -> IO (())

--

foreign import ccall "static hs_audioengine.h scl_schedule_options_set_repeats"
  f_scl_schedule_options_set_repeats :: Ptr (CChar) -> CInt -> IO (())

--

foreign import ccall "static hs_audioengine.h scl_schedule_options_set_interval"
  f_scl_schedule_options_set_interval :: Ptr (CChar) -> CInt -> IO (())

--

foreign import ccall "static hs_audioengine.h scl_schedule_now"
  f_scl_schedule_now :: Ptr (CChar) -> FunPtr (CInt -> IO (())) -> Ptr (CChar) -> Ptr (Ptr (CChar)) -> IO (Ptr (CChar))
foreign import ccall "wrapper"
  w_scl_schedule_now_1 :: (CInt -> IO (())) -> IO (FunPtr (CInt -> IO (())))

--

foreign import ccall "static hs_audioengine.h scl_schedule_later"
  f_scl_schedule_later :: Ptr (CChar) -> CInt -> FunPtr (CInt -> IO (())) -> Ptr (CChar) -> Ptr (Ptr (CChar)) -> IO (Ptr (CChar))
foreign import ccall "wrapper"
  w_scl_schedule_later_1 :: (CInt -> IO (())) -> IO (FunPtr (CInt -> IO (())))

--

foreign import ccall "static hs_audioengine.h scl_schedule_at"
  f_scl_schedule_at :: Ptr (CChar) -> CInt -> FunPtr (CInt -> IO (())) -> Ptr (CChar) -> Ptr (Ptr (CChar)) -> IO (Ptr (CChar))
foreign import ccall "wrapper"
  w_scl_schedule_at_1 :: (CInt -> IO (())) -> IO (FunPtr (CInt -> IO (())))

--

foreign import ccall "static hs_audioengine.h scl_default_send_options"
  f_scl_default_send_options :: IO (Ptr (CChar))

--

foreign import ccall "static hs_audioengine.h scl_send_options_get_kind"
  f_scl_send_options_get_kind :: Ptr (CChar) -> IO (CInt)

--

foreign import ccall "static hs_audioengine.h scl_send_options_get_processors"
  f_scl_send_options_get_processors :: Ptr (CChar) -> Ptr (CInt) -> IO (Ptr (Ptr (CChar)))

--

foreign import ccall "static hs_audioengine.h scl_send_options_get_devices"
  f_scl_send_options_get_devices :: Ptr (CChar) -> Ptr (CInt) -> IO (Ptr (Ptr (CChar)))

--

foreign import ccall "static hs_audioengine.h scl_send_options_get_channels"
  f_scl_send_options_get_channels :: Ptr (CChar) -> Ptr (CInt) -> IO (Ptr (CInt))

--

foreign import ccall "static hs_audioengine.h scl_send_options_set_kind"
  f_scl_send_options_set_kind :: Ptr (CChar) -> CInt -> IO (())

--

foreign import ccall "static hs_audioengine.h scl_send_options_set_processors"
  f_scl_send_options_set_processors :: Ptr (CChar) -> Ptr (Ptr (CChar)) -> CInt -> IO (())

--

foreign import ccall "static hs_audioengine.h scl_send_options_set_devices"
  f_scl_send_options_set_devices :: Ptr (CChar) -> Ptr (Ptr (CChar)) -> CInt -> IO (())

--

foreign import ccall "static hs_audioengine.h scl_send_options_set_channels"
  f_scl_send_options_set_channels :: Ptr (CChar) -> Ptr (CInt) -> CInt -> IO (())

--

foreign import ccall "static hs_audioengine.h scl_default_receive_options"
  f_scl_default_receive_options :: IO (Ptr (CChar))

--

foreign import ccall "static hs_audioengine.h scl_receive_options_get_kind"
  f_scl_receive_options_get_kind :: Ptr (CChar) -> IO (CInt)

--

foreign import ccall "static hs_audioengine.h scl_receive_options_get_processors"
  f_scl_receive_options_get_processors :: Ptr (CChar) -> Ptr (CInt) -> IO (Ptr (Ptr (CChar)))

--

foreign import ccall "static hs_audioengine.h scl_receive_options_get_devices"
  f_scl_receive_options_get_devices :: Ptr (CChar) -> Ptr (CInt) -> IO (Ptr (Ptr (CChar)))

--

foreign import ccall "static hs_audioengine.h scl_receive_options_get_channels"
  f_scl_receive_options_get_channels :: Ptr (CChar) -> Ptr (CInt) -> IO (Ptr (CInt))

--

foreign import ccall "static hs_audioengine.h scl_receive_options_set_kind"
  f_scl_receive_options_set_kind :: Ptr (CChar) -> CInt -> IO (())

--

foreign import ccall "static hs_audioengine.h scl_receive_options_set_processors"
  f_scl_receive_options_set_processors :: Ptr (CChar) -> Ptr (Ptr (CChar)) -> CInt -> IO (())

--

foreign import ccall "static hs_audioengine.h scl_receive_options_set_devices"
  f_scl_receive_options_set_devices :: Ptr (CChar) -> Ptr (Ptr (CChar)) -> CInt -> IO (())

--

foreign import ccall "static hs_audioengine.h scl_receive_options_set_channels"
  f_scl_receive_options_set_channels :: Ptr (CChar) -> Ptr (CInt) -> CInt -> IO (())

--

foreign import ccall "static hs_audioengine.h scl_send_now"
  f_scl_send_now :: Ptr (CChar) -> Ptr (Ptr (CChar)) -> CInt -> Ptr (CChar) -> Ptr (Ptr (CChar)) -> IO (Ptr (CChar))

--

foreign import ccall "static hs_audioengine.h scl_send_later"
  f_scl_send_later :: Ptr (CChar) -> CInt -> Ptr (Ptr (CChar)) -> CInt -> Ptr (CChar) -> Ptr (Ptr (CChar)) -> IO (Ptr (CChar))

--

foreign import ccall "static hs_audioengine.h scl_send_at"
  f_scl_send_at :: Ptr (CChar) -> CInt -> Ptr (Ptr (CChar)) -> CInt -> Ptr (CChar) -> Ptr (Ptr (CChar)) -> IO (Ptr (CChar))

--

foreign import ccall "static hs_audioengine.h scl_receive"
  f_scl_receive :: Ptr (CChar) -> FunPtr (CInt -> Ptr (Ptr (CChar)) -> CInt -> IO (())) -> Ptr (CChar) -> Ptr (Ptr (CChar)) -> IO (Ptr (CChar))
foreign import ccall "wrapper"
  w_scl_receive_1 :: (CInt -> Ptr (Ptr (CChar)) -> CInt -> IO (())) -> IO (FunPtr (CInt -> Ptr (Ptr (CChar)) -> CInt -> IO (())))

--

foreign import ccall "static hs_audioengine.h scl_receive_buffered"
  f_scl_receive_buffered :: Ptr (CChar) -> FunPtr (CInt -> Ptr (Ptr (Ptr (CChar))) -> Ptr (CInt) -> CInt -> IO (())) -> Ptr (CChar) -> Ptr (Ptr (CChar)) -> IO (Ptr (CChar))
foreign import ccall "wrapper"
  w_scl_receive_buffered_1 :: (CInt -> Ptr (Ptr (Ptr (CChar))) -> Ptr (CInt) -> CInt -> IO (())) -> IO (FunPtr (CInt -> Ptr (Ptr (Ptr (CChar))) -> Ptr (CInt) -> CInt -> IO (())))

--

foreign import ccall "static hs_audioengine.h scl_test_nothing"
  f_scl_test_nothing :: IO (())

--

foreign import ccall "static hs_audioengine.h scl_test_error"
  f_scl_test_error :: CInt -> Ptr (Ptr (CChar)) -> IO (())

--

foreign import ccall "static hs_audioengine.h scl_test_two_errors"
  f_scl_test_two_errors :: CInt -> Ptr (Ptr (CChar)) -> Ptr (Ptr (CChar)) -> IO (())

--

foreign import ccall "static hs_audioengine.h scl_test_pass_int"
  f_scl_test_pass_int :: CInt -> IO (CInt)

--

foreign import ccall "static hs_audioengine.h scl_test_pass_float"
  f_scl_test_pass_float :: CFloat -> IO (CFloat)

--

foreign import ccall "static hs_audioengine.h scl_test_pass_double"
  f_scl_test_pass_double :: CDouble -> IO (CDouble)

--

foreign import ccall "static hs_audioengine.h scl_test_pass_enum"
  f_scl_test_pass_enum :: CInt -> IO (CInt)

--

foreign import ccall "static hs_audioengine.h scl_test_pass_string"
  f_scl_test_pass_string :: Ptr (CChar) -> IO (Ptr (CChar))

--

foreign import ccall "static hs_audioengine.h scl_test_pass_atom"
  f_scl_test_pass_atom :: Ptr (CChar) -> IO (Ptr (CChar))

--

foreign import ccall "static hs_audioengine.h scl_test_pass_object"
  f_scl_test_pass_object :: Ptr (CChar) -> IO (Ptr (CChar))

--

foreign import ccall "static hs_audioengine.h scl_test_pass_list_int"
  f_scl_test_pass_list_int :: Ptr (CInt) -> CInt -> IO (())

--

foreign import ccall "static hs_audioengine.h scl_test_pass_list_string"
  f_scl_test_pass_list_string :: Ptr (Ptr (CChar)) -> CInt -> IO (())

--

foreign import ccall "static hs_audioengine.h scl_test_pass_list_object"
  f_scl_test_pass_list_object :: Ptr (Ptr (CChar)) -> CInt -> IO (())

--

foreign import ccall "static hs_audioengine.h scl_test_pass_list_atom"
  f_scl_test_pass_list_atom :: Ptr (Ptr (CChar)) -> CInt -> IO (())

--

foreign import ccall "static hs_audioengine.h scl_test_pass_list_list_int"
  f_scl_test_pass_list_list_int :: Ptr (Ptr (CInt)) -> Ptr (CInt) -> CInt -> IO (())

--

foreign import ccall "static hs_audioengine.h scl_test_return_int"
  f_scl_test_return_int :: IO (CInt)

--

foreign import ccall "static hs_audioengine.h scl_test_return_float"
  f_scl_test_return_float :: IO (CFloat)

--

foreign import ccall "static hs_audioengine.h scl_test_return_double"
  f_scl_test_return_double :: IO (CDouble)

--

foreign import ccall "static hs_audioengine.h scl_test_return_enum"
  f_scl_test_return_enum :: IO (CInt)

--

foreign import ccall "static hs_audioengine.h scl_test_return_string"
  f_scl_test_return_string :: IO (Ptr (CChar))

--

foreign import ccall "static hs_audioengine.h scl_test_return_atom"
  f_scl_test_return_atom :: IO (Ptr (CChar))

--

foreign import ccall "static hs_audioengine.h scl_test_return_object"
  f_scl_test_return_object :: IO (Ptr (CChar))

--

foreign import ccall "static hs_audioengine.h scl_test_return_list_int"
  f_scl_test_return_list_int :: Ptr (CInt) -> IO (Ptr (CInt))

--

foreign import ccall "static hs_audioengine.h scl_test_return_list_string"
  f_scl_test_return_list_string :: Ptr (CInt) -> IO (Ptr (Ptr (CChar)))

--

foreign import ccall "static hs_audioengine.h scl_test_return_list_object"
  f_scl_test_return_list_object :: Ptr (CInt) -> IO (Ptr (Ptr (CChar)))

--

foreign import ccall "static hs_audioengine.h scl_test_return_list_atom"
  f_scl_test_return_list_atom :: Ptr (CInt) -> IO (Ptr (Ptr (CChar)))

--

foreign import ccall "static hs_audioengine.h scl_test_return_list_list_int"
  f_scl_test_return_list_list_int :: Ptr (Ptr (CInt)) -> Ptr (CInt) -> IO (Ptr (Ptr (CInt)))

--

foreign import ccall "static hs_audioengine.h scl_free_error"
  f_scl_free_error :: Ptr (CChar) -> IO (())

--

foreign import ccall "static hs_audioengine.h scl_free_portaudio_error"
  f_scl_free_portaudio_error :: Ptr (CChar) -> IO (())

--

foreign import ccall "static hs_audioengine.h scl_free_portmidi_error"
  f_scl_free_portmidi_error :: Ptr (CChar) -> IO (())

--

foreign import ccall "static hs_audioengine.h scl_free_audio_host"
  f_scl_free_audio_host :: Ptr (CChar) -> IO (())

--

foreign import ccall "static hs_audioengine.h scl_free_audio_device"
  f_scl_free_audio_device :: Ptr (CChar) -> IO (())

--

foreign import ccall "static hs_audioengine.h scl_free_midi_device"
  f_scl_free_midi_device :: Ptr (CChar) -> IO (())

--

foreign import ccall "static hs_audioengine.h scl_free_future"
  f_scl_free_future :: Ptr (CChar) -> IO (())

--

foreign import ccall "static hs_audioengine.h scl_free_stream"
  f_scl_free_stream :: Ptr (CChar) -> IO (())

--

foreign import ccall "static hs_audioengine.h scl_free_processor"
  f_scl_free_processor :: Ptr (CChar) -> IO (())

--

foreign import ccall "static hs_audioengine.h scl_free_future_group"
  f_scl_free_future_group :: Ptr (CChar) -> IO (())

--

foreign import ccall "static hs_audioengine.h scl_free_atom"
  f_scl_free_atom :: Ptr (CChar) -> IO (())

--

foreign import ccall "static hs_audioengine.h scl_free_schedule_options"
  f_scl_free_schedule_options :: Ptr (CChar) -> IO (())

--

foreign import ccall "static hs_audioengine.h scl_free_send_options"
  f_scl_free_send_options :: Ptr (CChar) -> IO (())

--

foreign import ccall "static hs_audioengine.h scl_free_receive_options"
  f_scl_free_receive_options :: Ptr (CChar) -> IO (())

--

foreign import ccall "static hs_audioengine.h scl_free_array"
  f_scl_free_array :: Ptr (Ptr (CChar)) -> IO (())

--


-- Split end

