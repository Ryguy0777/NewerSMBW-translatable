#ifndef NEWER_H
#define NEWER_H 

extern int lastLevelIDs[];

void WriteAsciiToTextBox(nw4r::lyt::TextBox *tb, const char *source);
void WriteUnicodeToTextBox(nw4r::lyt::TextBox *tb, const wchar_t *source); //Reaz0n23


int getUnspentStarCoinCount();
int getStarCoinCount();
void getNewerLevelNumberString(int world, int level, wchar_t *dest);


#endif /* NEWER_H */
