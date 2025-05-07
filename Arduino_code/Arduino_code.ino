/*
    CODE SOURCE: https://github.com/codeforge11/TFT-DesktopClock
    Created by codeforge11
*/

#include <TFT.h>  
#include <SPI.h>
#include <SoftwareSerial.h>

#define cs   10
#define dc   9
#define rst  8

TFT TFTscreen = TFT(cs, dc, rst);

SoftwareSerial mySerial(10, 11);

int hour = -1, minute = -1; 
unsigned long lastUpdateTime = 0; 

const int centerX = 80;
const int centerY = 50;
const int radius = 20;
const int ballSize = 6;
const int ballNum = 8;

float angles[ballNum];

int brightness[ballNum];

bool isDataReceived = false;

void setup() {
    TFTscreen.begin();
    TFTscreen.background(0, 0, 0);
    TFTscreen.setRotation(3);
    Serial.begin(9600);

    for (int i = 0; i < ballNum;  i++) {
        angles[i] = (2 * 3.141 / ballNum) * i;
        brightness[i] = 50 + (i * 30);
    }

}

void loop() {

    if (!isDataReceived) {
        loading();
    }

    if (Serial.available() > 0) {

        String receivedData = Serial.readStringUntil('\n');

        if (receivedData.length() >= 5) {
            String hourString = receivedData.substring(0, 2);
            String minuteString = receivedData.substring(3, 5);

            hour = hourString.toInt();
            minute = minuteString.toInt();


            lastUpdateTime = millis();

            isDataReceived = true;

            refreshScreen();
        } else {
            Serial.println("Error: Invalid data format!");
        }
    }

    if (isDataReceived && hour != -1 && minute != -1) {

        unsigned long currentMillis = millis();

        if (currentMillis - lastUpdateTime >= 60000) {

            if (minute >= 60) {
                minute = 0;
                hour++;
                if (hour >= 24) {
                    hour = 0;
                }
            }
            minute++;

            lastUpdateTime = currentMillis;

            refreshScreen();
        }
    }

    delay(300);
    
}

void loading() {

    TFTscreen.background(0, 0, 0);

    for (int i = 0; i < ballNum; i++) {

        int ballX = centerX + radius * cos(angles[i]);
        int ballY = centerY + radius * sin(angles[i]);

        TFTscreen.fill(brightness[i], brightness[i], 255);
        TFTscreen.circle(ballX, ballY, ballSize);
        
        brightness[i] -= 12;
        if (brightness[i] < 50) brightness[i] = 255;

    }

    TFTscreen.stroke(255, 255, 255);
    TFTscreen.setTextSize(2);
    TFTscreen.text("Loading...", 10, 100);
}

void refreshScreen() {

    TFTscreen.background(250, 16, 200);

    char time[10];
    sprintf(time, "%02d:%02d", hour, minute);

    TFTscreen.setTextSize(5);
    TFTscreen.text(time, 0, 32);

}
