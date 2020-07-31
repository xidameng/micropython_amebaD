/*------------------------------------------------------------------------------
--                                                                            --
--       This software is confidential and proprietary and may be used        --
--        only as expressly authorized by a licensing agreement from          --
--                                                                            --
--                            Hantro Products Oy.                             --
--                                                                            --
--                   (C) COPYRIGHT 2006 HANTRO PRODUCTS OY                    --
--                            ALL RIGHTS RESERVED                             --
--                                                                            --
--                 The entire notice above must be reproduced                 --
--                  on all copies and should not be removed.                  --
--                                                                            --
--------------------------------------------------------------------------------
--
--  Description : ASIC low level controller
--
------------------------------------------------------------------------------*/
#ifndef __ENC_ASIC_CONTROLLER_H__
#define __ENC_ASIC_CONTROLLER_H__

#include "basetype.h"
#include "enccfg.h"
#include "ewl.h"
#include "encswhwregisters.h"

/* HW status register bits */
#define ASIC_STATUS_ALL                 0xFFD

#define ASIC_STATUS_RFC_BUFF_OVERFLOW   0x800
#define ASIC_STATUS_LINE_BUFFER_DONE    0x400
#define ASIC_STATUS_FUSE                0x200
#define ASIC_STATUS_SLICE_READY         0x100
#define ASIC_STATUS_HW_TIMEOUT          0x040
#define ASIC_STATUS_BUFF_FULL           0x020
#define ASIC_STATUS_HW_RESET            0x010
#define ASIC_STATUS_ERROR               0x008
#define ASIC_STATUS_FRAME_READY         0x004

#define ASIC_IRQ_LINE                   0x001

#define ASIC_STATUS_ENABLE              0x001

#define ASIC_H264_BYTE_STREAM           0x00
#define ASIC_H264_NAL_UNIT              0x01

#define ASIC_PENALTY_UNDEFINED          -1

#define ASIC_PENALTY_TABLE_SIZE         128

#define ASIC_FRAME_BUF_LUM_MAX          3
#define ASIC_FRAME_BUF_CHR_MAX          4

typedef enum
{
    IDLE = 0,   /* Initial state, both HW and SW disabled */
    HWON_SWOFF, /* HW processing, SW waiting for HW */
    HWON_SWON,  /* Both HW and SW processing */
    HWOFF_SWON, /* HW is paused or disabled, SW is processing */
    DONE
} bufferState_e;

typedef enum
{
    ASIC_VP8 = 1,
    ASIC_JPEG = 2,
    ASIC_H264 = 3
} asicCodingType_e;

typedef enum
{
    ASIC_P_16x16 = 0,
    ASIC_P_16x8 = 1,
    ASIC_P_8x16 = 2,
    ASIC_P_8x8 = 3,
    ASIC_I_4x4 = 4,
    ASIC_I_16x16 = 5
} asicMbType_e;

typedef enum
{
    ASIC_INTER = 0,
    ASIC_INTRA = 1,
    ASIC_MVC = 2,
    ASIC_MVC_REF_MOD = 3
} asicFrameCodingType_e;

enum
{
    ASIC_PENALTY_I16MODE0 = 0,
    ASIC_PENALTY_I16MODE1,
    ASIC_PENALTY_I16MODE2,
    ASIC_PENALTY_I16MODE3,
    ASIC_PENALTY_I4MODE0,
    ASIC_PENALTY_I4MODE1,
    ASIC_PENALTY_I4MODE2,
    ASIC_PENALTY_I4MODE3,
    ASIC_PENALTY_I4MODE4,
    ASIC_PENALTY_I4MODE5,
    ASIC_PENALTY_I4MODE6,
    ASIC_PENALTY_I4MODE7,
    ASIC_PENALTY_I4MODE8,
    ASIC_PENALTY_I4MODE9,
    ASIC_PENALTY_I16FAVOR,
    ASIC_PENALTY_I4_PREV_MODE_FAVOR,
    ASIC_PENALTY_COST_INTER,
    ASIC_PENALTY_DMV_COST_CONST,
    ASIC_PENALTY_INTER_FAVOR,
    ASIC_PENALTY_SKIP,
    ASIC_PENALTY_GOLDEN,
    ASIC_PENALTY_SPLIT4x4,
    ASIC_PENALTY_SPLIT8x4,
    ASIC_PENALTY_SPLIT8x8,
    ASIC_PENALTY_SPLIT16x8,
    ASIC_PENALTY_SPLIT_ZERO,
    ASIC_PENALTY_DMV_4P,
    ASIC_PENALTY_DMV_1P,
    ASIC_PENALTY_DMV_QP,
    ASIC_PENALTY_DZ_RATE0,
    ASIC_PENALTY_DZ_RATE1,
    ASIC_PENALTY_DZ_RATE2,
    ASIC_PENALTY_DZ_RATE3,
    ASIC_PENALTY_DZ_SKIP0,
    ASIC_PENALTY_DZ_SKIP1,

