/**
 ****************************************************************************************
 *
 * @file app_menu.c
 *
 * @brief Application DEMO menu
 *
 * Copyright (C) Quintic 2009-2013
 *
 * $Rev: $
 *
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "app_env.h"

static void app_menu_show(void);

static void app_menu_show_line(void)
{
    QPRINTF("*------------------------\r\n");
}

/**
*************************************************************************************
*** Show profile menu
*************************************************************************************
 */
static void app_menu_show_main(void)
{
    app_menu_show_line();
    QPRINTF("* QN BLE Main Menu\r\n");
    QPRINTF("* 1. GAP   Menu\r\n");
#if BLE_HR_COLLECTOR
    QPRINTF("* 2. HRPC  Menu\r\n");
#endif
#if BLE_HR_SENSOR
    QPRINTF("* 3. HRPS  Menu\r\n");
#endif
#if BLE_PROX_MONITOR
    QPRINTF("* 4. PROXM Menu\r\n");
#endif
#if BLE_FINDME_LOCATOR
    QPRINTF("* 6. FINDL Menu\r\n");
#endif
#if BLE_DIS_CLIENT
    QPRINTF("* 8. DISC Menu\r\n");
#endif
#if BLE_DIS_SERVER
    QPRINTF("* 9. DISS Menu\r\n");
#endif
#if BLE_BATT_CLIENT
    QPRINTF("* a. BASC Menu\r\n");
#endif
#if BLE_BATT_SERVER
    QPRINTF("* b. BASS Menu\r\n");
#endif
#if BLE_SP_CLIENT
    QPRINTF("* c. SCPPC Menu\r\n");
#endif
#if BLE_SP_SERVER
    QPRINTF("* d. SCPPS Menu\r\n");
#endif
#if BLE_HT_COLLECTOR
    QPRINTF("* e. HTPC Menu\r\n");
#endif
#if BLE_HT_THERMOM
    QPRINTF("* f. HTPT Menu\r\n");
#endif
#if BLE_BP_COLLECTOR
    QPRINTF("* g. BLPC Menu\r\n");
#endif
#if BLE_BP_SENSOR
    QPRINTF("* h. BLPS Menu\r\n");
#endif
#if BLE_TIP_CLIENT
    QPRINTF("* i. TIPC Menu\r\n");
#endif
#if BLE_TIP_SERVER
    QPRINTF("* j. TIPS Menu\r\n");
#endif
#if BLE_GL_COLLECTOR
    QPRINTF("* k. GLPC Menu\r\n");
#endif
#if BLE_GL_SENSOR
    QPRINTF("* m. GLPS Menu\r\n");
#endif
#if BLE_AN_CLIENT
    QPRINTF("* n. ANPC Menu\r\n");
#endif
#if BLE_AN_SERVER
    QPRINTF("* o. ANPS Menu\r\n");
#endif
#if BLE_CSC_COLLECTOR
    QPRINTF("* p. CSCPC Menu\r\n");
#endif
#if BLE_CSC_SENSOR
    QPRINTF("* q. CSCPS Menu\r\n");
#endif
#if BLE_PAS_CLIENT
    QPRINTF("* t. PASPC Menu\r\n");
#endif
#if BLE_PAS_SERVER
    QPRINTF("* u. PASPS Menu\r\n");
#endif
#if BLE_RSC_COLLECTOR
    QPRINTF("* v. RSCPC Menu\r\n");
#endif
#if BLE_RSC_SENSOR
    QPRINTF("* w. RSCPS Menu\r\n");
#endif

    QPRINTF("* r. Upper Menu\r\n");
    QPRINTF("* s. Show  Menu\r\n");
	app_menu_show_line();
}

static void app_menu_handler_main(void)
{
    switch (app_env.input[0])
    {
    case '1':
        app_env.menu_id = menu_gap;
        break;
#if BLE_HR_COLLECTOR
    case '2':
        app_env.menu_id = menu_hrpc;
        break;
#endif
#if BLE_HR_SENSOR
    case '3':
        app_env.menu_id = menu_hrps;
        break;
#endif
#if BLE_PROX_MONITOR
    case '4':
        app_env.menu_id = menu_proxm;
        break;
#endif
#if BLE_FINDME_LOCATOR
    case '6':
        app_env.menu_id = menu_findl;
        break;
#endif
#if BLE_DIS_CLIENT
    case '8':
        app_env.menu_id = menu_disc;
        break;
#endif
#if BLE_DIS_SERVER
    case '9':
        app_env.menu_id = menu_diss;
        break;
#endif
#if BLE_BATT_CLIENT
    case 'a':
        app_env.menu_id = menu_basc;
        break;
#endif
#if BLE_BATT_SERVER
    case 'b':
        app_env.menu_id = menu_bass;
        break;
#endif
#if BLE_SP_CLIENT
    case 'c':
        app_env.menu_id = menu_scppc;
        break;
#endif
#if BLE_SP_SERVER
    case 'd':
        app_env.menu_id = menu_scpps;
        break;
#endif
#if BLE_HT_COLLECTOR
    case 'e':
        app_env.menu_id = menu_htpc;
        break;
#endif
#if BLE_HT_THERMOM
    case 'f':
        app_env.menu_id = menu_htpt;
        break;
#endif
    
#if BLE_BP_COLLECTOR
    case 'g':
        app_env.menu_id = menu_blpc;
        break;
#endif
#if BLE_BP_SENSOR
    case 'h':
        app_env.menu_id = menu_blps;
        break;
#endif
#if BLE_TIP_CLIENT
    case 'i':
        app_env.menu_id = menu_tipc;
        break;
#endif
#if BLE_TIP_SERVER
    case 'j':
        app_env.menu_id = menu_tips;
        break;
#endif
#if BLE_GL_COLLECTOR
    case 'k':
        app_env.menu_id = menu_glpc;
        break;
#endif
#if BLE_GL_SENSOR
    case 'm':
        app_env.menu_id = menu_glps;
        break;
#endif
#if BLE_AN_CLIENT
    case 'n':
        app_env.menu_id = menu_anpc;
        break;
#endif
#if BLE_AN_SERVER
    case 'o':
        app_env.menu_id = menu_anps;
        break;
#endif
#if BLE_CSC_COLLECTOR
    case 'p':
        app_env.menu_id = menu_cscpc;
        break;
#endif
#if BLE_CSC_SENSOR
    case 'q':
        app_env.menu_id = menu_cscps;
        break;
#endif
#if BLE_PAS_CLIENT
    case 't':
        app_env.menu_id = menu_paspc;
        break;
#endif
#if BLE_PAS_SERVER
    case 'u':
        app_env.menu_id = menu_pasps;
        break;
#endif
#if BLE_RSC_COLLECTOR
    case 'v':
        app_env.menu_id = menu_rscpc;
        break;
#endif
#if BLE_RSC_SENSOR
    case 'w':
        app_env.menu_id = menu_rscps;
        break;
#endif

    case 'r':
    case 's':
    default:
        break;
    }

    app_menu_show();
}
/**
*************************************************************************************
*** show gap
*************************************************************************************
 */
static void app_menu_show_gap(void)
{
    app_menu_show_line();
    QPRINTF("* QN BLE GAP Menu\r\n");
    QPRINTF("* 1. Scan Start\r\n");
    QPRINTF("* 2. Scan Stop\r\n");
    QPRINTF("* 3. Advertisement Start\r\n");
    QPRINTF("* 4. Advertisement Stop\r\n");
    QPRINTF("* 5. Create Connection\r\n");
    QPRINTF("* 6. Disconnection\r\n");
    QPRINTF("* 7. Bond Request\r\n");
    QPRINTF("* 8. Set Device name\r\n");
    QPRINTF("* 9. Central Update Param\r\n");
    QPRINTF("* a. Peripheral Update Param\r\n");
    app_menu_show_line();
}

static void app_menu_handler_gap(void)
{
    switch (app_env.input[0])
    {
        case '1':
            app_eaci_cmd_scan(1);
            break;
        case '2':
            app_eaci_cmd_scan(0);
            break;
        case '3':
            app_eaci_cmd_adv(1, 0x0030, 0x0064);
            break;
        case '4':
            app_eaci_cmd_adv(0, 0, 0);
            break;
        case '5':
            app_env.menu_id = menu_gap_create_connection;
            app_menu_show();
            break;
        case '6':
            app_env.menu_id = menu_gap_disconnection;
            app_menu_show();
            break;
        case '7':
            app_env.menu_id = menu_gap_bond;
            app_menu_show();
            break;
        case '8':
            app_eaci_set_dev_name_cmd((sizeof(DEVICE_NAME) - 1), DEVICE_NAME);
            break;
        case '9':
            app_env.menu_id = menu_gap_update_param;
            app_menu_show();
            break;
        case 'a':
        {
            app_eaci_slave_update_param_cmd();
        }
            break;
        case 'r':
            app_env.menu_id = menu_main;
        case 's':
        default:
            app_menu_show();
            break;
    }
}

static void app_menu_show_gap_inq_record(void)
{
    if (app_env.inq_id > 0)
    {
        QPRINTF("\r\n");
        QPRINTF("Scan device record:\r\n");
        for (uint8_t i = 0; i < app_env.inq_id; i++)
        {
            QPRINTF("%d. %02X%02X:%02X:%02X%02X%02X\r\n", i, 
                app_env.inq_addr[i].addr.addr[5],
                app_env.inq_addr[i].addr.addr[4],
                app_env.inq_addr[i].addr.addr[3],
                app_env.inq_addr[i].addr.addr[2],
                app_env.inq_addr[i].addr.addr[1],
                app_env.inq_addr[i].addr.addr[0]);
        }
        QPRINTF("Please input the device index:\r\n");
    }
    else
    {
        QPRINTF("Scan device record is NULL.\r\n");
        app_env.menu_id = menu_gap;
    }
}

static void app_menu_show_gap_dev_record(void)
{
    if (app_env.conn_nb > 0)
    {
        QPRINTF("\r\n");
        QPRINTF("Connected device records (%d):\r\n", app_env.conn_nb);
        for (uint8_t idx = 0; idx < BLE_CONNECTION_MAX; idx++)
        {
            if (app_env.dev_rec[idx].free == false)
            {
                QPRINTF("%d. %02X%02X:%02X:%02X%02X%02X\r\n", idx, 
                    app_env.dev_rec[idx].addr.addr[5],
                    app_env.dev_rec[idx].addr.addr[4],
                    app_env.dev_rec[idx].addr.addr[3],
                    app_env.dev_rec[idx].addr.addr[2],
                    app_env.dev_rec[idx].addr.addr[1],
                    app_env.dev_rec[idx].addr.addr[0]);
            }
        }
        QPRINTF("Please input the device index:\r\n");
    }
    else
    {
        QPRINTF("Connected device record is NULL.\r\n");
        app_env.menu_id = menu_gap;
    }
}

static void app_menu_handler_gap_dev_select(void)
{
    if (app_env.input[0] - '0' < BLE_CONNECTION_MAX)
    {
        app_env.sel_id = app_env.input[0] - '0';

        switch (app_env.menu_id)
        {
            case menu_gap_bond:
                app_eaci_cmd_bond(&app_env.dev_rec[app_env.sel_id].addr);
                break;
            case menu_gap_create_connection:
                app_eaci_cmd_conn(app_env.inq_addr[app_env.sel_id].addr_type, &app_env.inq_addr[app_env.sel_id].addr,
                                  0x0018, 0x0028, 0x07D0);
                break;
            case menu_gap_update_param:
                app_eaci_cen_update_param_cmd(&app_env.inq_addr[app_env.sel_id].addr);
                break;
            case menu_gap_disconnection:
                app_eaci_cmd_disc(&app_env.dev_rec[app_env.sel_id].addr);
                break;
            case menu_htpc_enable:
            case menu_blpc_enable:
            case menu_hrpc_enable:
            case menu_glpc_enable:
            case menu_findl_enable:
            case menu_proxm_enable:
            case menu_tipc_enable:
            case menu_scppc_enable:
            case menu_disc_enable:
            case menu_basc_enable:
            case menu_anpc_enable:
            case menu_cscpc_enable:
            case menu_paspc_enable:
            case menu_rscpc_enable:
                QPRINTF("Select success.\r\n");
            default:
                break;
        }
    }
    switch (app_env.input[0])
    {
    case 'r':
    default:
        if (app_env.menu_id == menu_htpc_enable)
            app_env.menu_id = menu_htpc;
        else if (app_env.menu_id == menu_blpc_enable)
            app_env.menu_id = menu_blpc;
        else if (app_env.menu_id == menu_hrpc_enable)
            app_env.menu_id = menu_hrpc;
        else if (app_env.menu_id == menu_glpc_enable)
            app_env.menu_id = menu_glpc;
        else if (app_env.menu_id == menu_findl_enable)
            app_env.menu_id = menu_findl;
        else if (app_env.menu_id == menu_proxm_enable)
            app_env.menu_id = menu_proxm;
        else if (app_env.menu_id == menu_tipc_enable)
            app_env.menu_id = menu_tipc;
        else if (app_env.menu_id == menu_scppc_enable)
            app_env.menu_id = menu_scppc;
        else if (app_env.menu_id == menu_disc_enable)
            app_env.menu_id = menu_disc;
        else if (app_env.menu_id == menu_basc_enable)
            app_env.menu_id = menu_basc;
        else if (app_env.menu_id == menu_anpc_enable)
            app_env.menu_id = menu_anpc;
        else if (app_env.menu_id == menu_cscpc_enable)
            app_env.menu_id = menu_cscpc;
        else if (app_env.menu_id == menu_paspc_enable)
            app_env.menu_id = menu_paspc;
        else if (app_env.menu_id == menu_rscpc_enable)
            app_env.menu_id = menu_rscpc;
        else
            app_env.menu_id = menu_gap;
        break;
    case 's':
        app_menu_show();
        break;
    }
}

