/*
 * File: z_obj_skateblock.c
 * Overlay: ovl_Obj_Skateblock
 * Description: Pushable block that slides on ice
 */

#include "z_obj_skateblock.h"

#define FLAGS (ACTOR_FLAG_10)

#define THIS ((ObjSkateblock*)thisx)

void ObjSkateblock_Init(Actor* thisx, GlobalContext* globalCtx);
void ObjSkateblock_Destroy(Actor* thisx, GlobalContext* globalCtx);
void ObjSkateblock_Update(Actor* thisx, GlobalContext* globalCtx);
void ObjSkateblock_Draw(Actor* thisx, GlobalContext* globalCtx);

#if 0
const ActorInit Obj_Skateblock_InitVars = {
    ACTOR_OBJ_SKATEBLOCK,
    ACTORCAT_BG,
    FLAGS,
    GAMEPLAY_DANGEON_KEEP,
    sizeof(ObjSkateblock),
    (ActorFunc)ObjSkateblock_Init,
    (ActorFunc)ObjSkateblock_Destroy,
    (ActorFunc)ObjSkateblock_Update,
    (ActorFunc)ObjSkateblock_Draw,
};

// static InitChainEntry sInitChain[] = {
static InitChainEntry D_80A22AE0[] = {
    ICHAIN_F32_DIV1000(gravity, -1700, ICHAIN_CONTINUE),
    ICHAIN_F32_DIV1000(terminalVelocity, -20000, ICHAIN_CONTINUE),
    ICHAIN_VEC3F_DIV1000(scale, 100, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneForward, 4000, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneScale, 150, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneDownward, 200, ICHAIN_STOP),
};

#endif

extern InitChainEntry D_80A22AE0[];

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Obj_Skateblock/func_80A21150.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Obj_Skateblock/func_80A211F4.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Obj_Skateblock/func_80A212F0.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Obj_Skateblock/func_80A21370.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Obj_Skateblock/func_80A21548.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Obj_Skateblock/func_80A216D4.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Obj_Skateblock/func_80A21934.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Obj_Skateblock/func_80A21990.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Obj_Skateblock/func_80A21C30.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Obj_Skateblock/func_80A21C88.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Obj_Skateblock/func_80A21CB4.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Obj_Skateblock/func_80A21CD8.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Obj_Skateblock/func_80A21D1C.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Obj_Skateblock/func_80A21E9C.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Obj_Skateblock/func_80A21F68.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Obj_Skateblock/func_80A21F74.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Obj_Skateblock/ObjSkateblock_Init.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Obj_Skateblock/ObjSkateblock_Destroy.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Obj_Skateblock/func_80A22308.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Obj_Skateblock/func_80A22334.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Obj_Skateblock/func_80A2244C.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Obj_Skateblock/func_80A224A4.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Obj_Skateblock/func_80A22628.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Obj_Skateblock/func_80A2264C.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Obj_Skateblock/func_80A22728.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Obj_Skateblock/func_80A2273C.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Obj_Skateblock/func_80A227A4.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Obj_Skateblock/func_80A227C0.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Obj_Skateblock/ObjSkateblock_Update.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Obj_Skateblock/ObjSkateblock_Draw.s")
