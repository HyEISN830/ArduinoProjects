// 0.25098 * n(color max 255)
const double CAST_RATE = 0.25098;
// color nodes
const int COLOR_VECTOR[][3] = {
    {255, 0, 0},
    {0, 255, 0},
    {0, 0, 255}};
// color change rate(ms)
const int CHANGE_RATE = 10;

double colorCast(int color)
{
    if (color > 255)
    {
        return 255 * CAST_RATE;
    }
    else if (color <= 0)
    {
        color = 0;
    }

    return color * CAST_RATE;
}

void setLEDColor(uint8_t pin, int red, int green, int blue)
{
    neopixelWrite(pin, colorCast(red), colorCast(green), colorCast(blue));
}

void setup()
{
    // No need to initialize the RGB LED
    Serial.begin(115200);
    digitalWrite(RGB_BUILTIN, LOW); // Turn the RGB LED off
    delay(1000);
}

// the loop function runs over and over again forever
void loop()
{
#ifdef RGB_BUILTIN
    int arrSize = sizeof(COLOR_VECTOR) / sizeof(int);

    for (size_t i = 0; i < arrSize; i++)
    {
        const int *color = COLOR_VECTOR[i];

        if ((i + 1) > arrSize - 1) 
        {
            
        }

        delay(CHANGE_RATE);
    }
#endif
}