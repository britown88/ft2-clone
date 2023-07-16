#pragma once

//BSD 3 - clause license :
//
//Copyright(c) 2016 - 2023, Olav Sørensen
//All rights reserved.
//
//Redistribution and use in source and binary forms, with or without
//modification, are permitted provided that the following conditions are met :
//*Redistributions of source code must retain the above copyright
//notice, this list of conditions and the following disclaimer.
//* Redistributions in binary form must reproduce the above copyright
//notice, this list of conditions and the following disclaimer in the
//documentation and /or other materials provided with the distribution.
//* Neither the name of the <organization> nor the
//names of its contributors may be used to endorse or promote products
//derived from this software without specific prior written permission.
//
//THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
//ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
//WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
//DISCLAIMED.IN NO EVENT SHALL OLAV SØRENSEN BE LIABLE FOR ANY
//DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
//(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
//LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
//ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
//(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
//SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

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

#define MIN_BPM 32
#define MAX_BPM 255
#define MAX_SPEED 31
#define MAX_CHANNELS 32
#define TRACK_WIDTH (5 * MAX_CHANNELS)
#define C4_FREQ 8363
#define NOTE_C4 (4*12)
#define NOTE_OFF 97
#define MAX_NOTES (10*12*16+16)
#define MAX_PATTERNS 256
#define MAX_PATT_LEN 256
#define MAX_INST 128
#define MAX_SMP_PER_INST 16
#define MAX_ORDERS 256

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

typedef enum libft2_NoteCommands {
	libft2_Note_NONE = 0,
	libft2_Note_MIN = 1,  // C0
	libft2_Note_MAX = 96, // B7
	libft2_Note_OFF = 97  // ===
} libft2_NoteCommands;


typedef enum libft2_VolumeCommands {
	libft2_Volume_NONE = 0,
	libft2_Volume_SET = 1, // 16-80
	libft2_Volume_SLIDE_UP = 7,		//0x70	+	spd
	libft2_Volume_SLIDE_DWN = 6,		//0x60	-	spd
	libft2_Volume_FINESLIDE_UP = 9,	//0x90	🔼	spd
	libft2_Volume_FINESLIDE_DN = 8,  //0x80	🔽	spd
	libft2_Volume_VIBRATO_SPD = 10,	//0xA0	S	spd
	libft2_Volume_VIBRATO = 11,		//0xB0	V	spd
	libft2_Volume_PANNING_POS = 12,	//0xC0	P	pos
	libft2_Volume_PANSLIDE_RGHT = 13,//0xD0	▶	spd	
	libft2_Volume_PANSLIDE_LEFT = 14,//0xE0	◀	spd
	libft2_Volume_TONE_PORTAMENTO = 15,//0xF0 M	spd
} libft2_VolumeCommands;

typedef enum libft2_EffectCommands {
	libft2_EFX_ARPEGGIO = 0,	// 0: semi1 semi2
	libft2_EFX_PORTA_UP = 1,	// 1: speed
	libft2_EFX_PORTA_DN = 2,   // 2: speed
	libft2_EFX_PORTA_NOTE = 3, // 3: speed
	libft2_EFX_VIBRATO = 4,		// 4: rate depth
	libft2_EFX_PORTA_VSLIDE = 5,//5: speed
	libft2_EFX_VIBR_VSLIDE = 6,// 6: speed
	libft2_EFX_TREMOLO = 7,		// 7: rate depth
	libft2_EFX_PANNING_POS = 8,// 8: pos
	libft2_EFX_SAMP_OFF = 9,	// 9: offset
	libft2_EFX_VSLIDE = 10,		// A: upSpd downSpd
	libft2_EFX_JUMP_POS = 11,	// B: pos
	libft2_EFX_VSET = 12,		// C: vol
	libft2_EFX_PAT_BRK = 13,	// D: pos
	libft2_EFX_E_CMD = 14,		// use libft2_EffectECommands
	libft2_EFX_SPD_TEMPO = 15,	// F: spd/tempo 1-31 sets speed, 32-255 sets tempo...weird!
	libft2_EFX_GLOBAL_VSET = 16,//G: vol
	libft2_EFX_GLOBAL_VSLIDE=17,//H: upSpd downSpd
	libft2_EFX_SET_ENV_POS = 21,//L: pos
	libft2_EFX_PANSLIDE = 25,	// P: rightSpd leftSpd
	libft2_EFX_MULTI_RETRIG=27,// R: interval volChang
	libft2_EFX_TREMOR = 29,		// T: onTime offTime
	libft2_EFX_XFPORTA = 33,	// X (UP: 1 spd) (DN: 2 spd)

} libft2_EffectCommands;

typedef enum libft2_EffectECommands {
	libft2_ECMD_FILTER = 0,		// E:  01
	libft2_ECMD_FPORTA_UP = 1, // E1: speed
	libft2_ECMD_FPORTA_DN = 2, // E2: speed
	libft2_ECMD_GLISSANDO = 3, // E3: status
	libft2_ECMD_VIBR_CTRL = 4, // E4: type
	libft2_ECMD_FINE_TUNE = 5, // E5: tune
	libft2_ECMD_PAT_LOOP = 6,	// E6: count
	libft2_ECMD_TREM_CTRL = 7,	// E7: type
	libft2_ECMD_RETRIG_NOTE = 9,//E9: interval
	libft2_ECMD_FVSLIDE_UP = 10,//EA: speed
	libft2_ECMD_FVSLIDE_DN = 11,//EB: speed
	libft2_ECMD_NOTE_CUT = 12,  //EC: tick
	libft2_ECMD_NOTE_DELAY = 13,//ED: ticks
	libft2_ECMD_PAT_DELAY = 14, //EE: notes
} libft2_EffectECommands;

typedef struct libft2_notedata_t {
	libft2_NoteCommands note;
	libft2_VolumeCommands volCmd;
	uint8_t vol;
	uint8_t inst;

	libft2_EffectCommands efx;

	// these are split up and shifted correctly for the efx
	// if efx is E, use libft2_EffectECommands to read efx_x, param in efx_y
	uint8_t efx_x, efx_y;

}libft2_notedata_t;

void libft2_notedataFromNote(note_t* src, libft2_notedata_t* dest);


#ifdef __cplusplus
}
#endif
