#ifndef _Z64EFFECT_H_
#define _Z64EFFECT_H_

#include "PR/ultratypes.h"
#include "color.h"
#include "z64light.h"
#include "z64math.h"
#include "unk.h"

struct GraphicsContext;
struct GlobalContext;

#define SPARK_COUNT 3
#define BLURE_COUNT 25
#define SHIELD_PARTICLE_COUNT 3
#define TIRE_MARK_COUNT 15

#define TOTAL_EFFECT_COUNT SPARK_COUNT + BLURE_COUNT + SHIELD_PARTICLE_COUNT + TIRE_MARK_COUNT

/* G Effect Regs */

#define rgTexIdx regs[0]
#define rgScale regs[1]
#define rgTexIdxStep regs[2]
#define rgPrimColorR regs[3]
#define rgPrimColorG regs[4]
#define rgPrimColorB regs[5]
#define rgPrimColorA regs[6]
#define rgEnvColorR regs[7]
#define rgEnvColorG regs[8]
#define rgEnvColorB regs[9]
#define rgEnvColorA regs[10]
#define rgObjBankIdx regs[11]

typedef struct {
    /* 0x00 */ u8 active;
    /* 0x01 */ u8 unk1;
    /* 0x02 */ u8 unk2;
} EffectStatus; // size = 0x03

typedef struct {
    /* 0x00 */ Vec3f velocity;
    /* 0x0C */ Vec3f position;
    /* 0x18 */ Vec3s unkVelocity;
    /* 0x1E */ Vec3s unkPosition;
} EffectSparkElement; // size = 0x24

typedef struct {
    /* 0x000 */ Vec3s position;
    /* 0x008 */ s32 numElements; // "table_size"; calculated as uDiv * vDiv + 2
    /* 0x00C */ EffectSparkElement elements[32];
    /* 0x48C */ f32 speed;
    /* 0x490 */ f32 gravity;
    /* 0x494 */ u32 uDiv; // "u_div"
    /* 0x498 */ u32 vDiv; // "v_div"
    /* 0x49C */ Color_RGBA8 colorStart[4];
    /* 0x4AC */ Color_RGBA8 colorEnd[4];
    /* 0x4BC */ s32 timer;
    /* 0x4C0 */ s32 duration;
} EffectSparkInit; // size = 0x4C4

typedef struct {
    /* 0x000 */ Vec3s position;
    /* 0x008 */ s32 numElements; // "table_size"; calculated as uDiv * vDiv + 2
    /* 0x00C */ EffectSparkElement elements[32];
    /* 0x48C */ f32 speed;
    /* 0x490 */ f32 gravity;
    /* 0x494 */ u32 uDiv; // "u_div"
    /* 0x498 */ u32 vDiv; // "v_div"
    /* 0x49C */ Color_RGBA8 colorStart[4];
    /* 0x4AC */ Color_RGBA8 colorEnd[4];
    /* 0x4BC */ s32 timer;
    /* 0x4C0 */ s32 duration;
} EffectSpark; // size = 0x4C4

typedef struct {
    /* 0x00 */ s32 state;
    /* 0x04 */ s32 timer;
    /* 0x08 */ Vec3s p1;
    /* 0x0E */ Vec3s p2;
    /* 0x14 */ u16 flags;
} EffectBlureElement; // size = 0x18

typedef struct {
    /* 0x000 */ char unk_00[0x184];
    /* 0x184 */ u8 p1StartColor[4];
    /* 0x188 */ u8 p2StartColor[4];
    /* 0x18C */ u8 p1EndColor[4];
    /* 0x190 */ u8 p2EndColor[4];
    /* 0x194 */ s32 elemDuration;
    /* 0x198 */ s32 unkFlag;
    /* 0x19C */ s32 calcMode;
} EffectBlureInit1; // size = 0x1A0

