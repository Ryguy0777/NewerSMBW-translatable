#include "koopatlas/starcoin.h"
#include <game.h>

dWMStarCoin_c *dWMStarCoin_c::instance = 0;

dWMStarCoin_c *dWMStarCoin_c::build() {
	void *buffer = AllocFromGameHeap1(sizeof(dWMStarCoin_c));
	dWMStarCoin_c *c = new(buffer) dWMStarCoin_c;

	instance = c;
	return c;
}

dWMStarCoin_c::dWMStarCoin_c() : state(this) {
	layoutLoaded = false;
	visible = false;
	state.setState(&StateID_Hidden);
}

CREATE_STATE(dWMStarCoin_c, Hidden);
CREATE_STATE(dWMStarCoin_c, ShowWait);
CREATE_STATE(dWMStarCoin_c, ShowSectionWait);
CREATE_STATE(dWMStarCoin_c, Wait);
CREATE_STATE(dWMStarCoin_c, HideSectionWait);
CREATE_STATE(dWMStarCoin_c, HideWait);

int dWMStarCoin_c::onCreate() {

	if (!layoutLoaded) {
		bool gotFile = layout.loadArc("StarCoins.arc", false);
		if (!gotFile)
			return false;

		bool output = layout.build("StarCoins.brlyt");

		layout.layout.rootPane->trans.x = -112.0f;
		if (IsWideScreen()) {
			layout.layout.rootPane->scale.x = 0.735f;
		} else {
			layout.clippingEnabled = true;
			layout.clipX = 0;
			layout.clipY = 52;
			layout.clipWidth = 640;
			layout.clipHeight = 352;
			layout.layout.rootPane->scale.x = 0.731f;
			layout.layout.rootPane->scale.y = 0.7711f;
		}

		static const char *brlanNames[] = {
			"StarCoins_Show.brlan",
			"StarCoins_ShowSection.brlan",
			"StarCoins_HideSection.brlan",
			"StarCoins_ShowArrow.brlan",
			"StarCoins_HideArrow.brlan",
		};
		static const char *groupNames[] = {
			"base", "section", "section", "leftArrow", "leftArrow", "rightArrow", "rightArrow"
		};

		layout.loadAnimations(brlanNames, 5);
		layout.loadGroups(groupNames, (int[7]){0, 1, 2, 3, 4, 3, 4}, 7);
		layout.disableAllAnimations();

		layout.drawOrder = 1;

		for (int col = 0; col < COLUMN_COUNT; col++) {
			for (int shine = 0; shine < SHINE_COUNT; shine++) {
				char name[8];
				sprintf(name, "Shine%d%d", col, shine);
				Shine[col][shine] = layout.findPictureByName(name);
			}

			for (int row = 0; row < ROW_COUNT; row++) {
				char lname[12];
				sprintf(lname, "LevelName%d%d", col, row);
				LevelName[col][row] = layout.findTextBoxByName(lname);

				char coname[16], cname[8];
				for (int i = 0; i < 3; i++) {
					sprintf(coname, "CoinOutline%d%d%d", col, row, i);
					CoinOutline[col][row][i] = layout.findPictureByName(coname);

					sprintf(cname, "Coin%d%d%d", col, row, i);
					Coin[col][row][i] = layout.findPictureByName(cname);
				}
			}
		}

		static const char *tbNames[] = {
			"LeftTitle", "RightTitle", "TotalCoinCount", "UnspentCoinCount",
			"EarnedCoinCount", "EarnedCoinMax", "BtnBackText", "TotalCoinsTitle",
			"CollectedTitle", "UnspentTitle", "BtnWorldSelText", 
		};
		layout.getTextBoxes(tbNames, &LeftTitle, 11);

		static const char *picNames[] = {
			"DPadLeft", "DPadRight",
		};
		layout.getPictures(picNames, &DPadLeft, 2);

		DPadLeft->SetVisible(false);
		DPadRight->SetVisible(false);

		layoutLoaded = true;
	}

	this->msgRes = GetBMG();

	WriteBMGToTextBox(TotalCoinsTitle, msgRes, BMG_CAT_NEWER, 0x43, 0);
	WriteBMGToTextBox(CollectedTitle, msgRes, BMG_CAT_NEWER, 0x44, 0);
	WriteBMGToTextBox(UnspentTitle, msgRes, BMG_CAT_NEWER, 0x45, 0);
	WriteBMGToTextBox(BtnWorldSelText, msgRes, BMG_CAT_NEWER, 0x46, 0);

	return true;
}


