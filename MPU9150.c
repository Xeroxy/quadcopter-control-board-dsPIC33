#include "main.h"
#include "MPU9150.h"
#include "common.h"

GyroOutHL GOHL={0,0,0,0,0,0};
GyroOutRaw GOR={0,0,0};
GyroOffset GOS={0,0,0,0,0,0};
GyroOut GO={0,0,0,0,0,0};

AccelOutHL AOHL={0,0,0,0,0,0};
AccelOutRaw AOR={0,0,0};
AccelOffset AOS={0,0,0,0,0,0};
AccelOutRate AORa={0,0,0};
AccelOut AO={0,0,0};

MagOutHL MOHL={0,0,0,0,0,0};
MagOutRaw MOR={0,0,0};
MagOffset MOS={0,0,0,0,0,0};
MagOut MO={0,0,0};

void MPU9150TestI2C(){
    unsigned char Data = 0x00;
    unsigned char Data0 =0x00;
    Data=ByteReadI2C(MPU6050_ADDRESS, MPU6050_RA_WHO_AM_I);
    //Data0=ByteReadI2C(0b0011000, 0x00);
    if(Data==0x68){//
        printf("\n*I2C Read Test 0 Passed, MPU9150 Address: 0x%x", Data);
      //  if(Data0==0x0C){
      //      printf("\n*I2C Read Test 1 Passed, MPU9150 Mag Address: 0x%x", Data0);
      //  }
    }
    else{
        printf("\n*ERROR: I2C Read Test Failed, Stopping...");
        while(1){};
    }
}

