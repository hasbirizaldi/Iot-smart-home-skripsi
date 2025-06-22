// Nama   : Hasbi Rizaldi
// NPM    : 109180640047
// YNWA

#include <FirebaseESP8266.h>
#include  <ESP8266WiFi.h>
#include "DHT.h" 
#include <Servo.h>

#define ssid "Kop"  //WiFi SSID
#define password "00000000"  //WiFi Password
#define FIREBASE_HOST "kontrolrumah-4af5b-default-   rtdb.asiasoutheast1.firebasedatabase.app"     
 //Firebase Project URL hapus "https:" , "\" dan "/"
#define FIREBASE_AUTH "VJKNU8VYiMCq0oz4ypgCwgZO0iOBjIKP0f531gC1"      //Secret Code Firebase
                                  
#define DHTPIN 03  // inisialisasi Pin RX sebagai pin input sensor DHT11
#define DHTTYPE DHT11   //Inisialisasi DHT11
DHT dht11(DHTPIN, DHTTYPE);

Servo servo;

FirebaseData firebaseData;

//Jika Menggunakan Relay Module
int Device_1 = D1; //inisialisasi D0 Pin (Lampu Depan)
int Device_2 = D2; //inisialisasi D1 Pin (Lampu Ruang Tengah)
int Device_3 = D3; //inisialisasi D2 Pin (Lampu Ruang Belakang)
int Device_4 = D4; //inisialisasi D3 Pin (Lampu Kamar 1)
int Device_5 = D5; //inisialisasi D5 Pin (Lampu Kamar 2)
int Device_6 = D6; //inisialisasi D6 Pin (Kipas Angin)
int WifiLedOn = D0; //Inisialisasi Pin D0 Sebagai LED WiFi On
int WifiLedOff = D7; // Inisialisasi Pin D7 Sebagai LED WiFi Off

int pos = 0;

 void setup() {
  
 Serial.begin(9600); 

  servo.attach(15); //Inisialisasi Pin D8 sebagai input Motor Servo
  servo.write(0);
  delay(2000);
  
 dht11.begin();

  Firebase.begin(FIREBASE_HOST,FIREBASE_AUTH);
  
    pinMode(Device_1,OUTPUT); //inisialisasi  Device OUTPUT  
    pinMode(Device_2,OUTPUT); //inisialisasi  Device OUTPUT  
    pinMode(Device_3,OUTPUT); //inisialisasi  Device OUTPUT  
    pinMode(Device_4,OUTPUT); //inisialisasi  Device OUTPUT  
    pinMode(Device_5,OUTPUT); //inisialisasi  Device OUTPUT  
    pinMode(Device_6,OUTPUT); //inisialisasi  Device OUTPUT
    
    pinMode(WifiLedOn, OUTPUT);
    pinMode(WifiLedOff, OUTPUT);
    digitalWrite(WifiLedOn, LOW);

   WiFi.begin (ssid, password);
   while (WiFi.status() != WL_CONNECTED) {
   
    digitalWrite(WifiLedOff, LOW);
    delay(90);
    digitalWrite(WifiLedOff, HIGH);
    delay(90);
    Serial.println("Koneksi ke WiFi");
    Serial.print("");
  }

}

