/**
 ****************************************************************************************
 *
 * @file mpu6050.c
 *
 * @brief mpu6050 driver for QN9020.
 *
 * Copyright (C) TChip 2014
 *
 * $Rev: 1.0 $
 *
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @addtogroup  I2C
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "i2c.h"
#include "mpu6050.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "app_printf.h"

#define labs        abs
#define fabs(x)     (((x)>0)?(x):-(x))

/*
 * STRUCTURE DEFINITIONS
 ****************************************************************************************
 */

self_test_data ft_test_data = {0,0,0,0,0,0};
self_test_ft ft_data = {0.0,0.0,0.0,0.0,0.0,0.0};
sensor_data_out data_out = {0,0,0,0,0,0};

///Structure defining I2C environment parameters

/*
 * GLOBAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */
///I2C environment variable




/**
 ****************************************************************************************
 * @brief I2C interrupt handler, deal with master mode only.
 ****************************************************************************************
 */
void mpu6050_init(void)
{
        I2C_BYTE_WRITE(MPU6050_ADDR,PWR_MGMT1,DEVICE_RESET_EN);
        //delay 200ms,necessary
        delay(100000);
        I2C_BYTE_WRITE(MPU6050_ADDR,PWR_MGMT1,DEVICE_RESET_DIS);
        //delay 200ms,necessary
        delay(100000);
        //select the clk as gyro z-axis
        I2C_BYTE_WRITE(MPU6050_ADDR,PWR_MGMT1,CLK_SEL_AXIS_Z);
        //sampling rate of gyro :1khz
        I2C_BYTE_WRITE(MPU6050_ADDR,SMPLRT_DIV,(1000/50));
        //DLPF(accel:44hz,gyro:42hz)
        I2C_BYTE_WRITE(MPU6050_ADDR,CONFIG,0x06);
        //gyro range:max 2000¡ã/s
        I2C_BYTE_WRITE(MPU6050_ADDR,GYRO_CONFIG,0x00);
        //accel range:max 16g/s
        I2C_BYTE_WRITE(MPU6050_ADDR,ACCEL_CONFIG,0x10);
        //delay(200000);
        //Open int,set int to high_level,clear until data was readed
				        I2C_BYTE_WRITE(MPU6050_ADDR,0x1f,0x01);

	        I2C_BYTE_WRITE(MPU6050_ADDR,0x69,0x00);

        I2C_BYTE_WRITE(MPU6050_ADDR,0x20,0x01);

        I2C_BYTE_WRITE(MPU6050_ADDR,INT_PIN_CFG,0x30); //1011
        //delay(200000);
        //data ready produce a int
        I2C_BYTE_WRITE(MPU6050_ADDR,INT_ENABLE,0x01);
        //delay(200000);
}

/**
 ****************************************************************************************
 * @brief  i2c polling, deal with master mode only
 * @description
 *  This function is used repeatedly in i2c_read() or i2c_write() in order to
 *  complete a transfer.
 *****************************************************************************************
 */
 
int16_t mpu6050_get_data(int8_t saddr, int8_t reg)
{
        int16_t data,data_h,data_l;
        data_h = I2C_BYTE_READ(saddr,reg);
        data_l = I2C_BYTE_READ(saddr,reg+1);
        data  = (data_h << 8) + data_l;
        return data;
}



/**
 ****************************************************************************************
 * @brief Initialize the I2C controller
 * @param[in]    speed      SCL 1K: I2C_SCL_RATIO(1000)
 * @param[in]    buffer     i2c buffer  (point to a gobal memory)
 * @param[in]    size       i2c buffer len, = address size + data size
 * @description
 *  This function is used to initialize I2C in master mode. SCL speed is up to 400KHz. The function is also
 *  used to enable I2c interrupt, and enable NVIC I2C IRQ.
 *****************************************************************************************
 */
