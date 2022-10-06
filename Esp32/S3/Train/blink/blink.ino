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

double setColorCond(int color, double val) 
{
    return color < 0 ? abs(val) : -abs(val);
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
            const int *nextColor = COLOR_VECTOR[i + 1];
            double swapColor[] = {nextColor[0] - color[0], nextColor[1] - color[1], nextColor[2] - color[2]};
            int sColorR = abs(swapColor[0]);
            int sColorG = abs(swapColor[1]);
            int sColorB = abs(swapColor[2]);
            // int mColorR = sColorR / 1000;
            // int mColorG = sColorG / 1000;
            // int mColorB = sColorB / 1000;

            for (size_t i = 0; i < 1000 / CHANGE_RATE; i++)
            {
                setLEDColor(RGB_BUILTIN, colorCast(sColorR), colorCast(sColorG), colorCast(sColorB));
                delay(CHANGE_RATE);
            }
            
        }

        delay(CHANGE_RATE);
    }
#endif
}