int dWMStarCoin_c::onDelete() {
	return layout.free();
}


void dWMStarCoin_c::show() {
	if (state.getCurrentState() == &StateID_Hidden)
		state.setState(&StateID_ShowWait);
}


int dWMStarCoin_c::onExecute() {
	state.execute();

	if (visible) {
		layout.execAnimations();
		layout.update();
	}

	return true;
}

int dWMStarCoin_c::onDraw() {
	if (visible)
		layout.scheduleForDrawing();

	return true;
}


void dWMStarCoin_c::showLeftArrow() {
	if (!isLeftArrowVisible) {
		isLeftArrowVisible = true;
		layout.enableNonLoopAnim(SHOW_LEFT_ARROW);
		DPadLeft->SetVisible(true);
	}
}

void dWMStarCoin_c::showRightArrow() {
	if (!isRightArrowVisible) {
		isRightArrowVisible = true;
		layout.enableNonLoopAnim(SHOW_RIGHT_ARROW);
		DPadRight->SetVisible(true);
	}
}

void dWMStarCoin_c::hideLeftArrow() {
	if (isLeftArrowVisible) {
		isLeftArrowVisible = false;
		layout.enableNonLoopAnim(HIDE_LEFT_ARROW);
		DPadLeft->SetVisible(false);
	}
}

void dWMStarCoin_c::hideRightArrow() {
	if (isRightArrowVisible) {
		isRightArrowVisible = false;
		layout.enableNonLoopAnim(HIDE_RIGHT_ARROW);
		DPadRight->SetVisible(false);
	}
}

void dWMStarCoin_c::setLeftArrowVisible(bool value) {
	if (value)
		showLeftArrow();
	else
		hideLeftArrow();
}

void dWMStarCoin_c::setRightArrowVisible(bool value) {
	if (value)
		showRightArrow();
	else
		hideRightArrow();
}


bool dWMStarCoin_c::canScrollLeft() const {
	return (currentSectionIndex > 0);
}
bool dWMStarCoin_c::canScrollRight() const {
	return (currentSectionIndex < (availableSectionCount - 1));
}

void dWMStarCoin_c::loadInfo() {
	WriteBMGToTextBox(BtnBackText, GetBMG(), 3, 1, 0);

	int unspentCoins = getUnspentStarCoinCount();
	int coins = getStarCoinCount();

	WriteNumberToTextBox(&unspentCoins, UnspentCoinCount, false);
	WriteNumberToTextBox(&coins, TotalCoinCount, false);

	currentSection = -1;
	currentSectionIndex = -1;
	availableSectionCount = 0;

	SaveBlock *save = GetSaveFile()->GetBlock(-1);
	int wantedSection = save->newerWorldID;

	// figure out which sections should be available
	for (int i = 0; i < dLevelInfo_c::s_info.sectionCount(); i++) {
		dLevelInfo_c::section_s *section = dLevelInfo_c::s_info.getSectionByIndex(i);

		bool haveLevels = false;
		for (int j = 0; j < section->levelCount; j++) {
			dLevelInfo_c::entry_s *l = &section->levels[j];
			if (l->flags & 2) {
				if (save->GetLevelCondition(l->worldSlot, l->levelSlot) & COND_UNLOCKED) {
					haveLevels = true;
					break;
				}
			}
		}

		if (haveLevels) {
			if (i == wantedSection) {
				currentSection = wantedSection;
				currentSectionIndex = availableSectionCount;
			}
			sectionIndices[availableSectionCount++] = i;
		}
	}

	// if we didn't find the wanted one, use the first one available
	if (currentSectionIndex == -1) {
		currentSectionIndex = 0;
		currentSection = sectionIndices[0];
	}
}

