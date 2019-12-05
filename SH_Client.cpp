#include "SH_Client.h"

static void callback(char* topic, byte* payload, unsigned int length)
{
  Serial.printf("length: %d", length);
  char message[length + 1];
  char p;
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++)
  {
    p = (char)payload[i];
    message[i] = p;
  }
  message[length] = '\0';
  Serial.println(message);

  if ((char)payload[0] == '1')
  {
    Serial.println("LED on");
    // digitalWrite(BUILTIN_LED, HIGH); 
  }
  else
  {
    Serial.println("LED off");
    // digitalWrite(BUILTIN_LED, LOW);
  }
}

bool SH_Client::init()
{
  bool status;
  setup_wifi();
  initialized = true;
}

void SH_Client::refresh()
{
  while (!initialized)
  {
    init();
  }
  testswitch.refresh();
  if (testswitch.hasNewStatus())
  {
    // Serial.println(testswitch.getStatus());
    if (testswitch.getStatus() == 1)
    {
      Serial.println("Switching LED");
      // testled.light_switch();
    }
  }
  testled.refresh();
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

bool addGadget(SH_Gadget * gadget)
{
  uint8_t k;
  for (k = 0; k < MAX_GADGETS; k++)
  {
    if (gadgets[k] == nullptr)
    {
      gadgets[k] = gadget;
      return true;
    }
  }
  return false;
}

bool removeGadget(char * name)
{
  uint8_t k;
  for (k = 0; k < MAX_GADGETS; k++)
  {
    if (gadgets[k] != nullptr && strcmp(gadgets[0].getName(), name))
    {
      return removeGadget(k);
    }
  }
}

bool removeGadget(uint8_t index)
{
  gadgets[index] = nullptr;
}