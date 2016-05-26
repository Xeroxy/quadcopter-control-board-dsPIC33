#include "main.h"
#include "common.h"

EulerAngles EA={0,0,0};
float Kp, Ki;	// PI gain rate of covergence acc/magn
float q0, q1, q2, q3;	// Quaternions (estimated orientation)
float exInt, eyInt, ezInt;	// Integral error
float prevEx, prevEy, prevEz;

float halfT;

void initKinematics(){
    q0 = 1.0;
    q1 = 0.0;
    q2 = 0.0;
    q3 = 0.0;
    exInt = 0.0;
    eyInt = 0.0;
    ezInt = 0.0;

    prevEx = 0.0;
    prevEy = 0.0;
    prevEz = 0.0;

    Kp = 0.2;
    Ki = 0.0005;
}

void computeKinematics(float gx, float gy, float gz, float ax, float ay, float az, float magX, float magY, float magZ, float G_Dt){
    float norm;
    float vx, vy, vz;
    float ex, ey, ez;

    halfT = G_Dt/2;


    // normalise
    norm = sqrt(ax*ax + ay*ay + az*az);
    ax = ax / norm;
    ay = ay / norm;
    az = az / norm;

    // estimated direction gravity
    vx = 2*(q1*q3 - q0*q2);
    vy = 2*(q0*q1 + q2*q3);
    vz = q0*q0 - q1*q1 - q2*q2 + q3*q3;

    // error is sum of cross product between reference direction of fields and direction measured
    ex = (vy*az - vz*ay);
    ey = (vz*ax - vx*az);
    ez = (vx*ay - vy*ax);

    exInt = exInt + ex*Ki;
    if (isSwitched(prevEx, ex)) exInt = 0.0;
    prevEx = ex;

    eyInt = eyInt + ey*Ki;
    if (isSwitched(prevEy, ey)) eyInt = 0.0;
    prevEy = ey;

    ezInt = ezInt + ez*Ki;
    if (isSwitched(prevEz, ez)) ezInt = 0.0;
    prevEz = ez;

    // adjuste gyro
    gx = gx + Kp*ex + exInt;
    gy = gy + Kp*ey + eyInt;
    gz = gz + Kp*ez + ezInt;

    // integrate quaternion rate and normalise
    q0 = q0 + (-q1*gx - q2*gy - q3*gz)*halfT;
    q1 = q1 + (q0*gx + q2*gz - q3*gy)*halfT;
    q2 = q2 + (q0*gy - q1*gz + q3*gx)*halfT;
    q3 = q3 + (q0*gz + q1*gy - q2*gx)*halfT;

    // normalise quaternion
    norm = sqrt(q0*q0 + q1*q1 + q2*q2 + q3*q3);
    q0 = q0 / norm;
    q1 = q1 / norm;
    q2 = q2 / norm;
    q3 = q3 / norm;
}
void getEulerAngles() {

    EA.XAngle = atan2(2 * (q0*q1 + q2*q3), 1 - 2 *(q1*q1 + q2*q2));
    EA.YAngle = -asin(2 * (q1*q3 - q0*q2));//asin(2 * (q0*q2 - q1*q3));
    EA.ZAngle = atan2(2 * (q0*q3 + q1*q2), 1 - 2 *(q2*q2 + q3*q3));
     // to deg
           /* angles.x *= 57.29577f;
    angles.y *= 57.29577f;
    angles.z *= 57.29577f;*/
}