void dWMStarCoin_c::loadSectionInfo() {
	dLevelInfo_c::entry_s *visibleLevels[COLUMN_COUNT][ROW_COUNT];

	// reset everything... everything
	for (int i = 0; i < COLUMN_COUNT; i++) {
		for (int j = 0; j < SHINE_COUNT; j++)
			Shine[i][j]->SetVisible(false);

		for (int j = 0; j < ROW_COUNT; j++) {
			visibleLevels[i][j] = 0;

			LevelName[i][j]->SetVisible(false);

			for (int k = 0; k < 3; k++) {
				CoinOutline[i][j][k]->SetVisible(false);
				Coin[i][j][k]->SetVisible(false);
			}
		}
	}

	// get everything we'll need
	SaveBlock *save = GetSaveFile()->GetBlock(-1);
	dLevelInfo_c *linfo = &dLevelInfo_c::s_info;

	dLevelInfo_c::entry_s *names[COLUMN_COUNT];
	for (int i = 0; i < COLUMN_COUNT; i++)
		names[i] = linfo->searchByDisplayNum(currentSection, 100+i);

	bool useSubworlds = (COLUMN_COUNT > 1) && names[1];

	int currentPosition[COLUMN_COUNT];
	int currentColumn = 0; // only incremented in single-subworld mode

	for (int i = 0; i < COLUMN_COUNT; i++)
		currentPosition[i] = 0;

	dLevelInfo_c::section_s *section = linfo->getSectionByIndex(currentSection);

	int earnedCoins = 0, earnableCoins = 0;
	// earnedCoins is calculated later

	for (int i = 0; i < section->levelCount; i++) {
		dLevelInfo_c::entry_s *level = &section->levels[i];

		// only pay attention to real levels
		if (!(level->flags & 2))
			continue;

		earnableCoins += 3;

		// is this level unlocked?
		u32 conds = save->GetLevelCondition(level->worldSlot, level->levelSlot);

		if (!(conds & COND_UNLOCKED))
			continue;

		// well, let's give it a slot
		if (useSubworlds) {
			currentColumn = (level->flags & 0x400) ? 1 : 0;
		} else {
			if (currentPosition[currentColumn] >= ROW_COUNT)
				currentColumn++;
		}

		visibleLevels[currentColumn][currentPosition[currentColumn]++] = level;
	}

	// if the first column is empty, then move the second one over
	if (currentPosition[0] == 0 && useSubworlds) {
		for (int i = 0; i < currentPosition[1]; i++) {
			visibleLevels[0][i] = visibleLevels[1][i];
			visibleLevels[1][i] = 0;
		}

		names[0] = names[1];
		names[1] = 0;
	}

	// if the second column is empty, remove its name
	if (currentPosition[1] == 0 && useSubworlds)
		names[1] = 0;

	// work out the names
	WriteUnicodeToTextBox(LeftTitle, linfo->getNameForLevel(names[0]));
	if (names[1])
		WriteUnicodeToTextBox(RightTitle, linfo->getNameForLevel(names[1]));
	RightTitle->SetVisible(names[1] != 0);

	// load all level info
	for (int col = 0; col < COLUMN_COUNT; col++) {
		for (int row = 0; row < ROW_COUNT; row++) {
			dLevelInfo_c::entry_s *level = visibleLevels[col][row];
			if (!level)
				continue;

			u32 conds = save->GetLevelCondition(level->worldSlot, level->levelSlot);

			if (!(row & 1)) {
				int shineID = row / 2;
				if (shineID < SHINE_COUNT)
					Shine[col][shineID]->SetVisible(true);
			}

			for (int coin = 0; coin < 3; coin++) {
				CoinOutline[col][row][coin]->SetVisible(true);

				if (conds & (COND_COIN1 << coin)) {
					Coin[col][row][coin]->SetVisible(true);
					earnedCoins++;
				}
			}

			LevelName[col][row]->SetVisible(true);
			WriteUnicodeToTextBox(LevelName[col][row], linfo->getNameForLevel(level));
		}
	}

	// set up coin things
	WriteNumberToTextBox(&earnedCoins, EarnedCoinCount, false);
	WriteNumberToTextBox(&earnableCoins, EarnedCoinMax, false);
}


void dWMStarCoin_c::beginState_Hidden() { }
void dWMStarCoin_c::executeState_Hidden() { }
void dWMStarCoin_c::endState_Hidden() { }