void setupIMU(){

    //Sets sample rate to 1000/1+1 = 500Hz
    ByteWriteI2C(MPU6050_ADDRESS, MPU6050_RA_SMPLRT_DIV, 0x01);
    //Disable FSync, 48Hz DLPF Sample Rate = Gyroscope Output Rate / (1 + SMPLRT_DIV)
    ByteWriteI2C(MPU6050_ADDRESS, MPU6050_RA_CONFIG, 0x03);
    //Disable gyro self tests, scale of 500 degrees/s
    ByteWriteI2C(MPU6050_ADDRESS, MPU6050_RA_GYRO_CONFIG, 0b00001000);
    //Disable accel self tests, scale of +-2g, no DHPF
    ByteWriteI2C(MPU6050_ADDRESS, MPU6050_RA_ACCEL_CONFIG, 0b00000000);
    //Freefall threshold of <|0mg|
    ByteWriteI2C(MPU6050_ADDRESS, MPU6050_RA_FF_THR, 0x00);
    //Freefall duration limit of 0
    ByteWriteI2C(MPU6050_ADDRESS, MPU6050_RA_FF_DUR, 0x00);
    //Motion threshold of >0mg
    ByteWriteI2C(MPU6050_ADDRESS, MPU6050_RA_MOT_THR, 0x00);
    //Motion duration of >0s
    ByteWriteI2C(MPU6050_ADDRESS, MPU6050_RA_MOT_DUR, 0x00);
    //Zero motion threshold
    ByteWriteI2C(MPU6050_ADDRESS, MPU6050_RA_ZRMOT_THR, 0x00);
    //Zero motion duration threshold
    ByteWriteI2C(MPU6050_ADDRESS, MPU6050_RA_ZRMOT_DUR, 0x00);
    //Disable sensor output to FIFO buffer
    ByteWriteI2C(MPU6050_ADDRESS, MPU6050_RA_FIFO_EN, 0x00);

    //AUX I2C setup
    //Sets AUX I2C to single master control, plus other config
    ByteWriteI2C(MPU6050_ADDRESS, MPU6050_RA_I2C_MST_CTRL, 0x00);
    //Setup AUX I2C slaves
      ByteWriteI2C(MPU6050_ADDRESS, MPU6050_RA_I2C_SLV0_ADDR, 0x00);
      ByteWriteI2C(MPU6050_ADDRESS, MPU6050_RA_I2C_SLV0_REG, 0x00);
      ByteWriteI2C(MPU6050_ADDRESS, MPU6050_RA_I2C_SLV0_CTRL, 0x00);
      ByteWriteI2C(MPU6050_ADDRESS, MPU6050_RA_I2C_SLV1_ADDR, 0x00);
      ByteWriteI2C(MPU6050_ADDRESS, MPU6050_RA_I2C_SLV1_REG, 0x00);
      ByteWriteI2C(MPU6050_ADDRESS, MPU6050_RA_I2C_SLV1_CTRL, 0x00);
      ByteWriteI2C(MPU6050_ADDRESS, MPU6050_RA_I2C_SLV2_ADDR, 0x00);
      ByteWriteI2C(MPU6050_ADDRESS, MPU6050_RA_I2C_SLV2_REG, 0x00);
      ByteWriteI2C(MPU6050_ADDRESS, MPU6050_RA_I2C_SLV2_CTRL, 0x00);
      ByteWriteI2C(MPU6050_ADDRESS, MPU6050_RA_I2C_SLV3_ADDR, 0x00);
      ByteWriteI2C(MPU6050_ADDRESS, MPU6050_RA_I2C_SLV3_REG, 0x00);
      ByteWriteI2C(MPU6050_ADDRESS, MPU6050_RA_I2C_SLV3_CTRL, 0x00);
      ByteWriteI2C(MPU6050_ADDRESS, MPU6050_RA_I2C_SLV4_ADDR, 0x00);
      ByteWriteI2C(MPU6050_ADDRESS, MPU6050_RA_I2C_SLV4_REG, 0x00);
      ByteWriteI2C(MPU6050_ADDRESS, MPU6050_RA_I2C_SLV4_DO, 0x00);
      ByteWriteI2C(MPU6050_ADDRESS, MPU6050_RA_I2C_SLV4_CTRL, 0x00);
      ByteWriteI2C(MPU6050_ADDRESS, MPU6050_RA_I2C_SLV4_DI, 0x00);


      //MPU6050_RA_I2C_MST_STATUS //Read-only
      //Setup INT pin and AUX I2C pass through
    ByteWriteI2C(MPU6050_ADDRESS, MPU6050_RA_INT_PIN_CFG, 0x00);
    //Enable data ready interrupt
    ByteWriteI2C(MPU6050_ADDRESS, MPU6050_RA_INT_ENABLE, 0x00);

    //Slave out, dont care
    ByteWriteI2C(MPU6050_ADDRESS, MPU6050_RA_I2C_SLV0_DO, 0x00);
    ByteWriteI2C(MPU6050_ADDRESS, MPU6050_RA_I2C_SLV1_DO, 0x00);
    ByteWriteI2C(MPU6050_ADDRESS, MPU6050_RA_I2C_SLV2_DO, 0x00);
    ByteWriteI2C(MPU6050_ADDRESS, MPU6050_RA_I2C_SLV3_DO, 0x00);
    //More slave config
    ByteWriteI2C(MPU6050_ADDRESS, MPU6050_RA_I2C_MST_DELAY_CTRL, 0x00);
    //Reset sensor signal paths
    ByteWriteI2C(MPU6050_ADDRESS, MPU6050_RA_SIGNAL_PATH_RESET, 0x00);
    //Motion detection control
    ByteWriteI2C(MPU6050_ADDRESS, MPU6050_RA_MOT_DETECT_CTRL, 0x00);
    //Disables FIFO, AUX I2C, FIFO and I2C reset bits to 0
    ByteWriteI2C(MPU6050_ADDRESS, MPU6050_RA_USER_CTRL, 0x00);
    //Sets clock source to gyro reference w/ PLL
    ByteWriteI2C(MPU6050_ADDRESS, MPU6050_RA_PWR_MGMT_1, 0b00000010);
    //Controls frequency of wakeups in accel low power mode plus the sensor standby modes
    ByteWriteI2C(MPU6050_ADDRESS, MPU6050_RA_PWR_MGMT_2, 0x00);
    //Data transfer to and from the FIFO buffer
    ByteWriteI2C(MPU6050_ADDRESS, MPU6050_RA_FIFO_R_W, 0x00);

    printf("\nMPU9150 Setup Complete");
}

