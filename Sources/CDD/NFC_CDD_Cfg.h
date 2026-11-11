/*
 * NFC_CDD_Cfg.h
 *
 *  Created on: 30-Aug-2026
 *      Author: Shardul
 */

#ifndef SOURCES_CDD_NFC_CDD_CFG_H_
#define SOURCES_CDD_NFC_CDD_CFG_H_

/*******************************************************************************
**    Includes
*******************************************************************************/
#include "NFC_CDD_DevCfg.h"

/*******************************************************************************
**    Macros
*******************************************************************************/
/* @TODO NFC ETS: Optimize for total number of possible messages and sequences in a single cycle */
#define MAX_SUBSEQS         16u

/************************ Messages Count for Sequences ************************/
/* Message count for 48-Bit sequences */
#define MSGCNT_MC_SOFTRESET     1u
#define MSGCNT_MC_READDEVID     2u

/*******************************************************************************
**    Type definitions
*******************************************************************************/
/********************************* Message IDs ********************************/
typedef enum E_NFC_msgIdType_
{
    /* Read Messages */
	RMSG_RESERVED,
	RMSG_COMMAND_REG,
	RMSG_COM_IEN_REG,
	RMSG_DIV_IEN_REG,
	RMSG_COM_IRQ_REG,
	RMSG_DIV_IRQ_REG,
	RMSG_ERROR_REG,
	RMSG_STATUS1_REG,
	RMSG_STATUS2_REG,
	RMSG_FIFO_DATA_REG,
	RMSG_WATER_LEVEL_REG,
	RMSG_CONTROL_REG,
	RMSG_BIT_FRAMING_REG,
	RMSG_COLL_REG,
	RMSG_MODE_REG,
	RMSG_TX_MODE_REG,
	RMSG_RX_MODE_REG,
	RMSG_TX_CONTROL_REG,
	RMSG_TX_ASK_REG,
	RMSG_TX_SEL_REG,
	RMSG_RX_SEL_REG,
	RMSG_RX_THRESHOLD_REG,
	RMSG_DEMOD_REG,
	RMSG_MF_TX_REG,
	RMSG_MF_RX_REG,
	RMSG_SERIAL_SPEED_REG,
	RMSG_CRC_RESULT_MSB_REG,
	RMSG_CRC_RESULT_LSB_REG,
	RMSG_MODE_WIDTH_REG,
	RMSG_RF_CFG_REG,
	RMSG_GSN_REG,
	RMSG_CWGSP_REG,
	RMSG_MODGSP_REG,
	RMSG_TMODE_REG,
	RMSG_TPRESCALER_REG,
	RMSG_TRELOAD_MSB_REG,
	RMSG_TRELOAD_LSB_REG,
	RMSG_TCOUNTER_VAL_LSB,
	RMSG_TCOUNTER_VAL_MSB,
	RMSG_TEST_SEL1_REG,
	RMSG_TEST_SEL2_REG,
	RMSG_TEST_PIN_EN_REG,
	RMSG_TEST_PIN_VAL_REG,
	RMSG_TEST_BUS_REG,
	RMSG_AUTO_TEST_REG,
	RMSG_VERSION_REG,
	RMSG_ANALOG_TEST_REG,
	RMSG_TEST_DAC1_REG,
	RMSG_TEST_DAC2_REG,
	RMSG_TEST_ADC_REG,
    RMSG_NULL_REG,       /* Read register 00 to get response to previous read */
    RMSG_LAST,           /* Last Read Message */

    /* Static Messages */
	SMSG_NOP,
	SMSG_COMMAND_REG,
	SMSG_TMODE_REG,
	SMSG_TPRESCALER_REG,
	SMSG_TRELOAD_MSB_REG,
	SMSG_TRELOAD_LSB_REG,
	SMSG_TX_ASK_REG,
	SMSG_MODE_REG,
	SMSG_RF_CFG_REG,
	SMSG_TX_CONTROL_REG,
	SMSG_COM_IEN_REG,
	SMSG_DIV_EN_REG,
	SMSG_LAST,

    /* Dynamic Messages */
	DMSG_COMMAND_REG,
	DMSG_COM_IRQ_REG,
	DMSG_FIFO_LEVEL_REG,
	DMSG_FIFO_DATA_REG,
	DMSG_BIT_FRAMING_REG,
	DMSG_LAST,
	DMSG_ANY_REG,
	MSG_LIST_END,
} E_NFC_msgIdType;