static const int secretCode[] = {
	WPAD_UP,WPAD_UP,WPAD_DOWN,WPAD_DOWN,
	WPAD_LEFT,WPAD_RIGHT,WPAD_LEFT,WPAD_RIGHT,
	WPAD_ONE,WPAD_TWO,0
};
static const int secretCodeButtons = WPAD_UP|WPAD_DOWN|WPAD_LEFT|WPAD_RIGHT|WPAD_ONE|WPAD_TWO;
static int secretCodeIndex = 0;
static int minusCount = 0;
extern bool enableHardMode;
extern bool enableDebugMode;
extern u8 isReplayEnabled;

void dWMStarCoin_c::beginState_ShowWait() {
	visible = true;
	loadInfo();
	layout.enableNonLoopAnim(SHOW_ALL);
	layout.resetAnim(SHOW_SECTION);
	layout.resetAnim(SHOW_LEFT_ARROW);
	layout.resetAnim(SHOW_RIGHT_ARROW);

	secretCodeIndex = 0;
	minusCount = 0;
}
void dWMStarCoin_c::executeState_ShowWait() {
	if (!layout.isAnimOn(SHOW_ALL))
		state.setState(&StateID_ShowSectionWait);
}
void dWMStarCoin_c::endState_ShowWait() { }

void dWMStarCoin_c::beginState_ShowSectionWait() {
	loadSectionInfo();
	layout.enableNonLoopAnim(SHOW_SECTION);

	if (canScrollLeft())
		showLeftArrow();
	if (canScrollRight())
		showRightArrow();
}
void dWMStarCoin_c::executeState_ShowSectionWait() {
	if (!layout.isAnimOn(SHOW_SECTION))
		state.setState(&StateID_Wait);
}
void dWMStarCoin_c::endState_ShowSectionWait() { }

void dWMStarCoin_c::showSecretMessage(int title, int *body, int lineCount, int *body2, int lineCount2) {
	LeftTitle->SetVisible(true);
	WriteBMGToTextBox(LeftTitle, msgRes, BMG_CAT_NEWER, title, 0);
	RightTitle->SetVisible(false);

	for (int c = 0; c < COLUMN_COUNT; c++) {
		for (int i = 0; i < SHINE_COUNT; i++)
			Shine[c][i]->SetVisible(false);
		for (int r = 0; r < ROW_COUNT; r++) {
			LevelName[c][r]->SetVisible(false);
			for (int i = 0; i < 3; i++) {
				CoinOutline[c][r][i]->SetVisible(false);
				Coin[c][r][i]->SetVisible(false);
			}
		}
	}

	for (int i = 0; i < lineCount; i++) {
		LevelName[0][i]->SetVisible(true);
		WriteBMGToTextBox(LevelName[0][i], msgRes, BMG_CAT_NEWER, body[i], 0);
	}

	if (body2) {
		for (int i = 0; i < lineCount2; i++) {
			LevelName[1][i]->SetVisible(true);
			WriteBMGToTextBox(LevelName[1][i], msgRes, BMG_CAT_NEWER, body2[i], 0);
		}
	}
}

