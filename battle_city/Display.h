class Display
{
  public:
    static void Init();
    static void ClearScreen(uint16_t color);
    static void FillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
    static void DrawSprite(int16_t x, int16_t y, int16_t w, int16_t h, const byte* sprite, uint16_t frontColor, uint16_t backColor);
    static void DrawTransparentSprite(int16_t x, int16_t y, int16_t w, int16_t h, const byte* sprite, uint16_t color);
    static void DrawIndexedSprite(int16_t x, int16_t y, int16_t w, int16_t h, byte* sprite, uint16_t* colorMap);
    static void DrawText(int16_t x, int16_t y, const char* text, uint16_t color);
};