void mpu_self_get_ft_data(uint8_t saddr, uint8_t axis,self_test_data* st_data1)
{
    int8_t temp1,temp2;

    if (axis & XG)   
    {
        temp1 = I2C_BYTE_READ(saddr,SELF_TEST_X);
        st_data1->data_xg_test = temp1 & 0x1f;
        if (st_data1->data_xg_test)
            ft_data.ft_xg = (25.0*131.0*pow(1.046,(double_t)(ft_test_data.data_xg_test-1))) ;
        else
            ft_data.ft_xg = 0.0;
        QPRINTF("data_xg is : %d ",st_data1->data_xg_test);
    }
    if (axis & YG)
    {   
        temp1 = I2C_BYTE_READ(saddr,SELF_TEST_Y);
        st_data1->data_yg_test = temp1 & 0x1f;
        if (st_data1->data_yg_test)
            ft_data.ft_yg = (-25.0*131.0*pow(1.046,(double_t)(ft_test_data.data_yg_test-1))) ;
        else
            ft_data.ft_yg = 0.0;
        QPRINTF("data_yg is : %d ",st_data1->data_yg_test);
    }
    if (axis & ZG)
    {
        temp1 = I2C_BYTE_READ(saddr,SELF_TEST_Z);
        st_data1->data_zg_test = temp1 & 0x1f;
        if (st_data1->data_zg_test)
            ft_data.ft_zg = (25.0*131.0*pow(1.046,(double_t)(ft_test_data.data_zg_test-1))) ;
        else
            ft_data.ft_zg = 0.0;
        QPRINTF("data_zg is : %d ",st_data1->data_zg_test);
    }
    if (axis & XA)
    {
        temp1 = I2C_BYTE_READ(saddr,SELF_TEST_X);
        temp2 = I2C_BYTE_READ(saddr,SELF_TEST_A);
        st_data1->data_xa_test = ((temp1 & 0xe0) >> 3) + ((temp2 & 0x30) >> 4);
        if (st_data1->data_xa_test)
            ft_data.ft_xa = (4096.0*0.34*pow((0.92/0.34),(double_t)((ft_test_data.data_xa_test-1)/(double_t)(pow(2,5) - 2)))) ;
        else
            ft_data.ft_xa = 0.0;
        QPRINTF("data_xa is : %d ",st_data1->data_xa_test);
    }
    if (axis & YA)
    {   
        temp1 = I2C_BYTE_READ(saddr,SELF_TEST_Y);
        temp2 = I2C_BYTE_READ(saddr,SELF_TEST_A);
        st_data1->data_ya_test = ((temp1 & 0xe0) >> 3) + ((temp2 & 0x0C) >> 2);
        if (st_data1->data_ya_test)
            ft_data.ft_ya = (4096.0*0.34*pow((0.92/0.34),(double_t)((ft_test_data.data_ya_test-1)/(double_t)(pow(2,5) - 2)))) ;
        else
            ft_data.ft_ya = 0.0;
        QPRINTF("data_ya is : %d ",st_data1->data_ya_test);
    }
    if (axis & ZA)
    {
        temp1 = I2C_BYTE_READ(saddr,SELF_TEST_Z);
        temp2 = I2C_BYTE_READ(saddr,SELF_TEST_A);
        st_data1->data_za_test = ((temp1 & 0xe0) >> 3) + (temp2 & 0x03);
        if (st_data1->data_za_test)
            ft_data.ft_za = (4096.0*0.34*pow((0.92/0.34),(double_t)((ft_test_data.data_za_test-1)/(double_t)(pow(2,5) - 2)))) ;
        else
            ft_data.ft_za = 0.0;
        QPRINTF("data_za is : %d ",st_data1->data_za_test);
    }
    QPRINTF("\r\n");
}

/**
 ****************************************************************************************
 * @brief Check I2C bus is busy or free
 * @return Busy or free
 *****************************************************************************************
 */