typedef struct {
    /* 0x00 */ s32 calcMode;
    /* 0x04 */ u16 flags;
    /* 0x06 */ s16 addAngleChange;
    /* 0x08 */ u8 p1StartColor[4];
    /* 0x0C */ u8 p2StartColor[4];
    /* 0x10 */ u8 p1EndColor[4];
    /* 0x14 */ u8 p2EndColor[4];
    /* 0x18 */ u8 elemDuration;
    /* 0x19 */ u8 unkFlag;
    /* 0x1A */ u8 drawMode; // 0: simple; 1: simple with alt colors; 2+: smooth
    /* 0x1B */ u8 mode4Param;
    /* 0x1C */ Color_RGBA8 altPrimColor; // used with drawMode 1
    /* 0x20 */ Color_RGBA8 altEnvColor; // used with drawMode 1
} EffectBlureInit2; // size = 0x24

typedef struct {
    /* 0x000 */ EffectBlureElement elements[16];
    /* 0x180 */ s32 calcMode;
    /* 0x184 */ f32 mode4Param;
    /* 0x188 */ u16 flags;
    /* 0x18A */ s16 addAngleChange;
    /* 0x18C */ s16 addAngle;
    /* 0x18E */ Color_RGBA8 p1StartColor;
    /* 0x192 */ Color_RGBA8 p2StartColor;
    /* 0x196 */ Color_RGBA8 p1EndColor;
    /* 0x19A */ Color_RGBA8 p2EndColor;
    /* 0x19E */ u8 numElements; // "now_edge_num"
    /* 0x19F */ u8 elemDuration;
    /* 0x1A0 */ u8 unkFlag;
    /* 0x1A1 */ u8 drawMode; // 0: simple; 1: simple with alt colors; 2+: smooth
    /* 0x1A2 */ Color_RGBA8 altPrimColor; // used with drawMode 1
    /* 0x1A6 */ Color_RGBA8 altEnvColor; // used with drawMode 1
} EffectBlure; // size = 0x1AC

typedef struct {
    /* 0x00 */ f32 initialSpeed;
    /* 0x04 */ f32 endXChange;
    /* 0x08 */ f32 endX;
    /* 0x0C */ f32 startXChange;
    /* 0x10 */ f32 startX;
    /* 0x14 */ s16 yaw;
    /* 0x16 */ s16 pitch;
} EffectShieldParticleElement; // size = 0x18

typedef struct {
    /* 0x00 */ u8 numElements;
    /* 0x02 */ Vec3s position;
    /* 0x08 */ Color_RGBA8 primColorStart;
    /* 0x0C */ Color_RGBA8 envColorStart;
    /* 0x10 */ Color_RGBA8 primColorMid;
    /* 0x14 */ Color_RGBA8 envColorMid;
    /* 0x18 */ Color_RGBA8 primColorEnd;
    /* 0x1C */ Color_RGBA8 envColorEnd;
    /* 0x20 */ f32 deceleration;
    /* 0x24 */ f32 maxInitialSpeed;
    /* 0x28 */ f32 lengthCutoff;
    /* 0x2C */ u8 duration;
    /* 0x2E */ LightPoint lightPoint;
    /* 0x3C */ s32 lightDecay; // halves light radius every frame when set to 1
} EffectShieldParticleInit; // size = 0x40

typedef struct {
    /* 0x000 */ EffectShieldParticleElement elements[16];
    /* 0x180 */ u8 numElements;
    /* 0x182 */ Vec3s position;
    /* 0x188 */ Color_RGBA8 primColorStart;
    /* 0x18C */ Color_RGBA8 envColorStart;
    /* 0x190 */ Color_RGBA8 primColorMid;
    /* 0x194 */ Color_RGBA8 envColorMid;
    /* 0x198 */ Color_RGBA8 primColorEnd;
    /* 0x19C */ Color_RGBA8 envColorEnd;
    /* 0x1A0 */ f32 deceleration;
    /* 0x1A4 */ char unk_1A4[0x04];
    /* 0x1A8 */ f32 maxInitialSpeed;
    /* 0x1AC */ f32 lengthCutoff;
    /* 0x1B0 */ u8 duration;
    /* 0x1B1 */ u8 timer;
    /* 0x1B2 */ LightInfo lightInfo;
    /* 0x1C0 */ LightNode* lightNode;
    /* 0x1C4 */ s32 lightDecay; // halves light radius every frame when set to 1
} EffectShieldParticle; // size = 0x1C8

