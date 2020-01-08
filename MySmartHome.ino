
#include <ESP8266WiFi.h>

#define LED_GREEN_LIVING_ROOM D0
#define LED_RED_DINING_ROOM D1
#define LED_YELLOW_BATH_ROOM D2
#define LED_RED_EXIT D3

#define delayLed 150

int LED_GREEN_LIVING_ROOM_VALUE = LOW;
int LED_RED_DINING_ROOM_VALUE = LOW;
int LED_YELLOW_BATH_ROOM_VALUE = LOW;
int LED_RED_EXIT_VALUE = LOW;
int PARTY_MODE_VALUE = LOW;

String LIVING_ROOM_LED = "LIVING_ROOM_LED";
String DINING_ROOM_LED = "DINING_ROOM_LED";
String BATHROOM_LED = "BATHROOM_LED";
String EXIT_LED = "EXIT_LED";
String PARTY_MODE = "PARTY_MODE";

String ON = "ON";
String OFF = "OFF";

const char* ssid = "****YOUR_WIFI_SSID*****";
const char* password = "*****YOUR_WIFI_PASSWORD*****";

WiFiServer server(80);

void setup()
{
	Serial.begin(115200);
	connectToWiFi();
	setPinModes();
}

void loop()
{
	if (PARTY_MODE_VALUE == HIGH)
		letsParty();

	WiFiClient client = server.available();

	if (!client)
		return;

	if (client.available())
	{
		String request = client.readStringUntil('\r');
		Serial.println(request);
		client.flush();

		processRequest(request);
		sendWebSiteHtml(client);
	}

	delay(2);
}

void connectToWiFi()
{
	Serial.print("Connecting to ");
	Serial.println(ssid);

	WiFi.begin(ssid, password);

	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		Serial.print(".");
	}
	Serial.println("");
	Serial.println("WiFi connected");

	server.begin();

	Serial.println("Server started");
	Serial.print("Use this URL to connect: ");
	Serial.print("http://");
	Serial.print(WiFi.localIP());
}

void sendWebSiteHtml(WiFiClient client)
{
	client.println("HTTP/1.1 200 OK");
	client.println("Content-Type: text/html");
	client.println("");
	client.println("<!DOCTYPE HTML>");
	client.println("<html>");

	client.print("Green - Living Room is now: ");
	if (LED_GREEN_LIVING_ROOM_VALUE == LOW)
		client.print("Off");
	else
		client.print("On");

	client.print("<br>Red - Dining Room is now: ");
	if (LED_GREEN_LIVING_ROOM_VALUE == LOW)
		client.print("Off");
	else
		client.print("On");

	client.print("<br>Yellow - Bath Room is now: ");
	if (LED_GREEN_LIVING_ROOM_VALUE == LOW)
		client.print("Off");
	else
		client.print("On");

	client.print("<br>Red - Exit is now: ");
	if (LED_GREEN_LIVING_ROOM_VALUE == LOW)
		client.print("Off");
	else
		client.print("On");

	client.print("<br>Party Mode is now: ");
	if (PARTY_MODE_VALUE == LOW)
		client.print("Off");
	else
		client.print("On");

	client.println("<br><br>");
	client.println("<a href=\"/LIVING_ROOM_LED=ON\"\"><button>Green - Living Room Turn On </button></a>");
	client.println("<a href=\"/LIVING_ROOM_LED=OFF\"\"><button>Green - Living Room Turn Off </button></a><br />");
	client.println("<br><br>");
	client.println("<a href=\"/DINING_ROOM_LED=ON\"\"><button>Red - Dining Room Turn On </button></a>");
	client.println("<a href=\"/DINING_ROOM_LED=OFF\"\"><button>Red - Dining Room Turn Off </button></a><br />");
	client.println("<br><br>");
	client.println("<a href=\"/BATHROOM_LED=ON\"\"><button>Yellow - Bathroom Turn On </button></a>");
	client.println("<a href=\"/BATHROOM_LED=OFF\"\"><button>Yellow - Bathroom Turn Off </button></a><br />");
	client.println("<br><br>");
	client.println("<a href=\"/EXIT_LED=ON\"\"><button>Red - Exit Turn On </button></a>");
	client.println("<a href=\"/EXIT_LED=OFF\"\"><button>Red - Exit Turn Off </button></a><br />");
	client.println("<br><br>");
	client.println("<a href=\"/PARTY_MODE=ON\"\"><button>Party Mode Turn On </button></a>");
	client.println("<a href=\"/PARTY_MODE=OFF\"\"><button>Party Mode Turn Off </button></a>");
	client.println("</html>");

	delay(1);
	Serial.println("Client disonnected");
}