    ASIC_PENALTY_AMOUNT
};

typedef struct
{
    u32 irqDisable;
    u32 irqInterval;
    u32 traceMbTiming;
    u32 inputReadChunk;
    u32 mbsInCol;
    u32 mbsInRow;
    u32 qp;
    u32 qpMin;
    u32 qpMax;
    u32 constrainedIntraPrediction;
    u32 roundingCtrl;
    u32 frameCodingType;
    u32 codingType;
    u32 pixelsOnRow;
    u32 xFill;
    u32 yFill;
    u32 ppsId;
    u32 idrPicId;
    u32 frameNum;
    u32 picInitQp;
    i32 sliceAlphaOffset;
    i32 sliceBetaOffset;
    u32 filterDisable;
    u32 transform8x8Mode;
    u32 enableCabac;
    u32 cabacInitIdc;
    i32 chromaQpIndexOffset;
    u32 chromaSwap;
    u32 sliceSizeMbRows;
    u32 inputImageFormat;
    u32 inputImageRotation;
    u32 outputStrmBase;
    u32 outputStrmSize;
    u32 firstFreeBit;
    u32 strmStartMSB;
    u32 strmStartLSB;
    u32 rlcBase;
    u32 rlcLimitSpace;
    u32 sizeTblBase;
    u32 sliceReadyInterrupt;
    u32 recWriteBuffer;
    u32 recWriteDisable;
    u32 reconImageId;
    u32 internalImageLumBaseW;
    u32 internalImageChrBaseW;
    u32 internalImageLumTableBaseW;
    u32 internalImageChrTableBaseW;
    u32 internalImageLumBaseR[2];
    u32 internalImageChrBaseR[2];
    u32 internalImageLumTableBaseR[2];
    u32 internalImageChrTableBaseR[2];
    u32 refLumCompress;
    u32 refChrCompress;
    u32 rfcLumDiscreteMode;
    u32 rfcLumBufLimit;
    u32 rfcChrBufLimit;
    u32 rfcOverflowIRQEn;
    u32 inputLumBase;
    u32 inputCbBase;
    u32 inputCrBase;
    u32 cpDistanceMbs;
    u32 *cpTargetResults;
    const u32 *cpTarget;
    const i32 *targetError;
    const i32 *deltaQp;
    u32 rlcCount;
    u32 qpSum;
    u32 h264StrmMode;   /* 0 - byte stream, 1 - NAL units */
    u8 quantTable[8 * 8 * 2];
    u8 dmvPenalty[ASIC_PENALTY_TABLE_SIZE];
    u8 dmvQpelPenalty[ASIC_PENALTY_TABLE_SIZE];
    u32 ipolFilterMode;
    u32 jpegMode;
    u32 jpegSliceEnable;
    u32 jpegRestartInterval;
    u32 jpegRestartMarker;
    //u32 regMirror[ASIC_SWREG_AMOUNT];
#ifdef _VOE_NO_USE	//Modified by Devin
    u32 regMirror[ASIC_SWREG_AMOUNT];
#else
    u32 *regMirror;
#endif	
    u32 inputLumaBaseOffset;
    u32 inputLumaBaseOffsetVert;
    u32 inputChromaBaseOffset;
    u32 h264Inter4x4Disabled;
    u32 disableQuarterPixelMv;
    u32 vsNextLumaBase;
    u32 vsMode;
    u32 asicCfgReg;
    u32 asicHwId;
    i32 madQpDelta[3];
    u32 madThreshold[3];
    u32 madCount[3];
    u32 mvcAnchorPicFlag;
    u32 mvcPriorityId;
    u32 mvcViewId;
    u32 mvcTemporalId;
    u32 mvcInterViewFlag;
    u32 cirStart;
    u32 cirInterval;
    u32 intraSliceMap1;
    u32 intraSliceMap2;
    u32 intraSliceMap3;
    u32 intraAreaTop;
    u32 intraAreaLeft;
    u32 intraAreaBottom;
    u32 intraAreaRight;
    u32 roi1Top;
    u32 roi1Left;
    u32 roi1Bottom;
    u32 roi1Right;
    u32 roi2Top;
    u32 roi2Left;
    u32 roi2Bottom;
    u32 roi2Right;
    i32 roi1DeltaQp;
    i32 roi2DeltaQp;
    u32 roiUpdate;
    u32 mvOutputBase;
    u32 cabacCtxBase;
    u32 probCountBase;
    u32 segmentMapBase;
    u32 colorConversionCoeffA;
    u32 colorConversionCoeffB;
    u32 colorConversionCoeffC;
    u32 colorConversionCoeffE;
    u32 colorConversionCoeffF;
    u32 rMaskMsb;
    u32 gMaskMsb;
    u32 bMaskMsb;
    u32 partitionBase[8];
    u32 qpY1QuantDc[4];
    u32 qpY1QuantAc[4];
    u32 qpY2QuantDc[4];
    u32 qpY2QuantAc[4];
    u32 qpChQuantDc[4];
    u32 qpChQuantAc[4];
    u32 qpY1ZbinDc[4];
    u32 qpY1ZbinAc[4];
    u32 qpY2ZbinDc[4];
    u32 qpY2ZbinAc[4];
    u32 qpChZbinDc[4];
    u32 qpChZbinAc[4];
    u32 qpY1RoundDc[4];
    u32 qpY1RoundAc[4];
    u32 qpY2RoundDc[4];
    u32 qpY2RoundAc[4];
    u32 qpChRoundDc[4];
    u32 qpChRoundAc[4];
    u32 qpY1DequantDc[4];
    u32 qpY1DequantAc[4];
    u32 qpY2DequantDc[4];
    u32 qpY2DequantAc[4];
    u32 qpChDequantDc[4];
    u32 qpChDequantAc[4];
    u32 segmentEnable;
    u32 segmentMapUpdate;
    u32 mvRefIdx[2];
    u32 ref2Enable;
    u32 boolEncValue;
    u32 boolEncValueBits;
    u32 boolEncRange;
    u32 dctPartitions;
    u32 filterLevel[4];
    u32 filterSharpness;
    u32 zeroMvFavorDiv2;
    u32 splitMvMode;
    i32 lfRefDelta[4];
    i32 lfModeDelta[4];
    u32 deadzoneEnable;
    u8  dzCoeffRate[4*8*3*2];   /* Coeff 0&1 rate values for dead zone */
    u8  dzEobRate[4*8*2];       /* Eob token rate values for dead zone */
    u32 squaredError;
    u32 mse_mul256;
    u32 pen[4][ASIC_PENALTY_AMOUNT];    /* Penalty values for four segments */
    u32 scaledLumBase;
    u32 scaledWidth;
    u32 scaledHeight;
    u32 scaledWidthRatio;
    u32 scaledHeightRatio;
    u32 avgVar;
    u32 invAvgVar;
    u32 fieldPicFlag;
    u32 bottomFieldFlag;
    u32 fieldParity;
    u32 markCurrentLongTerm;
    u32 boostQp;
    u32 boostVar1;
    u32 boostVar2;
    u32 varLimitDiv32;
    u32 varInterFavorDiv16;
    u32 varMultiplier;
    u32 varAdd;
    u32 pskipMode;

    u32 mbWrPtr;
    u32 mbRrPtr;
    u32 lineBufferDepth;
    u32 lineBufferEn;
    u32 lineBufferHwHandShake;
    u32 lineBufferLoopBackEn;

    /* input compression related */
    u32 inputLeftOffset;
    u32 inputTopOffset;
    u32 inputLumTableBase;
    u32 inputChrTableBase;
    u32 vsNextLumaTableBase;

#ifdef ASIC_WAVE_TRACE_TRIGGER
    u32 vop_count;
#endif
} regValues_s;