/**
*************************************************************************************
*** Alert Notification Profile
*************************************************************************************
 */

#if BLE_AN_CLIENT
static void app_menu_show_anpc(void)
{
    app_menu_show_line();
    QPRINTF("* QN BLE ANPC Menu\r\n");
    QPRINTF("* 0. Select device\r\n");
    QPRINTF("* 1. Enable\r\n");
    QPRINTF("* 2. Read NEW ALERT CFG\r\n");
    QPRINTF("* 3. Read UNREAD ALERT STATUS CFG\r\n");
    QPRINTF("* 4. Write ANS New Alert Client CFG Start Notify\r\n");
    QPRINTF("* 5. Write ANS Unread Alert Status Client CFG Start Notify\r\n");
    QPRINTF("* 6. Write ANS Alert Notification Control Point\r\n");
    app_menu_show_line();
}

static void app_menu_handler_anpc(void)
{
    switch (app_env.input[0])
    {
        case '0':
            app_env.menu_id = menu_anpc_enable;
            break;
        case '1':
            app_eaci_data_req_anpc(EACI_MSG_DATA_REQ_ANPC_ENABLE, 6, (const uint8_t *)&app_env.dev_rec[app_env.sel_id].addr);
            break;
        case '2':
        {
            uint8_t rd_char[7] = {0,0,0,0,0,0,0};
            rd_char[0] = ANPC_RD_WR_NEW_ALERT_CFG; //New Alert Client Characteristic Configuration Descriptor
            memcpy((rd_char + 1), (const uint8_t *)&app_env.dev_rec[app_env.sel_id].addr, 6);
            app_eaci_data_req_anpc(EACI_MSG_DATA_REQ_ANPC_RD_CFG, 7, rd_char);
        }
            break;
        case '3':
        {
            uint8_t rd_char[7] = {0,0,0,0,0,0,0};
            rd_char[0] = ANPC_RD_WR_UNREAD_ALERT_STATUS_CFG; //Unread Alert Status Client Characteristic Configuration Descriptor
            memcpy((rd_char + 1), (const uint8_t *)&app_env.dev_rec[app_env.sel_id].addr, 6);
            app_eaci_data_req_anpc(EACI_MSG_DATA_REQ_ANPC_RD_CFG, 7, rd_char);
        }
            break;
        case '4':
        {
            uint8_t wr_char[9] = {0,0,0,0,0,0,0,0,0};
            wr_char[0] = ANPC_RD_WR_NEW_ALERT_CFG; //New Alert Client Characteristic Configuration Descriptor
            wr_char[1] = PRF_CLI_START_NTF;
            wr_char[2] = 0x00;
            memcpy((wr_char + 3), (const uint8_t *)&app_env.dev_rec[app_env.sel_id].addr, 6);
            app_eaci_data_req_anpc(EACI_MSG_DATA_REQ_ANPC_WR_VAL, 9, wr_char);
        }
            break;
        case '5':
        {
            uint8_t wr_char[9] = {0,0,0,0,0,0,0,0,0};
            wr_char[0] = ANPC_RD_WR_UNREAD_ALERT_STATUS_CFG; //New Alert Client Characteristic Configuration Descriptor
            wr_char[1] = PRF_CLI_START_NTF;
            wr_char[2] = 0x00;
            memcpy((wr_char + 3), (const uint8_t *)&app_env.dev_rec[app_env.sel_id].addr, 6);
            app_eaci_data_req_anpc(EACI_MSG_DATA_REQ_ANPC_WR_VAL, 9, wr_char);
        }
            break;
        case '6':
        {
            app_env.menu_id = menu_anpc_alert;
        }
            break;
        case 'r':
            app_env.menu_id = menu_main;
            break;
        case 's':
        default:
            break;
    }
    app_menu_show();
}

static void app_menu_show_anpc_alert(void)
{
    app_menu_show_line();
    QPRINTF("* QN BLE ANPC ALERT Menu\r\n");
    QPRINTF("* 1. Enable New Alert\r\n");
    QPRINTF("* 2. Enable New Unread Alert\r\n");
    QPRINTF("* 3. Disable All Alert\r\n");
    QPRINTF("* 4. Disable All Unread Alert\r\n");
    QPRINTF("* 5. Notify New Alert Immediately\r\n");
    QPRINTF("* 6. Notify New Unread Immediately\r\n");
    app_menu_show_line();
}

static void app_menu_handler_anpc_alert(void)
{
    switch (app_env.input[0])
    {
        case '1':
        {
            uint8_t wr_char[9] = {0,0,0,0,0,0,0,0,0};
            wr_char[0] = ANPC_WR_ALERT_NTF_CTNL_PT; //Write ANS Alert Notification Control Point
            wr_char[1] = CMD_ID_EN_NEW_IN_ALERT_NTF; //Enable New Incoming Alert Notification
            wr_char[2] = 0xff; //All supported category
            memcpy((wr_char + 3), (const uint8_t *)&app_env.dev_rec[app_env.sel_id].addr, 6);
            app_eaci_data_req_anpc(EACI_MSG_DATA_REQ_ANPC_WR_VAL, 9, wr_char);
        }
            break;
        case '2':
        {
            uint8_t wr_char[9] = {0,0,0,0,0,0,0,0,0};
            wr_char[0] = ANPC_WR_ALERT_NTF_CTNL_PT; //Write ANS Alert Notification Control Point
            wr_char[1] = CMD_ID_EN_UNREAD_CAT_STATUS_NTF; //Enable Unread Category Status Notification
            wr_char[2] = 0xff; //All supported category
            memcpy((wr_char + 3), (const uint8_t *)&app_env.dev_rec[app_env.sel_id].addr, 6);
            app_eaci_data_req_anpc(EACI_MSG_DATA_REQ_ANPC_WR_VAL, 9, wr_char);
        }
            break;
        case '3':
        {
            uint8_t wr_char[9] = {0,0,0,0,0,0,0,0,0};
            wr_char[0] = ANPC_WR_ALERT_NTF_CTNL_PT; //Write ANS Alert Notification Control Point
            wr_char[1] = CMD_ID_DIS_NEW_IN_ALERT_NTF; //Disable New Incoming Alert Notification
            wr_char[2] = 0xff; //All supported category
            memcpy((wr_char + 3), (const uint8_t *)&app_env.dev_rec[app_env.sel_id].addr, 6);
            app_eaci_data_req_anpc(EACI_MSG_DATA_REQ_ANPC_WR_VAL, 9, wr_char);
        }
            break;
        case '4':
        {
            uint8_t wr_char[9] = {0,0,0,0,0,0,0,0,0};
            wr_char[0] = ANPC_WR_ALERT_NTF_CTNL_PT; //Write ANS Alert Notification Control Point
            wr_char[1] = CMD_ID_DIS_UNREAD_CAT_STATUS_NTF; //Disable Unread Category Status Notification
            wr_char[2] = 0xff; //All supported category
            memcpy((wr_char + 3), (const uint8_t *)&app_env.dev_rec[app_env.sel_id].addr, 6);
            app_eaci_data_req_anpc(EACI_MSG_DATA_REQ_ANPC_WR_VAL, 9, wr_char);
        }
            break;
        case '5':
        {
            uint8_t wr_char[9] = {0,0,0,0,0,0,0,0,0};
            wr_char[0] = ANPC_WR_ALERT_NTF_CTNL_PT; //Write ANS Alert Notification Control Point
            wr_char[1] = CMD_ID_NTF_NEW_IN_ALERT_IMM; //Notify New Incoming Alert immediately
            wr_char[2] = 0xff; //All supported category
            memcpy((wr_char + 3), (const uint8_t *)&app_env.dev_rec[app_env.sel_id].addr, 6);
            app_eaci_data_req_anpc(EACI_MSG_DATA_REQ_ANPC_WR_VAL, 9, wr_char);
        }
            break;
        case '6':
        {
            uint8_t wr_char[9] = {0,0,0,0,0,0,0,0,0};
            wr_char[0] = ANPC_WR_ALERT_NTF_CTNL_PT; //Write ANS Alert Notification Control Point
            wr_char[1] = CMD_ID_NTF_UNREAD_CAT_STATUS_IMM; //Notify Unread Category Status immediately
            wr_char[2] = 0xff; //All supported category
            memcpy((wr_char + 3), (const uint8_t *)&app_env.dev_rec[app_env.sel_id].addr, 6);
            app_eaci_data_req_anpc(EACI_MSG_DATA_REQ_ANPC_WR_VAL, 9, wr_char);
        }
            break;
        case 'r':
            app_env.menu_id = menu_anpc;
            break;
        case 's':
        default:
            break;
    }
    app_menu_show();
}
#endif
#if BLE_AN_SERVER
static void app_menu_show_anps(void)
{
    app_menu_show_line();
    QPRINTF("* QN BLE ANPS Menu\r\n");
    QPRINTF("* 1. Update New Alert Char. value\r\n");
    QPRINTF("* 2. Update Unread Alert Status Char. value\r\n");
    app_menu_show_line();
}

static void app_menu_handler_anps(void)
{
    switch (app_env.input[0])
    {
        case '1':
        {
            uint8_t new_alert[5] = {0,0,0,0,0};
            new_alert[0] = ANPS_UPD_NEW_ALERT_OP_CODE;//operation Update New Alert Char. value 
            new_alert[1] = 0x01;//Information String Length 
            new_alert[2] = CAT_ID_NEWS;//Category ID
            new_alert[3] = 0x03;//Number of alerts
            new_alert[4] = 0x31;//Text String Information
            app_eaci_data_req_anps(EACI_MSG_DATA_REQ_ANPS_NTF_ALERT, 5, new_alert);
        }
            break;
        case '2':
        {
            uint8_t unnew_alert[3] = {0,0,0};
            unnew_alert[0] = ANPS_UPD_UNREAD_ALERT_STATUS_OP_CODE;//Update Unread Alert Status Char. value  
            unnew_alert[1] = CAT_ID_MISSED_CALL;//Category ID
            unnew_alert[2] = 0x06;//Number of alerts
            app_eaci_data_req_anps(EACI_MSG_DATA_REQ_ANPS_NTF_ALERT, 3, unnew_alert);
        }
            break;
        case 'r':
            app_env.menu_id = menu_main;
            break;
        case 's':
        default:
            break;
    }
    app_menu_show();
}
#endif

/**
*************************************************************************************
*** Battery Profile
*************************************************************************************
 */

#if BLE_BATT_CLIENT
static void app_menu_show_basc(void)
{
    app_menu_show_line();
    QPRINTF("* QN BLE BASC Menu\r\n");
    QPRINTF("* 0. Select device\r\n");
    QPRINTF("* 1. Enable\r\n");
    QPRINTF("* 2. Read BAS Battery Level\r\n");
    QPRINTF("* 3. Read BAS Battery cfg\r\n");
    QPRINTF("* 4. Start Battery Level Notification\r\n");
    QPRINTF("* 5. Stop Battery Level Notification\r\n");
    app_menu_show_line();
}