/******************************** Sequence IDs ********************************/
typedef enum E_NFC_seqIdType_
{
    /***************** 48-Bit Sequences *****************/
	MC_SOFTRESET = 0x00u ,         /* SoftReset the NFC device*/
    MC_READDEVID,                  /* Read Device ID */
    MC_LAST_SEQ                    /* Last Sequence Constant */
} E_NFC_seqIdType;

typedef enum E_NFC_MCCommandType_
{
	MC_NFC_WRITE,
    MC_NFC_READ,
	MC_NFC_NOP
} E_NFC_MCCommandType;

/* Read back Register Addresses */
typedef enum E_NFC_regAddrType_
{
    MC_REGADDR_RESERVED           = 0x00u,
    MC_REGADDR_COMMANDREG         = 0x01u,
    MC_REGADDR_COMIENRRG          = 0x02u,
    MC_REGADDR_DIVIENREG          = 0x03u,
    MC_REGADDR_COMIRQREG          = 0x04u,
    MC_REGADDR_DIVIRQREG          = 0x05u,
    MC_REGADDR_ERRORREG           = 0x06u,
    MC_REGADDR_STATUS1REG         = 0x07u,
    MC_REGADDR_STATUS2REG         = 0x08u,
    MC_REGADDR_FIFODATAREG        = 0x09u,
    MC_REGADDR_FIFOLEVELREG       = 0x0Au,
    MC_REGADDR_WATERLEVELREG      = 0x0Bu,
    MC_REGADDR_CONTROLREG         = 0x0Cu,
    MC_REGADDR_BITFRAMINGREG      = 0x0Du,
    MC_REGADDR_COLLREG            = 0x0Eu,
    MC_REGADDR_MODEREG            = 0x11u,
    MC_REGADDR_TXMODEREG          = 0x12u,
    MC_REGADDR_RXMODEREG          = 0x13u,
    MC_REGADDR_TXCONTROLREG       = 0x14u,
    MC_REGADDR_TXASKREG           = 0x15u,
    MC_REGADDR_TXSELREG           = 0x16u,
    MC_REGADDR_RXSELREG           = 0x17u,
    MC_REGADDR_RXTHRESHOLDREG     = 0x18u,
    MC_REGADDR_DEMODREG           = 0x19u,
    MC_REGADDR_MFTXREG            = 0x1Cu,
    MC_REGADDR_MFRXREG            = 0x1Du,
    MC_REGADDR_SERIALSPEEDREG     = 0x1Fu,
    MC_REGADDR_CRCRESULTREGMSB    = 0x21u,
    MC_REGADDR_CRCRESULTREGLSB    = 0x22u,
    MC_REGADDR_MODWIDTHREG        = 0x24u,
    MC_REGADDR_RFCFGREG           = 0x26u,
    MC_REGADDR_GSNREG             = 0x27u,
    MC_REGADDR_CWGSPREG           = 0x28u,
    MC_REGADDR_MODGSPREG          = 0x29u,
    MC_REGADDR_TMODEPREG          = 0x2Au,
    MC_REGADDR_TPRESCALERREG      = 0x2Bu,
    MC_REGADDR_TRELOADREGMSB      = 0x2Cu,
    MC_REGADDR_TRELOADREGLSB      = 0x2Du,
    MC_REGADDR_TCOUNTERVALREGMSB  = 0x2Eu,
    MC_REGADDR_TCOUNTERVALREGLSB  = 0x2Fu,
    MC_REGADDR_TESTSEL1REG        = 0x31u,
    MC_REGADDR_TESTSEL2REG        = 0x32u,
    MC_REGADDR_TESTPINENREG       = 0x33u,
    MC_REGADDR_TESTPINVALUEREG    = 0x34u,
    MC_REGADDR_TESTBUSREG         = 0x35u,
    MC_REGADDR_AUTOTESTREG        = 0x36u,
    MC_REGADDR_VERSIONREG         = 0x37u,
    MC_REGADDR_ANALOGTESTREG      = 0x38u,
    MC_REGADDR_TESTDAC1REG        = 0x39u,
    MC_REGADDR_TESTDAC2REG        = 0x3Au,
    MC_REGADDR_TESTADCREG         = 0x3Bu,
    MC_REGADDR_INVALIDREG         = 0x40u,
} E_NFC_regAddrType_;

