/**
 ****************************************************************************************
 *
 * @file otas_task.h
 *
 * @brief Header file - Quintic Private Profile Server Task.
 *
 * Copyright (C) Quintic 2013-2013
 *
 * $Rev: $
 *
 ****************************************************************************************
 */

#ifndef _OTAS_TASK_H_
#define _OTAS_TASK_H_

/// @cond

/**
 ****************************************************************************************
 * @addtogroup OTASTASK OTA Profile Server
 * @ingroup OTAS
 * @brief OTA Profile Server
 *
 * The OTAS_TASK is responsible for handling the messages coming in and out of the
 * @ref OTAS block of the BLE Host.
 *
 * @{
 ****************************************************************************************
 */


/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "app_config.h"

#if (BLE_OTA_SERVER)

#include <stdint.h>
#include "ke_task.h"

/*
 * DEFINES
 ****************************************************************************************
 */

/// Default firmware 2 address, it is limited from 0x04000 (firmware 1 address) to 0x20000 (flash size).
#define OTAS_FW2_ADDRESS  0x12000

/// Default decrypt key
#define OTAS_DECRYPT_KEY  ((uint8_t *)"\x11\x22\x33\x44\x55\x66\x77\x88\x99\x00\xAA\xBB\xCC\xDD\xEE\xFF")

// Check firmware 2 address
#if OTAS_FW2_ADDRESS >= 0x20000 || OTAS_FW2_ADDRESS <= 0x04000
    #error "The address of firmware2 is invalid. It is limited from 0x04000 to 0x20000"
#endif

#undef TASK_OTAS
#define TASK_OTAS TASK_PRF7

/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */

/// @endcond


/**
 ****************************************************************************************
 * @addtogroup OTAS
 * @{
 ****************************************************************************************
 */

enum
{
    OTAS_SVC_PRIVATE_UUID          = 0xFEE8,
};

/// Possible Returned Status
enum ota_status_t
{
    OTA_STATUS_OK,
    OTA_STATUS_FW2_ADDR_INVALID,
    OTA_STATUS_DEVICE_NOT_SUPPORT_OTA,
};

/// ota encrypt
enum ota_crypt_t
{
    OTA_ENABLE_ENCRYPT,
    OTA_DISABLE_ENCRYPT,
};

/// Possible states of the OTASS task
enum
{
    /// Disabled state
    OTAS_DISABLED,
    /// Idle state
    OTAS_IDLE,
    /// Connected state
    OTAS_CONNECTED,

    /// Number of defined states.
    OTAS_STATE_MAX,
};

/// Messages for Quintic OTA Server
enum
{
    ///Start the Quintic private Profile Server - at connection
    OTAS_ENABLE_REQ = KE_FIRST_MSG(TASK_OTAS),
    ///Disable profile role - at disconnection
    OTAS_DISABLE_REQ,
    ///Disable confirmation with configuration to save after profile disabled
    OTAS_DISABLE_CFM,
    ///Error indication to Host
    OTAS_ERROR_IND,
    ///Add OTASS into the database
    OTAS_CREATE_DB_REQ,
    ///Inform APP about DB creation status
    OTAS_CREATE_DB_CFM,
    ///Status of ota trasimition indication to app
    OTAS_TRANSIMIT_STATUS_IND,
    /// App send this message to control the otas start
    OTAS_CONTRL_APP_RESP,
    /// Time of wait for the start control message from app
    OTAS_CONTRL_TIMER,    
};

/**
    notify ota transimition status to app, 
    all the faults are in one type
*/
enum otas_Transimit_Status
{   
    ///get the meta data right and send app request to goon send brick data.
    OTA_STATUS_START_REQ = 0,
    
    ///ONGOING send recevied bytes
    OTA_STATUS_ONGOING,
    
    ///ok and STOP
    OTA_STATUS_FINISH_OK,
   
    ///fail and stop
    OTA_STATUS_FINISH_FAIL,
};



/// OTA start control flag
enum otas_ctrl_resp
{
    /// old way, control by profile
    START_OTA = 0,
    /// new way, control by app
    REJECT_OTA,
};


/// OTA start control flag
enum otas_ctrl_flag
{
    /// old way, control by profile
    PROFILE_CONTROL = 0,
    /// new way, control by app
    APP_CONTROL,
};


/*
 * APIs Structures
 ****************************************************************************************
 */

///Parameters of the @ref OTAS_CREATE_DB_REQ message
struct otas_create_db_req
{
    /// OTAS database configuration
    uint8_t features;
    /// RX characteristic number
    uint8_t rx_char_num;
};

/// Parameters of the @ref OTAS_ENABLE_REQ message
struct otas_enable_req
{
    ///Connection handle
    uint16_t conhdl;
    /// security level: b0= nothing, b1=unauthenticated, b2=authenticated, b3=authorized;
    /// b1 or b2 and b3 can go together
    uint8_t sec_lvl;
    ///Type of connection - will someday depend on button press length; can be CFG or DISCOVERY
    uint8_t con_type;
    /// Notification configuration
    uint32_t ntf_en;
};

/// Parameters of the @ref OTAS_DISABLE_REQ message
struct otas_disable_req
{
    ///Connection handle
    uint16_t conhdl;
};

///Parameters of the @ref OTAS_CREATE_DB_CFM message
struct otas_create_db_cfm
{
    ///Status
    uint8_t status;
};

///Parameters of the @ref OTAS_DISABLE_CFM message
struct otas_disable_cfm
{
    ///Connection handle
    uint16_t conhdl;
    /// Notification configuration
    uint16_t ntf_en;
};

///Parameters of the @ref OTAS_ERROR_IND message
struct otas_error_ind
{
    ///Connection handle
    uint16_t conhdl;
    ///Status
    uint8_t  status;
};


