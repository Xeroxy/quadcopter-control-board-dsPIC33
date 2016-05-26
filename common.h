#define dt 1

typedef struct {
    float OC1Output;
    float OC2Output;
    float OC3Output;
    float OC4Output;
}OCOutput;

typedef struct {
    float throttleInput;
    float yawInput;
    float pitchInput;
    float rollInput;
}InputCaptureData;

typedef struct {
    float throttle;
    float yawTarget;
    float rollTarget;
    float pitchTarget;
}InputCaptureOutput;

typedef struct {
    float Kp;
    float Ki;
    float Kd;
    float lastInput; // dernière erreur pour le calcul de dErr double lastTime; // instant de dernier calcul du PID
    float iErr; // accumulation de l'erreur intégrale
    float windupGuard; // valeur de Windup pour l?intégra
}PIDData;

enum{
    ROLL=0,PITCH,YAW,LAST
};

typedef struct {
    float roll;
    float pitch;
    float yaw;
}PIDOutput;

typedef struct {
    unsigned char XLow;
    unsigned char XHight;
    unsigned char YLow;
    unsigned char YHight;
    unsigned char ZLow;
    unsigned char ZHight;
}AccelOutHL;
typedef struct {
    unsigned char XLow;
    unsigned char XHight;
    unsigned char YLow;
    unsigned char YHight;
    unsigned char ZLow;
    unsigned char ZHight;
}GyroOutHL;
typedef struct {
    unsigned char XLow;
    unsigned char XHight;
    unsigned char YLow;
    unsigned char YHight;
    unsigned char ZLow;
    unsigned char ZHight;
}MagOutHL;

typedef struct {
    signed int X;
    signed int Y;
    signed int Z;
}AccelOutRaw;
typedef struct {
    signed int X;
    signed int Y;
    signed int Z;
}GyroOutRaw;
typedef struct {
    signed int X;
    signed int Y;
    signed int Z;
}MagOutRaw;

typedef struct {
    float roll;
    float pitch;
    float ZAngle;
}AccelOut;
typedef struct {
    float XRate;
    float YRate;
    float ZRate;
    float XAngle;
    float YAngle;
    float ZAngle;
}GyroOut;
typedef struct {
    float XAngle;
    float YAngle;
    float ZAngle;
}MagOut;

typedef struct {
    signed long XSum;
    signed long YSum;
    signed long ZSum;
    signed int X;
    signed int Y;
    signed int Z;
}GyroOffset;
typedef struct {
    signed long XSum;
    signed long YSum;
    signed long ZSum;
    signed int X;
    signed int Y;
    signed int Z;
}AccelOffset;
typedef struct {
    signed long XSum;
    signed long YSum;
    signed long ZSum;
    signed int X;
    signed int Y;
    signed int Z;
}MagOffset;

typedef struct {
    float XRate;
    float YRate;
    float ZRate;
}AccelOutRate;

typedef struct {
    float XAngle;
    float YAngle;
    float ZAngle;
}ComplementaryAngles;

typedef struct {
    float XAngle;
    float YAngle;
    float ZAngle;
}EulerAngles;

extern OCOutput OCO;
extern InputCaptureData ICD;
extern InputCaptureOutput ICO;

extern PIDData PIDD[LAST];
extern PIDOutput PIDO;

extern AccelOutHL AOHL;
extern AccelOutRaw AOR;
extern AccelOffset AOS;
extern AccelOut AO;
extern AccelOutRate AORa;

extern GyroOutHL GOHL;
extern GyroOutRaw GOR;
extern GyroOffset GOS;
extern GyroOut GO;


extern MagOutHL MOHL;
extern MagOutRaw MOR;
extern MagOut MO;



extern ComplementaryAngles CA;

extern EulerAngles EA;