static void app_menu_handler_basc(void)
{
    switch (app_env.input[0])
    {
        case '0':
            app_env.menu_id = menu_basc_enable;
            break;
        case '1':
            app_eaci_data_req_basc(EACI_MSG_DATA_REQ_BATT_ENABLE, 6, (const uint8_t *)&app_env.dev_rec[app_env.sel_id].addr);
            break;
        case '2':
            {
                uint8_t rd_char[7] = {0,0,0,0,0,0,0};
                rd_char[0] = BASC_RD_BAS_BATT_LEVEL;
                memcpy((rd_char + 1), (const uint8_t *)&app_env.dev_rec[app_env.sel_id].addr, 6);
                app_eaci_data_req_basc(EACI_MSG_DATA_REQ_BATT_RD_CHAR_VAL, 7, rd_char);
            }
            break;
        case '3':
            {
                uint8_t rd_char[7] = {0,0,0,0,0,0,0};
                rd_char[0] = BASC_RD_BAS_BATT_LEVEL_CLI_CFG;
                memcpy((rd_char + 1), (const uint8_t *)&app_env.dev_rec[app_env.sel_id].addr, 6);
                app_eaci_data_req_basc(EACI_MSG_DATA_REQ_BATT_RD_CHAR_VAL, 7, rd_char);
            }
            break;
        case '4':
            {
                uint8_t batt_cfg[8] = {0,0,0,0,0,0,0,0};
                batt_cfg[0] = PRF_CLI_START_NTF;
                batt_cfg[1] = 0x00;
                memcpy((batt_cfg + 2), (const uint8_t *)&app_env.dev_rec[app_env.sel_id].addr, 6);
                app_eaci_data_req_basc(EACI_MSG_DATA_REQ_BATT_CFG_NOTIFY, 8, batt_cfg);
            }
            break;
        case '5':
            {
                uint8_t batt_cfg[8] = {0,0,0,0,0,0,0,0};
                batt_cfg[0] = PRF_CLI_STOP_NTFIND;
                batt_cfg[1] = 0x00;
                memcpy((batt_cfg + 2), (const uint8_t *)&app_env.dev_rec[app_env.sel_id].addr, 6);
                app_eaci_data_req_basc(EACI_MSG_DATA_REQ_BATT_CFG_NOTIFY, 8, batt_cfg);
            }
            break;
        case 'r':
            app_env.menu_id = menu_main;
            break;
        case 's':
        default:
            break;
    }
    app_menu_show();
}
#endif

#if BLE_BATT_SERVER
static void app_menu_show_bass(void)
{
    app_menu_show_line();
    QPRINTF("* QN BLE BASS Menu\r\n");
    QPRINTF("* 1. Update Battery Level\r\n");
    app_menu_show_line();
}

static void app_menu_handler_bass(void)
{
    switch (app_env.input[0])
    {
        case '1':
        {
            uint8_t batt_level[8] = {0,0,0,0,0,0,0,0};
            batt_level[0] = 0;  //bas_instance
            batt_level[1] = 0x58;  //battery level
            memcpy((batt_level + 2), (const uint8_t *)&app_env.dev_rec[app_env.sel_id].addr, 6);
            app_eaci_data_req_bass(EACI_MSG_DATA_REQ_BASS_BATT_LEVEL_UPD, 8, batt_level);
        }
            break;
        case 'r':
            app_env.menu_id = menu_main;
            break;
        case 's':
        default:
            break;
    }
    app_menu_show();
}
#endif

/**
*************************************************************************************
*** Blood Pressure Profile
*************************************************************************************
 */
#if BLE_BP_COLLECTOR
static void app_menu_show_blpc(void)
{
    app_menu_show_line();
    QPRINTF("* QN BLE BLPC Menu\r\n");
    QPRINTF("* 0. Select device\r\n");
    QPRINTF("* 1. Enable\r\n");
    QPRINTF("* 2. Read BPS Blood pressure Features\r\n");
    QPRINTF("* 3. Read BPS Blood pressure Measurement Client Cfg. Desc\r\n");
    QPRINTF("* 4. Read BPS Intermdiate Cuff Pressure Client Cfg. Desc\r\n");
    QPRINTF("* 5. Blood Pressure Measurement Cfg. Stop\r\n");
    QPRINTF("* 6. Blood Pressure Measurement Cfg. Start Indicate\r\n");
    QPRINTF("* 7. Intermediate Cuff Pressure Measurement Cfg. Stop\r\n");
    QPRINTF("* 8. Intermediate Cuff Pressure Measurement Cfg. Start Notify\r\n");
    app_menu_show_line();
}

static void app_menu_handler_blpc(void)
{
    switch (app_env.input[0])
    {
        case '0':
            app_env.menu_id = menu_blpc_enable;
            break;
        case '1':
            app_eaci_data_req_blpc(EACI_MSG_DATA_REQ_BP_ENABLE, 6, (const uint8_t *)&app_env.dev_rec[app_env.sel_id].addr);
            break;
        case '2':
        {
            uint8_t char_code[7] = {0,0,0,0,0,0,0};
            char_code[0] = BLPC_RD_BPS_FEATURE;
            memcpy((char_code + 1), (const uint8_t *)&app_env.dev_rec[app_env.sel_id].addr, 6);
            app_eaci_data_req_blpc(EACI_MSG_DATA_REQ_BP_RD_CHAR_CODE, 7, char_code);
        }
            break;
        case '3':
        {
            uint8_t char_code[7] = {0,0,0,0,0,0,0};
            char_code[0] = BLPC_RD_BPS_BP_MEAS_CFG;
            memcpy((char_code + 1), (const uint8_t *)&app_env.dev_rec[app_env.sel_id].addr, 6);
            app_eaci_data_req_blpc(EACI_MSG_DATA_REQ_BP_RD_CHAR_CODE, 7, char_code);
        }
            break;
        case '4':
        {
            uint8_t char_code[7] = {0,0,0,0,0,0,0};
            char_code[0] = BLPC_RD_BPS_CP_MEAS_CFG;
            memcpy((char_code + 1), (const uint8_t *)&app_env.dev_rec[app_env.sel_id].addr, 6);
            app_eaci_data_req_blpc(EACI_MSG_DATA_REQ_BP_RD_CHAR_CODE, 7, char_code);
        }
            break;
        case '5':
        {
            uint8_t cfg_lvl[8] = {0,0,0,0,0,0,0,0};
            cfg_lvl[0] = BPS_BP_MEAS_CODE;
            cfg_lvl[1] = PRF_CLI_STOP_NTFIND;
            memcpy((cfg_lvl + 2), (const uint8_t *)&app_env.dev_rec[app_env.sel_id].addr, 6);
            app_eaci_data_req_blpc(EACI_MSG_DATA_REQ_BP_CFG_INDNTF, 8, cfg_lvl);
        }
            break;
        case '6':
        {
            uint8_t cfg_lvl[8] = {0,0,0,0,0,0,0,0};
            cfg_lvl[0] = BPS_BP_MEAS_CODE;
            cfg_lvl[1] = PRF_CLI_START_IND;
            memcpy((cfg_lvl + 2), (const uint8_t *)&app_env.dev_rec[app_env.sel_id].addr, 6);
            app_eaci_data_req_blpc(EACI_MSG_DATA_REQ_BP_CFG_INDNTF, 8, cfg_lvl);
        }
            break;
        case '7':
        {
            uint8_t cfg_lvl[8] = {0,0,0,0,0,0,0,0};
            cfg_lvl[0] = BPS_INTERM_CP_CODE;
            cfg_lvl[1] = PRF_CLI_STOP_NTFIND;
            memcpy((cfg_lvl + 2), (const uint8_t *)&app_env.dev_rec[app_env.sel_id].addr, 6);
            app_eaci_data_req_blpc(EACI_MSG_DATA_REQ_BP_CFG_INDNTF, 8, cfg_lvl);
        }
            break;
        case '8':
        {
            uint8_t cfg_lvl[8] = {0,0,0,0,0,0,0,0};
            cfg_lvl[0] = BPS_INTERM_CP_CODE;
            cfg_lvl[1] = PRF_CLI_START_NTF;
            memcpy((cfg_lvl + 2), (const uint8_t *)&app_env.dev_rec[app_env.sel_id].addr, 6);
            app_eaci_data_req_blpc(EACI_MSG_DATA_REQ_BP_CFG_INDNTF, 8, cfg_lvl);
        }
            break;

        case 'r':
            app_env.menu_id = menu_main;
            break;
        case 's':
        default:
            break;
    }
    app_menu_show();
}
#endif
#if BLE_BP_SENSOR
static void app_menu_show_blps(void)
{
    app_menu_show_line();
    QPRINTF("* QN BLE BLPS Menu\r\n");
    QPRINTF("* 1. Send Stable Blood Pressure Measurement value\r\n");
    QPRINTF("* 2. Send Intermediate Cuff Pressure Measurement value\r\n");
    app_menu_show_line();
}

static void app_menu_handler_blps(void)
{
    switch (app_env.input[0])
    {
        case '1':
        {
            uint8_t param[2] = {0, 0};
            param[0] = 0; //Blood Pressure Measurement
            param[1] = BPS_FLAG_MEAS_STATUS_PRESENT |BPS_FLAG_PULSE_RATE_PRESENT
                        | BPS_FLAG_TIME_STAMP_PRESENT | BPS_FLAG_KPA; // flag
            app_eaci_data_req_blps(EACI_MSG_DATA_REQ_BP_SEND_PRES, 2, param);
        }
            break;
        case '2':
        {
            uint8_t param[2] = {0, 0};
            param[0] = 1; //Intermediate Cuff Pressure Measurement
            param[1] = BPS_FLAG_MEAS_STATUS_PRESENT | BPS_FLAG_USER_ID_PRESENT |BPS_FLAG_PULSE_RATE_PRESENT
                        | BPS_FLAG_TIME_STAMP_PRESENT | BPS_FLAG_KPA; // flag
            app_eaci_data_req_blps(EACI_MSG_DATA_REQ_BP_SEND_PRES, 2, param);
        }
            break;
        case 'r':
            app_env.menu_id = menu_main;
            break;
        case 's':
        default:
            break;
    }
    app_menu_show();
}
#endif

/**
*************************************************************************************
*** Cycling Speed and Cadence profile
*************************************************************************************
 */

#if BLE_CSC_COLLECTOR
static void app_menu_show_cscpc(void)
{
    app_menu_show_line();
    QPRINTF("* QN BLE CSCPC Menu\r\n");
    QPRINTF("* 0. Select device\r\n");
    QPRINTF("* 1. Enable\r\n");
    QPRINTF("* 2. Read CSC Feature\r\n");
    QPRINTF("* 3. Read Sensor Location\r\n");
    QPRINTF("* 4. Read CSC Measurement Client Char. Configuration Descriptor\r\n");
    QPRINTF("* 5. Read SC Control Point Client Char. Configuration Descriptor\r\n");
    QPRINTF("* 6. Write CSC Measurement Client Char. Configuration Descriptor\r\n");
    QPRINTF("* 7. Write SC Control Point Client Char. Configuration Descriptor\r\n");
    QPRINTF("* 8. Send the value of the SC Control Point\r\n");
    app_menu_show_line();
}

static void app_menu_handler_cscpc(void)
{
    switch (app_env.input[0])
    {
        case '0':
            app_env.menu_id = menu_cscpc_enable;
            break;
        case '1':
            app_eaci_data_req_cscpc(EACI_MSG_DATA_REQ_CSCPC_ENABLE, 6, (const uint8_t *)&app_env.dev_rec[app_env.sel_id].addr);
            break;
        case '2':
        {
            uint8_t rd_char[7] = {0,0,0,0,0,0,0};
            rd_char[0] = CSCPC_RD_CSC_FEAT; //CSC Feature
            memcpy((rd_char + 1), (const uint8_t *)&app_env.dev_rec[app_env.sel_id].addr, 6);
            app_eaci_data_req_cscpc(EACI_MSG_DATA_REQ_CSCPC_RD_CODE, 7, rd_char);
        }
            break;
        case '3':
        {
            uint8_t rd_char[7] = {0,0,0,0,0,0,0};
            rd_char[0] = CSCPC_RD_SENSOR_LOC; //Sensor Location
            memcpy((rd_char + 1), (const uint8_t *)&app_env.dev_rec[app_env.sel_id].addr, 6);
            app_eaci_data_req_cscpc(EACI_MSG_DATA_REQ_CSCPC_RD_CODE, 7, rd_char);
        }
            break;
        case '4':
        {
            uint8_t rd_char[7] = {0,0,0,0,0,0,0};
            rd_char[0] = CSCPC_RD_WR_CSC_MEAS_CFG; //CSC Measurement Char. - Client Characteristic Configuration
            memcpy((rd_char + 1), (const uint8_t *)&app_env.dev_rec[app_env.sel_id].addr, 6);
            app_eaci_data_req_cscpc(EACI_MSG_DATA_REQ_CSCPC_RD_CODE, 7, rd_char);
        }
            break;
        case '5':
        {
            uint8_t rd_char[7] = {0,0,0,0,0,0,0};
            rd_char[0] = CSCPC_RD_WR_SC_CTNL_PT_CFG; //SC Control Point Char. - Client Characteristic Configuration
            memcpy((rd_char + 1), (const uint8_t *)&app_env.dev_rec[app_env.sel_id].addr, 6);
            app_eaci_data_req_cscpc(EACI_MSG_DATA_REQ_CSCPC_RD_CODE, 7, rd_char);
        }
            break;
        case '6':
        {
            uint8_t ntfind_cfg[9] = {0,0,0,0,0,0,0,0,0};
            ntfind_cfg[0] = CSCPC_RD_WR_CSC_MEAS_CFG; // (desc_code)CSC Measurement Char. - Client Characteristic Configuration
            ntfind_cfg[1] = PRF_CLI_START_NTF;
            ntfind_cfg[2] = 0x00; // Notify
            memcpy((ntfind_cfg + 3), (const uint8_t *)&app_env.dev_rec[app_env.sel_id].addr, 6);
            app_eaci_data_req_cscpc(EACI_MSG_DATA_REQ_CSCPC_WR_CFG, 9, ntfind_cfg);
        }
            break;
        case '7':
        {
            uint8_t ntfind_cfg[9] = {0,0,0,0,0,0,0,0,0};
            ntfind_cfg[0] = CSCPC_RD_WR_SC_CTNL_PT_CFG; // (desc_code) SC Control Point Client Char. Configuration Descriptor
            ntfind_cfg[1] = PRF_CLI_START_IND;
            ntfind_cfg[2] = 0x00; // Indication
            memcpy((ntfind_cfg + 3), (const uint8_t *)&app_env.dev_rec[app_env.sel_id].addr, 6);
            app_eaci_data_req_cscpc(EACI_MSG_DATA_REQ_CSCPC_WR_CFG, 9, ntfind_cfg);
        }
            break;
        case '8':
            app_env.menu_id = menu_cscpc_ctnl_pt;
            break;
        case 'r':
            app_env.menu_id = menu_main;
            break;
        case 's':
        default:
            break;
    }
    app_menu_show();
}

