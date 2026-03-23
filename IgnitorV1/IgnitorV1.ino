int D1=12; int D2=11; int D3=10; int D4=9; // Pines de Modulo RF
int L10=8; int L5=7; int L4=6; int L3=5; int L2=4; int L1=3; // Pines LEDS indicadores
int PinOut = 2;  // Pin de salida del calentador
bool b0=0;bool b1=0;bool b2=0;bool b3=0;
bool armed=false;
bool ignition=false;
unsigned long blinkingArmed=0; //Usado para permitir parpadeo del led en modo Armado
unsigned long count2cancel=0;
unsigned long count2unArm=0;
unsigned long countantiReb1=0;
unsigned long countantiReb2=0;
unsigned long countIGN=0;
bool antiReb1=false;
bool antiReb2=false;
bool unArm = false; 
bool unIGN = false;   
void setup() 
{
  pinMode(PinOut, OUTPUT);
  pinMode(L10, OUTPUT);pinMode(L5, OUTPUT);pinMode(L4, OUTPUT);pinMode(L3, OUTPUT);pinMode(L2, OUTPUT);pinMode(L1, OUTPUT);
  pinMode(D1, INPUT);pinMode(D2, INPUT);pinMode(D3, INPUT);pinMode(D4, INPUT);
  Serial.begin(9600);
  Serial.println("Start");
}