void mpu_self_test_active(uint8_t axis)
{
    uint8_t reg_g = 0x00,reg_a = 0x00;
    if (axis & XG)
        reg_g |= 0x80;
    if (axis & YG)
        reg_g |= 0x40;
    if (axis & ZG)
        reg_g |= 0x20;
    if (axis & XA)
        reg_a |= 0x80;
    if (axis & YA)
        reg_a |= 0x40;
    if (axis & ZA)
        reg_a |= 0x20;        
    
    uint8_t tmp;
    
    tmp = I2C_BYTE_READ(MPU6050_ADDR,GYRO_CONFIG);
    reg_g |= tmp;
    I2C_BYTE_WRITE(MPU6050_ADDR,GYRO_CONFIG,reg_g);
    tmp = I2C_BYTE_READ(MPU6050_ADDR,ACCEL_CONFIG);
    reg_a |= tmp;
    I2C_BYTE_WRITE(MPU6050_ADDR,ACCEL_CONFIG,reg_a);
}

void mpu_self_test_close(uint8_t axis)
{
    uint8_t reg_g = 0xFF,reg_a = 0xFF;
    if (axis & XG)
        reg_g &= 0x7f;
    if (axis & YG)
        reg_g &= 0xBF;
    if (axis & ZG)
        reg_g &= 0xDF;
    if (axis & XA)
        reg_a &= 0x7F;
    if (axis & YA)
        reg_a &= 0xBF;
    if (axis & ZA)
        reg_a &= 0xDF;        
    
    uint8_t tmp;
    
    tmp = I2C_BYTE_READ(MPU6050_ADDR,GYRO_CONFIG);
    reg_g &= tmp;
    I2C_BYTE_WRITE(MPU6050_ADDR,GYRO_CONFIG,reg_g);
    tmp = I2C_BYTE_READ(MPU6050_ADDR,ACCEL_CONFIG); //0x10
    reg_a &= tmp;
    I2C_BYTE_WRITE(MPU6050_ADDR,ACCEL_CONFIG,reg_a);
}
/**
 ****************************************************************************************
 * @brief Start a data reception.
 * @param[in]      saddr          slave device address (7bits, without R/W bit)
 * @description
 * This function is used to complete an I2C read transaction from start to stop. All the intermittent steps
 * are handled in the interrupt handler while the interrupt is enabled.
 * Before this function is called, the read length, write length, I2C master buffer,
 * and I2C state need to be filled. Please refer to I2C_BYTE_READ().
 * As soon as the end of the data transfer is detected, the callback function is called.
 *****************************************************************************************
 */
uint8_t mpu_self_test(uint8_t axis)
{
    uint8_t tmp_result = 0;
    sensor_data_out dis_st_data_out,en_st_data_out;
    mpu_self_get_ft_data(MPU6050_ADDR,(XG|YG|ZG|XA|YA|ZA),&ft_test_data);
    mpu_self_test_close(XG|YG|ZG|XA|YA|ZA);
    for (uint8_t i = 0;i < 10 ;i++)
    mpu6050_get_gyro_accel_data(&dis_st_data_out);
    QPRINTF("gyro_do xg_data:%d    ",dis_st_data_out.gyro_do.xg_data_out);
    QPRINTF("yg_data:%d    ",dis_st_data_out.gyro_do.yg_data_out);
    QPRINTF("zg_data:%d\r\n",dis_st_data_out.gyro_do.zg_data_out);
    QPRINTF("accel_do xa_data:%d   ",dis_st_data_out.accel_do.xa_data_out);
    QPRINTF("ya_data:%d   ",dis_st_data_out.accel_do.ya_data_out);
    QPRINTF("za_data:%d\r\n",dis_st_data_out.accel_do.za_data_out);
    delay(2000000);
    mpu_self_test_active(XG|YG|ZG|XA|YA|ZA);
    for (uint8_t i = 0;i < 10 ;i++)
    mpu6050_get_gyro_accel_data(&en_st_data_out);
    QPRINTF("gyro_do xg_data:%d    ",en_st_data_out.gyro_do.xg_data_out);
    QPRINTF("yg_data:%d    ",en_st_data_out.gyro_do.yg_data_out);
    QPRINTF("zg_data:%d\r\n",en_st_data_out.gyro_do.zg_data_out);
    QPRINTF("accel_do xa_data:%d   ",en_st_data_out.accel_do.xa_data_out);
    QPRINTF("ya_data:%d   ",en_st_data_out.accel_do.ya_data_out);
    QPRINTF("za_data:%d\r\n",en_st_data_out.accel_do.za_data_out);
    tmp_result = self_test_calculate(&dis_st_data_out,&en_st_data_out);
    //QPRINTF("ft_data is: xg 0x%x yg 0x%x zg 0x%x xa 0x%x ya 0x%x za 0x%x \r\n",
    //ft_test_data.data_xg_test,ft_test_data.data_yg_test,ft_test_data.data_zg_test,ft_test_data.data_xa_test,ft_test_data.data_ya_test,ft_test_data.data_za_test);
//    if (axis & XG)
//    {
//        //delay(1000000);
//        tmp_result |= mpu_self_test_one_axis(XG);
//    }
//    if (axis & YG)
//    {
//        //delay(1000000);
//        tmp_result |= mpu_self_test_one_axis(YG);
//    }
//    if (axis & ZG)
//    {
//        //delay(1000000);
//        tmp_result |= mpu_self_test_one_axis(ZG);
//    }
//    if (axis & XA)
//    {   
//        //delay(1000000);
//        tmp_result |= mpu_self_test_one_axis(XA);
//    }
//    if (axis & YA)
//    {
//        //delay(1000000);
//        tmp_result |= mpu_self_test_one_axis(YA);
//    }
//    if (axis & ZA)
//    {
//        //delay(1000000);
//        tmp_result |= mpu_self_test_one_axis(ZA);
//    }        
//     
    return tmp_result;
}