static void app_menu_show_cscpc_ctnl_pt_cfm(void)
{
    app_menu_show_line();
    QPRINTF("* QN BLE CSCPC Conrol Menu\r\n");
    QPRINTF("* 1. Update Sensor Location\r\n");
    QPRINTF("* 2. Set Cumulative Value\r\n");
    QPRINTF("* 3. Request Supported Sensor Locations\r\n");

    app_menu_show_line();
}

static void app_menu_handler_cscpc_ctnl_pt_cfm(void)
{
    switch (app_env.input[0])
    {
        case '1':
        {
            uint8_t sc_ctnl_pt[11] = {0,0,0,0,0,0,0,0,0,0,0};
            sc_ctnl_pt[0] = CSCP_CTNL_PT_OP_UPD_LOC;
            sc_ctnl_pt[1] = CSCP_LOC_IN_SHOE;
            sc_ctnl_pt[2] = 0x00;
            sc_ctnl_pt[3] = 0x00;
            sc_ctnl_pt[4] = 0x00;
            memcpy((sc_ctnl_pt + 5), (const uint8_t *)&app_env.dev_rec[app_env.sel_id].addr, 6);
            app_eaci_data_req_cscpc(EACI_MSG_DATA_REQ_CSCPC_SEND_CTNL_PT, 11, sc_ctnl_pt);
        }
            break;
        case '2':
        {
            uint8_t sc_ctnl_pt[11] = {0,0,0,0,0,0,0,0,0,0,0};
            sc_ctnl_pt[0] = CSCP_CTNL_PT_OP_SET_CUMUL_VAL;
            sc_ctnl_pt[1] = 0x00;
            sc_ctnl_pt[2] = 0x00;
            sc_ctnl_pt[3] = 0x00;
            sc_ctnl_pt[4] = 0x00; //Cumulative value(uint32_t)
            memcpy((sc_ctnl_pt + 5), (const uint8_t *)&app_env.dev_rec[app_env.sel_id].addr, 6);
            app_eaci_data_req_cscpc(EACI_MSG_DATA_REQ_CSCPC_SEND_CTNL_PT, 11, sc_ctnl_pt);
        }
            break;
        case '3':
        {
            uint8_t sc_ctnl_pt[11] = {0,0,0,0,0,0,0,0,0,0,0};
            sc_ctnl_pt[0] = CSCP_CTNL_PT_OP_REQ_SUPP_LOC;
            sc_ctnl_pt[1] = 0x0f;
            sc_ctnl_pt[2] = 0x00;
            sc_ctnl_pt[3] = 0x00;
            sc_ctnl_pt[4] = 0x00;
            memcpy((sc_ctnl_pt + 5), (const uint8_t *)&app_env.dev_rec[app_env.sel_id].addr, 6);
            app_eaci_data_req_cscpc(EACI_MSG_DATA_REQ_CSCPC_SEND_CTNL_PT, 11, sc_ctnl_pt);
        }
            break;
        case 'r':
            app_env.menu_id = menu_cscpc;
            break;
        case 's':
        default:
            break;
    }
    app_menu_show();
}
#endif

#if BLE_CSC_SENSOR
static void app_menu_show_cscps(void)
{
    app_menu_show_line();
    QPRINTF("* QN BLE CSCPS Menu\r\n");
    QPRINTF("* 1. Send the CSCP Sensor Measurement value\r\n");
    QPRINTF("* 2. Return the status of control point\r\n");
    app_menu_show_line();
}

static void app_menu_handler_cscps(void)
{
    switch (app_env.input[0])
    {
        case '1':
        {
            uint8_t csc_meas[15] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
            csc_meas[0] = CSCP_MEAS_WHEEL_REV_DATA_PRESENT | CSCP_MEAS_CRANK_REV_DATA_PRESENT;
            csc_meas[1] = 0xe8;
            csc_meas[2] = 0x03; //Cumulative Wheel Revolutions value
            csc_meas[3] = 0x32;
            csc_meas[4] = 0x00; //the Last Crank Event Time
            csc_meas[5] = 0xf4;
            csc_meas[6] = 0x01; //the Last Wheel Event Time
            csc_meas[7] = 0x3c;
            csc_meas[8] = 0x00; //Wheel Revolution since the last wheel event time
            memcpy((csc_meas + 9), (const uint8_t *)&app_env.dev_rec[app_env.sel_id].addr, 6);
            app_eaci_data_req_cscps(EACI_MSG_DATA_REQ_CSCPS_NTF_MEAS, 15, csc_meas);
        }
            break;
        case '2':
        {
            uint8_t csc_meas[11] = {0,0,0,0,0,0,0,0,0,0,0};
            csc_meas[0] = CSCP_CTNL_PT_OP_SET_CUMUL_VAL;
            csc_meas[1] = 0x00;
            csc_meas[2] = 0x00; 
            csc_meas[3] = 0x00;
            csc_meas[4] = 0x00; //Cumulative value(uint32_t)
            memcpy((csc_meas + 5), (const uint8_t *)&app_env.dev_rec[app_env.sel_id].addr, 6);
            app_eaci_data_req_cscps(EACI_MSG_DATA_REQ_CSCPS_CTNL_PT, 11, csc_meas);
        }
            break;
        case 'r':
            app_env.menu_id = menu_main;
            break;
        case 's':
        default:
            break;
    }
    app_menu_show();
}
#endif

/**
*************************************************************************************
*** DISC Profile
*************************************************************************************
 */

#if BLE_DIS_CLIENT
static void app_menu_show_disc(void)
{
    app_menu_show_line();
    QPRINTF("* QN BLE DISC Menu\r\n");
    QPRINTF("* 0.  Select device\r\n");
    QPRINTF("* 1.  Enable\r\n");
    QPRINTF("* 2.  Read Manufacturer Name\r\n");
    QPRINTF("* 3.  Read Model Number String\r\n");
    QPRINTF("* 4.  Read Serial Number String\r\n");
    QPRINTF("* 5.  Read Hardware Revision String\r\n");
    QPRINTF("* 6.  Read Firmware Revision String\r\n");
    QPRINTF("* 7.  Read TSoftware Revision String\r\n");
    QPRINTF("* 8.  Read System ID\r\n");
    QPRINTF("* 9.  Read IEEE\r\n");
    QPRINTF("* a.  Read PnP ID\r\n");
    app_menu_show_line();
}

static void app_menu_handler_disc(void)
{
    switch (app_env.input[0])
    {
        case '0':
            app_env.menu_id = menu_disc_enable;
            break;
        case '1':
            app_eaci_data_req_disc(EACI_MSG_DATA_REQ_DISC_ENABLE, 6, (const uint8_t *)&app_env.dev_rec[app_env.sel_id].addr);
            break;
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
        case 'a':
        {
            uint8_t char_code[7] = {0,0,0,0,0,0,0};
            
            switch (app_env.input[0])
            {
                case '2':
                    char_code[0] = DISC_MANUFACTURER_NAME_CHAR;
                    break;
                case '3':
                    char_code[0] = DISC_MODEL_NB_STR_CHAR;
                    break;
                case '4':
                    char_code[0] = DISC_SERIAL_NB_STR_CHAR;
                    break;
                case '5':
                    char_code[0] = DISC_HARD_REV_STR_CHAR;
                    break;
                case '6':
                    char_code[0] = DISC_FIRM_REV_STR_CHAR;
                    break;
                case '7':
                    char_code[0] = DISC_SW_REV_STR_CHAR;
                    break;
                case '8':
                    char_code[0] = DISC_SYSTEM_ID_CHAR;
                    break;
                case '9':
                    char_code[0] = DISC_IEEE_CHAR;
                    break;
                case 'a':
                    char_code[0] = DISC_PNP_ID_CHAR;
                    break;
                default:
                    break;
            }
            memcpy((char_code + 1), (const uint8_t *)&app_env.dev_rec[app_env.sel_id].addr, 6);
            app_eaci_data_req_disc(EACI_MSG_DATA_REQ_DISC_CHAR_RD, 7, char_code);
        }
            break;
        case 'r':
            app_env.menu_id = menu_main;
            break;
        case 's':
        default:
            break;
    }
    app_menu_show();
}
#endif
#if BLE_DIS_SERVER
static void app_menu_show_diss(void)
{
    app_menu_show_line();
    QPRINTF("* QN BLE DISS Menu\r\n");
    QPRINTF("* 0.  Write Manufacturer Name\r\n");
    QPRINTF("* 1.  Write Model Number String\r\n");
    QPRINTF("* 2.  Write Serial Number String\r\n");
    QPRINTF("* 3.  Write Hardware Revision String\r\n");
    QPRINTF("* 4.  Write Firmware Revision String\r\n");
    QPRINTF("* 5.  Write TSoftware Revision String\r\n");
    QPRINTF("* 6.  Write System ID\r\n");
    QPRINTF("* 7.  Write IEEE\r\n");
    QPRINTF("* 8.  Write PnP ID\r\n");
    app_menu_show_line();
}

static void app_menu_handler_diss(void)
{
    uint8_t char_code[1] = {0};
    switch (app_env.input[0])
    {
        case '0':
            char_code[0] = DIS_MANUFACTURER_NAME_CHAR;
            app_eaci_data_req_diss(EACI_MSG_DATA_REQ_DISS_CHAR_WR, 1, char_code);
            break;
        case '1':
            char_code[0] = DIS_MODEL_NB_STR_CHAR;
            app_eaci_data_req_diss(EACI_MSG_DATA_REQ_DISS_CHAR_WR, 1, char_code);
            break;
        case '2':
            char_code[0] = DIS_SERIAL_NB_STR_CHAR;
            app_eaci_data_req_diss(EACI_MSG_DATA_REQ_DISS_CHAR_WR, 1, char_code);
            break;
        case '3':
            char_code[0] = DIS_HARD_REV_STR_CHAR;
            app_eaci_data_req_diss(EACI_MSG_DATA_REQ_DISS_CHAR_WR, 1, char_code);
            break;
        case '4':
            char_code[0] = DIS_FIRM_REV_STR_CHAR;            app_eaci_data_req_diss(EACI_MSG_DATA_REQ_DISS_CHAR_WR, 1, char_code);
            break;
        case '5':
            char_code[0] = DIS_SW_REV_STR_CHAR;
            app_eaci_data_req_diss(EACI_MSG_DATA_REQ_DISS_CHAR_WR, 1, char_code);
            break;
        case '6':
            char_code[0] = DIS_SYSTEM_ID_CHAR;
            app_eaci_data_req_diss(EACI_MSG_DATA_REQ_DISS_CHAR_WR, 1, char_code);
            break;
        case '7':
            char_code[0] = DIS_IEEE_CHAR;
            app_eaci_data_req_diss(EACI_MSG_DATA_REQ_DISS_CHAR_WR, 1, char_code);
            break;
        case '8':
            char_code[0] = DIS_PNP_ID_CHAR;
            app_eaci_data_req_diss(EACI_MSG_DATA_REQ_DISS_CHAR_WR, 1, char_code);
            break;
        case 'r':
            app_env.menu_id = menu_main;
            break;
        case 's':
        default:
            break;
    }
    app_menu_show();
}
#endif
/**
*************************************************************************************
*** Find Me Profile
*************************************************************************************
 */

#if BLE_FINDME_LOCATOR
static void app_menu_show_findl(void)
{
    app_menu_show_line();
    QPRINTF("* QN BLE FMPL Menu\r\n");
    QPRINTF("* 0. Select device\r\n");
    QPRINTF("* 1. Enable\r\n");
    QPRINTF("* 2. Alert None\r\n");
    QPRINTF("* 3. Alert Mild\r\n");
    QPRINTF("* 4. Alert High\r\n");
    app_menu_show_line();
}

