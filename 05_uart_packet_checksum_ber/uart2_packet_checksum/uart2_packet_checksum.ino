#define RX2_PIN 16
#define TX2_PIN 17

int counter = 0;

void setup() {
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, RX2_PIN, TX2_PIN);

  Serial.println("UART2 packet lab started");
  Serial.println("Format: <counter,value,checksum>");
}

void loop() {
  int value = counter * 3;
  int checksum = counter + value;

  Serial2.print("<");
  Serial2.print(counter);
  Serial2.print(",");
  Serial2.print(value);
  Serial2.print(",");
  Serial2.print(checksum);
  Serial2.println(">");

  Serial.print("Sent packet: <");
  Serial.print(counter);
  Serial.print(",");
  Serial.print(value);
  Serial.print(",");
  Serial.print(checksum);
  Serial.println(">");

  delay(50);

  while (Serial2.available() > 0) {
    String packet = Serial2.readStringUntil('\n');
    packet.trim();

    Serial.print("Received packet: ");
    Serial.println(packet);
  }

  counter++;
  delay(1000);
}