int MPU9150CheckRegisters(){
    unsigned char Data = 0x00;
    unsigned char Failed = 0;

    Data=ByteReadI2C(MPU6050_ADDRESS, MPU6050_RA_SMPLRT_DIV);
    if(Data != 0x01) { printf("\nRegister check 1 failed, value should be 0x01, was 0x%x", Data); Failed = 1; }
    Data=ByteReadI2C(MPU6050_ADDRESS, MPU6050_RA_CONFIG);
    if(Data != 0x03) { printf("\nRegister check 2 failed, value should be 0x03, was 0x%x", Data); Failed = 1; }
    Data=ByteReadI2C(MPU6050_ADDRESS, MPU6050_RA_GYRO_CONFIG);
    if(Data != 0b00001000) { printf("\nRegister check 3 failed, value should be 0b00001000, was 0x%x", Data); Failed = 1; }
    Data=ByteReadI2C(MPU6050_ADDRESS, MPU6050_RA_ACCEL_CONFIG);
    if(Data != 0b00000000) { printf("\nRegister check 4 failed, value should be 0b00000000, was 0x%x", Data); Failed = 1; }
    Data=ByteReadI2C(MPU6050_ADDRESS, MPU6050_RA_FF_THR);
    if(Data != 0x00) { printf("\nRegister check 5 failed, value should be 0x00, was 0x%x", Data); Failed = 1; }
    Data=ByteReadI2C(MPU6050_ADDRESS, MPU6050_RA_FF_DUR);
    if(Data != 0x00) { printf("\nRegister check 6 failed, value should be 0x00, was 0x%x", Data); Failed = 1; }
    Data=ByteReadI2C(MPU6050_ADDRESS, MPU6050_RA_MOT_THR);
    if(Data != 0x00) { printf("\nRegister check 7 failed, value should be 0x00, was 0x%x", Data); Failed = 1; }
    Data=ByteReadI2C(MPU6050_ADDRESS, MPU6050_RA_MOT_DUR);
    if(Data != 0x00) { printf("\nRegister check 8 failed, value should be 0x00, was 0x%x", Data); Failed = 1; }
    Data=ByteReadI2C(MPU6050_ADDRESS, MPU6050_RA_ZRMOT_THR);
    if(Data != 0x00) { printf("\nRegister check 9 failed, value should be 0x00, was 0x%x", Data); Failed = 1; }
    Data=ByteReadI2C(MPU6050_ADDRESS, MPU6050_RA_ZRMOT_DUR);
    if(Data != 0x00) { printf("\nRegister check 10 failed, value should be 0x00, was 0x%x", Data); Failed = 1; }
    Data=ByteReadI2C(MPU6050_ADDRESS, MPU6050_RA_FIFO_EN);
    if(Data != 0x00) { printf("\nRegister check 11 failed, value should be 0x00, was 0x%x", Data); Failed = 1; }
    Data=ByteReadI2C(MPU6050_ADDRESS, MPU6050_RA_I2C_MST_CTRL);
    if(Data != 0x00) { printf("\nRegister check 12 failed, value should be 0x00, was 0x%x", Data); Failed = 1; }
    Data=ByteReadI2C(MPU6050_ADDRESS, MPU6050_RA_I2C_SLV0_ADDR);
    if(Data != 0x00) { printf("\nRegister check 13 failed, value should be 0x00, was 0x%x", Data); Failed = 1; }
    Data=ByteReadI2C(MPU6050_ADDRESS, MPU6050_RA_I2C_SLV0_REG);
    if(Data != 0x00) { printf("\nRegister check 14 failed, value should be 0x00, was 0x%x", Data); Failed = 1; }
    Data=ByteReadI2C(MPU6050_ADDRESS, MPU6050_RA_I2C_SLV0_CTRL);
    if(Data != 0x00) { printf("\nRegister check 15 failed, value should be 0x00, was 0x%x", Data); Failed = 1; }
    Data=ByteReadI2C(MPU6050_ADDRESS, MPU6050_RA_I2C_SLV1_ADDR);
    if(Data != 0x00) { printf("\nRegister check 16 failed, value should be 0x00, was 0x%x", Data); Failed = 1; }
    Data=ByteReadI2C(MPU6050_ADDRESS, MPU6050_RA_I2C_SLV1_REG);
    if(Data != 0x00) { printf("\nRegister check 17 failed, value should be 0x00, was 0x%x", Data); Failed = 1; }
    Data=ByteReadI2C(MPU6050_ADDRESS, MPU6050_RA_I2C_SLV1_CTRL);
    if(Data != 0x00) { printf("\nRegister check 18 failed, value should be 0x00, was 0x%x", Data); Failed = 1; }
    Data=ByteReadI2C(MPU6050_ADDRESS, MPU6050_RA_I2C_SLV2_ADDR);
    if(Data != 0x00) { printf("\nRegister check 19 failed, value should be 0x00, was 0x%x", Data); Failed = 1; }
    Data=ByteReadI2C(MPU6050_ADDRESS, MPU6050_RA_I2C_SLV2_REG);
    if(Data != 0x00) { printf("\nRegister check 20 failed, value should be 0x00, was 0x%x", Data); Failed = 1; }
    Data=ByteReadI2C(MPU6050_ADDRESS, MPU6050_RA_I2C_SLV2_CTRL);
    if(Data != 0x00) { printf("\nRegister check 21 failed, value should be 0x00, was 0x%x", Data); Failed = 1; }
    Data=ByteReadI2C(MPU6050_ADDRESS, MPU6050_RA_I2C_SLV3_ADDR);
    if(Data != 0x00) { printf("\nRegister check 22 failed, value should be 0x00, was 0x%x", Data); Failed = 1; }
    Data=ByteReadI2C(MPU6050_ADDRESS, MPU6050_RA_I2C_SLV3_REG);
    if(Data != 0x00) { printf("\nRegister check 23 failed, value should be 0x00, was 0x%x", Data); Failed = 1; }
    Data=ByteReadI2C(MPU6050_ADDRESS, MPU6050_RA_I2C_SLV3_CTRL);
    if(Data != 0x00) { printf("\nRegister check 24 failed, value should be 0x00, was 0x%x", Data); Failed = 1; }
    Data=ByteReadI2C(MPU6050_ADDRESS, MPU6050_RA_I2C_SLV4_ADDR);
    if(Data != 0x00) { printf("\nRegister check 25 failed, value should be 0x00, was 0x%x", Data); Failed = 1; }
    Data=ByteReadI2C(MPU6050_ADDRESS, MPU6050_RA_I2C_SLV4_REG);
    if(Data != 0x00) { printf("\nRegister check 26 failed, value should be 0x00, was 0x%x", Data); Failed = 1; }
    Data=ByteReadI2C(MPU6050_ADDRESS, MPU6050_RA_I2C_SLV4_DO);
    if(Data != 0x00) { printf("\nRegister check 27 failed, value should be 0x00, was 0x%x", Data); Failed = 1; }
    Data=ByteReadI2C(MPU6050_ADDRESS, MPU6050_RA_I2C_SLV4_CTRL);
    if(Data != 0x00) { printf("\nRegister check 28 failed, value should be 0x00, was 0x%x", Data); Failed = 1; }
    Data=ByteReadI2C(MPU6050_ADDRESS, MPU6050_RA_I2C_SLV4_DI);
    if(Data != 0x00) { printf("\nRegister check 29 failed, value should be 0x00, was 0x%x", Data); Failed = 1; }
    Data=ByteReadI2C(MPU6050_ADDRESS, MPU6050_RA_INT_PIN_CFG);
    if(Data != 0x00) { printf("\nRegister check 30 failed, value should be 0x00, was 0x%x", Data); Failed = 1; }
    Data=ByteReadI2C(MPU6050_ADDRESS, MPU6050_RA_INT_ENABLE);
    if(Data != 0x00) { printf("\nRegister check 31 failed, value should be 0x00, was 0x%x", Data); Failed = 1; }
    Data=ByteReadI2C(MPU6050_ADDRESS, MPU6050_RA_I2C_SLV0_DO);
    if(Data != 0x00) { printf("\nRegister check 32 failed, value should be 0x00, was 0x%x", Data); Failed = 1; }
    Data=ByteReadI2C(MPU6050_ADDRESS, MPU6050_RA_I2C_SLV1_DO);
    if(Data != 0x00) { printf("\nRegister check 33 failed, value should be 0x00, was 0x%x", Data); Failed = 1; }
    Data=ByteReadI2C(MPU6050_ADDRESS, MPU6050_RA_I2C_SLV2_DO);
    if(Data != 0x00) { printf("\nRegister check 34 failed, value should be 0x00, was 0x%x", Data); Failed = 1; }
    Data=ByteReadI2C(MPU6050_ADDRESS, MPU6050_RA_I2C_SLV3_DO);
    if(Data != 0x00) { printf("\nRegister check 35 failed, value should be 0x00, was 0x%x", Data); Failed = 1; }
    Data=ByteReadI2C(MPU6050_ADDRESS, MPU6050_RA_I2C_MST_DELAY_CTRL);
    if(Data != 0x00) { printf("\nRegister check 36 failed, value should be 0x00, was 0x%x", Data); Failed = 1; }
    Data=ByteReadI2C(MPU6050_ADDRESS, MPU6050_RA_SIGNAL_PATH_RESET);
    if(Data != 0x00) { printf("\nRegister check 37 failed, value should be 0x00, was 0x%x", Data); Failed = 1; }
    Data=ByteReadI2C(MPU6050_ADDRESS, MPU6050_RA_MOT_DETECT_CTRL);
    if(Data != 0x00) { printf("\nRegister check 38 failed, value should be 0x00, was 0x%x", Data); Failed = 1; }
    Data=ByteReadI2C(MPU6050_ADDRESS, MPU6050_RA_USER_CTRL);
    if(Data != 0x00) { printf("\nRegister 39 check failed, value should be 0x00, was 0x%x", Data); Failed = 1; }
    Data=ByteReadI2C(MPU6050_ADDRESS, MPU6050_RA_PWR_MGMT_1);
    if(Data != 0x02) { printf("\nRegister check 40 failed, value should be 0x02, was 0x%x", Data); Failed = 1; }
    Data=ByteReadI2C(MPU6050_ADDRESS, MPU6050_RA_PWR_MGMT_2);
    if(Data != 0x00) { printf("\nRegister check 41 failed, value should be 0x00, was 0x%x", Data); Failed = 1; }
    Data=ByteReadI2C(MPU6050_ADDRESS, MPU6050_RA_FIFO_R_W);
    if(Data != 0x00) { printf("\nRegister check 42 failed, value should be 0x00, was 0x%x", Data); Failed = 1; }

    if (Failed == 0) { printf("\n*Register value check passed"); }
    else { printf("\n*ERROR: Register value check failed");}

return(Failed);
}