static void app_menu_handler_findl(void)
{
    switch (app_env.input[0])
    {
        case '0':
            app_env.menu_id = menu_findl_enable;
            break;
        case '1':
            app_eaci_data_req_findl(EACI_MSG_DATA_REQ_FM_ENABLE, 6, (const uint8_t *)&app_env.dev_rec[app_env.sel_id].addr);
            break;
        case '2':
        case '3':
        case '4':
        {
            uint8_t param[7] = {0,0,0,0,0,0,0};
            
            switch (app_env.input[0])
            {
                case '2':
                    param[0] = FINDL_ALERT_NONE;
                    break;
                case '3':
                    param[0] = FINDL_ALERT_MILD;
                    break;
                case '4':
                    param[0] = FINDL_ALERT_HIGH;
                    break;
            }
            memcpy((param + 1), (const uint8_t *)&app_env.dev_rec[app_env.sel_id].addr, 6);
            app_eaci_data_req_findl(EACI_MSG_DATA_REQ_FM_WR_ALERT, 7, param);
        }
            break;
        case 'r':
            app_env.menu_id = menu_main;
            break;
        case 's':
        default:
            break;
    }
    app_menu_show();
}
#endif
/**
*************************************************************************************
*** Glucose Profile
*************************************************************************************
 */
#if BLE_GL_COLLECTOR
static void app_menu_show_glpc(void)
{
    app_menu_show_line();
    QPRINTF("* QN BLE GLPC Menu\r\n");
    QPRINTF("* 0. Select device\r\n");
    QPRINTF("* 1. Enable\r\n");
    QPRINTF("* 2. Register\r\n");
    QPRINTF("* 3. Read Glucose sensor features\r\n");
    QPRINTF("* 4. Report Stored Records\r\n");
    QPRINTF("* 5. Delete Stored Records\r\n");
    QPRINTF("* 6. Abort Operation\r\n");
    QPRINTF("* 7. Report Number of Stored Records\r\n");
    app_menu_show_line();
}

static void app_menu_handler_glpc(void)
{
    switch (app_env.input[0])
    {
        case '0':
            app_env.menu_id = menu_glpc_enable;
            break;
        case '1':
            app_eaci_data_req_glpc(EACI_MSG_DATA_REQ_GL_ENABLE, 6, (const uint8_t *)&app_env.dev_rec[app_env.sel_id].addr);
            break;
        case '2':
            app_eaci_data_req_glpc(EACI_MSG_DATA_REQ_GL_REGISTER, 6, (const uint8_t *)&app_env.dev_rec[app_env.sel_id].addr);
            break;
        case '3':
            app_eaci_data_req_glpc(EACI_MSG_DATA_REQ_GL_FEATURES_RD, 6, (const uint8_t *)&app_env.dev_rec[app_env.sel_id].addr);
            break;
        case '4':
        {
            uint8_t gl_recs[11] = {0,0,0,0,0,0,0,0,0,0,0};
            gl_recs[0] = GLP_OP_ALL_RECS; //operator
            gl_recs[1] = 0;
            gl_recs[2] = 0; //Min sequence number(0x0000)
            gl_recs[3] = 1;
            gl_recs[4] = 0; //Max sequence number(0x0001)
            memcpy((gl_recs + 5), (const uint8_t *)&app_env.dev_rec[app_env.sel_id].addr, 6);
            
            app_eaci_data_req_glpc(EACI_MSG_DATA_REQ_GL_RSR_RACP, 11, gl_recs);
        }
            break;
        case '5':
        {
            uint8_t gl_recs[11] = {0,0,0,0,0,0,0,0,0,0,0};
            gl_recs[0] = GLP_OP_ALL_RECS; //operator
            gl_recs[1] = 0;
            gl_recs[2] = 0; //Min sequence number(0x0000)
            gl_recs[3] = 1;
            gl_recs[4] = 0; //Max sequence number(0x0001)
            memcpy((gl_recs + 5), (const uint8_t *)&app_env.dev_rec[app_env.sel_id].addr, 6);
            
            app_eaci_data_req_glpc(EACI_MSG_DATA_REQ_GL_DSR_RACP, 11, gl_recs);
        }
            break;
        case '6':
        {
            uint8_t gl_recs[11] = {0,0,0,0,0,0,0,0,0,0,0};
            gl_recs[0] = 0; //operator-NULL
            gl_recs[1] = 0;
            gl_recs[2] = 0; // NULL
            gl_recs[3] = 0;
            gl_recs[4] = 0; // NULL
            memcpy((gl_recs + 5), (const uint8_t *)&app_env.dev_rec[app_env.sel_id].addr, 6);
            
            app_eaci_data_req_glpc(EACI_MSG_DATA_REQ_GL_AO_RACP, 11, gl_recs);
        }
            break;
        case '7':
        {
            uint8_t gl_recs[11] = {0,0,0,0,0,0,0,0,0,0,0};
            gl_recs[0] = GLP_OP_ALL_RECS; //operator
            gl_recs[1] = 0;
            gl_recs[2] = 0; //Min sequence number(0x0000)
            gl_recs[3] = 1;
            gl_recs[4] = 0; //Max sequence number(0x0001)
            memcpy((gl_recs + 5), (const uint8_t *)&app_env.dev_rec[app_env.sel_id].addr, 6);
            
            app_eaci_data_req_glpc(EACI_MSG_DATA_REQ_GL_RN_RACP, 11, gl_recs);
        }
            break;
        case 'r':
            app_env.menu_id = menu_main;
            break;
        case 's':
        default:
            break;
    }
    app_menu_show();
}
#endif
#if BLE_GL_SENSOR
static void app_menu_show_glps(void)
{
    app_menu_show_line();
    QPRINTF("* QN BLE GLPS Menu\r\n");
    QPRINTF("* 1. Send Glucose measurement without context information\r\n");
    QPRINTF("* 2. Send Glucose measurement with context information\r\n");
    app_menu_show_line();
}

static void app_menu_handler_glps(void)
{
    switch (app_env.input[0])
    {
        case '1':
            app_eaci_data_req_glps(EACI_MSG_DATA_REQ_GL_SEND_MEAS_WITHOUT_CTX);
            break;
        case '2':
            app_eaci_data_req_glps(EACI_MSG_DATA_REQ_GL_SEND_MEAS_WITH_CTX);
            break;
        case 'r':
            app_env.menu_id = menu_main;
            break;
        case 's':
        default:
            break;
    }
    app_menu_show();
}
#endif

/**
*************************************************************************************
*** heart rate Profile
*************************************************************************************
 */
#if BLE_HR_COLLECTOR
static void app_menu_show_hrpc(void)
{
    app_menu_show_line();
    QPRINTF("* QN BLE HRPC Menu\r\n");
    QPRINTF("* 0. Select device\r\n");
    QPRINTF("* 1. Enable\r\n");
    QPRINTF("* 2. Read Body Sensor Location\r\n");
    QPRINTF("* 3. Configure Heart Rate Measurement Stop Notify\r\n");
    QPRINTF("* 4. Configure Heart Rate Measurement Start Notify\r\n");
    app_menu_show_line();
}

static void app_menu_handler_hrpc(void)
{
    switch (app_env.input[0])
    {
        case '0':
            app_env.menu_id = menu_hrpc_enable;
            break;
        case '1':
            app_eaci_data_req_hrpc(EACI_MSG_DATA_REQ_HRPC_ENABLE, 6, (const uint8_t *)&app_env.dev_rec[app_env.sel_id].addr);
            break;
        case '2':
            app_eaci_data_req_hrpc(EACI_MSG_DATA_READ_HRPC_BODY_SENSOR, 6, (const uint8_t *)&app_env.dev_rec[app_env.sel_id].addr);
            break;
        case '3':
        case '4':
        {
            uint8_t hr_cfg[7] = {0,0,0,0,0,0,0};
            if (app_env.input[0] == '3')
            {
                hr_cfg[0] = PRF_CLI_STOP_NTFIND;
            }
            else
            {
                hr_cfg[0] = PRF_CLI_START_NTF;
            }
            memcpy((hr_cfg + 1), (const uint8_t *)&app_env.dev_rec[app_env.sel_id].addr, 6);
            app_eaci_data_req_hrpc(EACI_MSG_DATA_READ_HRPC_CFG_INDNTF, 7, hr_cfg);
        }
            break;
        case 'r':
            app_env.menu_id = menu_main;
            break;
        case 's':
        default:
            break;
    }
    app_menu_show();
}
#endif

#if BLE_HR_SENSOR
static void app_menu_show_hrps(void)
{
    app_menu_show_line();
    QPRINTF("* QN BLE HRPS Menu\r\n");
    QPRINTF("* 1. Heart Rate Send\r\n");
    app_menu_show_line();
}

static void app_menu_handler_hrps(void)
{
    switch (app_env.input[0])
    {
        case '1':
        {
            uint16_t param = 0x1234;
            app_eaci_data_req_hrps(EACI_MSG_DATA_REQ_HRPS_MEAS_VALUE, 2, (uint8_t *)&param);
        }
            break;
        case 'r':
            app_env.menu_id = menu_main;
            break;
        case 's':
        default:
            break;
    }
    app_menu_show();
}
#endif

/**
*************************************************************************************
*** Health Thermometer Profile
*************************************************************************************
 */
#if BLE_HT_COLLECTOR
static void app_menu_show_htpc(void)
{
    app_menu_show_line();
    QPRINTF("* QN BLE HTPC Menu\r\n");
    QPRINTF("* 0. Select device\r\n");
    QPRINTF("* 1. Enable\r\n");
    QPRINTF("* 2. Temperature Measurement Stop Indicate\r\n");
    QPRINTF("* 3. Temperature Measurement Start Indicate\r\n");
    QPRINTF("* 4. Intermediate Temperature Stop Notify\r\n");
    QPRINTF("* 5. Intermediate Temperature Start Notify\r\n");
    QPRINTF("* 6. Measurement Interval Stop Indicate\r\n");
    QPRINTF("* 7. Measurement Interval Start Indicate\r\n");
    QPRINTF("* 8. Read Measurement Interval\r\n");
    app_menu_show_line();
}

static void app_menu_handler_htpc(void)
{
    switch (app_env.input[0])
    {
        case '0':
            app_env.menu_id = menu_htpc_enable;
            break;
        case '1':
            app_eaci_data_req_htpc(EACI_MSG_DATA_REQ_HT_ENABLE, 6, (const uint8_t *)&app_env.dev_rec[app_env.sel_id].addr);
            break;
        case '2':
        {
            uint8_t cfg_lvl[7] = {0,0,0,0,0,0,0};
            cfg_lvl[0] = PRF_CLI_STOP_NTFIND;
            memcpy((cfg_lvl + 1), (const uint8_t *)&app_env.dev_rec[app_env.sel_id].addr, 6);
            app_eaci_data_req_htpc(EACI_MSG_DATA_REQ_HT_TEMP_MEAS, 7, cfg_lvl);        
        }
            break;
        case '3':
        {
            uint8_t cfg_lvl[7] = {0,0,0,0,0,0,0};
            cfg_lvl[0] = PRF_CLI_START_IND;
            memcpy((cfg_lvl + 1), (const uint8_t *)&app_env.dev_rec[app_env.sel_id].addr, 6);
            app_eaci_data_req_htpc(EACI_MSG_DATA_REQ_HT_TEMP_MEAS, 7, cfg_lvl);        
        }
            break;
        case '4':
        {
            uint8_t cfg_lvl[7] = {0,0,0,0,0,0,0};
            cfg_lvl[0] = PRF_CLI_STOP_NTFIND;
            memcpy((cfg_lvl + 1), (const uint8_t *)&app_env.dev_rec[app_env.sel_id].addr, 6);
            app_eaci_data_req_htpc(EACI_MSG_DATA_REQ_HT_INTM_TEMP, 7, cfg_lvl);        
        }
            break;
        case '5':
        {
            uint8_t cfg_lvl[7] = {0,0,0,0,0,0,0};
            cfg_lvl[0] = PRF_CLI_START_NTF;
            memcpy((cfg_lvl + 1), (const uint8_t *)&app_env.dev_rec[app_env.sel_id].addr, 6);
            app_eaci_data_req_htpc(EACI_MSG_DATA_REQ_HT_INTM_TEMP, 7, cfg_lvl);        
        }
            break;
        case '6':
        {
            uint8_t cfg_lvl[7] = {0,0,0,0,0,0,0};
            cfg_lvl[0] = PRF_CLI_STOP_NTFIND;
            memcpy((cfg_lvl + 1), (const uint8_t *)&app_env.dev_rec[app_env.sel_id].addr, 6);
            app_eaci_data_req_htpc(EACI_MSG_DATA_REQ_HT_MEAS_INTV, 7, cfg_lvl);
        }
            break;
        case '7':
        {
            uint8_t cfg_lvl[7] = {0,0,0,0,0,0,0};
            cfg_lvl[0] = PRF_CLI_START_IND;
            memcpy((cfg_lvl + 1), (const uint8_t *)&app_env.dev_rec[app_env.sel_id].addr, 6);
            app_eaci_data_req_htpc(EACI_MSG_DATA_REQ_HT_MEAS_INTV, 7, cfg_lvl);
        }
            break;
        case '8':
        {
            app_eaci_data_req_htpc(EACI_MSG_DATA_REQ_HT_RD_MEAS_INTV, 6, (const uint8_t *)&app_env.dev_rec[app_env.sel_id].addr);
        }
            break;
        case 'r':
            app_env.menu_id = menu_main;
            break;
        case 's':
        default:
            break;
    }
    app_menu_show();
}
#endif
#if BLE_HT_THERMOM
uint8_t flag_stable_meas = 0;
static void app_menu_show_htpt_flag(void)
{
    app_menu_show_line();
    QPRINTF("* QN BLE HTPT_TYPE Menu\r\n");
    QPRINTF("* 1. Send Temperature Measurement value(uint Celsius)\r\n");
    QPRINTF("* 2. Send Temperature Measurement value(uint Fahrenheit)\r\n");
    app_menu_show_line();
}
static void app_menu_handler_htpt_flag(void)
{
    switch (app_env.input[0])
    {
        case '1':
        case '2':
        {
            uint8_t flg_temp[] = {0, 0};
            flg_temp[0] = flag_stable_meas;
            flg_temp[1]= (app_env.input[0] - '1') | 0x06;
            app_eaci_data_req_htpt(EACI_MSG_DATA_REQ_HT_SEND_TEMP_VALUE, 2, flg_temp);
        }
            break;
        case 'r':
            app_env.menu_id = menu_htpt;
            break;
        case 's':
        default:
            break;
    }
    app_menu_show();
}

