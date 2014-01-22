
#include <fa/fa.h>
#include <fa/util.h>

/*
    This program reads an audio file and prints its info.
 */

void read_audio()
{
    buffer_t b = fa_pair_second(fa_buffer_read_audio(string("test/test.wav")));

    fa_print_ln(fa_buffer_meta(b));
    fa_print("The size is: %s\n", i64(fa_buffer_size(b)));
}

int main(int argc, char const *argv[])
{
    fa_initialize();

    read_audio();

    fa_terminate();
}
