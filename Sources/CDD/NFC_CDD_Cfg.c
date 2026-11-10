/*
 * NFC_CDD_Cfg.c
 *
 *  Created on: 30-Aug-2026
 *      Author: Shardul
 */

#include "NFC_CDD_Cfg.h"

const E_NFC_msgIdType gau8_vMsgList_MC_SOFTRESET[MSGCNT_MC_SOFTRESET] =
{
    SMSG_COMMAND_REG
};

const E_NFC_msgIdType gau8_vMsgList_MC_READDEVID[MSGCNT_MC_READDEVID] =
{
    RMSG_VERSION_REG,
    RMSG_NULL_REG,
};

/*******************************************************************************
 **    Global Variables
 *******************************************************************************/
/********************************** Messages **********************************/
const NFC_msg16Type gauni_vMsgsList[MSG_LIST_END] =
{
    /****************************** Read Message ******************************/
    {0x0080u}, /* RMSG_RESERVED */
    {0x0082u}, /* RMSG_COMMAND_REG */
    {0x0084u}, /* RMSG_COM_IEN_REG */
    {0x0086u}, /* RMSG_DIV_IEN_REG */
    {0x0088u}, /* RMSG_COM_IRQ_REG */
    {0x008Au}, /* RMSG_DIV_IRQ_REG */
    {0x008Cu}, /* RMSG_ERROR_REG */
    {0x008Eu}, /* RMSG_STATUS1_REG */
    {0x0090u}, /* RMSG_STATUS2_REG */
    {0x0094u}, /* RMSG_FIFO_DATA_REG */
    {0x0096u}, /* RMSG_WATER_LEVEL_REG */
    {0x0098u}, /* RMSG_CONTROL_REG */
    {0x009Au}, /* RMSG_BIT_FRAMING_REG */
    {0x009Cu}, /* RMSG_COLL_REG */
    {0x00A2u}, /* RMSG_MODE_REG */
    {0x00A4u}, /* RMSG_TX_MODE_REG */
    {0x00A6u}, /* RMSG_RX_MODE_REG */
    {0x00A8u}, /* RMSG_TX_CONTROL_REG */
    {0x00AAu}, /* RMSG_TX_ASK_REG */
    {0x00ACu}, /* RMSG_TX_SEL_REG */
    {0x00AEu}, /* RMSG_RX_SEL_REG */
    {0x00B0u}, /* RMSG_RX_THRESHOLD_REG */
    {0x00B2u}, /* RMSG_DEMOD_REG */
    {0x00B8u}, /* RMSG_MF_TX_REG */
    {0x00BAu}, /* RMSG_MF_RX_REG */
    {0x00BEu}, /* RMSG_SERIAL_SPEED_REG */
    {0x00C2u}, /* RMSG_CRC_RESULT_MSB_REG */
    {0x00C4u}, /* RMSG_CRC_RESULT_LSB_REG */
    {0x00C8u}, /* RMSG_MODE_WIDTH_REG */
    {0x00CCu}, /* RMSG_RF_CFG_REG */
    {0x00CEu}, /* RMSG_GSN_REG */
    {0x00D0u}, /* RMSG_CWGSP_REG */
    {0x00D2u}, /* RMSG_MODGSP_REG */
    {0x00D4u}, /* RMSG_TMODE_REG */
    {0x00D6u}, /* RMSG_TPRESCALER_REG */
    {0x00D8u}, /* RMSG_TRELOAD_MSB_REG */
    {0x00DAu}, /* RMSG_TRELOAD_LSB_REG */
    {0x00DCu}, /* RMSG_TCOUNTER_VAL_LSB */
    {0x00DEu}, /* RMSG_TCOUNTER_VAL_MSB */
    {0x00E2u}, /* RMSG_TEST_SEL1_REG */
    {0x00E4u}, /* RMSG_TEST_SEL2_REG */
    {0x00E6u}, /* RMSG_TEST_PIN_EN_REG */
    {0x00E8u}, /* RMSG_TEST_PIN_VAL_REG */
    {0x00EAu}, /* RMSG_TEST_BUS_REG */
    {0x00ECu}, /* RMSG_AUTO_TEST_REG */
    {0x00EEu}, /* RMSG_VERSION_REG */
    {0x00F0u}, /* RMSG_ANALOG_TEST_REG */
    {0x00F2u}, /* RMSG_TEST_DAC1_REG */
    {0x00F4u}, /* RMSG_TEST_DAC2_REG */
    {0x00F6u}, /* RMSG_AUTO_TEST_REG */
    {0x0000u}, /* RMSG_NULL_REG */
    {0x0000u}, /* RMSG_LAST */
    /***************************** Writes Messages ******************************/
    {0x0000u}, /* SMSG_NOP */
    {0x020Fu}, /* SMSG_COMMAND_REG */
    {0x548Du}, /* SMSG_TMODE_REG */
    {0x563Eu}, /* SMSG_TPRESCALER_REG */
    {0x5800u}, /* SMSG_TRELOAD_MSB_REG */
    {0x5A1Eu}, /* SMSG_TRELOAD_LSB_REG */
    {0x2A40u}, /* SMSG_TX_ASK_REG */
    {0x223Du}, /* SMSG_MODE_REG */
    {0x4C70u}, /* SMSG_RF_CFG_REG */
    {0x2883u}, /* SMSG_TX_CONTROL_REG */
    {0x04A3u}, /* SMSG_COM_IEN_REG */
    {0x0600u}, /* SMSG_DIV_EN_REG */
    {0x0000u}, /* SMSG_LAST */
    /****************************** Dynamic Message ******************************/
    {0x0200u}, /* DMSG_COMMAND_REG */
    {0x0800u}, /* DMSG_COM_IRQ_REG */
    {0x1400u}, /* DMSG_FIFO_LEVEL_REG */
    {0x1200u}, /* DMSG_FIFO_DATA_REG */
    {0x1A00u}, /* DMSG_BIT_FRAMING_REG */
    {0x0000u}, /* DMSG_LAST */
    {0x0000u}, /* DMSG_ANY_REG */
};
/**************************** Sequence Definitions*****************************/
const NFC_seqType gastr_vSeqList[MC_LAST_SEQ] =
{
    /*   seqId,                    msgCount,             timeStall,                msgList,                           FrameType   */
    {MC_SOFTRESET,           MSGCNT_MC_SOFTRESET,           0u,             gau8_vMsgList_MC_SOFTRESET,            WRITE_SEQUENCES},
    {MC_READDEVID,           MSGCNT_MC_READDEVID,           0u,             gau8_vMsgList_MC_READDEVID,            READ_SEQUENCES},
};