static void app_menu_show_htpt(void)
{
    app_menu_show_line();
    QPRINTF("* QN BLE HTPT Menu\r\n");
    QPRINTF("* 1. Send Measurement Interval value\r\n");
    QPRINTF("* 2. Send Intermediary Temperature \r\n");
    QPRINTF("* 3. Send Stable Temperature \r\n");
    app_menu_show_line();
}

static void app_menu_handler_htpt(void)
{
    switch (app_env.input[0])
    {
        case '1': {
            uint16_t param = 0x1122;
            app_eaci_data_req_htpt(EACI_MSG_DATA_REQ_HT_SEND_INTER_VALUE, 2, (uint8_t *)&param);
            }
            break;
        case '2':
        case '3':
        {
            flag_stable_meas = app_env.input[0] - '2';
            app_env.menu_id = menu_htpt_flag;
        }
            break;
        case 'r':
            app_env.menu_id = menu_main;
            break;
        case 's':
        default:
            break;
    }
    app_menu_show();
}
#endif

/**
*************************************************************************************
*** Phone Alert Status Profile
*************************************************************************************
 */

#if BLE_PAS_CLIENT
static void app_menu_show_paspc(void)
{
    app_menu_show_line();
    QPRINTF("* QN BLE PASPC Menu\r\n");
    QPRINTF("* 0. Select device\r\n");
    QPRINTF("* 1. Enable\r\n");
    QPRINTF("* 2. Read PAS Alert Status\r\n");
    QPRINTF("* 3. Read PAS Ringer Setting\r\n");
    QPRINTF("* 4. Read PAS Alert Status Client CFG.\r\n");
    QPRINTF("* 5. Read PAS Ringer Setting Client CFG.\r\n");
    QPRINTF("* 6. Ringer Control Point\r\n");
    QPRINTF("* 7. Write PAS Alert Status Client CFG. Notify\r\n");
    QPRINTF("* 8. Write PAS Ringer Setting Client CFG. Notify\r\n");
    app_menu_show_line();
}

static void app_menu_handler_paspc(void)
{
    switch (app_env.input[0])
    {
        case '0':
            app_env.menu_id = menu_paspc_enable;
            break;
        case '1':
            app_eaci_data_req_paspc(EACI_MSG_DATA_REQ_PASPC_ENABLE, 6, (const uint8_t *)&app_env.dev_rec[app_env.sel_id].addr);
            break;
        case '2':
        {
            uint8_t read_code[7] = {0,0,0,0,0,0,0};
            read_code[0] = PASPC_RD_ALERT_STATUS;
            memcpy((read_code + 1), (const uint8_t *)&app_env.dev_rec[app_env.sel_id].addr, 6);
            app_eaci_data_req_paspc(EACI_MSG_DATA_REQ_PASPC_RD, 7, read_code);
        }
            break;
        case '3':
        {
            uint8_t read_code[7] = {0,0,0,0,0,0,0};
            read_code[0] = PASPC_RD_RINGER_SETTING;
            memcpy((read_code + 1), (const uint8_t *)&app_env.dev_rec[app_env.sel_id].addr, 6);
            app_eaci_data_req_paspc(EACI_MSG_DATA_REQ_PASPC_RD, 7, read_code);
        }
            break;
        case '4':
        {
            uint8_t read_code[7] = {0,0,0,0,0,0,0};
            read_code[0] = PASPC_RD_WR_ALERT_STATUS_CFG;
            memcpy((read_code + 1), (const uint8_t *)&app_env.dev_rec[app_env.sel_id].addr, 6);
            app_eaci_data_req_paspc(EACI_MSG_DATA_REQ_PASPC_RD, 7, read_code);
        }
            break;
        case '5':
        {
            uint8_t read_code[7] = {0,0,0,0,0,0,0};
            read_code[0] = PASPC_RD_WR_RINGER_SETTING_CFG;
            memcpy((read_code + 1), (const uint8_t *)&app_env.dev_rec[app_env.sel_id].addr, 6);
            app_eaci_data_req_paspc(EACI_MSG_DATA_REQ_PASPC_RD, 7, read_code);
        }
            break;
        case '6':
            app_env.menu_id = menu_paspc_ringer_pt;
            break;
        case '7':
        {
            uint8_t write_code[9] = {0,0,0,0,0,0,0,0,0};
            write_code[0] = PASPC_RD_WR_ALERT_STATUS_CFG;
            write_code[1] = PRF_CLI_START_NTF;
            write_code[2] = 0x00;
            memcpy((write_code + 3), (const uint8_t *)&app_env.dev_rec[app_env.sel_id].addr, 6);
            app_eaci_data_req_paspc(EACI_MSG_DATA_REQ_PASPC_WR, 9, write_code);
        }
            break;
        case '8':
        {
            uint8_t write_code[9] = {0,0,0,0,0,0,0,0,0};
            write_code[0] = PASPC_RD_WR_RINGER_SETTING_CFG;
            write_code[1] = PRF_CLI_START_NTF;
            write_code[2] = 0x00;
            memcpy((write_code + 3), (const uint8_t *)&app_env.dev_rec[app_env.sel_id].addr, 6);
            app_eaci_data_req_paspc(EACI_MSG_DATA_REQ_PASPC_WR, 9, write_code);
        }
            break;
        case 'r':
            app_env.menu_id = menu_main;
            break;
        case 's':
        default:
            break;
    }
    app_menu_show();
}
static void app_menu_show_paspc_ringer_pt(void)
{
    app_menu_show_line();
    QPRINTF("* QN BLE PASPC Menu\r\n");
    QPRINTF("* 1. Silent Mode\r\n");
    QPRINTF("* 2. Mute Once\r\n");
    QPRINTF("* 3. Cancel Silent Mode\r\n");
    app_menu_show_line();
}

static void app_menu_handler_paspc_ringer_pt(void)
{
    switch (app_env.input[0])
    {
        case '1':
        {
            uint8_t write_code[9] = {0,0,0,0,0,0,0,0,0};
            write_code[0] = PASPC_WR_RINGER_CTNL_PT;
            write_code[1] = PASP_SILENT_MODE;
            write_code[2] = 0x00;
            memcpy((write_code + 3), (const uint8_t *)&app_env.dev_rec[app_env.sel_id].addr, 6);
            app_eaci_data_req_paspc(EACI_MSG_DATA_REQ_PASPC_WR, 9, write_code);
        }
            break;
        case '2':
        {
            uint8_t write_code[9] = {0,0,0,0,0,0,0,0,0};
            write_code[0] = PASPC_WR_RINGER_CTNL_PT;
            write_code[1] = PASP_MUTE_ONCE;
            write_code[2] = 0x00;
            memcpy((write_code + 3), (const uint8_t *)&app_env.dev_rec[app_env.sel_id].addr, 6);
            app_eaci_data_req_paspc(EACI_MSG_DATA_REQ_PASPC_WR, 9, write_code);
        }
            break;
        case '3':
        {
            uint8_t write_code[9] = {0,0,0,0,0,0,0,0,0};
            write_code[0] = PASPC_WR_RINGER_CTNL_PT;
            write_code[1] = PASP_CANCEL_SILENT_MODE;
            write_code[2] = 0x00;
            memcpy((write_code + 3), (const uint8_t *)&app_env.dev_rec[app_env.sel_id].addr, 6);
            app_eaci_data_req_paspc(EACI_MSG_DATA_REQ_PASPC_WR, 9, write_code);
        }
            break;
        case 'r':
            app_env.menu_id = menu_paspc;
            break;
        case 's':
        default:
            break;
    }
    app_menu_show();
}
#endif
#if BLE_PAS_SERVER
static void app_menu_show_pasps(void)
{
    app_menu_show_line();
    QPRINTF("* QN BLE PASPS Menu\r\n");
    QPRINTF("* 1. Update Alert Status Char. value\r\n");
    QPRINTF("* 2. Update Ringer Setting Char. value\r\n");
    app_menu_show_line();
}

static void app_menu_handler_pasps(void)
{
    switch (app_env.input[0])
    {
        case '1':
        {
            uint8_t update_val[8] = {0,0,0,0,0,0,0,0};
            update_val[0] = PASPS_UPD_ALERT_STATUS_OP_CODE;
            update_val[1] = PASP_RINGER_ACTIVE;
            memcpy((update_val + 2), (const uint8_t *)&app_env.dev_rec[app_env.sel_id].addr, 6);
            app_eaci_data_req_pasps(EACI_MSG_DATA_REQ_PASPS_UPDATE_VAL, 8, update_val);
        }
            break;
        case '2':
        {
            uint8_t update_val[8] = {0,0,0,0,0,0,0,0};
            update_val[0] = PASPS_UPD_RINGER_SETTING_OP_CODE;
            update_val[1] = PASP_RINGER_NORMAL;
            memcpy((update_val + 2), (const uint8_t *)&app_env.dev_rec[app_env.sel_id].addr, 6);
            app_eaci_data_req_pasps(EACI_MSG_DATA_REQ_PASPS_UPDATE_VAL, 8, update_val);
        }
            break;
        case 'r':
            app_env.menu_id = menu_main;
            break;
        case 's':
        default:
            break;
    }
    app_menu_show();
}
#endif

/**
*************************************************************************************
*** Proximity Profile
*************************************************************************************
 */
#if BLE_PROX_MONITOR
static void app_menu_show_proxm(void)
{
    app_menu_show_line();
    QPRINTF("* QN BLE PROXM Menu\r\n");
    QPRINTF("* 0. Select device\r\n");
    QPRINTF("* 1. Enable\r\n");
    QPRINTF("* 2. Read TX Power Level\r\n");
    QPRINTF("* 3. Write IAS ALERT_NONE\r\n");
    QPRINTF("* 4. Write IAS ALERT_MILD\r\n");
    QPRINTF("* 5. Write IAS ALERT_HIGH\r\n");
    QPRINTF("* 6. Write LLS ALERT_NONE\r\n");
    QPRINTF("* 7. Write LLS ALERT_MILD\r\n");
    QPRINTF("* 8. Write LLS ALERT_HIGH\r\n");
    app_menu_show_line();
}

static void app_menu_handler_proxm(void)
{
    switch (app_env.input[0])
    {
        case '0':
            app_env.menu_id = menu_proxm_enable;
            break;
        case '1':
            app_eaci_data_req_proxm(EACI_MSG_DATA_REQ_PROXM_ENABLE, 6, (const uint8_t *)&app_env.dev_rec[app_env.sel_id].addr);
            break;
        case '2':
            app_eaci_data_req_proxm(EACI_MSG_DATA_READ_PROXM_TX_POWER, 6, (const uint8_t *)&app_env.dev_rec[app_env.sel_id].addr);
            break;
        case '3':
        case '4':
        case '5':
        {
            uint8_t ias_lvl[7] = {0,0,0,0,0,0,0};

            switch (app_env.input[0])
            {
                case '3':
                    ias_lvl[0] = PROXM_ALERT_NONE;
                    break;
                case '4':
                    ias_lvl[0] = PROXM_ALERT_MILD;
                    break;
                case '5':
                    ias_lvl[0] = PROXM_ALERT_HIGH;
                    break;
                default:
                    break;
            }
            memcpy((ias_lvl + 1), (const uint8_t *)&app_env.dev_rec[app_env.sel_id].addr, 6);
            app_eaci_data_req_proxm(EACI_MSG_DATA_WRITE_PROXM_IAS, 7, ias_lvl);
        }
            break;
        case '6':
        case '7':
        case '8':
        {
            uint8_t ias_lvl[7] = {0,0,0,0,0,0,0};

            switch (app_env.input[0])
            {
                case '6':
                    ias_lvl[0] = PROXM_ALERT_NONE;
                    break;
                case '7':
                    ias_lvl[0] = PROXM_ALERT_MILD;
                    break;
                case '8':
                    ias_lvl[0] = PROXM_ALERT_HIGH;
                    break;
                default:
                    break;
            }
            memcpy((ias_lvl + 1), (const uint8_t *)&app_env.dev_rec[app_env.sel_id].addr, 6);
            app_eaci_data_req_proxm(EACI_MSG_DATA_WRITE_PROXM_LLS, 7, ias_lvl);
        }
            break;
        case 'r':
            app_env.menu_id = menu_main;
            break;
        case 's':
        default:
            break;
    }
    app_menu_show();
}
#endif