unsigned char getRate(){
    unsigned char Data=0x00;
    ByteReadI2C(MPU6050_ADDRESS, MPU6050_RA_SMPLRT_DIV);
    return Data;
}

//Gets raw accelerometer gyro & mag data, performs no processing
void getAccelValues(){
    AOHL.XHight=ByteReadI2C(MPU6050_ADDRESS, MPU6050_RA_ACCEL_XOUT_H);
    AOHL.XLow=ByteReadI2C(MPU6050_ADDRESS, MPU6050_RA_ACCEL_XOUT_L);
    AOHL.YHight=ByteReadI2C(MPU6050_ADDRESS, MPU6050_RA_ACCEL_YOUT_H);
    AOHL.YLow=ByteReadI2C(MPU6050_ADDRESS, MPU6050_RA_ACCEL_YOUT_L);
    AOHL.ZHight=ByteReadI2C(MPU6050_ADDRESS, MPU6050_RA_ACCEL_ZOUT_H);
    AOHL.ZLow=ByteReadI2C(MPU6050_ADDRESS, MPU6050_RA_ACCEL_ZOUT_L);

    AOR.X = ((AOHL.XHight<<8)|AOHL.XLow);
    AOR.Y = ((AOHL.YHight<<8)|AOHL.YLow);
    AOR.Z = ((AOHL.ZHight<<8)|AOHL.ZLow);
   // printf("\n*Accel Raw : \n *X:%d -- Y:%d -- Z:%d", AOR.X, AOR.Y, AOR.Z);
}

