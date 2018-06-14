#include <mpg123.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <time.h>

#include <dirent.h>

#include "sds.h"
#include "timing.h"
#include "math.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>

#include <pulse/simple.h>
#include <pulse/error.h>
#include <stdio.h>

#include <curses.h>


#define RATE 44100

#define DIT 1
#define DAH 2
#define CHAR_SPACE 4
#define WORD_SPACE 5

int set_data( int *data, int n, const char c )
{
	switch( c ) {
		case 'a':
			data[0] = DIT;
			data[1] = DAH;
			return 2;
		case 'b':
			data[0] = DAH;
			data[1] = DIT;
			data[2] = DIT;
			data[3] = DIT;
			return 4;
		case 'c':
			data[0] = DAH;
			data[1] = DIT;
			data[2] = DAH;
			data[3] = DIT;
			return 4;
		case 'd':
			data[0] = DAH;
			data[1] = DIT;
			data[2] = DIT;
			return 3;
		case 'e':
			data[0] = DIT;
			return 1;
		case 'f':
			data[0] = DIT;
			data[1] = DIT;
			data[2] = DAH;
			data[3] = DIT;
			return 4;
		case 'g':
			data[0] = DAH;
			data[1] = DAH;
			data[2] = DIT;
			return 3;
		case 'h':
			data[0] = DIT;
			data[1] = DIT;
			data[2] = DIT;
			data[3] = DIT;
			return 4;
		case 'i':
			data[0] = DIT;
			data[1] = DIT;
			return 2;
		case 'j':
			data[0] = DIT;
			data[1] = DAH;
			data[2] = DAH;
			data[3] = DAH;
			return 4;
		case 'k':
			data[0] = DAH;
			data[1] = DIT;
			data[2] = DAH;
			return 3;
		case 'l':
			data[0] = DIT;
			data[1] = DAH;
			data[2] = DIT;
			data[3] = DIT;
			return 4;
		case 'm':
			data[0] = DAH;
			data[1] = DAH;
			return 2;
		case 'n':
			data[0] = DAH;
			data[1] = DIT;
			return 2;
		case 'o':
			data[0] = DAH;
			data[1] = DAH;
			data[2] = DAH;
			return 3;
		case 'p':
			data[0] = DIT;
			data[1] = DAH;
			data[2] = DAH;
			data[3] = DIT;
			return 4;
		case 'q':
			data[0] = DAH;
			data[1] = DAH;
			data[2] = DIT;
			data[3] = DAH;
			return 4;
		case 'r':
			data[0] = DIT;
			data[1] = DAH;
			data[2] = DIT;
			return 3;
		case 's':
			data[0] = DIT;
			data[1] = DIT;
			data[2] = DIT;
			return 3;
		case 't':
			data[0] = DAH;
			return 1;
		case 'u':
			data[0] = DIT;
			data[1] = DIT;
			data[2] = DAH;
			return 3;
		case 'v':
			data[0] = DIT;
			data[1] = DIT;
			data[2] = DIT;
			data[3] = DAH;
			return 4;
		case 'w':
			data[0] = DIT;
			data[1] = DAH;
			data[2] = DAH;
			return 3;
		case 'x':
			data[0] = DAH;
			data[1] = DIT;
			data[2] = DIT;
			data[3] = DAH;
			return 4;
		case 'y':
			data[0] = DAH;
			data[1] = DIT;
			data[2] = DAH;
			data[3] = DAH;
			return 4;
		case 'z':
			data[0] = DAH;
			data[1] = DAH;
			data[2] = DIT;
			data[3] = DIT;
			return 4;
		case '1':
			data[0] = DIT;
			data[1] = DAH;
			data[2] = DAH;
			data[3] = DAH;
			data[4] = DAH;
			return 5;
		case '2':
			data[0] = DIT;
			data[1] = DIT;
			data[2] = DAH;
			data[3] = DAH;
			data[4] = DAH;
			return 5;
		case '3':
			data[0] = DIT;
			data[1] = DIT;
			data[2] = DIT;
			data[3] = DAH;
			data[4] = DAH;
			return 5;
		case '4':
			data[0] = DIT;
			data[1] = DIT;
			data[2] = DIT;
			data[3] = DIT;
			data[4] = DAH;
			return 5;
		case '5':
			data[0] = DIT;
			data[1] = DIT;
			data[2] = DIT;
			data[3] = DIT;
			data[4] = DIT;
			return 5;
		case '6':
			data[0] = DAH;
			data[1] = DIT;
			data[2] = DIT;
			data[3] = DIT;
			data[4] = DIT;
			return 5;
		case '7':
			data[0] = DAH;
			data[1] = DAH;
			data[2] = DIT;
			data[3] = DIT;
			data[4] = DIT;
			return 5;
		case '8':
			data[0] = DAH;
			data[1] = DAH;
			data[2] = DAH;
			data[3] = DIT;
			data[4] = DIT;
			return 5;
		case '9':
			data[0] = DAH;
			data[1] = DAH;
			data[2] = DAH;
			data[3] = DAH;
			data[4] = DIT;
			return 5;
		case '0':
			data[0] = DAH;
			data[1] = DAH;
			data[2] = DAH;
			data[3] = DAH;
			data[4] = DAH;
			return 5;
		default:
			return -1;
	}
}