/**
*************************************************************************************
*** Running Speed and Cadence profile
*************************************************************************************
 */

#if BLE_RSC_COLLECTOR
static void app_menu_show_rscpc(void)
{
    app_menu_show_line();
    QPRINTF("* QN BLE RSCPC Menu\r\n");
    QPRINTF("* 0. Select device\r\n");
    QPRINTF("* 1. Enable\r\n");
    QPRINTF("* 2. Read RSC Feature\r\n");
    QPRINTF("* 3. Read Sensor Location\r\n");
    QPRINTF("* 4. Read RSC Measurement CFG.\r\n");
    QPRINTF("* 5. Read SC Control Point CFG.\r\n");
    QPRINTF("* 6. Write RSC Measurement CFG. Notify\r\n");
    QPRINTF("* 7. SC Control Point CFG. Indication\r\n");
    QPRINTF("* 8. SC Control Point\r\n");
    app_menu_show_line();
}

static void app_menu_handler_rscpc(void)
{
    switch (app_env.input[0])
    {
        case '0':
            app_env.menu_id = menu_rscpc_enable;
            break;
        case '1':
            app_eaci_data_req_rscpc(EACI_MSG_DATA_REQ_RSCPC_ENABLE, 6, (const uint8_t *)&app_env.dev_rec[app_env.sel_id].addr);
            break;
        case '2':
        {
            uint8_t read_code[7] = {0,0,0,0,0,0,0};
            read_code[0] = RSCPC_RD_RSC_FEAT;
            memcpy((read_code + 1), (const uint8_t *)&app_env.dev_rec[app_env.sel_id].addr, 6);
            app_eaci_data_req_rscpc(EACI_MSG_DATA_REQ_RSCPC_RD, 7, read_code);
        }
            break;
        case '3':
        {
            uint8_t read_code[7] = {0,0,0,0,0,0,0};
            read_code[0] = RSCPC_RD_SENSOR_LOC;
            memcpy((read_code + 1), (const uint8_t *)&app_env.dev_rec[app_env.sel_id].addr, 6);
            app_eaci_data_req_rscpc(EACI_MSG_DATA_REQ_RSCPC_RD, 7, read_code);
        }
            break;
        case '4':
        {
            uint8_t read_code[7] = {0,0,0,0,0,0,0};
            read_code[0] = RSCPC_RD_WR_RSC_MEAS_CFG;
            memcpy((read_code + 1), (const uint8_t *)&app_env.dev_rec[app_env.sel_id].addr, 6);
            app_eaci_data_req_rscpc(EACI_MSG_DATA_REQ_RSCPC_RD, 7, read_code);
        }
            break;
        case '5':
        {
            uint8_t read_code[7] = {0,0,0,0,0,0,0};
            read_code[0] = RSCPC_RD_WR_SC_CTNL_PT_CFG;
            memcpy((read_code + 1), (const uint8_t *)&app_env.dev_rec[app_env.sel_id].addr, 6);
            app_eaci_data_req_rscpc(EACI_MSG_DATA_REQ_RSCPC_RD, 7, read_code);
        }
            break;
        case '6':
        {
            uint8_t ntfind_cfg[9] = {0,0,0,0,0,0,0,0,0};
            ntfind_cfg[0] = RSCPC_RD_WR_RSC_MEAS_CFG;
            ntfind_cfg[1] = PRF_CLI_START_NTF;
            ntfind_cfg[2] = 0x00;
            memcpy((ntfind_cfg + 3), (const uint8_t *)&app_env.dev_rec[app_env.sel_id].addr, 6);
            app_eaci_data_req_rscpc(EACI_MSG_DATA_REQ_RSCPC_CFG_NTFIND, 9, ntfind_cfg);
        }
            break;
        case '7':
        {
            uint8_t ntfind_cfg[9] = {0,0,0,0,0,0,0,0,0};
            ntfind_cfg[0] = RSCPC_RD_WR_SC_CTNL_PT_CFG;
            ntfind_cfg[1] = PRF_CLI_START_IND;
            ntfind_cfg[2] = 0x00;
            memcpy((ntfind_cfg + 3), (const uint8_t *)&app_env.dev_rec[app_env.sel_id].addr, 6);
            app_eaci_data_req_rscpc(EACI_MSG_DATA_REQ_RSCPC_CFG_NTFIND, 9, ntfind_cfg);
        }
            break;
        case '8':
            app_env.menu_id = menu_rscpc_ctnl_pt;
            break;
        case 'r':
            app_env.menu_id = menu_main;
            break;
        case 's':
        default:
            break;
    }
    app_menu_show();
}

static void app_menu_show_rscpc_ctnl_pt(void)
{
    app_menu_show_line();
    QPRINTF("* QN BLE RSCPC Conrol Menu\r\n");
    QPRINTF("* 1. Set Cumulative Value\r\n");
    QPRINTF("* 2. Update Sensor Location\r\n");
    QPRINTF("* 3. Request Supported Sensor Locations\r\n");
    app_menu_show_line();
}

static void app_menu_handler_rscpc_ctnl_pt(void)
{
    switch (app_env.input[0])
    {
        case '1':
        {
            uint8_t sc_ctnl_pt[11] = {0,0,0,0,0,0,0,0,0,0,0};
            sc_ctnl_pt[0] = RSCP_CTNL_PT_OP_SET_CUMUL_VAL;
            sc_ctnl_pt[1] = 0x00;
            sc_ctnl_pt[2] = 0x00;
            sc_ctnl_pt[3] = 0x00;
            sc_ctnl_pt[4] = 0x00;
            memcpy((sc_ctnl_pt + 5), (const uint8_t *)&app_env.dev_rec[app_env.sel_id].addr, 6);
            app_eaci_data_req_rscpc(EACI_MSG_DATA_REQ_RSCPC_CFG_CTNL_PT, 11, sc_ctnl_pt);
        }
            break;
        case '2':
        {
            uint8_t sc_ctnl_pt[11] = {0,0,0,0,0,0,0,0,0,0,0};
            sc_ctnl_pt[0] = RSCP_CTNL_PT_OP_UPD_LOC;
            sc_ctnl_pt[1] = RSCP_LOC_TOP_SHOE;
            sc_ctnl_pt[2] = 0x00;
            sc_ctnl_pt[3] = 0x00;
            sc_ctnl_pt[4] = 0x00;
            memcpy((sc_ctnl_pt + 5), (const uint8_t *)&app_env.dev_rec[app_env.sel_id].addr, 6);
            app_eaci_data_req_rscpc(EACI_MSG_DATA_REQ_RSCPC_CFG_CTNL_PT, 11, sc_ctnl_pt);
        }
            break;
        case '3':
        {
            uint8_t sc_ctnl_pt[11] = {0,0,0,0,0,0,0,0,0,0,0};
            sc_ctnl_pt[0] = RSCP_CTNL_PT_OP_REQ_SUPP_LOC;
            sc_ctnl_pt[1] = 0x0f;
            sc_ctnl_pt[2] = 0x00;
            sc_ctnl_pt[3] = 0x00;
            sc_ctnl_pt[4] = 0x00;
            memcpy((sc_ctnl_pt + 5), (const uint8_t *)&app_env.dev_rec[app_env.sel_id].addr, 6);
            app_eaci_data_req_rscpc(EACI_MSG_DATA_REQ_RSCPC_CFG_CTNL_PT, 11, sc_ctnl_pt);
        }
            break;
        case 'r':
            app_env.menu_id = menu_rscpc;
            break;
        case 's':
        default:
            break;
    }
    app_menu_show();
}
#endif
#if BLE_RSC_SENSOR
static void app_menu_show_rscps(void)
{
    app_menu_show_line();
    QPRINTF("* QN BLE RSCPS Menu\r\n");
    QPRINTF("* 1. Send the RSC Sensor Measurement value\r\n");
    app_menu_show_line();
}

