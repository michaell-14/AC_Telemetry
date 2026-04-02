#pragma once
#ifndef AC_DATA_H

#define AC_DATA_H

#include <iostream>
#include <vector>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <complex>
//#include <fftw3.h> // FFT library

struct handshaker{
    int32_t identifier = 1;
    int32_t version = 1;
    int32_t operationid = 0; // 0 for handshake; 1 for 400Hz rt data; 2 for on-demand update; 3 for dismiss
};

#pragma pack(push, 1) // Defiing structure of UDP data; need (push, 1) to avoid complier padding and formatting for network
struct CarData {
    char identifier; // identifier for structed data
    int32_t size; // size of data packet in bytes

    float speed_kmh; // speed in km/h
    float speed_mph; // in mph
    float speed_ms; // in m/s

    //abs
    int8_t isAbsEnabled; // ABS on or off
    int8_t isAbsInAction; // ABS status

    //tc
    int8_t isTcEnabled; // Traction control on or off
    int8_t isTcInAction; // Traction control status

    //pit
    int8_t isInPit;
    int8_t isEngineLimiterOn;

    //G force
    float accG_vertical;
    float accG_horizontal;
    float accG_frontal;

    //Timing
    int32_t lapTime;
    int32_t lastLap;
    int32_t bestLap;
    int32_t lapCount;

    //Engine
    float gas;
    float brake;
    float clutch;
    float engineRPM;
    float steer;
    int32_t gear;
    float cgHeight; // Height relative to centre of gravity

    //Wheel
    float wheelAngularSpeed[4]; // Angular speed of each wheel
    float slipAngle[4]; // Slip angle of each wheel
    float slipAngle_ContactPatch[4]; //slip angle on contact patch
    float slipRatio[4]; //slip ratio of each wheel
    float tyreSlip[4];
    float ndSlip[4];
    float load[4]; //load on each wheel
    float Dy[4]; //lateral friction
    float Mz[4]; //longitudinal friction
    float tyreDirtyLevel[4];

    float camberRAD[4]; //camber angle in radians
    float tyreRadius[4]; 
    float tyreLoadedRadius[4]; //radius of tyre under load

    //suspension
    float suspensionHeight[4]; //suspension height of each wheel
    float carPositionNormalized;
    float carSlope;
    float carCoordinates[3]; // x, y, z coordinates of the car in the track reference
};
#pragma pack(pop)

struct RTLap{
    int32_t carIdentifierNumber;
    int32_t lap;
    char driverName[50];
    char carName[50];
    int32_t time;
};

#endif // AC_DATA_H
