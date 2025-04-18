#define TINY_GSM_MODEM_SIM7600
#define TINY_GSM_DEBUG SerialMon
#define DUMP_AT_COMMANDS

#define SerialMon Serial
#define SerialAT Serial1

#include <TinyGsmClient.h>
#include <StreamDebugger.h>

const char apn[] = "your_apn";  // Fill this in if you want to test PDP later
const char gprsUser[] = "";
const char gprsPass[] = "";

#define MODEM_PWRKEY 4
#define MODEM_FLIGHT 25
#define MODEM_RX 26
#define MODEM_TX 27
#define LED_PIN 12

StreamDebugger debugger(SerialAT, SerialMon);
TinyGsm modem(debugger);

void powerOnModem() {
  pinMode(MODEM_PWRKEY, OUTPUT);
  digitalWrite(MODEM_PWRKEY, HIGH);
  delay(300);
  digitalWrite(MODEM_PWRKEY, LOW);

  pinMode(MODEM_FLIGHT, OUTPUT);
  digitalWrite(MODEM_FLIGHT, HIGH);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);
}

void listAvailableNetworks() {
  SerialMon.println("Scanning for available networks...");
  modem.sendAT("+COPS=?");
  String copsResult = modem.stream.readStringUntil('\n');
  SerialMon.println("Available Networks: ");
  SerialMon.println(copsResult);
}

void showCurrentNetworkInfo() {
  SerialMon.println("Querying current network info...");
  modem.sendAT("+COPS?");
  String cops = modem.stream.readStringUntil('\n');
  SerialMon.println("Current Network: ");
  SerialMon.println(cops);
}

void getRegistrationStatus() {
  SerialMon.println("Checking network registration status...");
  modem.sendAT("+CREG?");
  String reg = modem.stream.readStringUntil('\n');
  SerialMon.println("CREG status:");
  SerialMon.println(reg);
}

void setup() {
  SerialMon.begin(115200);
  SerialAT.begin(115200, SERIAL_8N1, MODEM_RX, MODEM_TX);
  delay(1000);

  powerOnModem();
  delay(3000);

  SerialMon.println("Waiting for modem...");
  while (!modem.testAT()) {
    SerialMon.print(".");
    delay(500);
  }

  SerialMon.println("\nModem online!");

  // List towers, show network
  listAvailableNetworks();
  delay(5000);

  showCurrentNetworkInfo();
  delay(2000);

  getRegistrationStatus();
  delay(2000);

  int16_t signal = modem.getSignalQuality();
  SerialMon.print("Signal Quality: ");
  SerialMon.println(signal);
}

void loop() {
  getRegistrationStatus();
  delay(10000);
}
