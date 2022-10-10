// 0.25098 * n(color max 255), from 64 / 255. LED birthness level 64
const double CAST_RATE = 0.25098;
// Color change interval(ms)
const int MS = 2000;
// color nodes
const int COLOR_NODES[][3] = {
    {255, 0, 0},
    {0, 255, 0},
    {0, 0, 255}};
// color change rate(ms)
const int CHANGE_RATE = 10;
// all colors
int COLOR_VECTOR[(sizeof(COLOR_NODES) / sizeof(COLOR_NODES[0])) * (MS / CHANGE_RATE)][3];

/// @brief
/// @param color
/// @return
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

/// @brief change RGB LED color
/// @param pin LED pin
/// @param red
/// @param green
/// @param blue
void setLEDColor(uint8_t pin, int red, int green, int blue)
{
    neopixelWrite(pin, colorCast(red), colorCast(green), colorCast(blue));
}

/// @brief color addition or subtraction
/// @param color difference from next color code
/// @param freq RGB change frequency
/// @return
double setColorCond(int color, int freq)
{
    double val = abs(color) / ((double)freq);

    return ((double)color) < 0.0 ? -abs(val) : abs(val);
}

void setup()
{
    int arrSize = sizeof(COLOR_NODES) / sizeof(COLOR_NODES[0]);
    int frequency = MS / CHANGE_RATE; // RGB color change frequency

    Serial.begin(115200);           // Com baud rate
    digitalWrite(RGB_BUILTIN, LOW); // Turn the RGB LED off

    for (size_t i = 0; i < arrSize; i++)
    {
        const int *color = COLOR_NODES[i];
        const int *nextColor = nullptr;
        double swapColor[3] = {color[0] + 0.0, color[1] + 0.0, color[2] + 0.0};

        if ((i + 1) < arrSize)
        {
            nextColor = COLOR_NODES[i + 1];
        }
        else if ((i + 1) == arrSize)
        {
            nextColor = COLOR_NODES[0];
        }

        if (nullptr != nextColor)
        {
            double rGapVal = setColorCond(nextColor[0] - color[0], frequency);
            double gGapVal = setColorCond(nextColor[1] - color[1], frequency);
            double bGapVal = setColorCond(nextColor[2] - color[2], frequency);

            for (size_t nextI = i * frequency; nextI < (i + 1) * frequency; nextI++)
            {
                swapColor[0] += rGapVal;
                swapColor[1] += gGapVal;
                swapColor[2] += bGapVal;

                COLOR_VECTOR[nextI][0] = (int)swapColor[0];
                COLOR_VECTOR[nextI][1] = (int)swapColor[1];
                COLOR_VECTOR[nextI][2] = (int)swapColor[2];
            }
        }
    }
}

// the loop function runs over and over again forever
void loop()
{
#ifdef RGB_BUILTIN
    if (nullptr != COLOR_VECTOR[0])
    {
        for (size_t i = 0; i < sizeof(COLOR_VECTOR) / sizeof(COLOR_VECTOR[0]); i++)
        {
            const int *color = COLOR_VECTOR[i];

            if (nullptr != color)
            {
                setLEDColor(RGB_BUILTIN, color[0], color[1], color[2]);
                delay(CHANGE_RATE);
            }
            else
            {
                Serial.println("An color error");
            }
        }
    }
    else
    {
        Serial.println("Color Error");
        delay(5000);
    }
#endif
}