size_t synth(
		int tone, float dit_length, float dah_length, float gap_length, float char_space_length, float word_space_length,
		int *data, char *buf, int max_samples
		)
{
	float angular_frequency = tone * 2.0 * M_PI;

	int mute = 1;
	float next_at = 0.0;
	int data_i = 0;

	for( int i = 0; i < max_samples; i++ ) {
		float t = ((float)i) / RATE;
		float x = sin( t * angular_frequency );
		int16_t xx = x * 32500;
		int16_t *p = (int16_t*) (buf + i*2);

		if( t >= next_at ) {
			if( !mute ) {
				// just finished a DIT or a DAH, insert a gap
				mute = 1;
				next_at = t + gap_length;
			} else {
				switch( data[data_i] ) {
					case DIT:
						mute = 0;
						next_at = t + dit_length;
						break;
					case DAH:
						mute = 0;
						next_at = t + dah_length;
						break;
					case CHAR_SPACE:
						mute = 1;
						next_at = t + char_space_length;
						break;
					case WORD_SPACE:
						mute = 1;
						next_at = t + word_space_length;
						break;
					case 0:
						return i*2;
					default:
						printf("got %d\n", data_i );
						printf("got %d\n", data[data_i] );
						assert(0);
				}
				data_i++;
			}
		}

		if( mute ) {
			*p = 0;
		} else {
			*p = xx;
		}
	}
	return -1;
}

void screen_printf(const char *fmt, ...) {
	char s[1024];
	va_list args;
	va_start(args, fmt);
	vsprintf(s, fmt, args);
	va_end(args);

	clear();
	mvaddstr(0, 0, s);
	refresh();
}

char get_random( int char_start, int char_end ) {
	int num = char_end - char_start;
	int n = random() % num;
	n += char_start;
	if( n < 27 ) {
		return 'a' + n;
	}
	n -= 27;
	if( n < 10 ) {
		return '0' + n;
	}
	return '\0';
}

int main(int argc, char *argv[])
{
	if( argc != 3 ) {
		fprintf(stderr, "usage: %s <char start> <char end>\n", argv[0]);
		return 1;
	}

	int char_start = atoi(argv[1]);
	int char_end   = atoi(argv[2]);
	if( char_end <= char_start ) {
		fprintf(stderr, "<char start> must be less than <char end>\n");
		return 1;
	}

	initscr();
	timeout(-1);

    refresh();

	int res;
	int error;

	srand( get_current_time_ms() );

	static const pa_sample_spec ss = {
		.format = PA_SAMPLE_S16LE,
		.rate = RATE,
		.channels = 1
	};

	pa_simple *pa_handle;
	pa_handle = pa_simple_new(NULL, "alexplayer", PA_STREAM_PLAYBACK, NULL, "playback", &ss, NULL, NULL, &error);
	if( pa_handle == NULL ) {
		fprintf(stderr, __FILE__": pa_simple_new() failed: %s\n", pa_strerror(error));
		assert( 0 );
	}

	int max_seconds = 10;
	size_t max_samples = RATE * max_seconds;
	size_t buf_size = max_samples * 2;

	float dit_length = 0.06;
	float dah_length = dit_length * 3;
	float gap_length = dit_length;
	float char_space_length = dit_length * 3;
	float word_space_length = dit_length * 7;

	char_space_length -= gap_length;
	word_space_length -= gap_length;
	assert( dit_length > 0.0 );
	assert( dah_length > dit_length );
	assert( gap_length > 0.0 );
	assert( char_space_length > gap_length );
	assert( word_space_length > char_space_length );

	float tone = 500;

	while(1) {
		char randomletter = get_random( char_start, char_end );

		screen_printf("listen...");

		int data[1024];
		res = set_data( data, 1024, randomletter );
		assert( res > 0 );
		data[res] = 0;

		char *buf = malloc( buf_size );

		size_t buf_len = synth( tone, dit_length, dah_length, gap_length, char_space_length, word_space_length, data, buf, max_samples );
		assert( buf_len > 0 );

		res = pa_simple_write( pa_handle, buf, buf_len, &error );
		assert( res == 0 );

		res = pa_simple_drain( pa_handle, &error );
		assert( res == 0 );


		screen_printf("what was that?");
		int guess = getch();

		if( guess == randomletter ) {
			screen_printf("correct!");
			sleep(2);
			continue;
		}
		screen_printf("wrong! %c", randomletter);
		sleep(2);

		for(int i = 0; i < 3; i++ ) {
			res = pa_simple_write( pa_handle, buf, buf_len, &error );
			assert( res == 0 );

			res = pa_simple_drain( pa_handle, &error );
			assert( res == 0 );

			sleep(1);
		}

	}
	endwin();
	return 0;
}