static void app_menu_handler_rscps(void)
{
    switch (app_env.input[0])
    {
        case '1':
        {
            uint8_t rsc_meas[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
            rsc_meas[0] = RSCP_MEAS_INST_STRIDE_LEN_PRESENT | RSCP_MEAS_TOTAL_DST_MEAS_PRESENT;
            rsc_meas[1] = 0x32; //inst_cad
            rsc_meas[2] = 0x3c; 
            rsc_meas[3] = 0x00; //inst_speed
            rsc_meas[4] = 0x32;
            rsc_meas[5] = 0x00; //inst_stride_len
            rsc_meas[6] = 0xd0;
            rsc_meas[7] = 0x07;
            rsc_meas[8] = 0x00;
            rsc_meas[9] = 0x00; //total_dist
            memcpy((rsc_meas + 10), (const uint8_t *)&app_env.dev_rec[app_env.sel_id].addr, 6);
            app_eaci_data_req_rscps(EACI_MSG_DATA_REQ_RSCPS_NTF_MEAS, 16, rsc_meas);
        }
            break;
        case 'r':
            app_env.menu_id = menu_main;
            break;
        case 's':
        default:
            break;
    }
    app_menu_show();
}

#endif

/**
*************************************************************************************
*** Scan Profile
*************************************************************************************
 */

#if BLE_SP_CLIENT
static void app_menu_show_scppc(void)
{
    app_menu_show_line();
    QPRINTF("* QN BLE SCPPC Menu\r\n");
    QPRINTF("* 0. Select device\r\n");
    QPRINTF("* 1. Enable\r\n");
    QPRINTF("* 2. Read Scan Refresh Ntf. Cfg. Value\r\n");
    QPRINTF("* 3. Write Scan Interval Window Characteristic Value\r\n");
    QPRINTF("* 4. Notification Configuration Stop  Ntf\r\n");
    QPRINTF("* 5. Notification Configuration Start Ntf\r\n");
    app_menu_show_line();
}

static void app_menu_handler_scppc(void)
{
    switch (app_env.input[0])
    {
        case '0':
            app_env.menu_id = menu_scppc_enable;
            break;
        case '1':
            app_eaci_data_req_scppc(EACI_MSG_DATA_REQ_SP_ENABLE, 6, (const uint8_t *)&app_env.dev_rec[app_env.sel_id].addr);
            break;
        case '2':
            app_eaci_data_req_scppc(EACI_MSG_DATA_REQ_SP_RD_CFG, 6, (const uint8_t *)&app_env.dev_rec[app_env.sel_id].addr);
            break;
        case '3':
            app_eaci_data_req_scppc(EACI_MSG_DATA_REQ_SP_WR_WD, 6, (const uint8_t *)&app_env.dev_rec[app_env.sel_id].addr);
            break;
        case '4':
        case '5':
        {
            app_env.input[0] -= '4';
            uint8_t ntf_cfg[7] = {0,0,0,0,0,0,0};
            ntf_cfg[0] = app_env.input[0];
            memcpy((ntf_cfg + 1), (const uint8_t *)&app_env.dev_rec[app_env.sel_id].addr, 6);
            app_eaci_data_req_scppc(EACI_MSG_DATA_REQ_SP_WR_MEAS, 7, ntf_cfg);
        }
            break;
        case 'r':
            app_env.menu_id = menu_main;
            break;
        case 's':
        default:
            break;
    }
    app_menu_show();
}
#endif
#if BLE_SP_SERVER
static void app_menu_show_scpps(void)
{
    app_menu_show_line();
    QPRINTF("* QN BLE SCPPS Menu\r\n");
    QPRINTF("* 1. Send the scan refresh value\r\n");
    app_menu_show_line();
}

static void app_menu_handler_scpps(void)
{
    switch (app_env.input[0])
    {
        case '1':
            app_eaci_data_req_scpps(EACI_MSG_DATA_REQ_SP_SCAN_REFRESH_REQ);
            break;
        case 'r':
            app_env.menu_id = menu_main;
            break;
        case 's':
        default:
            break;
    }
    app_menu_show();
}
#endif

/**
*************************************************************************************
*** Time Profile
*************************************************************************************
 */

#if BLE_TIP_CLIENT
static void app_menu_show_tipc(void)
{
    app_menu_show_line();
    QPRINTF("* QN BLE TIPC Menu\r\n");
    QPRINTF("* 0. Select device\r\n");
    QPRINTF("* 1. Enable\r\n");
    QPRINTF("* 2. Read CTS Current Time\r\n");
    QPRINTF("* 3. Read CTS Local Time Information\r\n");
    QPRINTF("* 4. Read CTS Reference Time Information\r\n");
    QPRINTF("* 5. Read CTS Current Time Cfg.\r\n");
    QPRINTF("* 6. Read NDCS Time With DST\r\n");
    QPRINTF("* 7. Read Time Update State\r\n");
    QPRINTF("* 8. Configuring the Current Time Characteristic Start Notify\r\n");
    QPRINTF("* 9. Writing the Time Update Control Point on the Server\r\n");
    app_menu_show_line();
}

static void app_menu_handler_tipc(void)
{
    switch (app_env.input[0])
    {
        case '0':
            app_env.menu_id = menu_tipc_enable;
            break;
        case '1':
            app_eaci_data_req_tipc(EACI_MSG_DATA_REQ_TIP_ENABLE, 6, (const uint8_t *)&app_env.dev_rec[app_env.sel_id].addr);
            break;
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        {
            uint8_t param[7] = {0,0,0,0,0,0,0};

            switch(app_env.input[0])
            {
                case '2':
                    param[0] = TIPC_RD_CTS_CURR_TIME;
                    break;
                case '3':
                    param[0] = TIPC_RD_CTS_LOCAL_TIME_INFO;
                    break;
                case '4':
                    param[0] = TIPC_RD_CTS_REF_TIME_INFO;
                    break;
                case '5':
                    param[0] = TIPC_RD_CTS_CURR_TIME_CLI_CFG;
                    break;
                case '6':
                    param[0] = TIPC_RD_NDCS_TIME_WITH_DST;
                    break;
                case '7':
                    param[0] = TIPC_RD_RTUS_TIME_UPD_STATE;
                    break;
                default:
                    break;
            }
            memcpy((param + 1), (const uint8_t *)&app_env.dev_rec[app_env.sel_id].addr, 6);
            app_eaci_data_req_tipc(EACI_MSG_DATA_REQ_TIP_CHAR_VAL_RD, 7, param);
        }
            break;
        case '8':
        {
            uint8_t param[7] = {0,0,0,0,0,0,0};
            param[0] = PRF_CLI_START_NTF;
            memcpy((param + 1), (const uint8_t *)&app_env.dev_rec[app_env.sel_id].addr, 6);
            app_eaci_data_req_tipc(EACI_MSG_DATA_REQ_TIP_CT_NTF_CFG, 7, param);
        }
            break;
        case '9':
        {
            uint8_t param[7] = {0,0,0,0,0,0,0};
            param[0] = TIPS_TIME_UPD_CTNL_PT_GET;
            memcpy((param + 1), (const uint8_t *)&app_env.dev_rec[app_env.sel_id].addr, 6);
            app_eaci_data_req_tipc(EACI_MSG_DATA_REQ_TIP_WR_UDP_CTNL_PT, 7, param);
        }
            break;
        case 'r':
            app_env.menu_id = menu_main;
            break;
        case 's':
        default:
            break;
    }
    app_menu_show();
}
#endif
#if BLE_TIP_SERVER
static void app_menu_show_tips(void)
{
    app_menu_show_line();
    QPRINTF("* QN BLE TIPS Menu\r\n");
    QPRINTF("* 1. Send update current time request\r\n");
    app_menu_show_line();
}

static void app_menu_handler_tips(void)
{
    switch (app_env.input[0])
    {
        case '1':
        {
            uint8_t param[8] = {221, 7, 5, 10, 6, 6, 6, 5};
            app_eaci_data_req_tips(EACI_MSG_DATA_TIPS_UPD_CURR_TIME, 8, param);
        }
            break;
        case 'r':
            app_env.menu_id = menu_main;
            break;
        case 's':
        default:
            break;
    }
    app_menu_show();
}
#endif

/**
 ****************************************************************************************
 * @brief Show the BLE demo application.
 *
 ****************************************************************************************
 */
static void app_menu_show(void)
{
    switch (app_env.menu_id)
    {
        case menu_start:
            break;
        case menu_main:
            app_menu_show_main();
            break;
        case menu_gap:
            app_menu_show_gap();
            break;
        case menu_gap_create_connection:
            app_menu_show_gap_inq_record();
            break;
        case menu_gap_bond:
        case menu_gap_disconnection:
        case menu_gap_update_param:
        case menu_hrpc_enable:
        case menu_proxm_enable:
        case menu_htpc_enable:
        case menu_blpc_enable:
        case menu_glpc_enable:
        case menu_findl_enable:
        case menu_tipc_enable:
        case menu_scppc_enable:
        case menu_disc_enable:
        case menu_basc_enable:
        case menu_anpc_enable:
        case menu_cscpc_enable:
        case menu_paspc_enable:
        case menu_rscpc_enable:
            app_menu_show_gap_dev_record();
            break;

#if BLE_AN_CLIENT
        case menu_anpc:
            app_menu_show_anpc();
            break;
        case menu_anpc_alert:
            app_menu_show_anpc_alert();
            break;
#endif
#if BLE_AN_SERVER
        case menu_anps:
            app_menu_show_anps();
            break;
#endif

#if BLE_BATT_CLIENT
        case menu_basc:
            app_menu_show_basc();
            break;
#endif
#if BLE_BATT_SERVER
        case menu_bass:
            app_menu_show_bass();
            break;
#endif

#if BLE_BP_COLLECTOR
        case menu_blpc:
            app_menu_show_blpc();
            break;
#endif
#if BLE_BP_SENSOR
        case menu_blps:
            app_menu_show_blps();
            break;
#endif

#if BLE_CSC_COLLECTOR
        case menu_cscpc:
            app_menu_show_cscpc();
            break;
        case menu_cscpc_ctnl_pt:
            app_menu_show_cscpc_ctnl_pt_cfm();
            break;
#endif
#if BLE_CSC_SENSOR
        case menu_cscps:
            app_menu_show_cscps();
            break;
#endif

#if BLE_DIS_CLIENT
        case menu_disc:
            app_menu_show_disc();
            break;
#endif
#if BLE_DIS_SERVER
        case menu_diss:
            app_menu_show_diss();
            break;
#endif

#if BLE_FINDME_LOCATOR
        case menu_findl:
            app_menu_show_findl();
            break;
#endif

#if BLE_GL_COLLECTOR
        case menu_glpc:
            app_menu_show_glpc();
            break;
#endif
#if BLE_GL_SENSOR
        case menu_glps:
            app_menu_show_glps();
            break;
#endif

#if BLE_HR_COLLECTOR
        case menu_hrpc:
            app_menu_show_hrpc();
            break;
#endif
#if BLE_HR_SENSOR
        case menu_hrps:
            app_menu_show_hrps();
            break;
#endif

#if BLE_HT_COLLECTOR
        case menu_htpc:
            app_menu_show_htpc();
            break;
#endif
#if BLE_HT_THERMOM
        case menu_htpt:
            app_menu_show_htpt();
            break;
        case menu_htpt_flag:
            app_menu_show_htpt_flag();
            break;
#endif

#if BLE_PAS_CLIENT
        case menu_paspc:
            app_menu_show_paspc();
            break;
        case menu_paspc_ringer_pt:
            app_menu_show_paspc_ringer_pt();
            break;
#endif
#if BLE_PAS_SERVER
        case menu_pasps:
            app_menu_show_pasps();
            break;
#endif

#if BLE_PROX_MONITOR
        case menu_proxm:
            app_menu_show_proxm();
            break;
#endif

#if BLE_RSC_COLLECTOR
        case menu_rscpc:
            app_menu_show_rscpc();
            break;
        case menu_rscpc_ctnl_pt:
            app_menu_show_rscpc_ctnl_pt();
            break;
#endif
#if BLE_RSC_SENSOR
        case menu_rscps:
            app_menu_show_rscps();
            break;
#endif

#if BLE_SP_CLIENT
        case menu_scppc:
            app_menu_show_scppc();
            break;
#endif
#if BLE_SP_SERVER
        case menu_scpps:
            app_menu_show_scpps();
            break;
#endif

#if BLE_TIP_CLIENT
        case menu_tipc:
            app_menu_show_tipc();
            break;
#endif
#if BLE_TIP_SERVER
        case menu_tips:
            app_menu_show_tips();
            break;
#endif

        case menu_findt:
        default:
            break;
    }
}

/**
 ****************************************************************************************
 * @brief Handler function of the BLE demo application.
 *
 ****************************************************************************************
 */
void app_menu_hdl(void)
{
    switch (app_env.menu_id)
    {
        case menu_start:
            break;
        case menu_main:
            app_menu_handler_main();
            break;
        case menu_gap:
            app_menu_handler_gap();
            break;

#if BLE_AN_CLIENT
        case menu_anpc:
            app_menu_handler_anpc();
            break;
        case menu_anpc_alert:
            app_menu_handler_anpc_alert();
            break;
#endif
#if BLE_AN_SERVER
        case menu_anps:
            app_menu_handler_anps();
            break;
#endif

#if BLE_BATT_CLIENT
        case menu_basc:
            app_menu_handler_basc();
            break;
#endif
#if BLE_BATT_SERVER
        case menu_bass:
            app_menu_handler_bass();
            break;
#endif

#if BLE_BP_COLLECTOR
        case menu_blpc:
            app_menu_handler_blpc();
            break;
#endif
#if BLE_BP_SENSOR
        case menu_blps:
            app_menu_handler_blps();
            break;
#endif

#if BLE_CSC_COLLECTOR
        case menu_cscpc:
            app_menu_handler_cscpc();
            break;
        case menu_cscpc_ctnl_pt:
            app_menu_handler_cscpc_ctnl_pt_cfm();
            break;
#endif
#if BLE_CSC_SENSOR
        case menu_cscps:
            app_menu_handler_cscps();
            break;
#endif

#if BLE_DIS_CLIENT
        case menu_disc:
            app_menu_handler_disc();
            break;
#endif
#if BLE_DIS_SERVER
        case menu_diss:
            app_menu_handler_diss();
            break;
#endif

#if BLE_FINDME_LOCATOR
        case menu_findl:
            app_menu_handler_findl();
            break;
#endif

#if BLE_GL_COLLECTOR
        case menu_glpc:
            app_menu_handler_glpc();
            break;
#endif
#if BLE_GL_SENSOR
        case menu_glps:
            app_menu_handler_glps();
            break;
#endif

#if BLE_HR_COLLECTOR
        case menu_hrpc:
            app_menu_handler_hrpc();
            break;
#endif
#if BLE_HR_SENSOR
        case menu_hrps:
            app_menu_handler_hrps();
            break;
#endif

#if BLE_HT_COLLECTOR
        case menu_htpc:
            app_menu_handler_htpc();
            break;
#endif
#if BLE_HT_THERMOM
        case menu_htpt:
            app_menu_handler_htpt();
            break;
        case menu_htpt_flag:
            app_menu_handler_htpt_flag();
            break;
#endif

#if BLE_PAS_CLIENT
        case menu_paspc:
            app_menu_handler_paspc();
            break;
        case menu_paspc_ringer_pt:
            app_menu_handler_paspc_ringer_pt();
            break;
#endif
#if BLE_PAS_SERVER
        case menu_pasps:
            app_menu_handler_pasps();
            break;
#endif

#if BLE_PROX_MONITOR
        case menu_proxm:
            app_menu_handler_proxm();
            break;
#endif

#if BLE_RSC_COLLECTOR
        case menu_rscpc:
            app_menu_handler_rscpc();
            break;
        case menu_rscpc_ctnl_pt:
            app_menu_handler_rscpc_ctnl_pt();
            break;
#endif
#if BLE_RSC_SENSOR
        case menu_rscps:
            app_menu_handler_rscps();
            break;
#endif

#if BLE_SP_CLIENT
        case menu_scppc:
            app_menu_handler_scppc();
            break;
#endif
#if BLE_SP_SERVER
        case menu_scpps:
            app_menu_handler_scpps();
            break;
#endif

#if BLE_TIP_CLIENT
        case menu_tipc:
            app_menu_handler_tipc();
            break;
#endif
#if BLE_TIP_SERVER
        case menu_tips:
            app_menu_handler_tips();
            break;
#endif

        case menu_gap_bond:
        case menu_gap_create_connection:
        case menu_gap_update_param:
        case menu_gap_disconnection:

        case menu_htpc_enable:
        case menu_blpc_enable:
        case menu_hrpc_enable:
        case menu_glpc_enable:
        case menu_findl_enable:
        case menu_proxm_enable:
        case menu_tipc_enable:
        case menu_scppc_enable:
        case menu_disc_enable:
        case menu_basc_enable:
        case menu_anpc_enable:
        case menu_cscpc_enable:
        case menu_paspc_enable:
        case menu_rscpc_enable:
            app_menu_handler_gap_dev_select();
            break;

        case menu_findt:

        default:
            break;
    }
}