/**
 ****************************************************************************************
 * @brief Start a data transmission.
 * @param[in]  saddr         slave device address(7bits, without R/W bit)
 * @description
 * This function is used to complete an I2C write transaction from start to stop. All the intermittent steps
 * are handled in the interrupt handler while the interrupt is enabled.
 * Before this function is called, the read length, write length, I2C master buffer,
 * and I2C state need to be filled. Please refer to I2C_BYTE_WRITE().
 * As soon as the end of the data transfer is detected, the callback function is called.
 *****************************************************************************************
 */
uint8_t mpu_self_test_one_axis(enum axis axis)
{
    uint8_t result = 0;
    double_t st_cft;
    int16_t tmp_data_out1,tmp_data_out2;
    //Î´¿ªÆô×Ô¼ìÊ±
    switch((uint8_t)axis)
    {
        case XG :
            tmp_data_out1 = (int16_t)mpu6050_get_data(MPU6050_ADDR,GYRO_XOUT_H);
            //delay(27000);
            QPRINTF("st:::dis:::xg is %d\r\n",tmp_data_out1);
            mpu_self_test_active(XG);
            //delay(27000);
            tmp_data_out2 =  (int16_t)mpu6050_get_data(MPU6050_ADDR,GYRO_XOUT_H);
            //delay(27000);
            mpu_self_test_close(XG);
            //delay(27000);
            QPRINTF("st:::en:::xg is %d\r\n",tmp_data_out2);
            st_cft = (((int16_t)tmp_data_out2 - (int16_t)tmp_data_out1)-ft_data.ft_xg)/ft_data.ft_xg;
            if (st_cft <0.14 && st_cft > (-0.14))
                result |= XG;
            break;
        case YG :
            tmp_data_out1 = (int16_t)mpu6050_get_data(MPU6050_ADDR,GYRO_YOUT_H);
            //delay(27000);
            QPRINTF("st:::dis:::yg is %d\r\n",(int16_t)tmp_data_out1);
            mpu_self_test_active(YG);
            //delay(27000);
            tmp_data_out2 =  (int16_t)mpu6050_get_data(MPU6050_ADDR,GYRO_YOUT_H);
            //delay(27000);
            mpu_self_test_close(YG);
            //delay(27000);
            QPRINTF("st:::en:::yg is %d\r\n",tmp_data_out2);
            st_cft = (((int16_t)tmp_data_out2 - (int16_t)tmp_data_out1)-ft_data.ft_yg)/ft_data.ft_yg;
            if (st_cft <0.14 && st_cft > (-0.14))
                result |= YG;
            break;
        case ZG :
            tmp_data_out1 = (int16_t)mpu6050_get_data(MPU6050_ADDR,GYRO_ZOUT_H);
            //delay(27000);
            QPRINTF("st:::dis:::zg is %d\r\n",tmp_data_out1);
            mpu_self_test_active(ZG);
            //delay(27000);
            tmp_data_out2 = (int16_t)mpu6050_get_data(MPU6050_ADDR,GYRO_ZOUT_H);
            //delay(27000);
            mpu_self_test_close(ZG);
            //delay(27000);
            QPRINTF("st:::en:::zg is %d\r\n",tmp_data_out2);
            st_cft = (((int16_t)tmp_data_out2 - (int16_t)tmp_data_out1)-ft_data.ft_zg)/ft_data.ft_zg;
            if (st_cft <0.14 && st_cft > (-0.14))
                result |= ZG;
            break;
        case XA :
            tmp_data_out1 = (int16_t)mpu6050_get_data(MPU6050_ADDR,ACCEL_XOUT_H);
            //delay(27000);
            QPRINTF("st:::dis:::xa is %d\r\n",tmp_data_out1);
            mpu_self_test_active(XA);
            //delay(27000);
            tmp_data_out2 =  (int16_t)mpu6050_get_data(MPU6050_ADDR,ACCEL_XOUT_H);
            //delay(27000);
            mpu_self_test_close(XA);
            //delay(27000);
            QPRINTF("st:::en:::xa is %d\r\n",tmp_data_out2);
            st_cft = (((int16_t)tmp_data_out2 - (int16_t)tmp_data_out1)-ft_data.ft_xa)/ft_data.ft_xa;
            if (st_cft <0.14 && st_cft > (-0.14))
                result |= XA;
            break;
        case YA :
            tmp_data_out1 = (int16_t)mpu6050_get_data(MPU6050_ADDR,ACCEL_YOUT_H);
            //delay(27000);
            QPRINTF("st:::dis:::ya is %d\r\n",tmp_data_out1);
            mpu_self_test_active(YA);
            //delay(27000);
            tmp_data_out2 =  (int16_t)mpu6050_get_data(MPU6050_ADDR,ACCEL_YOUT_H);
            //delay(27000);
            mpu_self_test_close(YA);
            //delay(27000);
            QPRINTF("st:::en:::ya is %d\r\n",tmp_data_out2);
            st_cft = (((int16_t)tmp_data_out2 - (int16_t)tmp_data_out1)-ft_data.ft_ya)/ft_data.ft_ya;
            if (st_cft <0.14 && st_cft > (-0.14))
                result |= YA;
            break;
        case ZA :
            tmp_data_out1 = (int16_t)mpu6050_get_data(MPU6050_ADDR,ACCEL_ZOUT_H);
            //delay(27000);
            QPRINTF("st:::dis:::za is %d\r\n",tmp_data_out1);
            mpu_self_test_active(ZA);
            //delay(27000);
            tmp_data_out2 =  (int16_t)mpu6050_get_data(MPU6050_ADDR,ACCEL_ZOUT_H);
            //delay(27000);
            mpu_self_test_close(ZA);
            //delay(27000);
            QPRINTF("st:::en:::za is %d\r\n",tmp_data_out2);
            st_cft = (((int16_t)tmp_data_out2 - (int16_t)tmp_data_out1)-ft_data.ft_za)/ft_data.ft_za;
            if (st_cft <0.14 && st_cft > (-0.14))
                result |= ZA;
            break;
        default :break;
    }
    return result;
}