void processRequest(String command)
{
	//LIVING ROOM
	if (command.indexOf(LIVING_ROOM_LED + ON) != -1)
	{
		turnOnLed(LED_GREEN_LIVING_ROOM);
		LED_GREEN_LIVING_ROOM_VALUE = HIGH;
	}
	else if (command.indexOf(LIVING_ROOM_LED + OFF) != -1)
	{
		turnOnLed(LED_GREEN_LIVING_ROOM);
		LED_GREEN_LIVING_ROOM_VALUE = LOW;
	}
	//DINING ROOM
	else if (command.indexOf(DINING_ROOM_LED + ON) != -1)
	{
		turnOnLed(LED_RED_DINING_ROOM);
		LED_RED_DINING_ROOM_VALUE = HIGH;
	}
	else if (command.indexOf(DINING_ROOM_LED + OFF) != -1)
	{
		turnOnLed(LED_GREEN_LIVING_ROOM);
		LED_RED_DINING_ROOM_VALUE = LOW;
	}
	//BATHROOM
	else if (command.indexOf(BATHROOM_LED + ON) != -1)
	{
		turnOnLed(LED_GREEN_LIVING_ROOM);
		LED_YELLOW_BATH_ROOM_VALUE = HIGH;
	}
	else if (command.indexOf(BATHROOM_LED + OFF) != -1)
	{
		turnOnLed(LED_GREEN_LIVING_ROOM);
		LED_YELLOW_BATH_ROOM_VALUE = LOW;
	}
	//EXIT
	else if (command.indexOf(EXIT_LED + ON) != -1)
	{
		turnOnLed(LED_RED_EXIT);
		LED_RED_EXIT_VALUE = HIGH;
	}
	else if (command.indexOf(EXIT_LED + OFF) != -1)
	{
		turnOnLed(LED_RED_EXIT);
		LED_RED_EXIT_VALUE = HIGH;
	}
	//PARTY MODE
	else if (command.indexOf(PARTY_MODE + ON) != -1)
	{
		PARTY_MODE_VALUE = HIGH;
	}
	else if (command.indexOf(PARTY_MODE + OFF) != -1)
	{
		PARTY_MODE_VALUE = LOW;
	}
}

void turnOnLed(int pin)
{
	digitalWrite(pin, HIGH);
}

void turnOffLed(int pin)
{
	digitalWrite(pin, LOW);
}

void setPinModes()
{
	pinMode(LED_GREEN_LIVING_ROOM, OUTPUT);
	pinMode(LED_RED_DINING_ROOM, OUTPUT);
	pinMode(LED_YELLOW_BATH_ROOM, OUTPUT);
	pinMode(LED_RED_EXIT, OUTPUT);
}

void letsParty()
{
	turnOffAll();

	digitalWrite(LED_GREEN_LIVING_ROOM, HIGH);
	delay(delayLed);
	digitalWrite(LED_RED_DINING_ROOM, HIGH);
	delay(delayLed);
	digitalWrite(LED_YELLOW_BATH_ROOM, HIGH);
	delay(delayLed);
	digitalWrite(LED_RED_EXIT, HIGH);

	delay(delayLed);
	turnOffAll();
	delay(delayLed);

	digitalWrite(LED_RED_EXIT, HIGH);
	delay(delayLed);
	digitalWrite(LED_YELLOW_BATH_ROOM, HIGH);
	delay(delayLed);
	digitalWrite(LED_RED_DINING_ROOM, HIGH);
	delay(delayLed);
	digitalWrite(LED_GREEN_LIVING_ROOM, HIGH);

	delay(delayLed);
	turnOffAll();
	delay(delayLed);
}

void turnOffAll()
{
	digitalWrite(LED_GREEN_LIVING_ROOM, LOW);
	digitalWrite(LED_RED_DINING_ROOM, LOW);
	digitalWrite(LED_YELLOW_BATH_ROOM, LOW);
	digitalWrite(LED_RED_EXIT, LOW);
}

