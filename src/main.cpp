
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <ArduinoLog.h>

#include "SerialMP3Player.h"


#if defined(SSID_PWD)
const char* pass = SSID_PWD;
#else
const char* pass = "*******";
#endif

const char* ssid = "TBD"; // SSID for microcontroller to FHEM server WiFi connection

const char* hostName = "WAKEUP";

// example actor request http://[IP]:[PORT]/fhem?cmd.[ACTOR]=set [ACTOR] on&room=[room]&XHR=1

const char* request = "fhem?cmd.";
const char* requestActor1 = "TBD"; // shutter actor 1
const char* requestActor2 = "TBD"; // shutter actor 2
const char* requestRoom = "TBD"; // room

#define TX D8 // serial interface mp3 board
#define RX D7 // serial interface mp3 board

SerialMP3Player mp3(RX, TX);

char c;  // char from Serial
char cmd=' ';
char cmd1=' ';

void menu(char op, int nval){
    // Menu
    switch (op){
        case '?':
        case 'h':
        Serial.println("SerialMP3Player Basic Commands:");
        Serial.println(" ? - Display Menu options. ");
        Serial.println(" P01 - Play 01 file");
        Serial.println(" F01 - Play 01 folder");
        Serial.println(" S01 - Play 01 file in loop");
        Serial.println(" V01 - Play 01 file, volume 30");
        Serial.println(" p - Play");
        Serial.println(" a - pause");
        Serial.println(" s - stop ");
        Serial.println(" > - Next");
        Serial.println(" < - Previous");
        Serial.println(" + - Volume UP");
        Serial.println(" - - Volume DOWN");
        Serial.println(" v15 - Set Volume to 15");
        Serial.println(" c - Query current file");
        Serial.println(" q - Query status");
        Serial.println(" x - Query folder count");
        Serial.println(" t - Query total file count");
        Serial.println(" r - Reset");
        Serial.println(" e - Sleep");
        Serial.println(" w - Wake up");
        break;

        case 'P':
        Serial.println("Play");
        mp3.play(nval);
        break;

        case 'F':
        Serial.println("Play Folder");
        mp3.playF(nval);
        break;

        case 'S':
        Serial.println("Play loop");
        mp3.playSL(nval);
        break;

        case 'V':
        Serial.println("Play file at 30 volume");
        mp3.play(nval,30);
        break;

        case 'p':
        Serial.println("Play");
        mp3.play();
        break;

        case 'a':
        Serial.println("Pause");
        mp3.pause();
        break;

        case 's':
        Serial.println("Stop");
        mp3.stop();
        break;

        case '>':
        Serial.println("Next");
        mp3.playNext();
        break;

        case '<':
        Serial.println("Previous");
        mp3.playPrevious();
        break;

        case '+':
        Serial.println("Volume UP");
        mp3.volUp();
        break;

        case '-':
        Serial.println("Volume Down");
        mp3.volDown();
        break;

        case 'v':
        Serial.println("Set to Volume");
        mp3.setVol(nval);
        mp3.qVol();
        break;

        case 'c':
        Serial.println("Query current file");
        mp3.qPlaying();
        break;

        case 'q':
        Serial.println("Query status");
        mp3.qStatus();
        break;

        case 'x':
        Serial.println("Query folder count");
        mp3.qTFolders();
        break;

        case 't':
        Serial.println("Query total file count");
        mp3.qTTracks();
        break;

        case 'r':
        Serial.println("Reset");
        mp3.reset();
        break;

        case 'e':
        Serial.println("Sleep");
        mp3.sleep();
        break;

        case 'w':
        Serial.println("Wake up");
        mp3.wakeup();
        break;
    }
}

void decode_c(){
    // Decode c looking for a specific command or a digit

    // if c is a 'v', 'P', 'F', 'S' or 'V' wait for the number XX
    if (c=='v' || c=='P' || c=='F' || c=='S' || c=='V'){
        cmd=c;
    } else {
        // maybe c is part of XX number
        if (c>='0' && c<='9'){
            // if c is a digit
            if (cmd1==' '){
                // if cmd1 is empty then c is the first digit
                cmd1 = c;
            } else {
                // if cmd1 is not empty c is the second digit
                menu(cmd, ((cmd1-'0')*10)+(c-'0'));
                cmd = ' ';
                cmd1 = ' ';
            }
        } else {
            // c is not a digit nor 'v', 'P', 'F' or 'S' so just call menu(c, nval);
            menu(c, 0);
        }
    }
}

void sendActorRequest(const char* requestActor) {

    HTTPClient http;
    const int httpPort = 8888; // TBD Host Port
    const char* host = "TBD"; // TBD Host IP

    String url = "http://";
    url += host;
    url += ":";
    url += httpPort;
    url += "/";
    url += request;
    url += requestActor;
    url += "=set%20";
    url += requestActor;
    url += "%20on";
    url += "&room=";
    url += requestRoom;
    url += "&XHR=1";

    http.begin(url);

    Serial.println(url); // Print the response payload

    int httpCode = http.GET();

    Serial.print("http result: ");
    Serial.println(httpCode);

    http.end();

}

void setup() {

    Serial.begin(9600); // start serial interface

    Log.begin(LOG_LEVEL_VERBOSE, &Serial);

    Serial.println("");

    delay(1000);

    mp3.showDebug(1); // enable mp3 board debugging

    mp3.begin(9600); // start mp3-communication
    delay(500); // wait for init

    mp3.sendCommand(CMD_SEL_DEV, 0, 2); //select sd-card
    delay(500); // wait for init

    menu('?',0); // print the menu options.

    pinMode(D2, INPUT_PULLUP);

}

const unsigned blockTimeMilliSeconds { 10 * 60 * 2 }; // ~100 ms cycle
unsigned currentTimeMilliSeconds { 0 };
bool isBlocked = false;

void unblock() {
    Serial.println("Not blocked anymore.");
    isBlocked = false;
    currentTimeMilliSeconds = 0;
}

void loop() {

    if (analogRead(A0) > 100 && not isBlocked) {
        mp3.play(4);
        isBlocked = true;
        Serial.println("Blocked.");
    }

    if (digitalRead(D2) == LOW) {
        mp3.stop();
        unblock();
        Serial.println("Stop.");
    }

    if (Serial.available()){
        c = Serial.read();
        decode_c();
    }

    if (mp3.available()){
        Serial.println(mp3.decodeMP3Answer());
    }

    if (currentTimeMilliSeconds >= blockTimeMilliSeconds) {
        unblock();
    }

    delay(100);

    currentTimeMilliSeconds++;

}
