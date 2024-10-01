
//DAMOS DEFINICIONES INICIALEs

int AA1 = 3;
int AA2 = 5;
int BB1 = 11;
int BB2 = 10;
int CC1 = 9;
int CC2 = 6;


int emfA = A0;
int emfB = A1;
int emfC = A2;

int fase = 1;
int sum = 0;|
int t;
int PotIn = A3;
int Delay = 4000;


static int delta = 0;
static int Lastdelta = -1;

unsigned long previousMillis = 0;

void setup() {
  //Serial.begin(250000);
  pinMode(AA1, OUTPUT);
  pinMode(AA2, OUTPUT);
  pinMode(BB1, OUTPUT);
  pinMode(BB2, OUTPUT);
  pinMode(CC1, OUTPUT);
  pinMode(CC2, OUTPUT);
  pinMode(PotIn, INPUT);
  pinMode(emfA, INPUT);
  pinMode(emfB, INPUT);
  pinMode(emfC, INPUT);

  //digitalWrite(enable,HIGH);
  //previousMillis = micros(); //ojo este codigo estaba OCULTO

}

//con el analog read leemmos las 3 entradas  de back emf. la suma y div en 3 da el cero virtual

void loop() {

  //Serial.println(t);
  int emA = analogRead(emfA);
  int emB = analogRead(emfB);
  int emC = analogRead(emfC);
  int sum = (emA + emB + emC) / 3;

  unsigned long currentMillis = micros();


  if (currentMillis - previousMillis >= Delay) {

    previousMillis += Delay;

    //Phase1 C-B,  AA1 EL A DE ARRIBA, AA2 EL A DE ABAJO. ACTIVAMOS EN CADA CASO LOS QUE NECESITAMOS PARA ENTRADA Y SALIDA
    switch (fase) {
      case 1:
        digitalWrite(AA1, LOW);
        digitalWrite(AA2, LOW);
        digitalWrite(BB1, LOW);
        digitalWrite(CC2, LOW);
        digitalWrite(BB2, HIGH);
        digitalWrite(CC1, HIGH);
        delta = emA - sum; //FLOTANTE MENOS COMUN, CERO O UNO. LO mismo q se hace con el comparador en v3. En cero es cero crossing de negativos a positivos o viceversa

        break;


      //Phase2 A-B
      case 2:
        digitalWrite(AA2, LOW);
        digitalWrite(BB1, LOW);
        digitalWrite(CC1, LOW);
        digitalWrite(CC2, LOW);
        digitalWrite(AA1, HIGH);
        digitalWrite(BB2, HIGH);
        delta = emC - sum;
        break;

      //Phase3 A-C
      case 3:
        digitalWrite(AA2, LOW);
        digitalWrite(BB1, LOW);
        digitalWrite(BB2, LOW);
        digitalWrite(CC1, LOW);
        digitalWrite(CC2, HIGH);
        digitalWrite(AA1, HIGH);
        delta = emB - sum;
        break;

      //Phase4 B-C
      case 4:
        digitalWrite(AA1, LOW);
        digitalWrite(AA2, LOW);
        digitalWrite(BB2, LOW);
        digitalWrite(CC1, LOW);
        digitalWrite(BB1, HIGH);
        digitalWrite(CC2, HIGH);
        delta = emA - sum;
        break;

      //Phase5 B-A
      case 5:
        digitalWrite(AA1, LOW);
        digitalWrite(BB2, LOW);
        digitalWrite(CC1, LOW);
        digitalWrite(CC2, LOW);
        digitalWrite(AA2, HIGH);
        digitalWrite(BB1, HIGH);
        delta = emC - sum;
        break;

      //Phase6 C-A
      case 6:
        digitalWrite(AA1, LOW);
        digitalWrite(BB1, LOW);
        digitalWrite(BB2, LOW);
        digitalWrite(CC2, LOW);
        digitalWrite(CC1, HIGH);
        digitalWrite(AA2, HIGH);
        delta = emB - sum;
        break;
    }

    if (Lastdelta < 0) {
      if (delta > 0)
      {
        Lastdelta = delta; //save the last delta
        fase = fase + 1; //pass to the next fase
        if (fase > 6) {
          fase = 1; //return to fase 1
        }
      }
    }//Zero cross from - to + . look the ><0 signs in lastdelta and delta

    if (Lastdelta > 0) {
      if (delta < 0)
      {
        Lastdelta = delta;
        fase = fase + 1;
        if (fase > 6) {
          fase = 1;
        }
      }
    }//Zero cross from + to -  . IF FASE > 6 THEN RETURN TO FASE 1

  }//Case ends

  //ojo q falta un ciclo para alimentarlo aun cuando no haya back emf, osea inicializacion

  t = analogRead(PotIn); //From the potentiometer
   //t1 = t*20-1023  //traslacion de 250-500mv a 0-5V, 250mv en terminos de porcentajes de 1023. para simplemente usar el rms del pwm.
  //Serial.println(t1) y hacemos el delay map con t1, OJO Q EN REALIDAD SI NO NOS PONEMOS A JODER CON LO DE 1-2MS TENEMOS MUCHA MAS LIBERTAD PARA AJUSTARNOS A LA SALIDA
  Delay = map(t, 0, 1023, 5, 1000); //we obtain the delay speed using the potentiometer, getting between 5 and 1000 uS in the swiching fases time
  //we map the values from 1 to 1000 us, THAT IS TO 0.001 to 1 ms so starting in 1ms min and
  //2ms max we control the full range of speed

  //el control que hicimos en la ultima version se hace con registros y puertos y no con
  //digital write pues es mucho mas rapido. 20-30 veces mas rapido.

} //loop ends at the end of EVERYTHING
