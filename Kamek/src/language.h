#pragma once
#include <game.h>
#include "nsmbwVer.h"

extern "C" int GetGameLanguageOriginal(int language);

/*
0:  JP
1:  EngEU
2:  GerEU
3:  FraEU
4:  SpaEU
5:  ItaEU
6:  NedEU
7:  CN
8:  TW
9:  KR
10: EngUS
11: SpaUS
12: FraUS
*/

int GetGameLanguage();

char* GetGameLanguageFolder();