/**
 ****************************************************************************************
 * @brief Read a byte data form i2c device
 * @param[in]  saddr         slave device address(7bits, without R/W bit)
 * @param[in]  reg_addr      device register address
 * @return     reg_data      read from i2c bus
 * @description
 * Read a byte data from slave device, the data address is 8 bits. If I2C device not need 
 * to specifiy a data address, the input param reg_addr should be set to 0, and i2c_env.i2cTxCount 
 * also should be set to 0
 *****************************************************************************************
 */
void mpu6050_get_gyro_accel_data(sensor_data_out* data_out1)
{
    data_out1->gyro_do.xg_data_out = mpu6050_get_data(MPU6050_ADDR,GYRO_XOUT_H);   
    data_out1->gyro_do.yg_data_out = mpu6050_get_data(MPU6050_ADDR,GYRO_YOUT_H);
    data_out1->gyro_do.zg_data_out = mpu6050_get_data(MPU6050_ADDR,GYRO_ZOUT_H);
    data_out1->accel_do.xa_data_out = mpu6050_get_data(MPU6050_ADDR,ACCEL_XOUT_H);
    data_out1->accel_do.ya_data_out = mpu6050_get_data(MPU6050_ADDR,ACCEL_YOUT_H);
    data_out1->accel_do.za_data_out = mpu6050_get_data(MPU6050_ADDR,ACCEL_ZOUT_H);
}

