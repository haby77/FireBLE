/**
 ****************************************************************************************
 *
 * @file mpu_6050.h
 *
 * @brief Header file of mpu_6050 for QN9020.
 *
 * Copyright (C) TChip 2014
 *
 * $Rev: 1.0 $
 *
 ****************************************************************************************
 */
#ifndef _MPU6050_H_
#define _MPU6050_H_

#include "stdint.h"

#include "math.h"
/**
 ****************************************************************************************
 * @defgroup I2C I2C Driver
 * @ingroup DRIVERS
 * @brief I2C driver
 *
 *  I2C is a bi-directional serial bus with two wires that provides a simple and efficient
 *  method of data exchange between devices. The I2C standard is a true multi-master bus
 *  including collision detection and arbitration that prevents data corruption if two or
 *  more masters attempt to control the bus simultaneously.
 *
 *  For QN9020, I2C device could act as master or slave and I2C driver can help user to
 *  use I2C functions easily. The main features of I2C are listed as follow:
 *    - Both I2C master and slave control are supported
 *    - Master baud rate is configurable.
 *    - Supports up to 400Kbps baud rate.
 *    - Master & slave support both 8 bit and 10 bit address mode.
 *    - Master supports SCL synchronization, and bus arbitration.
 *    - Slave supports SCL stretching.
 *    - 8 bit shift register for transform.
 *
 * @{
 *
 ****************************************************************************************
 */


/* \example  i2c_example.c
 * This is an example of how to use the I2C driver.
 */


/*
 * DEFINES
 ****************************************************************************************
 */

/// Define QN9020 I2C slave address
#define MPU6050_ADDR                 0x69

//register map
#define     SELF_TEST_X             reg_self_test_x
#define     SELF_TEST_Y             reg_self_test_y
#define     SELF_TEST_Z             reg_self_test_z
#define     SELF_TEST_A             reg_self_test_a
#define     SMPLRT_DIV              reg_simplrt_div
#define     CONFIG                  reg_config
#define     GYRO_CONFIG             reg_gyro_config
#define     ACCEL_CONFIG            reg_accel_config
#define     FIFO_EN                 reg_fifo_en
#define     INT_PIN_CFG             reg_int_pin_cfg 
#define     INT_ENABLE              reg_int_enable
#define     INT_STATUS              reg_int_status
#define     ACCEL_XOUT_H            reg_accel_xout_h
#define     ACCEL_XOUT_L            reg_accel_xout_l
#define     ACCEL_YOUT_H            reg_accel_yout_h
#define     ACCEL_YOUT_L            reg_accel_yout_l
#define     ACCEL_ZOUT_H            reg_accel_zout_h
#define     ACCEL_ZOUT_L            reg_accel_zout_l
#define     TEMP_OUT_H              reg_temp_out_h
#define     TEMP_OUT_L              reg_temp_out_L
#define     GYRO_XOUT_H             reg_gyro_xout_h
#define     GYRO_XOUT_L             reg_gyro_xout_l
#define     GYRO_YOUT_H             reg_gyro_yout_h
#define     GYRO_YOUT_L             reg_gyro_yout_l
#define     GYRO_ZOUT_H             reg_gyro_zout_h
#define     GYRO_ZOUT_L             reg_gyro_zout_l
#define     SIGNAL_PATH_RESET       reg_signal_path_reset
#define     USER_CTRL               reg_user_ctrl
#define     PWR_MGMT1               reg_pwr_mgmt1
#define     PWR_MGMT2               reg_pwr_mgmt2
#define     FIFO_COUNT_H            reg_fifo_count_h
#define     FIFO_COUNT_L            reg_fifo_count_l
#define     FIFO_R_W                reg_fifo_r_w
#define     WHO_AM_I                reg_who_am_i

#define     DEVICE_RESET_EN         0x80
#define     DEVICE_RESET_DIS        0x00
#define     TEMP_DIS                0x04