void loop() {
  
 Serial.begin(9600); 
 

//Jika WiFi Terhubung 
 if (WiFi.status() == WL_CONNECTED)
 {
    digitalWrite(WifiLedOn, HIGH);
    digitalWrite(WifiLedOff, LOW);
    
    Serial.println("Wifi terhubung"); 
    Serial.println(""); 

//Jika WiFi Terputus  
  }else if (WiFi.status() != WL_CONNECTED)
 {
    digitalWrite(WifiLedOn, LOW);
    digitalWrite(WifiLedOff, LOW);
    delay(90);
    digitalWrite(WifiLedOff, HIGH);
    delay(90);
    Serial.println("Koneksi WiFi Terputus");
    Serial.println("");
  }

 // Sensor DHT11 membaca suhu dan kelembaban
  float s = dht11.readTemperature();
  float k = dht11.readHumidity();

  // Memeriksa apakah sensor berhasil mambaca suhu dan kelembaban
  if (isnan(s) || isnan(k)) {
    Serial.println("Gagal membaca sensor DHT11");
    delay(1000);
    return;
  }
  
  // Menampilkan suhu dan kelembaban kepada serial monitor
  Serial.print("Suhu: ");
  Serial.print(s);
  Serial.println(" °C");
  Serial.print("Kelembaban: ");
  Serial.print(k);
  Serial.println(" %");
  Serial.println();
  
  // Memberikan status suhu dan kelembaban kepada firebase
  if (Firebase.setFloat(firebaseData, "/M/Suhu", s)){
      Serial.println("Suhu terkirim");
    } else{
      Serial.println("Suhu tidak terkirim");
      Serial.println("Karena: " + firebaseData.errorReason());
    } 
    
  if (Firebase.setFloat(firebaseData, "/M/Kelembaban", k)){

      Serial.println("Kelembaban terkirim");
      Serial.println();
    } else{

      Serial.println("Kelembaban tidak terkirim");
      Serial.println("Karena: Firebase " + firebaseData.errorReason());
    }
  delay(1000);
  
  if (Firebase.get(firebaseData,"KP/D1")) {
    if (firebaseData.dataType() == "string") {
      String De1 = firebaseData.stringData();
          if (De1=="1"){

              digitalWrite(Device_1,HIGH); 
              Serial.println("Lampu Depan Mati");
          }
          else if (De1=="0"){

              digitalWrite(Device_1,LOW);
              Serial.println("Lampu Depan Menyala");
            }}}
  if (Firebase.get(firebaseData,"KP/D2")) {
    if (firebaseData.dataType() == "string") {
      String De2 = firebaseData.stringData();
          if (De2=="1"){

              digitalWrite(Device_2,HIGH);
              Serial.println("Lampu Ruang Tengan Mati");
          }
              else if (De2=="0"){

              digitalWrite(Device_2,LOW);
              Serial.println("Lampu Ruang Tengah Menyala");
            }}}
  if (Firebase.get(firebaseData,"KP/D3")) {
    if (firebaseData.dataType() == "string") {
      String De3 = firebaseData.stringData();
          if (De3=="1"){

              digitalWrite(Device_3,HIGH);
              Serial.println("Lampu Ruang Belakang Mati");
          }
              else if (De3=="0"){

              digitalWrite(Device_3,LOW);
              Serial.println("Lampu Ruang Belakang Menyala");
            }}}
                      
  if (Firebase.get(firebaseData,"KP/D4")) {
    if (firebaseData.dataType() == "string") {
      String De4 = firebaseData.stringData();
          if (De4=="1"){

              digitalWrite(Device_4,HIGH);
              Serial.println("Lampu Kamar 1 Mati");
          }

              else if (De4=="0"){

              digitalWrite(Device_4,LOW);
              Serial.println("Lampu kamar 1 Menyala");

            }}}
  if (Firebase.get(firebaseData,"KP/D5")) {
    if (firebaseData.dataType() == "string") {
      String De5 = firebaseData.stringData();
          if (De5=="1"){
            
              digitalWrite(Device_5,HIGH);
              Serial.println("Lampu Kamar 2 Mati");
          }
              else if (De5=="0"){
              digitalWrite(Device_5,LOW);
              Serial.println("Lampu kamar 2 Menyala");
            }}}

  if (Firebase.get(firebaseData,"KT/D6")) {
    if (firebaseData.dataType() == "string") {
      String De6 = firebaseData.stringData();
          if (De6=="1"){
            
              digitalWrite(Device_6,HIGH);
              Serial.println("Kipas Angin Mati");
          }
              else if (De6=="0"){
                
              digitalWrite(Device_6,LOW);
              Serial.println("Kipas Angin Menyala");
            }}}                    

  if (Firebase.get(firebaseData,"KT/D7")) {
    if (firebaseData.dataType() == "string") {
      String De7 = firebaseData.stringData();
          if (De7=="1"){
            
          servo.write(180);
          Serial.println("Tirai Membuka");
          }
           else if (De7=="0"){

              servo.write(0);
              Serial.println("Tirai Menutup");
            }}}     
  
  }