void dWMStarCoin_c::beginState_Wait() { }
void dWMStarCoin_c::executeState_Wait() {
	int nowPressed = Remocon_GetPressed(GetActiveRemocon());

	// A and Plus (formerly B as well, prior to 1.30, but nunchuk scheme binds B to cancel)
	if ((GetActiveRemocon()->heldButtons == 0x810) && (nowPressed & 0x810)) {

		const int lineCountOn = 9, lineCountOff = 2;
		static int bmgsOn[lineCountOn] = {
			0xC, 0xD, 0xE, 0xF, 0x10,
			0xD, 0x11, 0x12, 0x13,
		};
		static int bmgsOff[lineCountOff] = {
			0x15, 0x16,
		};

		if (!enableHardMode) {
			enableHardMode = true;
			OSReport("Hard Mode enabled!\n");
			MapSoundPlayer(SoundRelatedClass, SE_VOC_MA_CS_COURSE_IN_HARD, 1);
			showSecretMessage(0xB, (int *)bmgsOn, lineCountOn);
		} else {
			enableHardMode = false;
			OSReport("Hard Mode disabled!\n");
			showSecretMessage(0x14, (int *)bmgsOff, lineCountOff);
		}
		return;
	}

	if (nowPressed & secretCodeButtons) {
		int nextKey = secretCode[secretCodeIndex];
		if (nowPressed & nextKey) {
			secretCodeIndex++;
			if (secretCode[secretCodeIndex] == 0) {
				secretCodeIndex = 0;
				MapSoundPlayer(SoundRelatedClass, SE_VOC_MA_THANK_YOU, 1);
				//enableDebugMode = !enableDebugMode;
				//OSReport("Debug mode toggled!\n");
				const int lineCountOn = 9, lineCountOff = 2;
				static int bmgsOn[lineCountOn] = {
					0x18, 0x19, 0x1A, 0x1B, 0x1C,
					0x1D, 0x1E, 0x1F, 0x20,
				};
				static int bmgsOff[lineCountOff] = {
					0x21, 0x22,
				};

				if (isReplayEnabled != 100) {
					isReplayEnabled = 100;
					OSReport("Replay Recording enabled!\n");
					showSecretMessage(0x17, (int *)bmgsOn, lineCountOn);
				} else {
					isReplayEnabled = 0;
					OSReport("Replay Recording disabled!\n");
					showSecretMessage(0x17, (int *)bmgsOff, lineCountOff);
				}
			}
			return;
		} else {
			secretCodeIndex = 0;
		}
	}

	if (nowPressed & WPAD_MINUS) {
		minusCount++;
		if (minusCount >= 16) {
			minusCount = 0;

			enableDebugMode = !enableDebugMode;

			if (enableDebugMode) {
				MapSoundPlayer(SoundRelatedClass, SE_VOC_MA_GET_PRIZE, 1);

				const int msgCount = 9;
				static int msgBmgs[msgCount] = {
					0x24, 0x25, 0x26, 0x27, 0x28,
					0x29, 0x2A, 0x2B, 0x2C,
				};
				const int msgCount2 = 9;
				static int msgBmgs2[msgCount2] = {
					0x2D, 0x2F, 0x30, 0x31, 0x32, //forgot entry 0xD02E, lol
					0x33, 0x34, 0xD, 0x35,
				};
				showSecretMessage(0x23, (int *)msgBmgs, msgCount, (int *)msgBmgs2, msgCount2);
			} else {
				const int msgCount = 6;
				static int msgBmgs[msgCount] = {
					0x36, 0x37, 0xD, 0x38, 0x39,
					0x3A,
				};
				static int hiddenBmg[] = {
					0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
					0x40, 0x41, 0x42
				};
				showSecretMessage(0x23, (int *)msgBmgs, msgCount, (int *)hiddenBmg, 0);
			}
		}
	} else if (nowPressed & WPAD_ONE) {
		MapSoundPlayer(SoundRelatedClass, SE_SYS_DIALOGUE_OUT_AUTO, 1);
		willExit = true;
		state.setState(&StateID_HideSectionWait);
	} else if ((nowPressed & WPAD_LEFT) && canScrollLeft()) {
		currentSection = sectionIndices[--currentSectionIndex];
		willExit = false;
		state.setState(&StateID_HideSectionWait);
	} else if ((nowPressed & WPAD_RIGHT) && canScrollRight()) {
		currentSection = sectionIndices[++currentSectionIndex];
		willExit = false;
		state.setState(&StateID_HideSectionWait);
	}
}
void dWMStarCoin_c::endState_Wait() { }

void dWMStarCoin_c::beginState_HideSectionWait() {
	layout.enableNonLoopAnim(HIDE_SECTION);
	if (willExit) {
		hideLeftArrow();
		hideRightArrow();
	} else {
		setLeftArrowVisible(canScrollLeft());
		setRightArrowVisible(canScrollRight());
	}
}
void dWMStarCoin_c::executeState_HideSectionWait() {
	if (!layout.isAnimOn(HIDE_SECTION)) {
		if (willExit)
			state.setState(&StateID_HideWait);
		else
			state.setState(&StateID_ShowSectionWait);
	}
}
void dWMStarCoin_c::endState_HideSectionWait() { }

void dWMStarCoin_c::beginState_HideWait() {
	layout.enableNonLoopAnim(SHOW_ALL, true);
	layout.grpHandlers[SHOW_ALL].frameCtrl.flags = 3; // NO_LOOP | REVERSE
}
void dWMStarCoin_c::executeState_HideWait() {
	if (!layout.isAnimOn(SHOW_ALL))
		state.setState(&StateID_Hidden);
}
void dWMStarCoin_c::endState_HideWait() {
	visible = false;
}

