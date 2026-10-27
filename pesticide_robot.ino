#define Left_M1 9
#define Left_M2 10
#define Right_M1 11
#define Right_M2 12

#define Relay_Grass A0
#define Relay_Spray A2

char cmd;

bool grassState = false;
bool sprayState = false;

unsigned long lastMoveTime = 0;
unsigned long lastMovementCommandTime = 0;

const unsigned long moveTimeout = 200;
const unsigned long relayBlockTime = 500;   // movement ke baad relay block

void setup()
{
  Serial.begin(9600);

  pinMode(Relay_Grass, OUTPUT);
  pinMode(Relay_Spray, OUTPUT);

  digitalWrite(Relay_Grass, HIGH);
  digitalWrite(Relay_Spray, HIGH);

  pinMode(Left_M1, OUTPUT);
  pinMode(Left_M2, OUTPUT);
  pinMode(Right_M1, OUTPUT);
  pinMode(Right_M2, OUTPUT);

  Stop_chair();

  Serial.println("System Ready");
}

void loop()
{
  if (Serial.available())
  {
    cmd = Serial.read();

    if (cmd == '\n' || cmd == '\r') return;

    Serial.print("Received: ");
    Serial.println(cmd);

    // ===== MOVEMENT =====
    if (cmd == 'f' || cmd == 'd' || cmd == 'l' || cmd == 'r')
    {
      lastMovementCommandTime = millis();
      lastMoveTime = millis();

      if (cmd == 'f') {
        digitalWrite(Left_M1, HIGH);
        digitalWrite(Left_M2, LOW);
        digitalWrite(Right_M1, HIGH);
        digitalWrite(Right_M2, LOW);
      }
      else if (cmd == 'd') {
        digitalWrite(Left_M1, LOW);
        digitalWrite(Left_M2, HIGH);
        digitalWrite(Right_M1, LOW);
        digitalWrite(Right_M2, HIGH);
      }
      else if (cmd == 'l') {
        digitalWrite(Left_M1, LOW);
        digitalWrite(Left_M2, HIGH);
        digitalWrite(Right_M1, HIGH);
        digitalWrite(Right_M2, LOW);
      }
      else if (cmd == 'r') {
        digitalWrite(Left_M1, HIGH);
        digitalWrite(Left_M2, LOW);
        digitalWrite(Right_M1, LOW);
        digitalWrite(Right_M2, HIGH);
      }
    }

    // ===== GRASS RELAY =====
    else if (cmd == 'a')
    {
      if (millis() - lastMovementCommandTime > relayBlockTime)
      {
        grassState = !grassState;
        digitalWrite(Relay_Grass, grassState ? LOW : HIGH);
        Serial.println(grassState ? "Grass ON" : "Grass OFF");
      }
      else
      {
        Serial.println("Relay Blocked (movement)");
      }
    }

    // ===== SPRAY RELAY =====
    else if (cmd == 's')
    {
      if (millis() - lastMovementCommandTime > relayBlockTime)
      {
        sprayState = !sprayState;
        digitalWrite(Relay_Spray, sprayState ? LOW : HIGH);
        Serial.println(sprayState ? "Spray ON" : "Spray OFF");
      }
      else
      {
        Serial.println("Spray Blocked (movement)");
      }
    }
  }

  // ===== AUTO STOP =====
  if (millis() - lastMoveTime > moveTimeout)
  {
    Stop_chair();
  }
}

void Stop_chair()
{
  digitalWrite(Left_M1, LOW);
  digitalWrite(Left_M2, LOW);
  digitalWrite(Right_M1, LOW);
  digitalWrite(Right_M2, LOW);
}