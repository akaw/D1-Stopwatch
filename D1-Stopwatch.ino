#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Button2.h>
#include <Chrono.h>

//Pins
#define BUTTON_STOP_PIN D5
#define BUTTON_RESUME_PIN D6
#define BUTTON_START_PIN D7

//Chrono lib
Chrono chrono(Chrono::SECONDS);

//Display
#define OLED_RESET 0
Adafruit_SSD1306 display(OLED_RESET); // 64x48 Pixel

//Buttons
Button2 buttonStart = Button2(BUTTON_START_PIN);
Button2 buttonStop = Button2(BUTTON_STOP_PIN);

String timeStr = String("00:00");
int seconds = 0, hours = 0, minutes = 0;

void setup()
{
    Serial.begin(115200);
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    display.display();
    delay(2000);
    display.clearDisplay();
    display.setTextColor(WHITE);

    buttonStart.setClickHandler(click);
    buttonStop.setClickHandler(click);

    Serial.println("");
    Serial.println("Ready!");
    chrono.stop();
}

void loop()
{
    buttonStart.loop();
    buttonStop.loop();
    wDisplay();
}

void drawCentreString(String buf, int y, int x = 32)
{
    int16_t x1, y1;
    uint16_t w, h;
    display.setTextWrap(false);
    display.getTextBounds(buf, x, y, &x1, &y1, &w, &h); //calc width of new string
    display.setCursor(x - w / 2, y);
    display.print(buf);
}

void wDisplay()
{
    if (chrono.isRunning())
    {
        seconds = chrono.elapsed();
        hours = seconds / 60 / 60;
        minutes = seconds / 60;
        minutes = minutes % 60;
        seconds = seconds % 60;
        if (hours < 10)
        {
            timeStr = "0";
            timeStr.concat(hours);
        }
        else
        {
            timeStr = "";
            timeStr.concat(hours);
        }
        timeStr.concat(":");
        if (minutes < 10)
        {
            timeStr.concat("0");
            timeStr.concat(minutes);
        }
        else
        {
            timeStr.concat(minutes);
        }
    }
    display.clearDisplay();
    display.setTextColor(WHITE);
    display.setTextSize(2);
    drawCentreString(timeStr, 10);
    display.setTextSize(1);
    drawCentreString(String(seconds), 32);
    display.display();
}

void click(Button2 &btn)
{
    if (btn == buttonStart)
    {
        chrono.restart();
    }
    if (btn == buttonStop)
    {
        if (chrono.isRunning())
        {
            chrono.stop();
        }
        else
        {
            chrono.resume();
        }
    }
}