/**
 ****************************************************************************************
 * @brief Read a byte data form i2c device
 * @param[in]  saddr         slave device address(7bits, without R/W bit)
 * @param[in]  reg_addr      device register address
 * @return     reg_data      read from i2c bus
 * @description
 * Read a byte data from slave device, the data address is 16 bits
 *
 *****************************************************************************************
 */
extern uint8_t self_test_calculate(sensor_data_out* data_out_dis,sensor_data_out* data_out_en)
{
    uint8_t result = 0;
    double_t st_cft = 0.0;
    sensor_data_out str_data_out = {0,0,0,0,0,0};
    str_data_out.gyro_do.xg_data_out = data_out_en->gyro_do.xg_data_out - 
                                       data_out_dis->gyro_do.xg_data_out;
    str_data_out.gyro_do.yg_data_out = data_out_en->gyro_do.yg_data_out - 
                                       data_out_dis->gyro_do.yg_data_out;
    str_data_out.gyro_do.zg_data_out = data_out_en->gyro_do.zg_data_out - 
                                       data_out_dis->gyro_do.zg_data_out;
    str_data_out.accel_do.xa_data_out = data_out_en->accel_do.xa_data_out - 
                                        data_out_dis->accel_do.xa_data_out;
    str_data_out.accel_do.ya_data_out = data_out_en->accel_do.ya_data_out - 
                                        data_out_dis->accel_do.ya_data_out;
    str_data_out.accel_do.za_data_out = data_out_en->accel_do.za_data_out - 
                                        data_out_dis->accel_do.za_data_out;
    st_cft = (str_data_out.gyro_do.xg_data_out-ft_data.ft_xg)/ft_data.ft_xg;
    if (st_cft <0.14 && st_cft > (-0.14))
    {
       QPRINTF("\r\nXG :%d OK!\r\n",XG);
       result |= XG;
    }
    else
    {
        QPRINTF("XG :%d FAILED!\r\n",XG);
    }
    st_cft = (str_data_out.gyro_do.yg_data_out-ft_data.ft_yg)/ft_data.ft_yg;
    if (st_cft <0.14 && st_cft > (-0.14))
    {
       QPRINTF("YG :%d OK!\r\n",YG);
       result |= YG;
    }
    else
    {
        QPRINTF("YG :%d FAILED!\r\n",YG);
    }
    st_cft = (str_data_out.gyro_do.zg_data_out-ft_data.ft_zg)/ft_data.ft_zg;
    if (st_cft <0.14 && st_cft > (-0.14))
    {
       QPRINTF("ZG :%d OK!\r\n",ZG);
       result |= ZG;
    }
    else
    {
        QPRINTF("ZG :%d FAILED!\r\n",ZG);
    }
    st_cft = (str_data_out.accel_do.xa_data_out -ft_data.ft_xa)/ft_data.ft_xa;
    if (st_cft <0.14 && st_cft > (-0.14))
    {
       QPRINTF("XA :%d OK!\r\n",XA);
       result |= XA;
    }
    else
    {
        QPRINTF("XA :%d FAILED!\r\n",XA);
    }
    st_cft = (str_data_out.accel_do.ya_data_out -ft_data.ft_ya)/ft_data.ft_ya;
    if (st_cft <0.14 && st_cft > (-0.14))
    {
       QPRINTF("YA :%d OK!\r\n",YA);
       result |= YA;
    }
    else
    {
        QPRINTF("YA :%d FAILED!\r\n",YA);
    }
    st_cft = (str_data_out.accel_do.za_data_out -ft_data.ft_za)/ft_data.ft_za;
    if (st_cft <0.14 && st_cft > (-0.14))
    {
       QPRINTF("ZA :%d OK!\r\n",ZA);
       result |= ZA;
    }
    else
    {
        QPRINTF("ZA :%d FAILED!\r\n",ZA);
    }
    //QPRINTF("result is :%d\r\n",result);
    return result;
}

