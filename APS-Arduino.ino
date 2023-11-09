#include <SD.h>
#include <HX711.h>

const int SPI_CIPO = 50;
const int SPI_COPI = 51;
const int SPI_SCK = 52;
const int SPI_CS = 53;

#define LOADCELL_CALIBRATION_FACTOR 42250
#define LOADCELL_DOUT_PIN 11
#define LOADCELL_SCK_PIN 10



class Sensor {
    int id;
    int refresh_delay;
    int nextReading = 0;

  public:
    Sensor(int id, int refresh_delay) {
      this->id = id;
      this->refresh_delay = refresh_delay;
    }

    virtual void pollSensor(unsigned long currentMillis) {
      if (currentMillis >= this->nextReading) {
        this->nextReading = millis() + this->refresh_delay;
        return;
      }
      
      auto data = this->readSensor();
      this->sendData(currentMillis, data);
    }

  protected:
    virtual float readSensor();
    virtual void sendData(unsigned long milliseconds, float data) {
      Serial.print(this->id);
      Serial.print(milliseconds);
      Serial.println(data);
    }
};

class LoadCellSensor : public Sensor {
    HX711 scale;

  public:
    LoadCellSensor(int id, int refresh_delay) : Sensor(id, refresh_delay) {
      this->scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
      this->scale.set_scale(LOADCELL_CALIBRATION_FACTOR);
      this->scale.tare();
    }

    virtual float readSensor() {
      return this->scale.get_units();
    }
};

const int sensorCount = 1;
Sensor* sensors[sensorCount];
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);



  int start = millis();
  //Serial.println(, 10);
  //double test = scale.get_units();
  int now = millis();
  Serial.println(now - start);

  sensors[0] = new LoadCellSensor(0,100);
}




void loop() {
  // put your main code here, to run repeatedly:
  for (int i = 0; i < sensorCount; i++) {
    sensors[0]->pollSensor(millis());
  }
  
  return;
}
