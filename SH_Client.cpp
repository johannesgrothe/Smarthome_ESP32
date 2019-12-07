#include "SH_Client.h"

bool SH_Client::init()
{
  mqttClient.setServer(mqttServer, 1883);

  using std::placeholders::_1;
  using std::placeholders::_2;
  using std::placeholders::_3;
  mqttClient.setCallback(std::bind( &SH_Client::callback, this, _1,_2,_3));

  setup_wifi();
  setup_mqtt();
  mqttClient.subscribe("homebridge/from/#");
  mqttClient.subscribe("homebridge/to/#");
  mqttClient.subscribe("debug/in");
  mqttClient.publish("debug/out", "SH_Client gestartet.");
  initialized = true;
  return true;
}

void SH_Client::refresh()
{
  while (!initialized)
  {
    init();
  }
  uint8_t k;
  for (k = 0; k < gadgets_pointer; k++)
  {
    // Serial.println(gadgets[k].getName());
  }
  mqttClient.loop();
}

void SH_Client::setup_wifi()
{
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WIFI_SSID);

  WiFi.begin(WIFI_SSID, WIFI_PW);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(2500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void SH_Client::setup_mqtt()
{
  while (!mqttClient.connected())
  {
    Serial.print("Attempting MQTT connection...");
    if (mqttClient.connect("arduinoClient")) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

bool SH_Client::addGadget(SH_Gadget * gadget)
{
  Serial.printf("Adding Gadget '%s':\n", gadget->getName());
  if (gadgets_pointer == MAX_GADGETS) return false;
  gadgets[gadgets_pointer] = gadget;
  gadgets_pointer ++;
  char buffer[200];
  gadget->getRegisterStr(&buffer[0]);
  Serial.println("  Unregistering previous Gadget...");
  unregisterGadget(gadget);
  Serial.println("  Registering new Gadget...");
  registerGadget(gadget);
  Serial.println("  Done.");
  return true;
}

bool SH_Client::unregisterGadget(SH_Gadget * input)
{
  char bufmsg[100];
  sprintf(bufmsg, "{\"name\": \"%s\"}", input->getName());
  mqttClient.publish("homebridge/to/remove", bufmsg);
}

bool SH_Client::registerGadget(SH_Gadget * input)
{
  char bufmsg[200];
  if (input->getRegisterStr(&bufmsg[0]))
  {
    mqttClient.publish("homebridge/to/add", bufmsg);
  }
  else
  {
    Serial.println("  Fehler bei der Registrierung.");
    return false;
  }
  return true;
}