/**
 ****************************************************************************************
 * @brief Read n byte data form i2c device
 * @param[in]  saddr         slave device address(7bits, without R/W bit)
 * @param[in]  reg_addr      device register address
 * @param[in]  buffer        Pointer to read data buffer
 * @param[in]  len           read data length
 * @description
 * Read n byte data from slave device, read start address is 8 bits and the data will be
 * stored in buffer, n is the specified length
 *****************************************************************************************
 */


/**
 ****************************************************************************************
 * @brief Read n byte data form i2c device
 * @param[in]  saddr         slave device address(7bits, without R/W bit)
 * @param[in]  reg_addr      device register address
 * @param[in]  buffer        Pointer to read data buffer
 * @param[in]  len           read data length
 * @description
 * Read n byte data from slave device, read start address is 16 bits and the data will be
 * stored in buffer, n is the specified length
 *****************************************************************************************
 */


/**
 ****************************************************************************************
 * @brief Write a byte data to i2c device
 * *
 * @param[in]  saddr         slave device address(7bits, without R/W bit)
 * @param[in]  reg_addr      device register address
 * @param[in]  reg_data      byte data
 * @description
 *  Write a byte data to a 8 bits address of slave device
 *****************************************************************************************
 */


/**
 ****************************************************************************************
 * @brief Write a byte data to i2c device
 * *
 * @param[in]  saddr         slave device address(7bits, without R/W bit)
 * @param[in]  reg_addr      device register address
 * @param[in]  reg_data      byte data
 * @description
 *  Write a byte data to a 16 bits address of slave device
 *****************************************************************************************
 */


/**
 ****************************************************************************************
 * @brief Write n byte data to i2c device
 * *
 * @param[in]  saddr         slave device address(7bits, without R/W bit)
 * @param[in]  reg_addr      device register address
 * @param[in]  buffer        pointer to write data
 * @param[in]  len           write data length
 * @description
 *  Write n byte data to slave device. The write starting address is 8 bits. The data is from
 *  the buffer and n is a specified length
 *****************************************************************************************
 */


/**
 ****************************************************************************************
 * @brief Write n byte data to i2c device
 * *
 * @param[in]  saddr         slave device address(7bits, without R/W bit)
 * @param[in]  reg_addr      device register address
 * @param[in]  buffer        pointer to write data
 * @param[in]  len           write data length
 * @description
 *  Write n byte data to slave device. The write starting address is 16 bits. The data is from
 *  the buffer and n is a specified length
 *****************************************************************************************
 */


/*
 * STRUCTURE DEFINITIONS
 ****************************************************************************************
 */

///Structure defining I2C environment parameters


/*
 * GLOBAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */
///I2C environment variable


/**
 ****************************************************************************************
 * @brief Initializes the I2C controller
 * @param[in]    buffer     i2c buffer (point to a gobal memory)
 * @param[in]    size       i2c buffer len, = address size + data size
 * @description
 *  The function is used to initialize I2C in slave mode, this function is also
 *  used to enable I2c interrupt, and enable NVIC I2C IRQ.
 *****************************************************************************************
 */

/// @} I2C
