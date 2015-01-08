/**
 ****************************************************************************************
 *
 * @file co_bt.h
 *
 * @brief This file contains the common Bluetooth defines, enumerations and structures
 *        definitions for use by all modules in BLE stack.
 *
 * Copyright (C) RivieraWaves 2009-2012
 *
 * $Rev: $
 *
 ****************************************************************************************
 */
/**
 ****************************************************************************************
 *
 * @file co_bt.h
 *
 * @brief Quintic revised and cut.
 *
 * Copyright (C) Quintic 2012-2013
 *
 * $Rev: 1.0 $
 *
 ****************************************************************************************
 */
#ifndef CO_BT_H_
#define CO_BT_H_

/**
 ****************************************************************************************
 * @addtogroup CO_BT Common Bluetooth defines
 * @ingroup COMMON
 * @brief Common Bluetooth definitions and structures.
 *
 * @{
 ****************************************************************************************
 */


/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include <stdint.h>
#include <stdbool.h>


/*
 * DEFINES
 ****************************************************************************************
 */

///Length of fields in Bluetooth messages, in number of bytes
#define EVT_MASK_LEN        0x08
#define BD_ADDR_LEN         0x06
#define ACCESS_ADDR_LEN     0x04
#define LE_PASSKEY_LEN      0x04
#define BD_NAME_SIZE        0x20
#define ADV_DATA_LEN        0x1F
#define BLE_DATA_LEN        0x1B
#define SCAN_RSP_DATA_LEN   0x1F
#define LE_CHNL_MAP_LEN     0x05
#define KEY_LEN             0x10
#define CFM_LEN             0x10
#define ENC_DATA_LEN        0x10
#define RAND_VAL_LEN        0x10
#define RAND_NB_LEN         0x08
#define LE_FEATS_LEN        0x08
#define SUPP_CMDS_LEN       0x40
#define LMP_FEATS_LEN       0x08
#define LE_STATES_LEN       0x08
#define WHITE_LIST_LEN      0x0A
#define LE_FREQ_LEN         0x28
#define LE_FREQ_LEN         0x28
#define CRC_INIT_LEN        0x03
#define SESS_KEY_DIV_LEN    0x08
#define INIT_VECT_LEN       0x04
#define MIC_LEN             0x04

/*
 * ENUMERATIONS
 ****************************************************************************************
 */

///Advertising HCI Type
enum
{
    ///Connectable Undirected advertising
    ADV_CONN_UNDIR                = 0x00,
    ///Connectable directed advertising
    ADV_CONN_DIR,
    ///Discoverable undirected advertising
    ADV_DISC_UNDIR,
    ///Non-connectable undirected advertising
    ADV_NONCONN_UNDIR,
    ///Enumeration end value for advertising type value check
    ADV_END
};

///BD address type
enum
{
    ///Public BD address
    ADDR_PUBLIC                   = 0x00,
    ///Random BD Address
    ADDR_RAND,
    ///Enumeration end value for BD address type value check
    ADDR_END
};

///Advertising channels enables
enum
{
    ///Byte value for advertising channel map for channel 37 enable
    ADV_CHNL_37_EN                = 0x01,
    ///Byte value for advertising channel map for channel 38 enable
    ADV_CHNL_38_EN,
    ///Byte value for advertising channel map for channel 39 enable
    ADV_CHNL_39_EN                = 0x04,
    ///Byte value for advertising channel map for channel 37, 38 and 39 enable
    ADV_ALL_CHNLS_EN              = 0x07,
    ///Enumeration end value for advertising channels enable value check
    ADV_CHNL_END
};

///Advertising filter policy
enum
{
    ///Allow both scan and connection requests from anyone
    ADV_ALLOW_SCAN_ANY_CON_ANY    = 0x00,
    ///Allow both scan req from White List devices only and connection req from anyone
    ADV_ALLOW_SCAN_WLST_CON_ANY,
    ///Allow both scan req from anyone and connection req from White List devices only
    ADV_ALLOW_SCAN_ANY_CON_WLST,
    ///Allow scan and connection requests from White List devices only
    ADV_ALLOW_SCAN_WLST_CON_WLST,
    ///Enumeration end value for advertising filter policy value check
    ADV_ALLOW_SCAN_END
};