void getGyroValues(){
    GOHL.XHight=ByteReadI2C(MPU6050_ADDRESS, MPU6050_RA_GYRO_XOUT_H);
    GOHL.XLow=ByteReadI2C(MPU6050_ADDRESS, MPU6050_RA_GYRO_XOUT_L);
    GOHL.YHight=ByteReadI2C(MPU6050_ADDRESS, MPU6050_RA_GYRO_YOUT_H);
    GOHL.YLow=ByteReadI2C(MPU6050_ADDRESS, MPU6050_RA_GYRO_YOUT_L);
    GOHL.ZHight=ByteReadI2C(MPU6050_ADDRESS, MPU6050_RA_GYRO_ZOUT_H);
    GOHL.ZLow=ByteReadI2C(MPU6050_ADDRESS, MPU6050_RA_GYRO_ZOUT_L);

    GOR.X = ((GOHL.XHight<<8)|GOHL.XLow) - GOS.X;
    GOR.Y = ((GOHL.YHight<<8)|GOHL.YLow) - GOS.Y;
    GOR.Z = ((GOHL.ZHight<<8)|GOHL.ZLow) - GOS.Z;
}

void getMagValues(){
    MOHL.XHight=ByteReadI2C(MPU9150_RA_MAG_ADDRESS, MPU9150_RA_MAG_XOUT_H);
    MOHL.XLow=ByteReadI2C(MPU9150_RA_MAG_ADDRESS, MPU9150_RA_MAG_XOUT_L);
    MOHL.YHight=ByteReadI2C(MPU9150_RA_MAG_ADDRESS, MPU9150_RA_MAG_YOUT_H);
    MOHL.YLow=ByteReadI2C(MPU9150_RA_MAG_ADDRESS, MPU9150_RA_MAG_YOUT_L);
    MOHL.ZHight=ByteReadI2C(MPU9150_RA_MAG_ADDRESS, MPU9150_RA_MAG_ZOUT_H);
    MOHL.ZLow=ByteReadI2C(MPU9150_RA_MAG_ADDRESS, MPU9150_RA_MAG_ZOUT_L);

    MOR.X = ((MOHL.XHight<<8)|MOHL.XLow);
    MOR.Y = ((MOHL.YHight<<8)|MOHL.YLow);
    MOR.Z = ((MOHL.ZHight<<8)|MOHL.ZLow);
}

