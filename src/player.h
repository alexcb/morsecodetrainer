#pragma once

#include <ao/ao.h>
#include <mpg123.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>

#include "playlist_manager.h"
#include "httpget.h"

#define PLAYER_ARTIST_LEN 1024
#define PLAYER_TITLE_LEN 1024

typedef void (*MetadataObserver)(bool playing, const char *playlist_name, const char *artist, const char *title, void *data);

typedef struct Player
{
	int driver;
	ao_device *dev;
	ao_sample_format format;
	int channels, encoding;
	long rate;

	mpg123_handle *mh;
	unsigned char *buffer;
	int copied;
	size_t buffer_size;
	size_t done;
	int err;

	pthread_t thread;

	struct httpdata hd;

	PlaylistManager *playlist_manager;

	int num_metadata_observers;
	MetadataObserver metadata_observers[2];
	void* metadata_observers_data[2];

	char artist[PLAYER_ARTIST_LEN];
	char title[PLAYER_TITLE_LEN];

	// when true play, when false, pause / stop
	volatile bool playing;

	// set to true when player should
	// query playlist for which file to play (used when changing tracks)
	volatile bool restart;
} Player;


int start_player( Player *player );

