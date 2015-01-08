/**
 ****************************************************************************************
 *
 * @file app_menu.c
 *
 * @brief Application DEMO menu
 *
 * Copyright (C) Quintic 2012-2013
 *
 * $Rev: 1.0 $
 *
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "app_env.h"
#include "app_eapi.h"

#if QN_DEMO_MENU
#include "app_menu.h"

static void app_menu_show_line(void)
{
    QPRINTF("*------------------------\r\n");
}

static void app_menu_show_start(void)
{
    app_menu_show_line();
    QPRINTF("* QN BLE Demo Application\r\n");
}

static void app_menu_show_main(void)
{
    app_menu_show_line();
    QPRINTF("* QN BLE Main Menu\r\n");
    QPRINTF("* 1. GAP   Menu\r\n");
#if QN_SVC_DISC_USED
    QPRINTF("* 2. GATT  Menu\r\n");
#endif
#if BLE_HT_COLLECTOR
    QPRINTF("* 4. HTPC  Menu\r\n");
#endif
#if BLE_BP_COLLECTOR
    QPRINTF("* 5. BLPC  Menu\r\n");
#endif
#if BLE_HR_COLLECTOR
    QPRINTF("* 6. HRPC  Menu\r\n");
#endif
#if BLE_GL_COLLECTOR
    QPRINTF("* 7. GLPC  Menu\r\n");
#endif
#if BLE_FINDME_LOCATOR
    QPRINTF("* 8. FMPL  Menu\r\n");
#endif
#if BLE_PROX_MONITOR
    QPRINTF("* 9. PROXM Menu\r\n");
#endif
#if BLE_TIP_CLIENT
    QPRINTF("* a. TIPC  Menu\r\n");
#endif
#if (BLE_HID_BOOT_HOST || BLE_HID_REPORT_HOST)
    QPRINTF("* b. HOGH  Menu\r\n");
#endif
#if BLE_SP_CLIENT
    QPRINTF("* c. SCPPC Menu\r\n");
#endif
#if BLE_DIS_CLIENT
    QPRINTF("* d. DISC  Menu\r\n");
#endif
#if BLE_BATT_CLIENT
    QPRINTF("* e. BASC  Menu\r\n");
#endif
#if BLE_AN_CLIENT
    QPRINTF("* f. ANPC  Menu\r\n");
#endif
#if BLE_CSC_COLLECTOR
    QPRINTF("* h. CSCPC  Menu\r\n");
#endif
#if BLE_RSC_COLLECTOR
    QPRINTF("* i. RSCPC  Menu\r\n");
#endif
#if BLE_PAS_CLIENT
    QPRINTF("* g. PASPC  Menu\r\n");
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
#if QN_SVC_DISC_USED
    case '2':
        app_env.menu_id = menu_gatt;
        break;
#endif
#if BLE_HT_COLLECTOR
    case '4':
        app_env.menu_id = menu_htpc;
        break;
#endif
#if BLE_BP_COLLECTOR
    case '5':
        app_env.menu_id = menu_blpc;
        break;
#endif
#if BLE_HR_COLLECTOR
    case '6':
        app_env.menu_id = menu_hrpc;
        break;
#endif
#if BLE_GL_COLLECTOR
    case '7':
        app_env.menu_id = menu_glpc;
        break;
#endif
#if BLE_FINDME_LOCATOR
    case '8':
        app_env.menu_id = menu_findl;
        break;
#endif
#if BLE_PROX_MONITOR
    case '9':
        app_env.menu_id = menu_proxm;
        break;
#endif
#if BLE_TIP_CLIENT
    case 'a':
        app_env.menu_id = menu_tipc;
        break;
#endif
#if (BLE_HID_BOOT_HOST || BLE_HID_REPORT_HOST)
    case 'b':
        app_env.menu_id = menu_hogph;
        break;
#endif
#if BLE_SP_CLIENT
    case 'c':
        app_env.menu_id = menu_scppc;
        break;
#endif
#if BLE_DIS_CLIENT
    case 'd':
        app_env.menu_id = menu_disc;
        break;
#endif
#if BLE_BATT_CLIENT
    case 'e':
        app_env.menu_id = menu_basc;
        break;
#endif
#if BLE_AN_CLIENT
    case 'f':
        app_env.menu_id = menu_anpc;
        break;
#endif
#if BLE_CSC_COLLECTOR
    case 'h':
        app_env.menu_id = menu_cscpc;
        break;
#endif
#if BLE_RSC_COLLECTOR
    case 'i':
        app_env.menu_id = menu_rscpc;
        break;
#endif
#if BLE_PAS_CLIENT
    case 'g':
        app_env.menu_id = menu_paspc;
        break;
#endif
    case 'r':
    case 's':
    default:
        break;
    }
    app_menu_show();
}

static void app_menu_show_gap(void)
{
    app_menu_show_line();
    QPRINTF("* QN BLE GAP Menu\r\n");
    QPRINTF("* 1. Inquiry Start\r\n");
    QPRINTF("* 2. Inquiry Stop\r\n");
    QPRINTF("* 3. Advertisement Start\r\n");
    QPRINTF("* 4. Advertisement Stop\r\n");
    QPRINTF("* 5. Create Connection\r\n");
    QPRINTF("* 6. Disconnection\r\n");
    QPRINTF("* 7. Pair Request\r\n");
    QPRINTF("* 8. UnPair\r\n");
    app_menu_show_line();
}

static void app_menu_handler_gap(void)
{
    switch (app_env.input[0])
    {
    case '1':
        eapi_app_gap_dev_scan(TRUE);
        break;
    case '2':
        eapi_app_gap_dev_scan(FALSE);
        break;
    case '3':
        QPRINTF("Advertising start.\r\n");
        eapi_app_gap_advertising(TRUE);
        break;
    case '4':
        QPRINTF("Advertising stop.\r\n");
        eapi_app_gap_advertising(FALSE);
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
        app_env.menu_id = menu_gap_unpair;
        app_menu_show();
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
    if (app_env.inq_idx > 0)
    {
        QPRINTF("\r\n");
        QPRINTF("Inquiry device record:\r\n");
        for (uint8_t i = 0; i < app_env.inq_idx; i++)
        {
            QPRINTF("%d. %02X%02X:%02X:%02X%02X%02X\r\n", i, 
                app_env.inq_addr[i].addr[5],
                app_env.inq_addr[i].addr[4],
                app_env.inq_addr[i].addr[3],
                app_env.inq_addr[i].addr[2],
                app_env.inq_addr[i].addr[1],
                app_env.inq_addr[i].addr[0]);
        }
        QPRINTF("a. All device\r\n"); 
        QPRINTF("Please input the device index:\r\n");
    }
    else
    {
        QPRINTF("Inquiry device record is NULL.\r\n");
        app_env.menu_id = menu_gap;
    }
}

static void app_menu_show_gap_dev_record(void)
{
    if (app_get_link_nb())
    {
        QPRINTF("\r\n");
        QPRINTF("Connected device records (%d):\r\n", app_get_link_nb());
        for (uint8_t idx = 0; idx < BLE_CONNECTION_MAX; idx++)
        {
            struct bd_addr peer_addr;
            if (app_get_bd_addr_by_idx(idx, &peer_addr))
            {
                QPRINTF("%d. %02X%02X:%02X:%02X%02X%02X\r\n", idx, 
                    peer_addr.addr[5],
                    peer_addr.addr[4],
                    peer_addr.addr[3],
                    peer_addr.addr[2],
                    peer_addr.addr[1],
                    peer_addr.addr[0]);
            }
        }
        if (app_env.menu_id != menu_gatt_disc_all_svc
         && app_env.menu_id != menu_gap_bond
         && app_env.menu_id != menu_htpc_enable
         && app_env.menu_id != menu_blpc_enable
         && app_env.menu_id != menu_hrpc_enable
         && app_env.menu_id != menu_glpc_enable
         && app_env.menu_id != menu_findl_enable
         && app_env.menu_id != menu_proxm_enable
         && app_env.menu_id != menu_tipc_enable
         && app_env.menu_id != menu_hogpbh_enable
         && app_env.menu_id != menu_hogprh_enable
         && app_env.menu_id != menu_scppc_enable
         && app_env.menu_id != menu_disc_enable
         && app_env.menu_id != menu_basc_enable
         && app_env.menu_id != menu_anpc_enable
         && app_env.menu_id != menu_cscpc_enable
         && app_env.menu_id != menu_paspc_enable
         && app_env.menu_id != menu_rscpc_enable
         )
            QPRINTF("a. All device\r\n");
        QPRINTF("Please input the device index:\r\n");
    }
    else
    {
        QPRINTF("Connected device record is NULL.\r\n");
        if (app_env.menu_id == menu_gatt_disc_all_svc)
            app_env.menu_id = menu_gatt;
        else if (app_env.menu_id == menu_htpc_enable)
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
        else if (app_env.menu_id == menu_hogpbh_enable)
            app_env.menu_id = menu_hogph;
        else if (app_env.menu_id == menu_hogprh_enable)
            app_env.menu_id = menu_hogph;
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
    }
}

static void app_menu_show_gap_bond_record(void)
{
    uint8_t nb = app_get_bond_nb();
    if (nb > 0)
    {
        QPRINTF("\r\n");
        QPRINTF("Bonded device records (%d):\r\n", nb);
        for (uint8_t idx = 0; idx < BLE_CONNECTION_MAX; idx++)
        {
            if (app_get_bond_status(idx, SMP_KDIST_ENCKEY|SMP_KDIST_SIGNKEY|SMP_KDIST_SIGNKEY))
            {
                QPRINTF("%d. %02X%02X:%02X:%02X%02X%02X\r\n", idx, 
                    app_env.dev_rec[idx].bonded_info.peer_addr.addr[5],
                    app_env.dev_rec[idx].bonded_info.peer_addr.addr[4],
                    app_env.dev_rec[idx].bonded_info.peer_addr.addr[3],
                    app_env.dev_rec[idx].bonded_info.peer_addr.addr[2],
                    app_env.dev_rec[idx].bonded_info.peer_addr.addr[1],
                    app_env.dev_rec[idx].bonded_info.peer_addr.addr[0]);
            }
        }
        if (app_env.menu_id != menu_gatt_disc_all_svc
            &&
            app_env.menu_id != menu_gap_unpair)
            QPRINTF("a. All device\r\n");
        QPRINTF("Please input the device index:\r\n");
    }
    else
    {
        QPRINTF("Bonded device record is NULL.\r\n");
        app_env.menu_id = menu_gap;
    }
}

static void app_menu_handler_gap_dev_select(void)
{
    if (app_env.input[0] - '0' < BLE_CONNECTION_MAX
     || app_env.input[0] == 'a')
    {
        app_env.select_idx = app_env.input[0] - '0';

        switch (app_env.menu_id)
        {
        case menu_gap_bond:
            {
                struct bd_addr addr;
                if (app_get_bd_addr_by_idx(app_env.select_idx, &addr))
                {                    
                    eapi_app_gap_security_req(&addr);
                }
                else
                    QPRINTF("The selected device is not in connected status.\r\n");
            }
            break;
        case menu_gap_unpair:
            {
                struct bd_addr addr;
                if (app_get_bd_addr_by_idx(app_env.select_idx, &addr))
                {
                    app_gap_unpair_req(&addr, app_get_bond_nb());
                }
                else
                    QPRINTF("The selected device is not in bonded status.\r\n");
            }
            break;
        case menu_gap_create_connection:
            if (app_env.input[0] == 'a')
                for (uint8_t i = 0; i < app_env.inq_idx; i++)
                    eapi_app_gap_conn_req(TRUE,&app_env.inq_addr[i]);
            else
                eapi_app_gap_conn_req(1,&app_env.inq_addr[app_env.select_idx]);
            break;
        case menu_gap_disconnection:
            if (app_env.input[0] == 'a')
            {
                for (uint8_t idx = 0; idx < BLE_CONNECTION_MAX; idx++)
                {
                    uint16_t conhdl = app_get_conhdl_by_idx(idx);
                    if (conhdl != 0xFFFF)
                    {
                        app_gap_discon_req(conhdl);
                    }
                }
            }
            else
            {
                uint16_t conhdl = app_get_conhdl_by_idx(app_env.select_idx);
                if (conhdl != 0xFFFF)
                    app_gap_discon_req(conhdl);
                else
                    QPRINTF("The selected device is not in connected status.\r\n");
            }
            break;
        case menu_gatt_disc_all_svc:
            {
                uint16_t conhdl = app_get_conhdl_by_idx(app_env.select_idx);
                if (conhdl != 0xFFFF)
                {
#if QN_SVC_DISC_USED
                    app_env.dev_rec[app_env.select_idx].nb_svc = 0;
                    app_gatt_disc_svc_req(GATT_DISC_ALL_SVC, conhdl);
#endif
                }
                else
                    QPRINTF("The selected device is not in connected status.\r\n");
            }
            break;
#if BLE_HT_COLLECTOR
            case menu_htpc_enable:
            {
                uint8_t idx = app_env.select_idx;
                uint16_t conhdl = app_get_conhdl_by_idx(idx);
                if (0xFFFF != conhdl
                    &&
                    false == app_get_client_service_status(idx, ATT_SVC_HEALTH_THERMOM)
                    )
                {
                    eapi_prf_client_enable_req(EAPI_PRF_THERMOMETER, conhdl);
                }
                else
                    QPRINTF("Enable HTPC disallowed.\r\n");
            }
            break;
#endif
#if BLE_BP_COLLECTOR
            case menu_blpc_enable:
            {
                uint8_t idx = app_env.select_idx;
                uint16_t conhdl = app_get_conhdl_by_idx(idx);
                if (0xFFFF != conhdl
                    &&
                    false == app_get_client_service_status(idx, ATT_SVC_BLOOD_PRESSURE)
                    )
                {
                    eapi_prf_client_enable_req(EAPI_PRF_BLOODPRESSURE, conhdl);
                }
                else
                    QPRINTF("Enable BLPC disallowed.\r\n");
            }
            break;
#endif
#if BLE_HR_COLLECTOR
            case menu_hrpc_enable:
            {
                uint8_t idx = app_env.select_idx;
                uint16_t conhdl = app_get_conhdl_by_idx(idx);
                if (0xFFFF != conhdl
                    &&
                    false == app_get_client_service_status(idx, ATT_SVC_HEART_RATE)
                    )
                {
                    eapi_prf_client_enable_req(EAPI_PRF_HEARTRATE, conhdl);
                }
                else
                    QPRINTF("Enable HRPC disallowed.\r\n");
            }
            break;
#endif
#if BLE_GL_COLLECTOR
            case menu_glpc_enable:
            {
                uint8_t idx = app_env.select_idx;
                uint16_t conhdl = app_get_conhdl_by_idx(idx);
                if (0xFFFF != conhdl
                    &&
                    false == app_get_client_service_status(idx, ATT_SVC_GLUCOSE)
                    )
                {
#if QN_SVC_CONTENT_USED	
                    if (app_check_service_content_available(conhdl, ATT_SVC_GLUCOSE))
                        app_glpc_enable_req(&app_glpc_env->gls, conhdl);
                    else
#endif
                        app_glpc_enable_req(NULL, conhdl);
                }        
                else
                    QPRINTF("Enable GLPC disallowed.\r\n");
            }
            break;
#endif
#if BLE_FINDME_LOCATOR
            case menu_findl_enable:
            {
                uint8_t idx = app_env.select_idx;
                uint16_t conhdl = app_get_conhdl_by_idx(idx);
                if (0xFFFF != conhdl
                    &&
                    false == app_get_client_service_status(idx, ATT_SVC_IMMEDIATE_ALERT)
                    )
                {
                    eapi_prf_client_enable_req(EAPI_PRF_FINDME, conhdl);
                }
                else
                    QPRINTF("Enable FMPL disallowed.\r\n");
            }
            break;
#endif
#if BLE_PROX_MONITOR
            case menu_proxm_enable:
            {
                uint8_t idx = app_env.select_idx;
                uint16_t conhdl = app_get_conhdl_by_idx(idx);
                if (0xFFFF != conhdl
                    &&
                    false == app_get_client_service_status(idx, ATT_SVC_LINK_LOSS)
                    )
                {
                    eapi_prf_client_enable_req(EAPI_PRF_PROXIMITY, conhdl);
                }
                else
                    QPRINTF("Enable PROXM disallowed.\r\n");
            }
            break;
#endif
#if BLE_TIP_CLIENT
            case menu_tipc_enable:
            {
                uint8_t idx = app_env.select_idx;
                uint16_t conhdl = app_get_conhdl_by_idx(idx);
                if (0xFFFF != conhdl
                    &&
                    false == app_get_client_service_status(idx, ATT_SVC_CURRENT_TIME)
                    )
                {
                    eapi_prf_client_enable_req(EAPI_PRF_TIME, conhdl);
                }
                else
                    QPRINTF("Enable TIPC disallowed.\r\n");
            }
            break;
#endif
#if BLE_HID_BOOT_HOST
            case menu_hogpbh_enable:
            {
                uint8_t idx = app_env.select_idx;
                uint16_t conhdl = app_get_conhdl_by_idx(idx);
                if (0xFFFF != conhdl
                    &&
                    false == app_get_client_service_status(idx, ATT_SVC_HID)
                    )
                {
#if QN_SVC_CONTENT_USED	
                    if (app_check_service_content_available(conhdl, ATT_SVC_HID))
                        app_hogpbh_enable_req(app_hogpbh_env->hids_nb, &app_hogpbh_env->hids[0], conhdl);
                    else
#endif
                        app_hogpbh_enable_req(0, NULL, conhdl);
                }        
                else
                    QPRINTF("Enable HID Boot Host disallowed.\r\n");
            }
            break;
#endif
#if BLE_HID_REPORT_HOST
            case menu_hogprh_enable:
            {
                uint8_t idx = app_env.select_idx;
                uint16_t conhdl = app_get_conhdl_by_idx(idx);
                if (0xFFFF != conhdl
                    &&
                    false == app_get_client_service_status(idx, ATT_SVC_HID)
                    )
                {
#if QN_SVC_CONTENT_USED	
                    if (app_check_service_content_available(conhdl, ATT_SVC_HID))
                        app_hogprh_enable_req(app_hogprh_env->hids_nb, &app_hogprh_env->hids[0], conhdl);
                    else
#endif
                        app_hogprh_enable_req(0, NULL, conhdl);
                }        
                else
                    QPRINTF("Enable HID Report Host disallowed.\r\n");
            }
            break;
#endif
#if BLE_SP_CLIENT
            case menu_scppc_enable:
            {
                uint8_t idx = app_env.select_idx;
                uint16_t conhdl = app_get_conhdl_by_idx(idx);
                if (0xFFFF != conhdl
                    &&
                    false == app_get_client_service_status(idx, ATT_SVC_SCAN_PARAMETERS)
                    )
                {
                    eapi_prf_client_enable_req(EAPI_PRF_SCAN, conhdl);
                }
                else
                    QPRINTF("Enable SCPPC disallowed.\r\n");
            }
            break;
#endif
#if BLE_DIS_CLIENT
            case menu_disc_enable:
            {
                uint8_t idx = app_env.select_idx;
                uint16_t conhdl = app_get_conhdl_by_idx(idx);
                if (0xFFFF != conhdl
                    &&
                    false == app_get_client_service_status(idx, ATT_SVC_DEVICE_INFO)
                    )
                {
                    eapi_prf_client_enable_req(EAPI_PRF_DEVICEINFO, conhdl);
                }
                else
                    QPRINTF("Enable DISC disallowed.\r\n");
            }
            break;
#endif
#if BLE_BATT_CLIENT
            case menu_basc_enable:
            {
                uint8_t idx = app_env.select_idx;
                uint16_t conhdl = app_get_conhdl_by_idx(idx);
                if (0xFFFF != conhdl
                    &&
                    false == app_get_client_service_status(idx, ATT_SVC_BATTERY_SERVICE)
                    )
                {
                    eapi_prf_client_enable_req(EAPI_PRF_BATTERY, conhdl);
                }
                else
                    QPRINTF("Enable BASC disallowed.\r\n");
            }
            break;
#endif
#if BLE_AN_CLIENT
            case menu_anpc_enable:
            {
                uint8_t idx = app_env.select_idx;
                uint16_t conhdl = app_get_conhdl_by_idx(idx);
                if (0xFFFF != conhdl
                    &&
                    false == app_get_client_service_status(idx, ATT_SVC_ALERT_NTF)
                    )
                {
                    eapi_prf_client_enable_req(EAPI_PRF_ALERT, conhdl);
                }
                else
                    QPRINTF("Enable ANPC disallowed.\r\n");
            }
            break;
#endif
#if BLE_CSC_COLLECTOR
            case menu_cscpc_enable:
            {
                uint8_t idx = app_env.select_idx;
                uint16_t conhdl = app_get_conhdl_by_idx(idx);
                if (0xFFFF != conhdl
                    &&
                    false == app_get_client_service_status(idx, ATT_SVC_CYCLING_SPEED_CADENCE)
                    )
                {
                    eapi_prf_client_enable_req(EAPI_PRF_CSC, conhdl);
                }
                else
                    QPRINTF("Enable CSCPC disallowed.\r\n");
            }
            break;
#endif
#if BLE_PAS_CLIENT
            case menu_paspc_enable:
            {
                uint8_t idx = app_env.select_idx;
                uint16_t conhdl = app_get_conhdl_by_idx(idx);
                if (0xFFFF != conhdl
                    &&
                    false == app_get_client_service_status(idx, ATT_SVC_PHONE_ALERT_STATUS)
                    )
                {
                    eapi_prf_client_enable_req(EAPI_PRF_PAS, conhdl);
                }
                else
                    QPRINTF("Enable PASPC disallowed.\r\n");
            }
            break;
#endif
#if BLE_RSC_COLLECTOR
            case menu_rscpc_enable:
            {
                uint8_t idx = app_env.select_idx;
                uint16_t conhdl = app_get_conhdl_by_idx(idx);
                if (0xFFFF != conhdl
                    &&
                    false == app_get_client_service_status(idx, ATT_SVC_RUNNING_SPEED_CADENCE)
                    )
                {
                    eapi_prf_client_enable_req(EAPI_PRF_RSC, conhdl);
                }
                else
                    QPRINTF("Enable RSCPC disallowed.\r\n");
            }
            break;
#endif
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
        else if (app_env.menu_id == menu_hogpbh_enable)
            app_env.menu_id = menu_hogph;
        else if (app_env.menu_id == menu_hogprh_enable)
            app_env.menu_id = menu_hogph;
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
        else if (app_env.menu_id == menu_gatt_disc_all_svc)
            app_env.menu_id = menu_gatt;
        else
            app_env.menu_id = menu_gap;
        break;
    case 's':
        app_menu_show();
        break;
    }
}

#if QN_SVC_DISC_USED
static void app_menu_show_gatt(void)
{
    app_menu_show_line();
    QPRINTF("* QN BLE GATT Menu\r\n");
    QPRINTF("* 1. Discovery All Services\r\n");
    QPRINTF("* 2. Display All Services\r\n");
    app_menu_show_line();
}

static void app_menu_show_gatt_disp_all_svc(void)
{
    if (app_get_link_nb())
    {
        QPRINTF("\r\n");
        QPRINTF("Service record:\r\n");
        for (uint8_t idx = 0; idx < BLE_CONNECTION_MAX; idx++)
        {
            struct bd_addr peer_addr;
            if (app_get_bd_addr_by_idx(idx, &peer_addr))
            {
                QPRINTF("%d. %02X%02X:%02X:%02X%02X%02X\r\n", idx, 
                    peer_addr.addr[5],
                    peer_addr.addr[4],
                    peer_addr.addr[3],
                    peer_addr.addr[2],
                    peer_addr.addr[1],
                    peer_addr.addr[0]);
                for (uint8_t i = 0; i < app_env.dev_rec[idx].nb_svc; i++)
                {
                     QPRINTF("%04X ",app_env.dev_rec[idx].svc_rec[i].attr_hdl);
                }
                QPRINTF("\r\n");
            }
        }
    }
    else
    {
        QPRINTF("Service record is NULL.\r\n");
    }
    app_env.menu_id = menu_gatt;
}

static void app_menu_handler_gatt(void)
{
    switch (app_env.input[0])
    {
    case '1':
        app_env.menu_id = menu_gatt_disc_all_svc;
        break;
    case '2':
        app_env.menu_id = menu_gatt_disp_all_svc;
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

#if BLE_USE_CRYPT
static void app_menu_show_smp(void)
{
    app_menu_show_line();
    QPRINTF("* QN BLE SMP Menu\r\n");
    QPRINTF("* 1. Security Level setting\r\n");
    QPRINTF("* Default: ");
    switch (app_env.sec_lvl_set)
    {
    case GAP_SEC1_AUTH_PAIR_ENC:
        QPRINTF("M1L3\r\n");
        break;
    case GAP_SEC1_NOAUTH_PAIR_ENC:
        QPRINTF("M1L2\r\n");
        break;
    case GAP_NO_SEC:
    default:
        QPRINTF("M1L1\r\n");
        break;
    }
    QPRINTF("* Note: Change before Adv/Scan/Connect\r\n");
    QPRINTF("* 2. IO Capbility setting\r\n");
    QPRINTF("* Default: ");
    switch (app_env.iocap_set)
    {
    case SMP_IO_CAP_KB_DISPLAY:
        QPRINTF("Keyboard Display\r\n");
        break;
    case SMP_IO_CAP_DISPLAY_ONLY:
        QPRINTF("Display Only\r\n");
        break;
    case SMP_IO_CAP_NO_INPUT_NO_OUTPUT:
    default:
        QPRINTF("No Input No Output\r\n");
        break;
    }
    app_menu_show_line();
}

static void app_menu_handler_smp(void)
{
    switch (app_env.input[0])
    {
    case '1':
        app_env.menu_id = menu_smp_sec_lvl_set;
        break;
    case '2':
        app_env.menu_id = menu_smp_io_cap_set;
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

static void app_menu_show_smp_sec_lvl_set(void)
{
    app_menu_show_line();
    QPRINTF("* QN BLE Security Level\r\n");
    QPRINTF("* 0. M1L1\r\n");
    QPRINTF("* 1. M1L2\r\n");
    QPRINTF("* 2. M1L3\r\n");
    QPRINTF("* 3. M2L1\r\n");
    QPRINTF("* 4. M2L2\r\n");
    app_menu_show_line();
}

static void app_menu_handler_smp_sec_lvl_set(void)
{
    switch (app_env.input[0])
    {
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
        app_env.sec_lvl_set = app_env.input[0] - '0';
        app_gap_set_sec_req(app_env.sec_lvl_set);
        break;
    case 'r':
        app_env.menu_id = menu_smp;
        break;
    case 's':
    default:
        break;
    }
    app_menu_show();
}

static void app_menu_show_smp_iocap_set(void)
{
    app_menu_show_line();
    QPRINTF("* QN BLE SMP IO Capability\r\n");
    QPRINTF("* 0. Display Only\r\n");
    QPRINTF("* 1. Display YesNo\r\n");
    QPRINTF("* 2. Keyboard Only\r\n");
    QPRINTF("* 3. No Input No Output\r\n");
    QPRINTF("* 4. Keyboard Display\r\n");
    app_menu_show_line();
}

static void app_menu_handler_smp_iocap_set(void)
{
    switch (app_env.input[0])
    {
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
        app_env.iocap_set = app_env.input[0] - '0';
        break;
    case 'r':
        app_env.menu_id = menu_smp;
        break;
    case 's':
    default:
        break;
    }
    app_menu_show();
}

static void app_menu_show_smp_tk_input(void)
{
    QPRINTF("Please input the passkey:\r\n");
}

static void app_menu_handler_smp_tk_input(void)
{
    struct smp_key tk = {0};
    uint32_t i = atoi((char *)app_env.input);
    if (i <= 999999)
    {
        app_i_to_array(i, (char *)tk.key);
        app_smpc_tk_req_rsp(app_env.tk_idx, CO_ERROR_NO_ERROR, &tk);
    }
    else
    {
        app_smpc_tk_req_rsp(app_env.tk_idx, !CO_ERROR_NO_ERROR, &tk);
    }
    app_env.menu_id = menu_gap;
}
#endif

#if BLE_RSC_COLLECTOR
static void app_menu_show_rscpc(void)
{
    app_menu_show_line();
    QPRINTF("* QN BLE RSCPC Menu\r\n");
    QPRINTF("* 1. Enable\r\n");
    QPRINTF("* 2. Read RSC Feature\r\n");
    QPRINTF("* 3. Read Sensor Location\r\n");
    QPRINTF("* 4. Confiure RSC Measurement\r\n");
    QPRINTF("* 5. Confiure SC Control Point\r\n");
    QPRINTF("* 6. Update Sensor Location\r\n");
    app_menu_show_line();
}

static void app_menu_handler_rscpc(void)
{
    switch (app_env.input[0])
    {
    case '1':
        app_env.menu_id = menu_rscpc_enable;
        break;
    case '2':
        if (true == app_get_client_service_status(0, ATT_SVC_RUNNING_SPEED_CADENCE))
        {
            eapi_prf_client_rd_char_req(EAPI_PRF_RSC, app_rscpc_env->conhdl, RSCPC_RD_RSC_FEAT);
        }
        break;
    case '3':
        if (true == app_get_client_service_status(0, ATT_SVC_RUNNING_SPEED_CADENCE))
        {
            eapi_prf_client_rd_char_req(EAPI_PRF_RSC, app_rscpc_env->conhdl, RSCPC_RD_SENSOR_LOC);
        }
        break;
    case '4':
        if (true == app_get_client_service_status(0, ATT_SVC_RUNNING_SPEED_CADENCE))
        {
            eapi_prf_client_write_char_req(EAPI_PRF_RSC, app_rscpc_env->conhdl,
                                            RSCPC_RD_WR_RSC_MEAS_CFG, PRF_CLI_START_NTF);
        }
        break;
    case '5':
        if (true == app_get_client_service_status(0, ATT_SVC_RUNNING_SPEED_CADENCE))
        {
            eapi_prf_client_write_char_req(EAPI_PRF_RSC, app_rscpc_env->conhdl,
                                            RSCPC_RD_WR_SC_CTNL_PT_CFG, PRF_CLI_START_IND);
        }
        break;
    case '6':
        if (true == app_get_client_service_status(0, ATT_SVC_RUNNING_SPEED_CADENCE))
        {
            eapi_prf_client_write_char_req(EAPI_PRF_RSC, app_rscpc_env->conhdl,
                                            RSCP_CTNL_PT_OP_UPD_LOC, RSCP_LOC_OTHER);
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

#if BLE_PAS_CLIENT
static void app_menu_show_paspc(void)
{
    app_menu_show_line();
    QPRINTF("* QN BLE PASPC Menu\r\n");
    QPRINTF("* 1. Enable\r\n");
    QPRINTF("* 2. Read PAS Alert Status\r\n");
    QPRINTF("* 3. Read PAS Ringer Setting\r\n");
    QPRINTF("* 4. Write PAS Alert Status Client Characteristic CFG. Start Notify\r\n");
    QPRINTF("* 5. Write PAS Ringer Setting Client Characteristic CFG. Start Notify\r\n");
    QPRINTF("* 6. Write PAS Ringer Control Point\r\n");
    app_menu_show_line();
}

static void app_menu_handler_paspc(void)
{
    switch (app_env.input[0])
    {
    case '1':
        app_env.menu_id = menu_paspc_enable;
        break;
    case '2':
        if (true == app_get_client_service_status(0, ATT_SVC_PHONE_ALERT_STATUS))
        {
            eapi_prf_client_rd_char_req(EAPI_PRF_PAS, app_paspc_env->conhdl, PASPC_RD_ALERT_STATUS);
        }
        break;
    case '3':
        if (true == app_get_client_service_status(0, ATT_SVC_PHONE_ALERT_STATUS))
        {
            eapi_prf_client_rd_char_req(EAPI_PRF_PAS, app_paspc_env->conhdl, PASPC_RD_RINGER_SETTING);
        }
        break;
    case '4':
        if (true == app_get_client_service_status(0, ATT_SVC_PHONE_ALERT_STATUS))
        {
            eapi_prf_client_write_char_req(EAPI_PRF_PAS, app_paspc_env->conhdl,
                                            PASPC_RD_WR_ALERT_STATUS_CFG, PRF_CLI_START_NTF);
        }
        break;
    case '5':
        if (true == app_get_client_service_status(0, ATT_SVC_PHONE_ALERT_STATUS))
        {
            eapi_prf_client_write_char_req(EAPI_PRF_PAS, app_paspc_env->conhdl,
                                            PASPC_RD_WR_RINGER_SETTING_CFG, PRF_CLI_START_NTF);
        }
        break;
    case '6':
        if (true == app_get_client_service_status(0, ATT_SVC_PHONE_ALERT_STATUS))
        {
            eapi_prf_client_write_char_req(EAPI_PRF_PAS, app_paspc_env->conhdl,
                                            PASPC_WR_RINGER_CTNL_PT, PASP_SILENT_MODE);
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

#if BLE_CSC_COLLECTOR
static void app_menu_show_cscpc(void)
{
    app_menu_show_line();
    QPRINTF("* QN BLE CSCPC Menu\r\n");
    QPRINTF("* 1. Enable\r\n");
    QPRINTF("* 2. Read CSC Feature\r\n");
    QPRINTF("* 3. Read Sensor Location\r\n");
    QPRINTF("* 4. Confiure CSC Measurement\r\n");
    QPRINTF("* 5. Confiure SC Control Point\r\n");
    app_menu_show_line();
}

static void app_menu_handler_cscpc(void)
{
    switch (app_env.input[0])
    {
    case '1':
        app_env.menu_id = menu_cscpc_enable;
        break;
    case '2':
        if (true == app_get_client_service_status(0, ATT_SVC_CYCLING_SPEED_CADENCE))
        {
            eapi_prf_client_rd_char_req(EAPI_PRF_CSC, app_cscpc_env->conhdl, CSCPC_RD_CSC_FEAT);
        }
        break;
    case '3':
        if (true == app_get_client_service_status(0, ATT_SVC_CYCLING_SPEED_CADENCE))
        {
            eapi_prf_client_rd_char_req(EAPI_PRF_CSC, app_cscpc_env->conhdl, CSCPC_RD_SENSOR_LOC);
        }
        break;
    case '4':
        if (true == app_get_client_service_status(0, ATT_SVC_CYCLING_SPEED_CADENCE))
        {
            eapi_prf_client_write_char_req(EAPI_PRF_CSC, app_cscpc_env->conhdl,
                                            CSCPC_RD_WR_CSC_MEAS_CFG, PRF_CLI_START_NTF);
        }
        break;
    case '5':
        if (true == app_get_client_service_status(0, ATT_SVC_CYCLING_SPEED_CADENCE))
        {
            eapi_prf_client_write_char_req(EAPI_PRF_CSC, app_cscpc_env->conhdl,
                                            CSCPC_RD_WR_SC_CTNL_PT_CFG, PRF_CLI_START_IND);
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

#if BLE_AN_CLIENT
static void app_menu_show_anpc(void)
{
    app_menu_show_line();
    QPRINTF("* QN BLE ANPC Menu\r\n");
    QPRINTF("* 1. Enable\r\n");
    QPRINTF("* 2. Read New Alert CFG\r\n");
    QPRINTF("* 3. Read Unread Alert Status CFG\r\n");
    QPRINTF("* 4. Write ANS New Alert Client CFG Start Notify\r\n");
    app_menu_show_line();
}

static void app_menu_handler_anpc(void)
{
    switch (app_env.input[0])
    {
    case '1':
        app_env.menu_id = menu_anpc_enable;
        break;
    case '2':
        if (true == app_get_client_service_status(0, ATT_SVC_ALERT_NTF))
        {
            eapi_prf_client_rd_char_req(EAPI_PRF_ALERT, app_anpc_env->conhdl, ANPC_RD_WR_NEW_ALERT_CFG);
        }
        break;
    case '3':
        if (true == app_get_client_service_status(0, ATT_SVC_ALERT_NTF))
        {
            eapi_prf_client_rd_char_req(EAPI_PRF_ALERT, app_anpc_env->conhdl, ANPC_RD_WR_UNREAD_ALERT_STATUS_CFG);
        }
        break;
    case '4':
        if (true == app_get_client_service_status(0, ATT_SVC_ALERT_NTF))
        {
            eapi_prf_client_write_char_req(EAPI_PRF_ALERT, app_anpc_env->conhdl,
                                            ANPC_RD_WR_NEW_ALERT_CFG, PRF_CLI_START_NTF);
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

#if BLE_HT_COLLECTOR
static void app_menu_show_htpc(void)
{
    app_menu_show_line();
    QPRINTF("* QN BLE HTPC Menu\r\n");
    QPRINTF("* 1. Enable\r\n");
    app_menu_show_line();
}

static void app_menu_handler_htpc(void)
{
    switch (app_env.input[0])
    {
    case '1':
        app_env.menu_id = menu_htpc_enable;
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

#if BLE_BP_COLLECTOR
static void app_menu_show_blpc(void)
{
    app_menu_show_line();
    QPRINTF("* QN BLE BLPC Menu\r\n");
    QPRINTF("* 1. Enable\r\n");
    app_menu_show_line();
}

static void app_menu_handler_blpc(void)
{
    switch (app_env.input[0])
    {
    case '1':
        app_env.menu_id = menu_blpc_enable;
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

#if BLE_HR_COLLECTOR
static void app_menu_show_hrpc(void)
{
    app_menu_show_line();
    QPRINTF("* QN BLE HRPC Menu\r\n");
    QPRINTF("* 1. Enable\r\n");
    app_menu_show_line();
}

static void app_menu_handler_hrpc(void)
{
    switch (app_env.input[0])
    {
    case '1':
        app_env.menu_id = menu_hrpc_enable;
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

#if BLE_GL_COLLECTOR
static void app_menu_show_glpc(void)
{
    app_menu_show_line();
    QPRINTF("* QN BLE GLPC Menu\r\n");
    QPRINTF("* 1. Enable\r\n");
    app_menu_show_line();
}

static void app_menu_handler_glpc(void)
{
    switch (app_env.input[0])
    {
    case '1':
        app_env.menu_id = menu_glpc_enable;
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

#if BLE_FINDME_LOCATOR
static void app_menu_show_findl(void)
{
    app_menu_show_line();
    QPRINTF("* QN BLE FMPL Menu\r\n");
    QPRINTF("* 1. Enable\r\n");
    QPRINTF("* 3. Alert None\r\n");
    QPRINTF("* 4. Alert Mild\r\n");
    QPRINTF("* 5. Alert High\r\n");
    app_menu_show_line();
}

static void app_menu_handler_findl(void)
{
    switch (app_env.input[0])
    {
    case '1':
        app_env.menu_id = menu_findl_enable;
        break;
    case '3':
    case '4':
    case '5':
        if (true == app_get_client_service_status(0, ATT_SVC_IMMEDIATE_ALERT))
            eapi_prf_client_write_char_req(EAPI_PRF_FINDME, app_findl_env->conhdl, 
                                            app_env.input[0]-'3', 0);
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

#if BLE_PROX_MONITOR
static void app_menu_show_proxm(void)
{
    app_menu_show_line();
    QPRINTF("* QN BLE PROXM Menu\r\n");
    QPRINTF("* 1. Enable\r\n");
    QPRINTF("* 2. Read TX Power Level\r\n");
    QPRINTF("* 3. Alert None\r\n");
    QPRINTF("* 4. Alert Mild\r\n");
    QPRINTF("* 5. Alert High\r\n");
    app_menu_show_line();
}

static void app_menu_handler_proxm(void)
{
    switch (app_env.input[0])
    {
    case '1':
        app_env.menu_id = menu_proxm_enable;
        break;
    case '2':
        if (true == app_get_client_service_status(0, ATT_SVC_TX_POWER))
        {
            eapi_prf_client_rd_char_req(EAPI_PRF_PROXIMITY, app_proxm_env->conhdl, 1);
        }
        break;
    case '3':
    case '4':
    case '5':
        if (true == app_get_client_service_status(0, ATT_SVC_TX_POWER))
#if QN_SVC_CONTENT_USED
            eapi_prf_client_write_char_req(EAPI_PRF_PROXIMITY,
                                            app_proxm_env->conhdl,
                                            (app_proxm_env->txps.svc.shdl == ATT_INVALID_HANDLE) ? 
                                            PROXM_SET_LK_LOSS_ALERT : PROXM_SET_IMMDT_ALERT,
                                            app_env.input[0]-'3');
#else
            eapi_prf_client_write_char_req(EAPI_PRF_PROXIMITY,
                                            app_proxm_env->conhdl,
                                            PROXM_SET_LK_LOSS_ALERT,
                                            app_env.input[0]-'3');
#endif
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

#if BLE_TIP_CLIENT
static void app_menu_show_tipc(void)
{
    app_menu_show_line();
    QPRINTF("* QN BLE TIPC Menu\r\n");
    QPRINTF("* 1. Enable\r\n");
    app_menu_show_line();
}

static void app_menu_handler_tipc(void)
{
    switch (app_env.input[0])
    {
    case '1':
        app_env.menu_id = menu_tipc_enable;
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

#if (BLE_HID_BOOT_HOST || BLE_HID_REPORT_HOST)
static void app_menu_show_hogph(void)
{
    app_menu_show_line();
    QPRINTF("* QN BLE HOGH Menu\r\n");
#if BLE_HID_BOOT_HOST
    QPRINTF("* 1. Enable Boot Mode\r\n");
#endif    
#if BLE_HID_REPORT_HOST
    QPRINTF("* 3. Enable Report Mode\r\n");
#endif    
    app_menu_show_line();
}

static void app_menu_handler_hogph(void)
{
    switch (app_env.input[0])
    {
    case '1':
        app_env.menu_id = menu_hogpbh_enable;
        break;
    case '3':
        app_env.menu_id = menu_hogprh_enable;
        break;
    case '4':
#if BLE_HID_REPORT_HOST
        if (true == app_get_client_service_status(0, ATT_SVC_HID))
        {               
            app_hogprh_cfg_ntf_req(0, PRF_CLI_STOP_NTFIND, 0, app_hogprh_env->conhdl);
            app_hogprh_disable_req(app_hogprh_env->conhdl);
            app_set_client_service_status(app_hogprh_env->conhdl, ATT_SVC_HID, false);
        }
#endif
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

#if BLE_SP_CLIENT
static void app_menu_show_scppc(void)
{
    app_menu_show_line();
    QPRINTF("* QN BLE SCPPC Menu\r\n");
    QPRINTF("* 1. Enable\r\n");
    app_menu_show_line();
}

static void app_menu_handler_scppc(void)
{
    switch (app_env.input[0])
    {
    case '1':
        app_env.menu_id = menu_scppc_enable;
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

#if BLE_DIS_CLIENT
static void app_menu_show_disc(void)
{
    app_menu_show_line();
    QPRINTF("* QN BLE DISC Menu\r\n");
    QPRINTF("* 1. Enable\r\n");
    app_menu_show_line();
}

static void app_menu_handler_disc(void)
{
    switch (app_env.input[0])
    {
    case '1':
        app_env.menu_id = menu_disc_enable;
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

#if BLE_BATT_CLIENT
static void app_menu_show_basc(void)
{
    app_menu_show_line();
    QPRINTF("* QN BLE BASC Menu\r\n");
    QPRINTF("* 1. Enable\r\n");
    app_menu_show_line();
}

static void app_menu_handler_basc(void)
{
    switch (app_env.input[0])
    {
    case '1':
        app_env.menu_id = menu_basc_enable;
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
void app_menu_show(void)
{
    switch (app_env.menu_id)
    {
    case menu_start:
        app_menu_show_start();
        break;
    case menu_main:
        app_menu_show_main();
        break;
    case menu_gap:
        app_menu_show_gap();
        break;
#if QN_SVC_DISC_USED
    case menu_gatt:
        app_menu_show_gatt();
        break;
    case menu_gatt_disp_all_svc:
        app_menu_show_gatt_disp_all_svc();
        break;
#endif
    case menu_gap_create_connection:
        app_menu_show_gap_inq_record();
        break;
    case menu_gap_unpair:
        app_menu_show_gap_bond_record();
        break;
    case menu_gap_bond:
    case menu_gap_disconnection:
    case menu_gatt_disc_all_svc:
    case menu_htpc_enable:
    case menu_blpc_enable:
    case menu_hrpc_enable:
    case menu_glpc_enable:
    case menu_findl_enable:
    case menu_tipc_enable:
    case menu_proxm_enable:
    case menu_hogpbh_enable:
    case menu_hogprh_enable:
    case menu_scppc_enable:
    case menu_disc_enable:
    case menu_basc_enable:
    case menu_anpc_enable:
    case menu_rscpc_enable:
    case menu_paspc_enable:
    case menu_cscpc_enable:
        app_menu_show_gap_dev_record();
        break;
#if BLE_USE_CRYPT
    case menu_smp:
        app_menu_show_smp();
        break;
    case menu_smp_sec_lvl_set:
        app_menu_show_smp_sec_lvl_set();
        break;
    case menu_smp_io_cap_set:
        app_menu_show_smp_iocap_set();
        break;
    case menu_smp_tk_input:
        app_menu_show_smp_tk_input();
        break;
#endif

#if BLE_RSC_COLLECTOR
    case menu_rscpc:
        app_menu_show_rscpc();
        break;
#endif
#if BLE_PAS_CLIENT
    case menu_paspc:
        app_menu_show_paspc();
        break;
#endif
#if BLE_CSC_COLLECTOR
    case menu_cscpc:
        app_menu_show_cscpc();
        break;
#endif
#if BLE_AN_CLIENT
    case menu_anpc:
        app_menu_show_anpc();
        break;
#endif
#if BLE_HT_COLLECTOR
    case menu_htpc:
        app_menu_show_htpc();
        break;
#endif
#if BLE_BP_COLLECTOR
    case menu_blpc:
        app_menu_show_blpc();
        break;
#endif
#if BLE_HR_COLLECTOR
    case menu_hrpc:
        app_menu_show_hrpc();
        break;
#endif
#if BLE_GL_COLLECTOR
    case menu_glpc:
        app_menu_show_glpc();
        break;
#endif
#if BLE_FINDME_LOCATOR
    case menu_findl:
        app_menu_show_findl();
        break;
#endif
#if BLE_PROX_MONITOR
    case menu_proxm:
        app_menu_show_proxm();
        break;
#endif
#if BLE_TIP_CLIENT
    case menu_tipc:
        app_menu_show_tipc();
        break;
#endif
#if (BLE_HID_BOOT_HOST || BLE_HID_REPORT_HOST)
    case menu_hogph:
        app_menu_show_hogph();
        break;
#endif
#if BLE_SP_CLIENT
    case menu_scppc:
        app_menu_show_scppc();
        break;
#endif
#if BLE_DIS_CLIENT
    case menu_disc:
        app_menu_show_disc();
        break;
#endif
#if BLE_BATT_CLIENT
    case menu_basc:
        app_menu_show_basc();
        break;
#endif

    case menu_htpt:
    case menu_blps:
    case menu_hrpt:
    case menu_glps:
    case menu_findt:
    case menu_proxr:
    case menu_tips:
    case menu_hogpd:
    case menu_scpps:
    case menu_diss:
    case menu_bass:
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
#if QN_SVC_DISC_USED
    case menu_gatt:
        app_menu_handler_gatt();
        break;
#endif
#if BLE_USE_CRYPT
    case menu_smp:
        app_menu_handler_smp();
        break;
#endif

#if BLE_RSC_COLLECTOR
    case menu_rscpc:
        app_menu_handler_rscpc();
        break;
#endif
#if BLE_PAS_CLIENT
    case menu_paspc:
        app_menu_handler_paspc();
        break;
#endif
#if BLE_CSC_COLLECTOR
    case menu_cscpc:
        app_menu_handler_cscpc();
        break;
#endif
#if BLE_AN_CLIENT
    case menu_anpc:
        app_menu_handler_anpc();
        break;
#endif
#if BLE_HT_COLLECTOR
    case menu_htpc:
        app_menu_handler_htpc();
        break;
#endif
#if BLE_BP_COLLECTOR
    case menu_blpc:
        app_menu_handler_blpc();
        break;
#endif
#if BLE_HR_COLLECTOR
    case menu_hrpc:
        app_menu_handler_hrpc();
        break;
#endif
#if BLE_GL_COLLECTOR
    case menu_glpc:
        app_menu_handler_glpc();
        break;
#endif
#if BLE_FINDME_LOCATOR
    case menu_findl:
        app_menu_handler_findl();
        break;
#endif
#if BLE_PROX_MONITOR
    case menu_proxm:
        app_menu_handler_proxm();
        break;
#endif
#if BLE_TIP_CLIENT
    case menu_tipc:
        app_menu_handler_tipc();
        break;
#endif
#if (BLE_HID_BOOT_HOST || BLE_HID_REPORT_HOST)
    case menu_hogph:
        app_menu_handler_hogph();
        break;
#endif
#if BLE_SP_CLIENT
    case menu_scppc:
        app_menu_handler_scppc();
        break;
#endif
#if BLE_DIS_CLIENT
    case menu_disc:
        app_menu_handler_disc();
        break;
#endif
#if BLE_BATT_CLIENT
    case menu_basc:
        app_menu_handler_basc();
        break;
#endif
    case menu_gap_bond:
    case menu_gap_unpair:
    case menu_gap_create_connection:
    case menu_gap_disconnection:
#if QN_SVC_DISC_USED
    case menu_gatt_disc_all_svc:
#endif
    case menu_htpc_enable:
    case menu_blpc_enable:
    case menu_hrpc_enable:
    case menu_glpc_enable:
    case menu_findl_enable:
    case menu_proxm_enable:
    case menu_tipc_enable:
    case menu_hogpbh_enable:
    case menu_hogprh_enable:
    case menu_scppc_enable:
    case menu_disc_enable:
    case menu_basc_enable:
    case menu_anpc_enable:
    case menu_cscpc_enable:
    case menu_paspc_enable:
    case menu_rscpc_enable:
        app_menu_handler_gap_dev_select();
        break;
#if BLE_USE_CRYPT
    case menu_smp_sec_lvl_set:
        app_menu_handler_smp_sec_lvl_set();
        break;
    case menu_smp_io_cap_set:
        app_menu_handler_smp_iocap_set();
        break;
     case menu_smp_tk_input:
        app_menu_handler_smp_tk_input();
        break;
#endif

    case menu_htpt:
    case menu_blps:
    case menu_hrpt:
    case menu_glps:
    case menu_findt:
    case menu_proxr:
    case menu_tips:
    case menu_hogpd:
    case menu_scpps:
    case menu_diss:
    case menu_bass:
    default:
        break;
    }
}

/**
 ****************************************************************************************
 * @brief Callback function of MENU.
 *
 ****************************************************************************************
 */