void calibrateGyros(){
    int x=0;
    for(x=0; x<5000; x++){
        getGyroValues();
        GOS.XSum += GOR.X;
        GOS.YSum += GOR.Y;
        GOS.ZSum += GOR.Y;
        __delay_ms(1);
    }
    GOS.X = (GOS.XSum/5000);//*(int)AccScale;
    GOS.Y = (GOS.YSum/5000);//*(int)AccScale;
    GOS.Z = (GOS.ZSum/5000);//*(int)AccScale;

    printf("\nGyro Calibration Complete");
    printf("\n*Gyro X offset sum: %ld Gyro X offset: %d", GOS.XSum, GOS.X);
    printf("\n*Gyro Y offset sum: %ld Gyro Y offset: %d", GOS.YSum, GOS.Y);
    printf("\n*Gyro Z offset sum: %ld Gyro Z offset: %d", GOS.ZSum, GOS.Z);
}

void calibrateAccel(){
    int x=0;
    for(x=0; x<10000; x++){
        getAccelValues();
        AOS.XSum += AOR.X;
        AOS.YSum += AOR.Y;
        AOS.ZSum += AOR.Z;
    }
    AOS.X = AOS.XSum/5000;
    AOS.Y = AOS.YSum/5000;
    AOS.Z = AOS.ZSum/5000;

    printf("\nAccel Calibration Complete");
    printf("\n*Accel X offset sum: %ld Gyro X offset: %d", AOS.XSum, AOS.X);
    printf("\n*Accel Y offset sum: %ld Gyro Y offset: %d", AOS.YSum, AOS.Y);
    printf("\n*Accel Z offset sum: %ld Gyro Z offset: %d", AOS.ZSum, AOS.Z);
}
void calibrateMag(){
    int x=0;
    for(x=0; x<5000; x++){
        getMagValues();
        MOS.XSum += MOR.X;
        MOS.YSum += MOR.Y;
        MOS.ZSum += MOR.Z;
    }
    MOS.X = MOS.XSum/5000;
    MOS.Y = MOS.YSum/5000;
    MOS.Z = MOS.ZSum/5000;

    printf("\nAccel Calibration Complete");
    printf("\n*Accel X offset sum: %ld Gyro X offset: %d", MOS.XSum, MOS.X);
    printf("\n*Accel Y offset sum: %ld Gyro Y offset: %d", MOS.YSum, MOS.Y);
    printf("\n*Accel Z offset sum: %ld Gyro Z offset: %d", MOS.ZSum, MOS.Z);
}
void getAccelRate(){
    AORa.XRate=(((float)AOR.X-(float)AOS.X)/(float)AccScale);
    AORa.YRate=(((float)AOR.Y-(float)AOS.Y)/(float)AccScale);
    AORa.ZRate=(((float)AOR.Z-(float)AOS.Z)/(float)AccScale);
    printf("\n*Accel Rate: *X:%f -- Y:%f -- Z:%f", AORa.XRate, AORa.YRate, AORa.ZRate);
}
void getGyroRate(){
    GO.XRate=(float)(((float)GOR.X-(float)GOS.X)/(float)GyroScale)*(float)(-1);
    GO.YRate=(float)(((float)GOR.Y-(float)GOS.Y)/(float)GyroScale)*(float)(1);
    GO.ZRate=(float)(((float)GOR.Z-(float)GOS.Z)/(float)GyroScale)*(float)(-1);
    //printf("\n*Gyro Rate: *X:%f -- Y:%f -- Z:%f", GO.XRate, GO.YRate, GO.ZRate);
}
void getAccelAngles(){
    float magXZ = sqrt((AORa.XRate*AORa.XRate)+(AORa.ZRate*AORa.ZRate));
    float magYZ = sqrt((AORa.YRate*AORa.YRate)+(AORa.ZRate*AORa.ZRate));


    AO.pitch=atan(AORa.YRate/magXZ)*toDeg;
    AO.roll=atan(AORa.XRate/magYZ)*toDeg;

    /*if (AORa.ZRate<0) {
        AO.XAngle = -acos(AORa.XRate/R)*toDeg;
        AO.YAngle = -acos(AORa.YRate/R)*toDeg;
    }
    else {
        AO.XAngle = acos(AORa.XRate/R)*toDeg;
        AO.YAngle = acos(AORa.YRate/R)*toDeg;
    }

    AO.ZAngle = acos(AORa.ZRate/R)*toDeg;

    // Shifts the angles so as to be 0 on horizontal position
    AO.XAngle -= 90; //-90
    if (AO.XAngle<-180) {
        AO.XAngle +=360;
    }
    AO.YAngle -= 90;
    if (AO.YAngle<-180) {
    AO.YAngle +=360;
    }

    AO.XAngle = -AO.XAngle;
    AO.YAngle = -AO.YAngle;

    // Ensures that Z gets negative if pointing downward
    /*if (angles.x<0)
    {
    angles.z = - angles.z;
    }*/
     printf("\n*Accel Angles: *roll:%f -- pitch:%f -- Z:%f", AO.roll, AO.pitch, AO.ZAngle);
}
void getGyroAngles(){
    GO.XAngle = GO.XRate*dt;
    GO.YAngle = GO.YRate*dt;
    GO.ZAngle = GO.ZRate*dt;
    printf("\n*Gyro Angles: *X:%f -- Y:%f -- Z:%f", GO.XAngle, GO.YAngle, GO.ZAngle);

}
void getMotionFromRaw(){
}
////Converts the already acquired accelerometer data into 3D euler angles
//void getAccelAngles(){
//   // double R=sqrt((acc->x*acc->x) + (acc->y*acc->y) + (acc->z*acc->z));
//    AO.XAngle = 57.295*atan((float)AO.Y/ sqrt(pow((float)AO.Z,2)+pow((float)AO.X,2)));
//    AO.YAngle = 57.295*atan((float)-AO.X/ sqrt(pow((float)AO.Z,2)+pow((float)AO.Y,2)));
//}
//
//void getGyroAngles(){
//    GO.XRate = (float)GOR.X/GyroXSensitivity;
//    GO.YRate = (float)GOR.Y/GyroYSensitivity;
//    GO.ZRate = (float)GOR.Z/GyroZSensitivity;
//
//    GO.XAngle += GO.XRate*dt;
//    GO.YAngle += GO.YRate*dt;
//    GO.ZAngle += GO.ZRate*dt;
//}