///Parameters of the @ref OTAS_TRASIMIT_STATUS_IND message
struct otas_transimit_status_ind
{
    ///Status
    enum otas_Transimit_Status  status;
    /** Details about the ota status
        Status                  | status detail description information 
    -	OTA_STATUS_START_REQ    : Total size
    -	OTA_STATUS_ONGOING      : Received bytes
    -	OTA_STATUS_FINISH_OK    : NULL 
    -	OTA_STATUS_FINISH_FAIL  : Error type
    */
    uint16_t status_des;
};

///Parameters of the otas start control relevant.
struct otas_ctrl_info
{
    ///OTA start control flag : by profile or app
    enum otas_ctrl_flag ctrl_flag; 
    
    /// reseved for future    
    uint8_t reserved;
};


/// Parameters of the @ref OTAS_CONTRL_APP_RESP message
struct otas_ctrl_resp_req
{
    /// Security level
    enum otas_ctrl_resp ctrl_resp;

};

/// The app information of active or inactive region
struct otas_app_information_t
{
    uint32_t inactive_app_start_addr;   // inactive flash block start address
    uint32_t inactive_app_end_addr;     // inactive flash block end address
};
/// @}OTAS


/*
 * TASK DESCRIPTOR DECLARATIONS
 ****************************************************************************************
 */

/// @cond
/**
 ****************************************************************************************
 * @brief Register otas descriptor
 ****************************************************************************************
 */
void task_otas_desc_register(void);

/// @endcond


/// @cond
/**
 ****************************************************************************************
 * @brief Initialization of the OTA Server module.
 * @param[in] fw2_start_addr  Firmware 2 start address. It is limited from 0x04000 
 *                            (firmware 1 address) to 0x20000 (flash size).
 * @param[in] crypt           - OTA_ENABLE_ENCRYPT: Enable encrypt.
 *                            - OTA_DISABLE_ENCRYPT: Disable encrypt.
 * @param[in] key             If crypt is OTA_ENCRYPT, the key is AES 128 key (16bytes).
 *                            Otherwise the key is ignored (it may be set to NULL).
 *
 * @returns 
 * - OTA_STATUS_OK : OTAS initialize successfully.
 * - OTA_STATUS_FW2_ADDR_INVALID : OTAS initialize fail, firmware address is invalid.
 * - OTA_STATUS_DEVICE_NOT_SUPPORT_OTA : OTAS initialize fail, the device is not supported ota.
 *                                       And the device firmware is not dowloaded by "QnISPStudio"
 *   
 * @description
 * This function performs all the initializations of the OTAS module.
 ****************************************************************************************
 */
enum ota_status_t otas_init(uint32_t fw2_start_addr, enum ota_crypt_t crypt, const uint8_t key[16]);


/**
 ****************************************************************************************
 * @brief Initialization of the OTA start control method.
 * @param[in] *pctrl_info  pointer to the userco
 *
 * @description
 * ota start control by app or by profile initialize
 ****************************************************************************************
 */
void otas_control(struct otas_ctrl_info *pctrl_info);

/**
 ****************************************************************************************
 * @brief Create OTA Server service database - at initiation
 * @param[in] rx_char_num    OTA RX characteristic number.
 * @response OTAS_CREATE_DB_CFM
 *
 * @description
 *  Create the OTA service database - at initiation.
 ****************************************************************************************
 */
void app_otas_create_db(void);

/*
 ****************************************************************************************
 * @brief Start the OTA Server profile - at connection     *//**
 *
 * @param[in] conhdl Connection handle for which the OTA Server role is enabled
 * @param[in] sec_lvl Security level required for protection of attributes.
 * Service Hide and Disable are not permitted. Possible values are:
 * - PERM_RIGHT_ENABLE : Enable access
 * - PERM_RIGHT_UNAUTH : Access Requires Unauthenticated link
 * - PERM_RIGHT_AUTH   : Access Requires Authenticated link
 *
 * @response None
 * @description
 * This function is used for enabling the Reporter role of the Proximity profile.
 * After calling this function, the services are unhidden from the peer discove.
 ****************************************************************************************
 */
void app_otas_enable_req(uint16_t conhdl, uint8_t sec_lvl);

/**
 ****************************************************************************************
 * @brief Disable profile role - at disconnection
 * @param[in] conhdl    Handle of connect
 * @response PRF_ERR_INVALID_PARAM or OTAS_DISABLE_CFM
 *
 * @description
 *  Stop the OTA service server - at connection.
 ****************************************************************************************
 */
void app_otas_disable_req(uint16_t conhdl);

/*
 ****************************************************************************************
 * @brief response the ota to start data trasfer or reject  *//**
 * @param[in] ctrl_resp Connection handle for which the profile Reporter role is enabled
 *  - START_OTA: old way, control by profile
 *  - REJECT   : control by app
 *   
 * @response None
 * @description
 * response the ota to start data trasfer or reject
 * This will transfer msg OTAS_CONTRL_APP_RESP to ota server to stop or stop 
 ****************************************************************************************
 */
void app_ota_ctrl_resp(enum otas_ctrl_resp ctrl_resp);

/*
 ****************************************************************************************
 * @brief   get otas the inactive flash block information*//**
 * @param[in] ota_app_information information of inactive flash block
 * @response  
 *          - true : get the rigth information
 *          - false : program is in ota communication 
 * @description
 * This function must be called after calling OTAS_INIT()
 * get otas the inactive flash block information : 
 * -  app inactive flash block start address
 * -  app inactive flash block end address
  ****************************************************************************************
 */
uint8_t otas_get_app_info(struct otas_app_information_t  *ota_app_information);



#endif /* BLE_OTA_SERVER */

/// @} OTASSTASK
/// @endcond

#endif /* _OTAS_TASK_H_ */