const E_NFC_regIndexType genu_MCRegAddrToIndex[MC_REGADDR_INVALIDREG + 1u] = {
    MC_REGINDEX_RESERVED,
	MC_REGINDEX_RESERVED,
    MC_REGINDEX_COMMANDREG,
    MC_REGINDEX_COMIENRRG,
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
	MC_REGINDEX_RESERVED,
    MC_REGINDEX_MODEREG,
    MC_REGINDEX_TXMODEREG,
    MC_REGINDEX_RXMODEREG,
    MC_REGINDEX_TXCONTROLREG,
    MC_REGINDEX_TXASKREG,
    MC_REGINDEX_TXSELREG,
    MC_REGINDEX_RXSELREG,
    MC_REGINDEX_RXTHRESHOLDREG,
    MC_REGINDEX_DEMODREG,
    MC_REGINDEX_RESERVED,
    MC_REGINDEX_RESERVED,
    MC_REGINDEX_MFTXREG,
    MC_REGINDEX_MFRXREG,
    MC_REGINDEX_RESERVED,
    MC_REGINDEX_SERIALSPEEDREG,
    MC_REGINDEX_RESERVED,
    MC_REGINDEX_CRCRESULTREGMSB,
    MC_REGINDEX_CRCRESULTREGLSB,
    MC_REGINDEX_RESERVED,
    MC_REGINDEX_MODWIDTHREG,
    MC_REGINDEX_RESERVED,
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
	MC_REGINDEX_RESERVED,
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
};
