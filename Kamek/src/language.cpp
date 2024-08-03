#include <language.h>

int GetGameLanguage() {
	// See exceptionHandler.cpp
	int num;
	NSMBWVer version = getNsmbwVer();
	switch(version) {
		case pal:
		case pal2:
			return GetGameLanguageOriginal(4);
		case ntsc:
		case ntsc2:
			num = GetGameLanguageOriginal(4);
			if (num == 4) return 10;
			else return num+8;
		case jpn:
		case jpn2:
			return 0;
		case kor:
			return 8;
		case twn:
			return 7;
		default:
			return -1;
	}
}

char* GetGameLanguageFolder() {
	switch(GetGameLanguage()) {
		case 0:
			return "/JP/";
		case 1:
			return "/EU/EngEU/";
		case 2:
			return "/EU/GerEU/";
		case 3:
			return "/EU/FraEU/";
		case 4:
			return "/EU/SpaEU/";
		case 5:
			return "/EU/ItaEU/";
		case 6:
			return "/EU/NedEU/";
		case 7:
			return "/TW/";
		case 8:
			return "/KR/";
		case 9:
			return "/US/EngUS/";
		case 10:
			return "/US/SpaUS/";
		case 11:
			return "/US/FraUS/";
	}
	return "/EU/EngEU/";
}