typedef struct {
    /* 0x00 */ u16 flags;
    /* 0x02 */ Vec3s p1;
    /* 0x08 */ Vec3s p2;
    /* 0x0E */ s16 life;
    /* 0x10 */ UNK_TYPE1 pad10[0x4];
    /* 0x14 */ CollisionPoly* colPoly;
} EffectTireMarkElement; // size = 0x18

typedef struct {
    /* 0x0 */ s16 unk0;
    /* 0x2 */ s16 elemDuration;
    /* 0x4 */ Color_RGBA8 color;
} EffectTireMarkInit; // size = 0x8

typedef struct {
    /* 0x000 */ EffectTireMarkElement elements[64];
    /* 0x600 */ s16 unk600;
    /* 0x602 */ s16 numElements;
    /* 0x604 */ s16 elemDuration;
    /* 0x606 */ Color_RGBA8 color;
} EffectTireMark; // size = 0x60C

typedef struct {
    /* 0x0000 */ struct GlobalContext* globalCtx;
    struct {
        EffectStatus status;
        EffectSpark effect;
    } /* 0x0004 */ sparks[SPARK_COUNT];
    struct {
        EffectStatus status;
        EffectBlure effect;
    } /* 0x0E5C */ blures[BLURE_COUNT];
    struct {
        EffectStatus status;
        EffectShieldParticle effect;
    } /* 0x388C */ shieldParticles[SHIELD_PARTICLE_COUNT];
    struct {
        EffectStatus status;
        EffectTireMark effect;
    } /* 0x3DF0 */ tireMarks[TIRE_MARK_COUNT];
} EffectContext; // size = 0x98E0

typedef struct {
    /* 0x00 */ u32 size;
    /* 0x04 */ void (*init)(void* effect, void* initParams);
    /* 0x08 */ void (*destroy)(void* effect);
    /* 0x0C */ s32 (*update)(void* effect);
    /* 0x10 */ void (*draw)(void* effect, struct GraphicsContext* gfxCtx);
} EffectInfo; // size = 0x14

typedef enum {
    /* 0x00 */ EFFECT_SPARK,
    /* 0x01 */ EFFECT_BLURE1,
    /* 0x02 */ EFFECT_BLURE2,
    /* 0x03 */ EFFECT_SHIELD_PARTICLE,
    /* 0x04 */ EFFECT_TIRE_MARK
} EffectType;

/* Effect Soft Sprites */

struct EffectSs;

typedef u32 (*EffectSsInitFunc)(struct GlobalContext* globalCtx, u32 index, struct EffectSs* effectSs, void* initParams);
typedef void(*EffectSsUpdateFunc)(struct GlobalContext* globalCtx, u32 index, struct EffectSs* particle);
typedef void(*EffectSsDrawFunc)(struct GlobalContext* globalCtx, u32 index, struct EffectSs* particle);

typedef struct {
    /* 0x00 */ u32 type;
    /* 0x04 */ EffectSsInitFunc init;
} EffectSsInit; // size = 0x08

typedef struct {
    /* 0x00 */ u32 vromStart;
    /* 0x04 */ u32 vromEnd;
    /* 0x08 */ void* vramStart;
    /* 0x0C */ void* vramEnd;
    /* 0x10 */ void* loadedRamAddr;
    /* 0x14 */ EffectSsInit* initInfo;
    /* 0x18 */ u8 unk18; // Always 1?
} EffectSsOverlay; // size = 0x1C

