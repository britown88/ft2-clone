#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _WIN32
#include <wchar.h>
#endif

#ifdef _WIN32

// Windows
typedef wchar_t UNICHAR;
#define UNICHAR_STRCPY(a, b) wcscpy(a, b)
#define UNICHAR_STRNCPY(a, b, c) wcsncpy(a, b, c)
#define UNICHAR_STRCMP(a, b) wcscmp(a, b)
#define UNICHAR_STRNCMP(a, b, c) wcsncmp(a, b, c)
#define UNICHAR_STRCAT(a, b) wcscat(a, b)
#define UNICHAR_STRDUP(a) _wcsdup(a)
#define UNICHAR_FOPEN(a, b) _wfopen(a, L ## b)
#define UNICHAR_CHDIR(a) _wchdir(a)
#define UNICHAR_GETCWD(a, b) _wgetcwd(a, b)
#define UNICHAR_RENAME(a, b) _wrename(a, b)
#define UNICHAR_REMOVE(a) _wremove(a)
#define UNICHAR_STRLEN(a) wcslen(a)
#else

// other OSes
typedef char UNICHAR;
#define UNICHAR_STRCPY(a, b) strcpy(a, b)
#define UNICHAR_STRNCPY(a, b, c) strncpy(a, b, c)
#define UNICHAR_STRCMP(a, b) strcmp(a, b)
#define UNICHAR_STRNCMP(a, b, c) strncmp(a, b, c)
#define UNICHAR_STRCAT(a, b) strcat(a, b)
#define UNICHAR_STRDUP(a) strdup(a)
#define UNICHAR_FOPEN(a, b) fopen(a, b)
#define UNICHAR_CHDIR(a) chdir(a)
#define UNICHAR_GETCWD(a, b) getcwd(a, b)
#define UNICHAR_RENAME(a, b) rename(a, b)
#define UNICHAR_REMOVE(a) remove(a)
#define UNICHAR_STRLEN(a) strlen(a)
#endif

#include <stdbool.h>
#include <stdint.h>

#define MAX_ORDERS 256
#define MAX_INST 128
#define MAX_PATTERNS 256

typedef struct pattNote_t // must be packed!
{
	uint8_t note, instr, vol, efx, efxData;
} note_t;

typedef struct sample_t
{
	char name[22 + 1];
	bool isFixed;
	int8_t finetune, relativeNote, * dataPtr, * origDataPtr;
	uint8_t volume, flags, panning;
	int32_t length, loopStart, loopLength;

	// fix for resampling interpolation taps
	int8_t leftEdgeTapSamples8[32];
	int16_t leftEdgeTapSamples16[32];
	int16_t fixedSmp[32];
	int32_t fixedPos;
} sample_t;

typedef struct instr_t
{
	bool midiOn, mute;
	uint8_t midiChannel, note2SampleLUT[96];
	uint8_t volEnvLength, panEnvLength;
	uint8_t volEnvSustain, volEnvLoopStart, volEnvLoopEnd;
	uint8_t panEnvSustain, panEnvLoopStart, panEnvLoopEnd;
	uint8_t volEnvFlags, panEnvFlags;
	uint8_t autoVibType, autoVibSweep, autoVibDepth, autoVibRate;
	uint16_t fadeout;
	int16_t volEnvPoints[12][2], panEnvPoints[12][2], midiProgram, midiBend;
	int16_t numSamples; // used by loader only
	sample_t smp[16];
} instr_t;

typedef struct song_t
{
	bool pBreakFlag, posJumpFlag, isModified;
	char name[20 + 1], instrName[1 + MAX_INST][22 + 1];
	uint8_t curReplayerTick, curReplayerRow, curReplayerSongPos, curReplayerPattNum; // used for audio/video sync queue
	uint8_t pattDelTime, pattDelTime2, pBreakPos, orders[MAX_ORDERS];
	int16_t songPos, pattNum, row, currNumRows;
	uint16_t songLength, songLoopStart, BPM, speed, initialSpeed, globalVolume, tick;
	int32_t numChannels;

	uint32_t playbackSeconds;
	uint64_t playbackSecondsFrac;
} song_t;

extern int16_t patternNumRows[MAX_PATTERNS];
extern song_t song;
extern instr_t* instr[128 + 4];
extern note_t* pattern[MAX_PATTERNS];

int libft2_loadModule(UNICHAR* path);
void libft2_unloadModule();

// EXTREMELY LOW-LEVEL ACCESS DO NOT TOUCH

// int16_t[MAX_PATTERNS]
int16_t** libft2_getPatternNumRows();

song_t* libft2_getSong();

// instr_t*[128+4]
instr_t*** libft2_getInstruments();

// note_t *pattern[MAX_PATTERNS];
note_t*** libft2_getPatterns();

#ifdef __cplusplus
}
#endif
