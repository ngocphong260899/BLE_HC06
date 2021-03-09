#include "pzem004t.h"

#include "led_button.h"
#include "m_typedef.h"

PZEM004Tv30 pzem(&Serial2); /* Cam Cheo Day */

float voltage;
float current;
float power;
float energy;
float frequency;
float pf;

void pzem_init(void)
{
    pzem_reader();
    pzem_show();
}

void pzem_reader(void)
{
    if ((digitalRead(LED1_PIN) | digitalRead(LED2_PIN) | digitalRead(LED3_PIN))) {
        voltage = pzem.voltage();
        current = pzem.current();
        power = pzem.power();
        energy = pzem.energy();
        frequency = pzem.frequency();
        pf = pzem.pf();
    }
}

void pzem_show(void)
{
    if (voltage != NAN) {
        Serial.print("Voltage: "); 
        Serial.print(voltage); 
        Serial.println("V");
    } else {
        Serial.println("Error reading voltage");
    }

    if(current != NAN){
        Serial.print("Current: ");
        Serial.print(current);
        Serial.println("A");
    } else {
        Serial.println("Error reading current");
    }

    if(current != NAN){
        Serial.print("Power: ");
        Serial.print(power);
        Serial.println("W");
    } else {
        Serial.println("Error reading power");
    }

    if(current != NAN){
        Serial.print("Energy: ");
        Serial.print(energy,3);
        Serial.println("kWh");
    } else {
        Serial.println("Error reading energy");
    }

    if(current != NAN){
        Serial.print("Frequency: ");
        Serial.print(frequency, 1);
        Serial.println("Hz");
    } else {
        Serial.println("Error reading frequency");
    }

    if(current != NAN){
        Serial.print("PF: ");
        Serial.println(pf);
    } else {
        Serial.println("Error reading power factor");
    }
    Serial.println();
}

void pzem_hander(void)
{
    pzem_reader();
    // pzem_show();
}