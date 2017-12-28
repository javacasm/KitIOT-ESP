
#define LED_PIN D4     // Usaremos el led integrado en la placa conectado a GPIO2/D4

// Patillaje de NodeMCU v3 http://pihome.pl/wp-content/uploads/2016/09/ESP8266-NodeMCU-Pinout.png

void setup() {
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);     // Inicializamos the LED_PIN pin as an output
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(LED_PIN, LOW);   
  Serial.println("Led apagado");
  delay(1000);                      // Wait for a second
  digitalWrite(LED_PIN, HIGH);  // Turn the LED off by making the voltage HIGH
  Serial.println("Led encendido");
  delay(2000);                      // Wait for two seconds (to demonstrate the active low LED)
}
