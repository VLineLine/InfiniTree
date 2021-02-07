#include <WS2812FX.h>
#include <stdint.h>
#define ENCODER_OPTIMIZE_INTERRUPTS
#include <Encoder.h>

#define LED_COUNT 62

#define LED_PIN 2
#define ENC_A_PIN 3
#define ENC_B_PIN 4
#define GND_OUT_PIN 5
#define ENC_BTN_PIN 6

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

Encoder rotary_encoder(ENC_A_PIN,ENC_B_PIN);

long prev_enc, enc_read;

int brghtns = 120;
int modeIndex = 0;
int prev_A;
int btn_mode = 0;
int change = 1;

unsigned long myTime;

int list[NR_OF_MODES] = {44,   44,  12,   14,   14,   17,  16,   38,   31,   32,   33,   20,   21,   22,  23,  19,  45,   55};
uint16_t light_speed = 400;
uint32_t tick = 0;
uint32_t prev_tick =0;
int BtnPushedTime = 0;

void setup() {
  pinMode(ENC_BTN_PIN, INPUT_PULLUP);
  pinMode(GND_OUT_PIN, OUTPUT);

  attachInterrupt(ENC_BTN_PIN, btn_handler, RISING);

  digitalWrite(GND_OUT_PIN, LOW);

  prev_enc = rotary_encoder.read();

  myTime=millis();

  Serial.begin(9600);
  
  ws2812fx.init();
  ws2812fx.setBrightness(255);
  ws2812fx.setSpeed(400);
  ws2812fx.setColor(0xFFFFFF);
  ws2812fx.setMode(list[2]);
  ws2812fx.start();
}

void loop() {
  ws2812fx.service();
  if (tick >= 4294967294){
    tick=0;
  }
  else{
    tick++;
  }

  enc_read = rotary_encoder.read();
  if(enc_read!=prev_enc){
    if(enc_read<prev_enc){
      encoder_handler_up();
    }
    else{
      encoder_handler_down();
    }
    prev_enc=enc_read;
  }
  
  delay(20);
}

void encoder_handler_up(void){
  switch(btn_mode){
  case 0:
    dimUp();
    break;
  case 1:
    changeModeUp();
    break;
  case 2:
    changeSpeedUp();
    break;
  }
} 


void encoder_handler_down(void){
  switch(btn_mode){
  case 0:
    dimDown();
    break;
  case 1:
    changeModeDown();
    break;
  case 2:
    changeSpeedDown();
    break;
  }
}

void btn_handler(void){
  if(tick != prev_tick){
    if(btn_mode < 2){
      btn_mode++;
    }
    else{
      btn_mode = 0;
    }
  }
  prev_tick = tick;
}

void dimUp(){
  brghtns = brghtns + 1 + int(brghtns/10);
  if (brghtns >= 255) {
    brghtns = 255;
  }
  ws2812fx.setBrightness(brghtns);
}

void dimDown(){
  brghtns = brghtns - (1 + int(brghtns/10));      
  if (brghtns  <= 0) {
    brghtns = 0;
  }
  ws2812fx.setBrightness(brghtns);
}

void changeModeUp(){
  modeIndex++;
  if ( modeIndex >= NR_OF_MODES ) modeIndex = 0;
  ws2812fx.setColor(0xFFFFFF);
  ws2812fx.setMode(list [modeIndex]);
}

void changeModeDown(){
  if ( modeIndex == 0 ){
    modeIndex = (NR_OF_MODES-1);
  }
  else{
    modeIndex--;
  }
  ws2812fx.setColor(0xFFFFFF);
  ws2812fx.setMode(list [modeIndex]);
}

void changeSpeedUp(){
    if (light_speed <=100){
    light_speed =0;
  }
  else{
    light_speed = light_speed -100;
  }
  ws2812fx.setSpeed(light_speed);
}

void changeSpeedDown(){
    light_speed = light_speed + 100;
  if (light_speed > 6000){
    light_speed = 6000;
  }
  ws2812fx.setSpeed(light_speed);
}
