// ======================================= SATOS v1.5 =========================================
// --------------------------------------- Apache 2.0 -----------------------------------------

#include <Arduino.h>
// Watchdog
#include <esp_task_wdt.h>
// Can't wipe its past memory 
#include <Preferences.h>

#define WDT_TIMEOUT_SECONDS 11
// DECLARE YOUR CUSTOM FUNCTIONS HERE AND VARS IF NEEDED

float Gettemp() {
  return temperatureRead();
}

int altitude = 0;
int system_pin = 2;
bool lowPwrMsg = false;
bool e0mmsg = false;
bool LEO = false;
bool starttask = false;
String retStr = "";
bool karman = false;
// Altitudes
bool e100m = false;
bool e500m = false;
bool e800m = false;
bool e1500m = false;
bool eKarman = false;
// -----------------
Preferences preferences;

void setup() {
  setCpuFrequencyMhz(240);
  Serial.begin(115200);
  pinMode(system_pin, OUTPUT);
  delay(6000);
  Serial.println("========== ---- SATOS ---- Type help for help ==========");
  preferences.begin("satos", false);
  altitude = preferences.getInt("altitude", 0);      // Default to 0 if nothing saved
  karman = preferences.getBool("karman", false);     // Default to false
  starttask = preferences.getBool("starttask", false);
  LEO = preferences.getBool("LEO", false);
  esp_task_wdt_init(WDT_TIMEOUT_SECONDS, true);
  esp_task_wdt_add(NULL);
}

void loop() {
  esp_task_wdt_reset();
  float tempC = Gettemp();
  if (e100m) {
  altitude = 100;
  retStr += "Reached 100M\n";
  e100m = false;
}


// ADD YOUR COMMANDS HERE TO CONTROL YOUR VARIABLES AND FLIGHT OPERATIONS
if (Serial.available()) {
String input = Serial.readStringUntil('X');
input.trim();
if (input == "alt100m") {
if (e100m == false){
      e100m = true;
      Serial.println("NOW IS TRUE = e100m");
  }
  else {
    e100m = false;
    Serial.println("NOW IS FALSE = e100m");
  }
}
else if (input == "alt500m"){
  if (e500m == false){
      e500m = true;
      Serial.println("NOW IS TRUE = e500m");
  }
  else {
    e500m = false;
    Serial.println("NOW IS FALSE = e500m");
  }
}
else if (input == "alt800m") {
  if (e800m == false){
      e800m = true;
      Serial.println("NOW IS TRUE = e800m");
  }
  else {
    e800m = false;
    Serial.println("NOW IS FALSE = e800m");
  }
}
else if (input == "alt1.5km") {
  if (e1500m == false){
      e1500m = true;
      Serial.println("NOW IS TRUE = e1500m");
  }
  else {
    e1500m = false;
    Serial.println("NOW IS FALSE = e1500m");
  }
}
else if (input == "altkarmapass") {
  if (eKarman == false){
      eKarman = true;
      Serial.println("NOW IS TRUE = eKarman");
  }
  else {
    eKarman = false;
    Serial.println("NOW IS FALSE = eKerman");
  }
}
else if (input == "leo") {
  LEO = true;
  preferences.putBool("LEO", LEO);
  Serial.println("LEO IS SET TO TRUE");
}
else if (input == "deploy") {
  if (!starttask) {
  Serial.println("Deploying payload");
  starttask = true;
  preferences.putBool("starttask", starttask);
  }
  else {
    starttask = false;
    Serial.println("Stopped Payload");
  }
}

else if (input == "") return;
else if (input == "help") {
  // ADD OTHER COMMANDS AS YOU WANT
  Serial.println("====================SATOS Help Center==========================");
  Serial.println("Every command ends with a capital X. Example: deployX");
  Serial.println("Never use a capital X inside a command name.");
  Serial.println("");
  Serial.println("FLIGHT");
  Serial.println("  alt100m       set altitude to 100 m");
  Serial.println("  alt500m       set altitude to 500 m");
  Serial.println("  alt800m       set altitude to 800 m");
  Serial.println("  alt1.5km      set altitude to 1500 m");
  Serial.println("  altkarmapass  pass the Karman line (100 km)");
  Serial.println("  leo           reach low earth orbit");
  Serial.println("");
  Serial.println("PAYLOAD");
  Serial.println("  deploy        start tasks (send again to stop)");
  Serial.println("");
  Serial.println("OTHER");
  Serial.println("  help          show this list");
  Serial.println("=====================================================");
}
else {
  Serial.println("Command Not Found Error. Type help for more info.");
}
}
if (e500m) {
  altitude = 500;
  retStr += "Reached 500M\n";
  preferences.putInt("altitude", altitude);
  e500m = false;
}

if (e800m) {
  altitude = 800;
  retStr += "Reached 800M\n";
  preferences.putInt("altitude", altitude);
  e800m = false;
}

if (e1500m) {
  altitude = 1500;
  retStr += "Reached ~1.5KM\n";
  preferences.putInt("altitude", altitude);
  e1500m = false;
}

if (eKarman) {
  altitude = 100 * 1000;
  retStr += "Passed Karman Line\n";
  preferences.putInt("altitude", altitude);
  eKarman = false;
}

if (LEO) {
  LEO = false;
  retStr += "Reached LEO\n";
}
  if(!e0mmsg && altitude == 0) { // In meters
    Serial.println("currently in 0 meter");
    e0mmsg = true;
  }
  if (altitude >= 100 * 1000 && !karman) {
    Serial.println("Reached Past karman line, leaving earth");
    // YOUR EARLY LEO TASK
    karman = true;
    preferences.putBool("karman", karman);
    retStr += "Kerman line\n";
  }
  if (starttask) {
    // HERE, PUT ALL YOUR TASKS THAT WILL BE EXECUTED IN SPACE
    // Packs all known sersor and temp outputs and terminal outputs to earth.json
    // EDIT THIS PART FOR YOUR CUSTOM SYSTEM AND READS
    retStr += "CPU Temp: " + String(tempC) + "\n" + "Sent logs \n";
    // ADD OTHER THINGS TO THE RETSTR FOR YOUR MODULE OUTPUTS THEN SEND IT TO EARTH
    retStr= "";
    
  }
  
}