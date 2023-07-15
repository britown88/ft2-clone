#include "../libft2/libft2.h"

int main(int argc, char* argv[]) {
   //if (!libft2_startup()) {
      if (!libft2_loadModule(L"D:/Downloads/Arpeggio.xm")) {
         //int i = 'fuck';
         //i++;
         //song_t* song = libft2_getSong();
         //int16_t** numRows = libft2_getPatternNumRows();
         //instr_t*** instruments = libft2_getInstruments();
         //note_t*** patterns = libft2_getPatterns();
         // 
         song;
         instr;
         pattern;
         patternNumRows;
         
         //patterns = 0;
         libft2_unloadModule();
      }

      if (!libft2_loadModule(L"D:/Downloads/Arpeggio.xm")) {
         int i = 'fuck';
         i++;
         song_t* song = libft2_getSong();
         int16_t** numRows = libft2_getPatternNumRows();
         instr_t*** instruments = libft2_getInstruments();
         note_t*** patterns = libft2_getPatterns();
         //patterns = 0;
         libft2_unloadModule();
      }
      //libft2_shutdown();
   //}
}