typedef struct EffectSs {
    /* 0x00 */ Vec3f pos;
    /* 0x0C */ Vec3f velocity;
    /* 0x18 */ Vec3f accel;
    /* 0x24 */ EffectSsUpdateFunc update;
    /* 0x28 */ EffectSsDrawFunc draw;
    /* 0x2C */ Vec3f vec;
    /* 0x38 */ void* gfx;
    /* 0x3C */ Actor* actor;
    /* 0x40 */ s16 regs[13]; // These are particle-specific
    /* 0x5A */ u16 flags; // bit 0: set if this entry is not considered free on a priority tie bit 1: ? bit 2: ?
    /* 0x5C */ s16 life; // -1 means this entry is free
    /* 0x5E */ u8 priority; // Lower number mean higher priority
    /* 0x5F */ u8 type;
} EffectSs; // size = 0x60

typedef struct {
    /* 0x0 */ EffectSs* data_table; // Name from debug assert
    /* 0x4 */ s32 searchIndex;
    /* 0x8 */ s32 size;
} EffectSsInfo; // size = 0xC

typedef enum {
    /* 0x00 */ EFFECT_SS_DUST,
    /* 0x01 */ EFFECT_SS_KIRAKIRA,
    /* 0x02 */ EFFECT_SS_UNSET_02, // Deleted
    /* 0x03 */ EFFECT_SS_BOMB2,
    /* 0x04 */ EFFECT_SS_BLAST,
    /* 0x05 */ EFFECT_SS_G_SPK,
    /* 0x06 */ EFFECT_SS_D_FIRE,
    /* 0x07 */ EFFECT_SS_BUBBLE,
    /* 0x08 */ EFFECT_SS_UNSET_08, // Deleted
    /* 0x09 */ EFFECT_SS_G_RIPPLE,
    /* 0x0A */ EFFECT_SS_G_SPLASH,
    /* 0x0B */ EFFECT_SS_UNSET_0B,
    /* 0x0C */ EFFECT_SS_G_FIRE,
    /* 0x0D */ EFFECT_SS_LIGHTNING,
    /* 0x0E */ EFFECT_SS_DT_BUBBLE,
    /* 0x0F */ EFFECT_SS_HAHEN,
    /* 0x10 */ EFFECT_SS_STICK,
    /* 0x11 */ EFFECT_SS_SIBUKI,
    /* 0x12 */ EFFECT_SS_UNSET_12, // Deleted
    /* 0x13 */ EFFECT_SS_UNSET_13, // Deleted
    /* 0x14 */ EFFECT_SS_STONE1,
    /* 0x15 */ EFFECT_SS_HITMARK,
    /* 0x16 */ EFFECT_SS_FHG_FLASH,
    /* 0x17 */ EFFECT_SS_K_FIRE,
    /* 0x18 */ EFFECT_SS_SOLDER_SRCH_BALL,
    /* 0x19 */ EFFECT_SS_KAKERA,
    /* 0x1A */ EFFECT_SS_ICE_PIECE,
    /* 0x1B */ EFFECT_SS_EN_ICE,
    /* 0x1C */ EFFECT_SS_FIRE_TAIL,
    /* 0x1D */ EFFECT_SS_EN_FIRE,
    /* 0x1E */ EFFECT_SS_EXTRA,
    /* 0x1F */ EFFECT_SS_UNSET_1F, // Deleted
    /* 0x20 */ EFFECT_SS_DEAD_DB,
    /* 0x21 */ EFFECT_SS_DEAD_DD,
    /* 0x22 */ EFFECT_SS_DEAD_DS,
    /* 0x23 */ EFFECT_SS_UNSET_23, // Deleted
    /* 0x24 */ EFFECT_SS_ICE_SMOKE,
    /* 0x25 */ EFFECT_EN_ICE_BLOCK,
    /* 0x26 */ EFFECT_SS_SBN,
    /* 0x27 */ EFFECT_SS_MAX
} EffectSsType;

#endif
