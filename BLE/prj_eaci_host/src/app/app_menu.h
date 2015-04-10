/**
 ****************************************************************************************
 *
 * @file app_menu.h
 *
 * @brief Application DEMO menu
 *
 * Copyright (C) Quintic Corp 2009-2012
 *
 * $Rev: $
 *
 ****************************************************************************************
 */

#ifndef _APP_MENU_H_
#define _APP_MENU_H_

/*
 * DEFINES
 ****************************************************************************************
 */
typedef enum
{
    menu_start,
    menu_main,
    menu_gap,
    menu_gap_create_connection,
    menu_gap_update_param,
    menu_gap_disconnection,
    menu_gap_bond,
    menu_htpc,
    menu_htpc_enable,
    menu_htpt,
    menu_blpc,
    menu_blpc_enable,
    menu_blps,
    menu_hrpc,
    menu_hrpc_enable,
    menu_hrps,
    menu_glpc,
    menu_glpc_enable,
    menu_glps,
    menu_findl,
    menu_findl_enable,
    menu_findt,
    menu_proxm,
    menu_proxm_enable,
    menu_proxr,
    menu_tipc,
    menu_tipc_enable,
    menu_tips,
    menu_scppc,
    menu_scppc_enable,
    menu_scpps,
    menu_disc,
    menu_disc_enable,
    menu_diss,
    menu_basc,
    menu_basc_enable,
    menu_bass,
    menu_anpc_enable,
    menu_anpc,
    menu_anpc_alert,
    menu_anps,
    menu_cscpc,
    menu_cscpc_enable,
    menu_cscpc_ctnl_pt,
    menu_cscps,
    menu_cscps_enable,
    menu_paspc,
    menu_paspc_enable,
    menu_paspc_ringer_pt,
    menu_pasps,
    menu_pasps_enable,
    menu_rscpc,
    menu_rscpc_enable,
    menu_rscpc_ctnl_pt,
    menu_rscps,
    menu_rscps_enable,
    menu_rscps_ctnl_pt_cfm,
    
    menu_htpt_flag,
    menu_max
} MENU_ID;
 
/*
 ****************************************************************************************
 * @brief Handler function of the BLE demo application.
 *
 ****************************************************************************************
 */
void app_menu_hdl(void);

#endif // _APP_MENU_H_

