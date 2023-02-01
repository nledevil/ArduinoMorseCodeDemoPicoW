#define IS_AP
#define CW_KEY      13
#define CW_TONE     14

const char *ssid = "CW DEMO"; // The name of the Wi-Fi network that will be created
const char *password = "thereisnospoon";   // The password required to connect to it, leave blank for an open network

const char *id = "1"; // ID of the Unit, 1 is the AP
const char *version = "v1.0.0"; // Version Line

/*-----------------------OTA------------------------*/
const char *OTAName = "CWOTA";
const char *OTAPassword = "CWOTA";

/*------------------ CW Variables ------------------*/
// Debounce Variables
byte lastKeyState = LOW;
unsigned long debounceDuration = 20; // millis
unsigned long lastTimeKeyStateChanged = 0;
// CW Lengths in ms
// under 150 dit, over 150 dah
int holdtime = 150;
// Time Between Characters
int charPadTime = 200;
// time between words
int wordPadTime = 1000;

bool bootupStart = true;
bool buttonPressed = false;
bool buttonHeld = false;
bool debounced = false;

unsigned long pressStart = -1;
unsigned long releaseStart = -1;
unsigned long longReleaseStart = -1;

char json[] = "{\".-\":\"a\",\"-...\":\"b\",\"-.-.\":\"c\",\"-..\":\"d\",\".\":\"e\",\"..-.\":\"f\",\"--.\":\"g\",\"....\":\"h\",\"..\":\"i\",\".---\":\"j\",\"-.-\":\"k\",\".-..\":\"l\",\"--\":\"m\",\"-.\":\"n\",\"---\":\"o\",\".--.\":\"p\",\"--.-\":\"q\",\".-.\":\"r\",\"...\":\"s\",\"-\":\"t\",\"..-\":\"u\",\"...-\":\"v\",\".--\":\"w\",\"-..-\":\"x\",\"-.--\":\"y\",\"--..\":\"z\",\".----\":\"1\",\"..---\":\"2\",\"...--\":\"3\",\"....-\":\"4\",\".....\":\"5\",\"-....\":\"6\",\"--...\":\"7\",\"---..\":\"8\",\"----.\":\"9\",\"-----\":\"0\",\"--..--\":\",\",\".-.-.-\":\".\",\"..--..\":\"?\",\"-.-.-.\":\";\",\"---...\":\":\",\"-..-.\":\"/\",\"-....-\":\"-\",\".----.\":\"'\",\"-.--.\":\"(\",\"-.--.-\":\")\",\"..--.-\":\"_\",\".--.-.\":\"@\",\"-.-.--\":\"!\",\".-...\":\"&\",\"-...-\":\"=\",\".-.-.\":\"+\",\"...-..-\":\"$\",\".-..-.\":\"\\\"\",\"..--\":\" \"}";
String cBuffer = "";
String wBuffer = "";