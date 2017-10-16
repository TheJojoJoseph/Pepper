#include <SPI.h>
#include "SH1106.h"
#include "SH1106Ui.h"


/* Hardware Wemos D1 mini SPI pins
 D5 GPIO14   CLK         - D0 pin OLED display
 D6 GPIO12   MISO (DIN)  - not connected
 D7 GPIO13   MOSI (DOUT) - D1 pin OLED display
 D1 GPIO5    RST         - RST pin OLED display
 D2 GPIO4    DC          - DC pin OLED
 D8 GPIO15   CS / SS     - CS pin OLED display
*/

// Pin definitions for SPI

// Node MCU
#define OLED_RESET  D1   // RESET
#define OLED_DC     D2   // Data/Command
#define OLED_CS     D8   // Chip select

SH1106 display(true, OLED_RESET, OLED_DC, OLED_CS); // FOR SPI

SH1106Ui ui     ( &display );

bool msOverlay(SH1106 *display, SH1106UiState* state) {
  display->setTextAlignment(TEXT_ALIGN_RIGHT);
  display->setFont(ArialMT_Plain_10);
  display->drawString(128, 0,"Pepper 1.0");
  return true;
}


bool drawFrame1(SH1106 *display, SH1106UiState* state, int x, int y) {
  // Demonstrates the 3 included default sizes. The fonts come from SH1106Fonts.h file
  // Besides the default fonts there will be a program to convert TrueType fonts into this format
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->setFont(ArialMT_Plain_10);
  display->drawString(0 + x, 10 + y, "www.rajagiritech.ac.in");

  display->setFont(ArialMT_Plain_24);
  display->drawString(0 + x, 20 + y, "Pepper 1.0");
 
  display->setFont(ArialMT_Plain_10);
  display->drawString(0 + x, 44 + y, "Smart Energy Monitor");


  return false;
}

bool drawFrame2(SH1106 *display, SH1106UiState* state, int x, int y) {
  // Text alignment demo
  display->setFont(ArialMT_Plain_10);

  // The coordinates define the left starting point of the text
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->drawString(0 + x, 11 + y, "Jojo Joseph Kunnathucherry");

  // The coordinates define the center of the text
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->drawString(0 + x, 22, "Solamen Paul Alexander");

  // The coordinates define the right end of the text
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->drawString(0 + x, 33, "Harikrishan N P");

   // The coordinates define the right end of the text
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->drawString(0 + x, 44, "Vyshakh M V");
  return false;
}

bool drawFrame3(SH1106 *display, SH1106UiState* state, int x, int y) {
  // Demo for drawStringMaxWidth:
  // with the third parameter you can define the width after which words will be wrapped.
  // Currently only spaces and "-" are allowed for wrapping
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->setFont(ArialMT_Plain_10);
  display->drawStringMaxWidth(0 + x, 10 + y, 128, "Pepper is a smart energy monitoring device that's capable of accurately monitor your energy usage.");
  return false;
}
bool drawFrame4(SH1106 *display, SH1106UiState* state, int x, int y) {
  // Demo for drawStringMaxWidth:
  // with the third parameter you can define the width after which words will be wrapped.
  // Currently only spaces and "-" are allowed for wrapping
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->setFont(ArialMT_Plain_10);
  display->drawStringMaxWidth(0 + x, 10 + y, 128, "It even predict monthly usage and timely remind you to conserve energy when ever possible. ");
  return false;
}
bool drawFrame5(SH1106 *display, SH1106UiState* state, int x, int y) {
  // Demo for drawStringMaxWidth:
  // with the third parameter you can define the width after which words will be wrapped.
  // Currently only spaces and "-" are allowed for wrapping
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->setFont(ArialMT_Plain_10);
  display->drawStringMaxWidth(0 + x, 10 + y, 128, "https://github.com/TheJojoJoseph/Pepper");
  return false;
}
// how many frames are there?
int frameCount = 5;
// this array keeps function pointers to all frames
// frames are the single views that slide from right to left
bool (*frames[])(SH1106 *display, SH1106UiState* state, int x, int y) = { drawFrame1, drawFrame2, drawFrame3, drawFrame4 ,drawFrame5};

bool (*overlays[])(SH1106 *display, SH1106UiState* state)             = { msOverlay };
int overlaysCount = 1;

void setup() {
 
  Serial.begin(115200);
  Serial.println();
  Serial.println();


  ui.setTargetFPS(30);

//  ui.setActiveSymbole(activeSymbole);
//  ui.setInactiveSymbole(inactiveSymbole);

  // You can change this to
  // TOP, LEFT, BOTTOM, RIGHT
  ui.setIndicatorPosition(BOTTOM);

  // Defines where the first frame is located in the bar.
  ui.setIndicatorDirection(LEFT_RIGHT);

  // You can change the transition that is used
  // SLIDE_LEFT, SLIDE_RIGHT, SLIDE_TOP, SLIDE_DOWN
  ui.setFrameAnimation(SLIDE_LEFT);

  // Add frames
  ui.setFrames(frames, frameCount);

  // Add overlays
  ui.setOverlays(overlays, overlaysCount);

  // Inital UI takes care of initalising the display too.
  ui.init();

  display.flipScreenVertically();

}

void loop() {
  int remainingTimeBudget = ui.update();

  if (remainingTimeBudget > 0) {
   
    // You can do some work here
    // Don't do stuff if you are below your
    // time budget.
    delay(remainingTimeBudget);
     
    }

 

}
