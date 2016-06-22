#include "robot.h"

#define ENC1_PINA	32
#define ENC1_PINB	18
#define ENC2_PINA	42
#define ENC2_PINB	19
#define DIST1_PIN	A0
#define DIST2_PIN	A1
#define MOT1_PIN_IN1	8
#define MOT1_PIN_IN2	12
#define MOT1_PIN_EN		6
#define MOT2_PIN_IN1	7
#define MOT2_PIN_IN2	4
#define MOT2_PIN_EN		5
#define BATTERY_VOLTAGE	9000u

Robot::Robot(uint8_t ID):
	_ID(ID),
	_encoder1(new EncoderSensor(ENC1_PINA,ENC1_PINB)),
	_encoder2(new EncoderSensor(ENC2_PINA,ENC2_PINB)),
	_speed1(new DifferenceSensor(_encoder1)),
	_speed2(new DifferenceSensor(_encoder2)),
	_distance1(new Sharp41S(DIST1_PIN)),
	_distance2(new Sharp41S(DIST2_PIN)),
	_motor1(new L293D(MOT1_PIN_IN1,MOT1_PIN_IN2,MOT1_PIN_EN,BATTERY_VOLTAGE)),
	_motor2(new L293D(MOT2_PIN_IN1,MOT2_PIN_IN2,MOT2_PIN_EN,BATTERY_VOLTAGE)),
	p11(0.0f),p12(0.0f),p13(0.0f),p21(0.0f),p22(0.0f),p23(0.0f),p31(0.0f),p32(0.0f),p33(0.0f),
	x1(0.0f),x2(0.0f),x3(0.0f)
{
	//do something else
}

void Robot::init(){
	//initialize the robot - sort of starting procedure
}

void Robot::controllerHook(){
	//do something that is periodic
	MicroOS::setGPoutInt(0,_encoder1->readRawValue());
	MicroOS::setGPoutInt(1,_encoder2->readRawValue());
	MicroOS::setGPoutFloat(0,_speed1->readCalibratedValue());
	MicroOS::setGPoutFloat(1,_speed2->readCalibratedValue());
	MicroOS::setGPoutFloat(2,_distance1->readCalibratedValue());
	MicroOS::setGPoutFloat(3,_distance2->readCalibratedValue());
	
	
	// 1. Read the sensors.
	float u1 = 1.0f, u2 = 1.0f; //inputs to the motors
	float s1 = 2.0f, s2 = 2.0f; //sensor measurements
	
	// Kalman filter
	// 2. Prediction
#define DT	0.01f
	x1 = x1 + DT*cosf(x3)*u1;
	x2 = x2 + DT*sinf(x3)*u1;
	x3 = x3 + DT*u2;
	
	float t1 = -u1*sinf(x3)*DT;
	float t2 = u2*cosf(x3)*DT;
	
#define Q11	0.01f
#define Q22	0.01f
#define Q33	0.0002f 	
	
	p11 = p11 + t1*p31 + t1*p13 + t1*t1*p33 + Q11;
	p12 = p12 + t1*p32 + t2*p13 + t1*t2*p33;
	p13 = p13 + t1*p33;
	p21 = p21 + t2*p31 + t1*p23 + t1*t2*p33;
	p22 = p22 + t2*p32 + t2*p23 +t2*t2*p33 + Q22;
	p23 = p23 + t2*p33;
	p31 = p31 + t1*p33;
	p32 = p32 + t2*p33;
	p33 = p33 + Q33;
	
	// 3. Correction
	// 3.1. Compute H
	float h11 = -1.0f/sqrt(1.0f + s1*s1);
	float h12 = -s1/sqrt(1.0f + s1*s1);
	float h21 = -s2/sqrt(1.0f + s2*s2);
	float h22 = -1.0f/sqrt(1.0f + s2*s2);

#define R11	0.1f
#define R22	0.1f
	// 3.2. Compute S
	float s11 = (h11*p11 + h12*p21)*h11 + (h11*p12 + h12*p22)*h12 + R11;
	float s12 = (h11*p11 + h12*p21)*h21 + (h11*p12 + h12*p22)*h22;
	float s21 = (h21*p11 + h22*p21)*h11 + (h21*p12 + h22*p22)*h12;
	float s22 = (h21*p11 + h22*p21)*h21 + (h21*p12 + h22*p22)*h22 + R22;
	
	// 3.3 Compute S-inverse
	float detS = s11*s22 - s12*s21;
	float s11inv = s22/detS;
	float s12inv = -s12/detS;
	float s21inv = -s21/detS;
	float s22inv = s11/detS;
	
	// 3.4. Compute kalman gain
	float l11 = (p11*h11 + p12*h12)*s11inv + (p11*h21 + p12*h22)*s21inv;
	float l12 = (p11*h11 + p12*h12)*s12inv + (p11*h21 + p12*h22)*s22inv;
	float l21 = (p21*h11 + p22*h12)*s11inv + (p21*h21 + p22*h22)*s21inv;
	float l22 = (p21*h11 + p22*h12)*s12inv + (p21*h21 + p22*h22)*s22inv;
	float l31 = (p31*h11 + p32*h12)*s11inv + (p31*h21 + p32*h22)*s21inv;
	float l32 = (p31*h11 + p32*h12)*s12inv + (p31*h21 + p32*h22)*s22inv;
	
	// 3.5. Update the covariance matrix
	float p11post = p11 + l11*(h11*p11 + h12*p21) + l12*(h21*p11 + h22*p21);
	float p12post = p12 + l11*(h11*p12 + h12*p22) + l12*(h21*p12 + h22*p22);
	float p13post = p13 + l11*(h11*p13 + h12*p23) + l12*(h21*p13 + h22*p23);
	float p21post = p21 + l21*(h11*p11 + h12*p21) + l22*(h21*p11 + h22*p21);
	float p22post = p22 + l21*(h11*p12 + h12*p22) + l22*(h21*p12 + h22*p22);
	float p23post = p23 + l21*(h11*p13 + h12*p23) + l22*(h21*p13 + h22*p23);
	float p31post = p31 + l31*(h11*p11 + h12*p21) + l32*(h21*p11 + h22*p21);
	float p32post = p32 + l31*(h11*p12 + h12*p22) + l32*(h21*p12 + h22*p22);
	float p33post = p33 + l31*(h11*p13 + h12*p23) + l32*(h21*p13 + h22*p23);
	
	p11 = p11post;
	p12 = p12post;
	p13 = p13post;
	p21 = p21post;
	p22 = p22post;
	p23 = p23post;
	p31 = p31post;
	p32 = p32post;
	p33 = p33post;
	
	// 3.6. Update the state estimate
	float y1 = s1 - (h11*x1+h12*x2);
	float y2 = s2 - (h21*x1+h22*x2);
	
	float x1 = x1 + l11*y1+l12*y2;
	float x2 = x2 + l21*y1+l22*y2;
	float x3 = x3 + l31*y1+l32*y2;
	
}

void Robot::setID(uint8_t ID){
    _ID = ID;
}

uint8_t Robot::ID()
{
    return _ID;
}
