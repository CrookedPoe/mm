/*
 * File: z_bg_ingate.c
 * Overlay: ovl_Bg_Ingate
 * Description: Swamp Tour Boat
 */

#include "z_bg_ingate.h"

#define FLAGS (ACTOR_FLAG_10 | ACTOR_FLAG_20)

#define THIS ((BgIngate*)thisx)

void BgIngate_Init(Actor* thisx, GlobalContext* globalCtx);
void BgIngate_Destroy(Actor* thisx, GlobalContext* globalCtx);
void BgIngate_Update(Actor* thisx, GlobalContext* globalCtx);
void BgIngate_Draw(Actor* thisx, GlobalContext* globalCtx);

#if 0
const ActorInit Bg_Ingate_InitVars = {
    ACTOR_BG_INGATE,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_SICHITAI_OBJ,
    sizeof(BgIngate),
    (ActorFunc)BgIngate_Init,
    (ActorFunc)BgIngate_Destroy,
    (ActorFunc)BgIngate_Update,
    (ActorFunc)BgIngate_Draw,
};

#endif

extern UNK_TYPE D_060006B0;
extern UNK_TYPE D_060016DC;

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Bg_Ingate/func_80953A90.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Bg_Ingate/func_80953B40.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Bg_Ingate/func_80953BEC.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Bg_Ingate/func_80953DA8.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Bg_Ingate/func_80953E38.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Bg_Ingate/func_80953EA4.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Bg_Ingate/func_80953F14.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Bg_Ingate/func_80953F8C.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Bg_Ingate/func_80953F9C.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Bg_Ingate/func_809541B8.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Bg_Ingate/func_809542A0.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Bg_Ingate/func_80954340.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Bg_Ingate/func_809543D4.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Bg_Ingate/BgIngate_Init.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Bg_Ingate/BgIngate_Destroy.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Bg_Ingate/BgIngate_Update.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Bg_Ingate/BgIngate_Draw.s")
