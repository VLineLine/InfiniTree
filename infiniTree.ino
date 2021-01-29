#include <WS2812FX.h>


#define LED_COUNT 62
#define LED_PIN 2
#define SW_R_PIN 3
#define SW_L_PIN 4
#define GND_OUT_PIN 6
#define NR_OF_MODES 18
#define TIMER_MS 10000

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
WS2812FX ws2812fx = WS2812FX(LED_COUNT, LED_PIN, NEO_RGB + NEO_KHZ800);

unsigned long last_change = 0;
unsigned long now = 0;
int brghtns = 255;
int modeInc = 0;
int modeIndex = 0;

//Best fit modes for lamp
//OLD:
//int list[12] = {44, 12, 14, 16, 17, 20, 21, 23, 26, 19, 38, 55};
//int list[21] = {12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 31, 32, 33, 37, 41, 44, 45, 54, 55 };

int list[NR_OF_MODES] =      {44,   44,  12,   14,   14,   17,  16,   38,   31,   32,   33,   20,   21,   22,  23,  19,  45,   55};
int Speedlist[NR_OF_MODES] = {1000, 200, 3000, 4000, 700,  150, 4000, 1300, 1000, 1300, 1300, 4000, 300,  300, 400, 400, 1000, 500};
int mode;
int speed;
int Tick = 0;
int BtnPushedTime = 0;

void setup() {
  pinMode(SW_R_PIN, INPUT_PULLUP);
  pinMode(SW_L_PIN, INPUT_PULLUP);
  pinMode(GND_OUT_PIN, OUTPUT);
  digitalWrite(GND_OUT_PIN, LOW);
  Serial.begin(9600);
  ws2812fx.init();
  ws2812fx.setBrightness(255);
  ws2812fx.setSpeed(4000);
  ws2812fx.setColor(0xFFFFFF);
  ws2812fx.setMode(list[2]);
  ws2812fx.start();

}


void loop() {
  ws2812fx.service();

  
  if (digitalRead(SW_R_PIN) == 0) {
    //BtnPushedTime = millis();
    //Start timer on first hold
    if (Tick == 0) {
      BtnPushedTime = millis();
    }

    Tick = 1;
    Serial.print("Right btn pushed");
    if ((millis() - BtnPushedTime) > 1000) {
      //ws2812fx.stop();
      brghtns = brghtns - (1 + int(brghtns/10));      
      if (brghtns  <= 0) {
        brghtns = 0;
      }
      ws2812fx.setBrightness(brghtns);
     // Tick = 0;
    }
  }

  if (digitalRead(SW_L_PIN) == 0) {
    //BtnPushedTime = millis();

    //Start timer on first hold
    if (Tick == 0) {
      BtnPushedTime = millis();
    }
    Tick = -1;
    Serial.print("Left btn pushed");

    if ((millis() - BtnPushedTime) > 1000) {
      brghtns = brghtns + 1 + int(brghtns/10);
      if (brghtns >= 255) {
        brghtns = 255;
      }
      ws2812fx.setBrightness(brghtns);
      //ws2812fx.start();
     // Tick = 0;
    }


  }

  Serial.println(Tick != 0);
  Serial.println(digitalRead(SW_R_PIN) == 0);
  Serial.println(digitalRead(SW_L_PIN) == 0);
  if (Tick != 0 && digitalRead(SW_R_PIN) == 1 && digitalRead(SW_L_PIN) == 1) {
    Serial.print("Changing mod : ");
    //Change mode

    modeIndex += Tick;
    if (modeIndex < 0) {
      modeIndex = 0;
    }

    int mode = list [modeIndex % NR_OF_MODES];
    ws2812fx.setMode(mode);

    ws2812fx.setSpeed(speed);
    Serial.print("Modechanged to : ");
    Serial.println(ws2812fx.getMode());
    Tick = 0;
  }


  delay(20);
}