///Advertising enables
enum
{
    ///Disable advertising
    ADV_DIS                       = 0x00,
    ///Enable advertising
    ADV_EN,
    ///Enumeration end value for advertising enable value check
    ADV_EN_END
};

///LE Scan type
enum
{
    ///Passive scan
    SCAN_PASSIVE                  = 0x00,
    ///Active scan
    SCAN_ACTIVE,
    ///Enumeration end value for scan type value check
    SCAN_END
};

///Scan filter policy
enum
{
    ///Allow advertising packets from anyone
    SCAN_ALLOW_ADV_ALL            = 0x00,
    ///Allow advertising packets from White List devices only
    SCAN_ALLOW_ADV_WLST,
    ///Enumeration end value for scan filter policy value check
    SCAN_ALLOW_ADV_END
};

///Le Scan enables
enum
{
    ///Disable scan
    SCAN_DIS                      = 0x00,
    ///Enable scan
    SCAN_EN,
    ///Enumeration end value for scan enable value check
    SCAN_EN_END
};

///Filter duplicates
enum
{
    ///Disable filtering of duplicate packets
    SCAN_FILT_DUPLIC_DIS          = 0x00,
    ///Enable filtering of duplicate packets
    SCAN_FILT_DUPLIC_EN,
    ///Enumeration end value for scan duplicate filtering value check
    SCAN_FILT_DUPLIC_END
};

///Initiator Filter policy
enum
{
    ///Initiator will ignore White List
    INIT_FILT_IGNORE_WLST         = 0x00,
    ///Initiator will use White List
    INIT_FILT_USE_WLST,
    ///Enumeration end value for initiator filter policy value check
    INIT_FILT_END
};

/// Constant defining the role
enum
{
    ///Master role
    ROLE_MASTER,
    ///Slave role
    ROLE_SLAVE,
    ///Enumeration end value for role value check
    ROLE_END
};

/*
 * STRUCTURE DEFINITONS
 ****************************************************************************************
 */

///BD name structure
struct bd_name
{
    ///length for name
    uint8_t  namelen;
    ///array of bytes for name
    uint8_t  name[BD_NAME_SIZE];
};

///BD Address structure
struct bd_addr
{
    ///6-byte array address value
    uint8_t  addr[BD_ADDR_LEN];
};

///Advertising data structure
struct adv_data
{
    ///Maximum length data bytes array
    uint8_t        data[ADV_DATA_LEN];
};

///Scan response data structure
struct scan_rsp_data
{
    ///Maximum length data bytes array
    uint8_t        data[SCAN_RSP_DATA_LEN];
};

///Channel map structure
struct le_chnl_map
{
    ///5-byte channel map array
    uint8_t map[LE_CHNL_MAP_LEN];
};

///Random number structure
struct rand_nb
{
    ///8-byte array for random number
    uint8_t     nb[RAND_NB_LEN];
};

///Advertising report structure
struct adv_report
{
    ///Event type
    uint8_t        evt_type;
    ///Advertising address type: public/random
    uint8_t        adv_addr_type;
    ///Advertising address value
    struct bd_addr adv_addr;
    ///Data length in advertising packet
    uint8_t        data_len;
    ///Data of advertising packet
    uint8_t        data[ADV_DATA_LEN];
    ///RSSI value for advertising packet
    int8_t        rssi;
};

///Supported LE Features structure
struct le_features
{
    ///8-byte array for LE features
    uint8_t feats[LE_FEATS_LEN];
};

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */
/**
 ****************************************************************************************
 * @brief Compares two Bluetooth device addresses
 *
 * This function checks if the two bd address are equal.
 *
 * @param[in] bd_address1        Pointer on the first bd address to be compared.
 * @param[in] bd_address2        Pointer on the second bd address to be compared.
 *
 * @return result of the comparison (true or false).
 *
 ****************************************************************************************
 */
typedef bool (*p_co_bt_bdaddr_compare)(struct bd_addr const *bd_address1,
                                       struct bd_addr const *bd_address2);
#define co_bt_bdaddr_compare ((p_co_bt_bdaddr_compare)(_co_bt_bdaddr_compare))

/// @} CO_BT
#endif // CO_BT_H_