#define     CLK_SEL_8MHZ            0x00
#define     CLK_SEL_AXIS_X          0x01
#define     CLK_SEL_AXIS_Y          0x02
#define     CLK_SEL_AXIS_Z          0x03
#define     CLK_SEL_32_768          0x04
#define     CLK_SEL_19_2_MHz        0x05
#define     CLK_STOP                0x07


/*
 * STRUCT DEFINITIONS
 *****************************************************************************************
 */
typedef struct self_test_data_tag
{
    int8_t data_xg_test;
    int8_t data_yg_test;
    int8_t data_zg_test;
    int8_t data_xa_test;
    int8_t data_ya_test;
    int8_t data_za_test;
} self_test_data ;
extern self_test_data ft_test_data;

typedef struct self_test_ft_tag
{
    double_t ft_xg;
    double_t ft_yg;
    double_t ft_zg;
    double_t ft_xa;
    double_t ft_ya;
    double_t ft_za;
} self_test_ft;
extern self_test_ft ft_data;

typedef struct gyro_data_out_tag
{
    int16_t xg_data_out;
    int16_t yg_data_out;
    int16_t zg_data_out;
}gyro_data_out;

typedef struct accel_data_out_tag
{
    int16_t xa_data_out;
    int16_t ya_data_out;
    int16_t za_data_out;
}accel_data_out;

typedef struct sensor_data_out_tag
{
    gyro_data_out gyro_do;
    accel_data_out accel_do;
}sensor_data_out;
extern  sensor_data_out data_out;
/*
 * ENUMERATION DEFINITIONS
 *****************************************************************************************
 */
 
enum    reg_mpu6050
{
        //register of self_test
        reg_self_test_x = 0x0D,
        reg_self_test_y,
        reg_self_test_z,
        reg_self_test_a,
        
        //register of component config
        reg_simplrt_div = 0x19,
        reg_config,
        reg_gyro_config,
        reg_accel_config,
        reg_fifo_en,
        
        //register of int config
        reg_int_pin_cfg = 0x37,
        reg_int_enable,
        reg_int_status = 0x3A,
        
        //register of sample data output
        reg_accel_xout_h,
        reg_accel_xout_l,
        reg_accel_yout_h,
        reg_accel_yout_l,
        reg_accel_zout_h,
        reg_accel_zout_l,
        reg_temp_out_h,
        reg_temp_out_l,
        reg_gyro_xout_h,
        reg_gyro_xout_l,
        reg_gyro_yout_h,
        reg_gyro_yout_l,
        reg_gyro_zout_h,
        reg_gyro_zout_l,
        
        //register of power manage
        reg_signal_path_reset = 0x68,
        reg_user_ctrl = 0x6A,
        reg_pwr_mgmt1,
        reg_pwr_mgmt2,
        
        //register of fifo ctrl
        reg_fifo_count_h = 0x72,
        reg_fifo_count_l,
        reg_fifo_r_w,
        
        //who am i
        reg_who_am_i = 0x75,
};

enum    axis
{
    XG = 0x01,
    YG = 0x02,
    ZG = 0x04,
    XA = 0x08,
    YA = 0x10,
    ZA = 0x20
};

/*
 * FUNCTION DEFINITIONS
 ****************************************************************************************
 */
  
/**
 ****************************************************************************************
 * @brief   Reset I2C module
 * @description
 *  This function is used to reset I2C module
 *
 *****************************************************************************************
 */



/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */
extern void mpu6050_init(void);
extern int16_t mpu6050_get_data(int8_t saddr, int8_t reg);
extern void mpu_self_get_data(uint8_t saddr,uint8_t axis,self_test_ft* st_data1);
extern void mpu_self_test_active(uint8_t axis);
extern void mpu_self_test_close(uint8_t axis);
extern uint8_t mpu_self_test(uint8_t axis);
extern uint8_t mpu_self_test_one_axis(enum axis axis);
extern void mpu6050_get_gyro_accel_data(sensor_data_out* data_out);
extern uint8_t self_test_calculate(sensor_data_out* data_out_dis,sensor_data_out* data_out_en);

#endif /* end _MPU6050_H_ */
