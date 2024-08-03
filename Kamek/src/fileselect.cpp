#include <game.h>
#include "levelinfo.h"

class dDateFile_c : public dBase_c {
    public:
        u32 unk;
        m2d::EmbedLayout_c layout;

        mEf::es2 efClass0_0, efClass0_1, efClass0_2, efClass0_3, efClass0_4;
        mEf::es2 efClass1_0, efClass1_1, efClass1_2, efClass1_3, efClass1_4;

        dStateWrapper_c<dDateFile_c> stateThing;
        u32 nextActionToExecute;

        nw4r::lyt::Pane 
            *rootPane, *N_dateFile_00, *N_dateFile_01,
            *N_NewFile_00, *N_save, *W_select_00, *W_shadow_00;
        
        nw4r::lyt::TextBox
            *T_worldNumber_00, *T_worldNumber_01, *T_x_00, *T_zanki_00,
            *T_fileNumber_00, *T_fileNumber_01, *T_NewdateFile_00, *T_NewdateFileS;
        
        nw4r::lyt::Picture 
            *P_Star_00, *P_Star_01, *P_Star_02, *P_Star_03, *P_Star_04, 
            *P_Star_05, *P_Star_06, *P_Star_07, *P_Star_08, *P_Star_09;
        
        f32 inexplicably_float;
        u32 fileNum_maybe;
        bool hasLoaded;
        u8 doActuallyDrawMe, isNew, doesHaveBalloon_maybe, mustLoadMyInfo;
        u8 isWaitingForAnim, probablyActivatesHit, doesDrawStarEffects;
        u8 inited_to_1_E50, unk2[3];
        u32 _E54;
        
        //new
        void setFileTitle();
        void setFileName(SaveBlock *save);
};

void dDateFile_c::setFileTitle() {
    //TODO: Adjust this layout (and others?) to fit longer translations
    WriteBMGToTextBox(T_fileNumber_01, GetBMG(), BMG_CAT_NEWER, (0x47 + fileNum_maybe), 0);
}

#define BMG_CAT_WORLD_WARP 0xde

void dDateFile_c::setFileName(SaveBlock *save) {
    dScript::Res_c *msgRes = GetBMG();
    const wchar_t *worldName;

    nw4r::lyt::Picture *Picture_00 = layout.findPictureByName("Picture_00");
    nw4r::lyt::TextBox *ExitCount = layout.findTextBoxByName("ExitCount");
    nw4r::lyt::TextBox *StarCoinCount = layout.findTextBoxByName("StarCoinCount");
    StarCoinCount->size.x = 100.0;

    //handle name and colors
    if (save->worldBMGCat < BMG_CAT_KP_NAME) {
        worldName = msgRes->findStringForMessageID(BMG_CAT_NEWER, 0x4B);
    } else if (save->worldBMGCat == BMG_CAT_WORLD_WARP) {
        worldName = msgRes->findStringForMessageID(BMG_CAT_NEWER, 0x4A);
    } else {
        worldName = msgRes->findStringForMessageID(save->worldBMGCat, save->worldBMGID);
    }

    T_worldNumber_01->SetString(worldName);
    T_worldNumber_01->colour1 = save->fsTextColours[0];
    T_worldNumber_01->colour2 = save->fsTextColours[1];

    Picture_00->colours[0] = save->fsHintColours[0];
    Picture_00->colours[1] = save->fsHintColours[0];
    Picture_00->colours[2] = save->fsHintColours[1];
    Picture_00->colours[3] = save->fsHintColours[1];

    int exits = 0;
    int starCoins = 0;
    for (int i = 0; i < dLevelInfo_c::s_info.sectionCount(); i++) {
		dLevelInfo_c::section_s *section = dLevelInfo_c::s_info.getSectionByIndex(i);

		for (int j = 0; j < section->levelCount; j++) {
			dLevelInfo_c::entry_s *l = &section->levels[j];
			if (l->flags & 2) {
				//OSReport("Checking %d-%d...\n", l->worldSlot+1, l->levelSlot+1);
				u32 cond = save->GetLevelCondition(l->worldSlot, l->levelSlot);
				if ((l->flags & 0x10) && (cond & COND_NORMAL))
					exits++;
				if ((l->flags & 0x20) && (cond & COND_SECRET))
					exits++;
				if (cond & COND_COIN1)
					starCoins++;
				if (cond & COND_COIN2)
					starCoins++;
				if (cond & COND_COIN3)
					starCoins++;
			}
		}
	}
    OSReport("Done, got %d coins and %d exits\n", starCoins, exits);
    //coinCount = coinCount - save->spentStarCoins;

    WriteNumberToTextBox(&exits, ExitCount, 1);
    WriteNumberToTextBox(&starCoins, StarCoinCount, 0);
}  