void app_menu_cb(ke_msg_id_t const msgid, void *param)
{
    ke_state_t state = (ke_state_get(TASK_APP));

    if (APP_INIT == state)
    {
        switch (msgid)
        {
        case GAP_READY_EVT:
        case GAP_RESET_REQ_CMP_EVT:
            app_env.menu_id = menu_start;
            app_menu_show();
            break;
        case GAP_READ_VER_REQ_CMP_EVT:
            QPRINTF("*\r\n");
            QPRINTF("* Version:\r\n");
            QPRINTF("*   AP%sHL%1x.%1x.%1xLL%1x.%1x.%1x\r\n",
                QN_APP_VER,
                ((struct gap_read_ver_req_cmp_evt *)param)->host_ver,
                ((struct gap_read_ver_req_cmp_evt *)param)->host_subver>>8&0xFF,
                ((struct gap_read_ver_req_cmp_evt *)param)->host_subver&0xFF,
                ((struct gap_read_ver_req_cmp_evt *)param)->lmp_ver,
                ((struct gap_read_ver_req_cmp_evt *)param)->lmp_subver>>8&0xFF,
                ((struct gap_read_ver_req_cmp_evt *)param)->lmp_subver&0xFF);
            break;
        case GAP_READ_BDADDR_REQ_CMP_EVT:
            QPRINTF("* BD Address:\r\n");
            QPRINTF("*          %02X%02X:%02X:%02X%02X%02X\r\n",
                ((struct gap_read_bdaddr_req_cmp_evt *)param)->addr.addr[5],
                ((struct gap_read_bdaddr_req_cmp_evt *)param)->addr.addr[4],
                ((struct gap_read_bdaddr_req_cmp_evt *)param)->addr.addr[3],
                ((struct gap_read_bdaddr_req_cmp_evt *)param)->addr.addr[2],
                ((struct gap_read_bdaddr_req_cmp_evt *)param)->addr.addr[1],
                ((struct gap_read_bdaddr_req_cmp_evt *)param)->addr.addr[0]);
            QPRINTF("*\r\n");
            QPRINTF("* @ %11s, %8s\r\n",
                __DATE__, 
                __TIME__);
            QPRINTF("*------------------------\r\n");
            app_env.menu_id = menu_main;
            app_menu_show();
            break;
        default:
            break;
        }
    }
    else
    if (APP_IDLE == state)
    {
        switch (msgid)
        {
        case SMPC_TK_REQ_IND:
            if (app_env.tk_type != 0)
            {
                app_env.tk_idx = ((struct smpc_tk_req_ind *)param)->idx;
                app_env.menu_id = menu_smp_tk_input;
                app_menu_show();
            }
            break;
        default:
            break;
        }
    }
}

#endif
/// @} APP