void loop() 
{
  b0 = digitalRead(D1);b1 = digitalRead(D2);b2 = digitalRead(D3);b3 = digitalRead(D4);
  Serial.print("Bits: ");Serial.print(b3);Serial.print(b2);Serial.print(b1);Serial.print(b0);Serial.print(" Armed: ");Serial.print(armed);
  Serial.print(" Count2ArmdDif: ");Serial.print(millis()-count2unArm);Serial.print(" unArm: ");Serial.print(unArm);Serial.print(" AntiReb2: ");Serial.println(millis() - countantiReb2);
  if (armed==true)
  {
    digitalWrite(L10,LOW);digitalWrite(L5,HIGH);digitalWrite(L4,LOW);digitalWrite(L3,LOW);digitalWrite(L1,LOW);
    if(millis()-blinkingArmed>=500){digitalWrite(L2, !digitalRead(L2));blinkingArmed=millis();}
    // Cancelacion de armado - INICIO
    if(b0 && !b1 && !b2 && !b3)
    {
      digitalWrite(L4,HIGH);digitalWrite(L3,HIGH);
      antiReb2=false;
      if(!unArm){count2unArm=millis();unArm=true;}
         //if(!b0 && !b1 && !b2 && !b3){if(millis()-count2unArm>=200){unArm=false;digitalWrite(L5,LOW);digitalWrite(L2,HIGH);digitalWrite(L3,HIGH);digitalWrite(L4,HIGH);Serial.println("Aquí XD");}}
      else if(millis()-count2unArm>=2000)
      { armed=false; 
        unArm=false;       
        Serial.println("Unarmed");
        digitalWrite(L5,LOW);
        for (int i = 0; i < 4; i++){digitalWrite(L2,HIGH);digitalWrite(L3,HIGH);digitalWrite(L4,HIGH);delay(1000/4);
            digitalWrite(L2,LOW);digitalWrite(L3,LOW);digitalWrite(L4,LOW);delay(1000/4);}
        antiReb2=false;    
      }
    }
    else {if (!antiReb2 && unArm) {countantiReb2 = millis();antiReb2 = true;}else if (millis() - countantiReb2 >= 300 && unArm){unArm = false;antiReb2 = false;}
          if (unArm){digitalWrite(L4,HIGH);digitalWrite(L3,HIGH);}} // Antirebote 
    // Cancelacion de armado - FIN

    // Ignición - INICIO
    if(!b0 && !b1 && !b2 && b3)
    {
      digitalWrite(L10,LOW);digitalWrite(L5,LOW);digitalWrite(L4,LOW);digitalWrite(L3,LOW);digitalWrite(L2,LOW);digitalWrite(L1,LOW);
      Serial.println("Count");
      ignition=true;
      while(ignition==true)
      {
        b0 = digitalRead(D1);b1 = digitalRead(D2);b2 = digitalRead(D3);b3 = digitalRead(D4);
        delay(1);
        // Cancelacion de ignición - INICIO
        if(!b0 && !b1 && !b2 && !b3)
        {
          antiReb1=false;
          if(!unIGN){count2cancel=millis();unIGN=true;}
            //if(!b0 && !b1 && !b2 && !b3){if(millis()-count2unArm>=200){unArm=false;digitalWrite(L5,LOW);digitalWrite(L2,HIGH);digitalWrite(L3,HIGH);digitalWrite(L4,HIGH);Serial.println("Aquí XD");}}
          else if(millis()-count2cancel>=300)
          { ignition=false;
            digitalWrite(PinOut,LOW); 
            unIGN=false;       
            Serial.println("Deactivated");
            digitalWrite(L5,LOW);
            for (int i = 0; i < 4; i++){digitalWrite(L10,LOW);digitalWrite(L5,LOW);digitalWrite(L4,LOW);digitalWrite(L3,LOW);digitalWrite(L2,LOW);digitalWrite(L1,HIGH);delay(1000/4);
                digitalWrite(L10,LOW);digitalWrite(L5,LOW);digitalWrite(L4,LOW);digitalWrite(L3,LOW);digitalWrite(L2,LOW);digitalWrite(L1,LOW);delay(1000/4);}
            antiReb1=false;
            armed=false;   
          }
        }
        else {if (!antiReb1 && unIGN) {countantiReb1 = millis();antiReb1 = true;}else if (millis() - countantiReb1 >= 200 && unIGN){unArm = false;antiReb1 = false;}
              if (unArm){digitalWrite(L4,HIGH);digitalWrite(L3,HIGH);}} // Antirebote 
        // Cancelacion de ignición - FIN
        if(countIGN==1){Serial.println("10 seg");digitalWrite(L10,HIGH);}
        if(countIGN==500){digitalWrite(L10,LOW);}
        if(countIGN==1000){Serial.println("9 seg");digitalWrite(L10,HIGH);}
        if(countIGN==1500){digitalWrite(L10,LOW);}
        if(countIGN==2000){Serial.println("8 seg");digitalWrite(L10,HIGH);}
        if(countIGN==2500){digitalWrite(L10,LOW);}
        if(countIGN==3000){Serial.println("7 seg");digitalWrite(L10,HIGH);}
        if(countIGN==3500){digitalWrite(L10,LOW);}
        if(countIGN==4000){Serial.println("6 seg");digitalWrite(L10,HIGH);}
        if(countIGN==5000){Serial.println("5 seg");digitalWrite(L5,HIGH);}
        if(countIGN==6000){Serial.println("4 seg");digitalWrite(L4,HIGH);}
        if(countIGN==7000){Serial.println("3 seg");digitalWrite(L3,HIGH);}
        if(countIGN==8000){Serial.println("2 seg");digitalWrite(L2,HIGH);}
        if(countIGN==9000){Serial.println("1 seg");digitalWrite(L1,HIGH);}
        if(countIGN==10000)
        {
          Serial.print("---IGNITION---");
          digitalWrite(PinOut, HIGH);
          for(int i=0; i<10;i++)
          {
            digitalWrite(L10,HIGH);digitalWrite(L5,HIGH);digitalWrite(L4,HIGH);digitalWrite(L3,HIGH);digitalWrite(L2,HIGH);digitalWrite(L1,HIGH);
            delay(500);
            digitalWrite(L10,LOW);digitalWrite(L5,LOW);digitalWrite(L4,LOW);digitalWrite(L3,LOW);digitalWrite(L2,LOW);digitalWrite(L1,LOW);
            delay(500);
          }
          ignition=false;
          countIGN=0;
          armed=false;
          digitalWrite(PinOut, LOW);
        }
        countIGN++;
      }
    }
    // Ignición - FIN
  }
  else
  {
    armed=false;
    ignition=false;
    unArm=false;
    unIGN=false;
    countIGN=0;
    digitalWrite(PinOut, LOW);
    if(b0 && !b1 && !b2 && !b3){armed=true;digitalWrite(L10,LOW);digitalWrite(L5,HIGH);digitalWrite(L4,LOW);digitalWrite(L3,LOW);digitalWrite(L2,LOW);digitalWrite(L1,LOW);
      antiReb2=millis();Serial.println("Armed");}
    else{digitalWrite(L10,LOW);digitalWrite(L5,LOW);digitalWrite(L4,LOW);digitalWrite(L3,LOW);digitalWrite(L2,LOW);digitalWrite(L1,HIGH);}
  }
}
