.set sp,1
.set rtoc,2

.text
.extern OSReport

.extern enableNonLoopAnim__Q23m2d13EmbedLayout_cFib
.global FSShowCD, FSHideCD
FSShowCD:
	addi r3, r3, 0x74
	li r4, 0x2A
	b enableNonLoopAnim__Q23m2d13EmbedLayout_cFib

FSHideCD:
	addi r3, r3, 0x74
	li r4, 0x2B
	b enableNonLoopAnim__Q23m2d13EmbedLayout_cFib

.global FSShowCD_OpenWait
FSShowCD_OpenWait:
	addi r3, r31, 0x74
	li r4, 0x2A
	bl enableNonLoopAnim__Q23m2d13EmbedLayout_cFib
	lwz r0, 0x14(r1)
	lwz r31, 0xC(r1)
	mtlr r0
	addi r1, r1, 0x10
	blr

#.global FSHideCD_IWOSAEW
#FSHideCD_IWOSAEW:
#	addi r3, r31, 0x74
#	li r4, 0x2B
#	bl enableNonLoopAnim__Q23m2d13EmbedLayout_cFib
#	lwz r0, 0x14(r1)
#	lwz r31, 0xC(r1)
#	mtlr r0
#	addi r1, r1, 0x10
#	blr

.extern returnNiceTitle
.extern setFileTitle__11dDateFile_cFv
.global DFNiceTitle
DFNiceTitle:
	stwu sp, -0x80 (sp) #Push stack, make space for 29 registers
    stmw r3, 0x8 (sp)

    mr r3,r30
    bl setFileTitle__11dDateFile_cFv

    lmw r3, 0x8 (sp)
    addi sp, sp, 0x80 #Pop stack

    b returnNiceTitle

.extern returnNiceWorld
.extern setFileName__11dDateFile_cFP9SaveBlock
.global DFNiceWorldName
DFNiceWorldName:
	stwu sp, -0x80 (sp) #Push stack, make space for 29 registers
    stmw r3, 0x8 (sp)

	mr r3, r30
    mr r4, r31
	bl setFileName__11dDateFile_cFP9SaveBlock

.extern ResetAllCompletionCandidates__Fv
.extern ClearOldPathAvailabilityData__Fv
	bl ResetAllCompletionCandidates__Fv
	bl ClearOldPathAvailabilityData__Fv

    lmw r3, 0x8 (sp)
    addi sp, sp, 0x80 #Pop stack

    b returnNiceWorld

.global FSDebugStates
FSDebugStates:
	stwu r1, -0x10(r1)
	mflr r0
	stw r0, 0x14(r1)
	stw r31, 0xC(r1)
	stw r30, 0x8(r1)
	mr r31, r3
	mr r30, r4

	lis r3, FSStateDebugStr@h
	ori r3, r3, FSStateDebugStr@l
	crclr 4*cr1+eq
	lwz r4, 4(r4)
	bl OSReport

	mr r3, r31
	mr r4, r30
	lwzu r12, 0x18(r3)
	lwz r12, 0x18(r12)
	mtctr r12
	bctrl

	lwz r31, 0xC(r1)
	lwz r30, 0x8(r1)
	lwz r0, 0x14(r1)
	mtlr r0
	addi r1, r1, 0x10
	blr

.global DefaultSavefileInfo
DefaultSavefileInfo:
	addi r4, r3, 0x6FC
	lis r5, DefaultSavefileInfoData@h
	ori r5, r5, DefaultSavefileInfoData@l
	lis r6, DefaultSavefileInfoDataEnd@h
	ori r6, r6, DefaultSavefileInfoDataEnd@l
DSFICopyLoop:
	lwz r7, 0(r5)
	stw r7, 0(r4)
	addi r4, r4, 4
	addi r5, r5, 4
	cmpw r5, r6
	blt DSFICopyLoop
	blr

.extern SaveFileInstance
.extern SaveFileBuffer
.extern GetBlock__8SaveFileFi
.extern UpdateSavefileChecksums
.global FixUpExistingSavefile
FixUpExistingSavefile:
	stwu r1, -0x10(r1)
	mflr r0
	stw r0, 0x14(r1)
	stw r31, 0xC(r1)
	stw r30, 8(r1)
	mr r30, r3
	
	li r31, 0
fixupNextBlock:
	lis r3, SaveFileInstance@h
	ori r3, r3, SaveFileInstance@l
	lwz r3, 0(r3)
	mr r4, r31
	bl GetBlock__8SaveFileFi

	lbz r4, 2(r3)
	clrlwi. r4, r4, 31
	beq dontFixThisOne
	bl DefaultSavefileInfo
dontFixThisOne:

	# this is the one whose checksum is checked
	lis r3, SaveFileBuffer@h
	ori r3, r3, SaveFileBuffer@l
	addi r3, r3, 0x6A0
	mulli r4, r31, 0x980
	add r3, r3, r4
	lbz r4, 2(r3)
	clrlwi. r4, r4, 31
	beq dontFixThisOne2
	bl DefaultSavefileInfo
dontFixThisOne2:

	addi r31, r31, 1
	cmpwi r31, 6
	blt fixupNextBlock

	lis r3, SaveFileBuffer@h
	ori r3, r3, SaveFileBuffer@l
	subi r3, r3, 0x20
	bl UpdateSavefileChecksums

	lis r3, SaveFileInstance@h
	ori r3, r3, SaveFileInstance@l
	lwz r3, 0(r3)
	bl UpdateSavefileChecksums

	mr r3, r30
	lwz r30, 8(r1)
	lwz r31, 0xC(r1)
	lwz r0, 0x14(r1)
	mtlr r0
	addi r1, r1, 0x10
	blr


.section .ctors,4
.long FixUpExistingSavefile

.data
FSStateDebugStr: .string "State: %s\n"

.global FSDummyBrlanName, FSDummyGroup, FSDummyPane
FSDummyBrlanName: .string "fileSelectBase_18_DummyAnim.brlan"
FSDummyGroup: .string "DummyGroup"
FSDummyPane: .string "DummyPane"

.global A00_window
A00_window: .string "A00_window"
.global ShowEraseCopy, HideEraseCopy
ShowEraseCopy: .string "fileSelectBase_18_ShowEraseCopy.brlan"
HideEraseCopy: .string "fileSelectBase_18_HideEraseCopy.brlan"
.global N_posFile1_00
N_posFile1_00: .string "N_posFile1_00"
.global CDHolderGroup
CDHolderGroup: .string "CDHolderGroup"

.align 4
DefaultSavefileInfoData:
.byte 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 # 16
.byte 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 # 16
.long 0xFFFF99FF,0x1FB423FF
.long 0x173714FF,0x3C9135FF
.long 0xFFFF99FF,0x1FB423FF
.short 0x75
.byte 0x2E,0xB
.byte 1,1
.byte 0,10
.byte 0xD1,0x01

DefaultSavefileInfoDataEnd:
.long 0
