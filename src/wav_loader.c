#include "wav_loader.h"

#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>


int load_wav(const char *path, char *buf, int buf_len)
{
	int num_bytes = 0;
	char data[1024*1024];
	int fd = open( path, O_RDONLY, 0);
	assert( read(fd, data, 4) == 4 );
	assert( memcmp( data, "RIFF", 4) == 0 );

	uint32_t chunk_size;
	assert( read(fd, &chunk_size, 4) == 4 );

	assert( read(fd, data, 4) == 4 );
	assert( memcmp( data, "WAVE", 4) == 0 );

	assert( read(fd, data, 4) == 4 );
	assert( memcmp( data, "fmt ", 4) == 0 );

	uint32_t subchunk_size;
	assert( read(fd, &subchunk_size, 4) == 4 );
	assert( subchunk_size == 16 );

	uint16_t audio_format;
	assert( read(fd, &audio_format, 2) == 2 );
	assert( audio_format == 1 );

	uint16_t num_chan;
	assert( read(fd, &num_chan, 2) == 2 );

	uint32_t sample_rate;
	assert( read(fd, &sample_rate, 4) == 4 );

	uint32_t byte_rate;
	assert( read(fd, &byte_rate, 4) == 4 );

	uint16_t block_align;
	assert( read(fd, &block_align, 2) == 2 );

	uint16_t bits_per_sample;
	assert( read(fd, &bits_per_sample, 2) == 2 );

	// skip to data
	for(;;) {
		assert( read(fd, data, 4) == 4 );
		if( memcmp( data, "data", 4) == 0 ) {
			break;
		}
	}

	uint32_t data_size; //  NumSamples * NumChannels * BitsPerSample/8
	assert( read(fd, &data_size, 4) == 4 );
	//printf("data_size: %d\n", data_size);
	//printf("chan: %d\n", num_chan);
	//printf("sample_rate: %d\n", sample_rate);
	//printf("bits_per_sample: %d\n", bits_per_sample);
	//assert( data_size == num_chan * sample_rate * bits_per_sample/8 );

	while(data_size > 0 ) {
		int n = num_chan * bits_per_sample/8;
		int m = num_chan * n;
		if( read( fd, data, m ) != m ) {
			break;
		}
		data_size -= m;

		// assuming 24bit to 16bit; we just simply discard the lowest byte of the 24bit sample
		// and we only keep the first channel of data
		memcpy( buf, data, n );
		buf_len -= n;
		buf += n;

		num_bytes += n;
	}

	//uint32_t data_size;
	//assert( read(fd, &data_size, 4) == 4 );

	close(fd);
	return num_bytes;
}