/* Read back Register Indexes */
typedef enum E_NFC_regIndexType_
{
    MC_REGINDEX_COMMANDREG,
    MC_REGINDEX_COMIENRRG ,
    MC_REGINDEX_DIVIENREG,
    MC_REGINDEX_COMIRQREG,
    MC_REGINDEX_DIVIRQREG,
    MC_REGINDEX_ERRORREG,
    MC_REGINDEX_STATUS1REG,
    MC_REGINDEX_STATUS2REG,
    MC_REGINDEX_FIFODATAREG,
    MC_REGINDEX_FIFOLEVELREG,
    MC_REGINDEX_WATERLEVELREG,
    MC_REGINDEX_CONTROLREG,
    MC_REGINDEX_BITFRAMINGREG,
    MC_REGINDEX_COLLREG,
    MC_REGINDEX_MODEREG,
    MC_REGINDEX_TXMODEREG,
    MC_REGINDEX_RXMODEREG,
    MC_REGINDEX_TXCONTROLREG,
    MC_REGINDEX_TXASKREG,
    MC_REGINDEX_TXSELREG,
    MC_REGINDEX_RXSELREG,
    MC_REGINDEX_RXTHRESHOLDREG,
    MC_REGINDEX_DEMODREG,
    MC_REGINDEX_MFTXREG,
    MC_REGINDEX_MFRXREG,
    MC_REGINDEX_SERIALSPEEDREG,
    MC_REGINDEX_CRCRESULTREGMSB,
    MC_REGINDEX_CRCRESULTREGLSB,
    MC_REGINDEX_MODWIDTHREG,
    MC_REGINDEX_RFCFGREG,
    MC_REGINDEX_GSNREG,
    MC_REGINDEX_CWGSPREG,
    MC_REGINDEX_MODGSPREG,
    MC_REGINDEX_TMODEPREG,
    MC_REGINDEX_TPRESCALERREG,
    MC_REGINDEX_TRELOADREGMSB,
    MC_REGINDEX_TRELOADREGLSB,
    MC_REGINDEX_TCOUNTERVALREGMSB,
    MC_REGINDEX_TCOUNTERVALREGLSB,
    MC_REGINDEX_TESTSEL1REG,
    MC_REGINDEX_TESTSEL2REG,
    MC_REGINDEX_TESTPINENREG,
    MC_REGINDEX_TESTPINVALUEREG,
    MC_REGINDEX_TESTBUSREG,
    MC_REGINDEX_AUTOTESTREG,
    MC_REGINDEX_VERSIONREG,
    MC_REGINDEX_ANALOGTESTREG,
    MC_REGINDEX_TESTDAC1REG,
    MC_REGINDEX_TESTDAC2REG,
    MC_REGINDEX_TESTADCREG,
    MC_REGINDEX_RESERVED,
} E_NFC_regIndexType;

typedef enum {
	READ_SEQUENCES,
	WRITE_SEQUENCES
}Frame_Type_t;
/* Datatype for Sequences */
typedef struct NFC_seqType_
{
    E_NFC_seqIdType id;
    uint8_t msgCount;
    uint16_t timeStall;
    const E_NFC_msgIdType *msgList;
    Frame_Type_t FrameType;
} NFC_seqType;

/* Datatype for 16bit write message */
typedef struct NFC_msg16BF_
{
   uint8_t rfu       :1u;
   uint8_t reg       :6u;
   uint8_t rw        :1u;
   uint8_t data;
}NFC_msg16BF;

/* Access 16bit in different ways */
typedef union NFC_msg16Type_
{
    uint16_t   u16;
    uint8_t  u8[2];
    NFC_msg16BF bf;
}NFC_msg16Type;
/*******************************************************************************
**    External Linkages
*******************************************************************************/
extern const NFC_msg16Type gauni_vMsgsList[MSG_LIST_END];
extern const NFC_seqType gastr_vSeqList[MC_LAST_SEQ];
extern const E_NFC_regIndexType genu_MCRegAddrToIndex[MC_REGADDR_INVALIDREG + 1u];

extern const E_NFC_msgIdType gau8_vMsgList_MC_SOFTRESET[MSGCNT_MC_SOFTRESET];
extern const E_NFC_msgIdType gau8_vMsgList_MC_READDEVID[MSGCNT_MC_READDEVID];

#endif /* SOURCES_CDD_NFC_CDD_CFG_H_ */