typedef struct
{
    const void *ewl;
    regValues_s regs;
    EWLLinearMem_t internalImageLuma[ASIC_FRAME_BUF_LUM_MAX];
    EWLLinearMem_t internalImageChroma[ASIC_FRAME_BUF_CHR_MAX];
    EWLLinearMem_t scaledImage;
    EWLLinearMem_t cabacCtx;
    EWLLinearMem_t mvOutput;
    EWLLinearMem_t probCount;
    EWLLinearMem_t segmentMap;
    u32 sizeTblSize;
    EWLLinearMem_t sizeTbl;
    u32 traceRecon;
    u32 internalImageLumSize;
    u32 internalImageChrSize;
    u32 irqStatus;
} asicData_s;

/*------------------------------------------------------------------------------
    4. Function prototypes
------------------------------------------------------------------------------*/
i32 EncAsicControllerInit(asicData_s * asic);

i32 EncAsicMemAlloc_V2(asicData_s * asic, u32 width, u32 height,
                       u32 scaledWidth, u32 scaledHeight,
                       u32 encodingType, u32 numRefBuffsLum, u32 numRefBuffsChr);
void EncAsicMemFree_V2(asicData_s * asic);

/* Functions for controlling ASIC */
void EncAsicSetQuantTable(asicData_s * asic,
                          const u8 * lumTable, const u8 * chTable);

void EncAsicGetRegisters(const void *ewl, regValues_s * val);
u32 EncAsicGetStatus(const void *ewl);

u32 EncAsicGetId(const void *ewl);

void EncAsicFrameStart(const void *ewl, regValues_s * val);

void EncAsicStop(const void *ewl);

void EncAsicRecycleInternalImage(asicData_s *asic, u32 numViews, u32 viewId,
        u32 anchor, u32 numRefBuffsLum, u32 numRefBuffsChr);

i32 EncAsicCheckStatus_V2(asicData_s *asic);
u32 *EncAsicGetMvOutput(asicData_s *asic, u32 mbNum);
void EncAsicClearStatusBit(const void *ewl, u32 statusBit);
void EncAsicClearLineBufIrqStatus(const void *ewl, regValues_s * val);

#endif
