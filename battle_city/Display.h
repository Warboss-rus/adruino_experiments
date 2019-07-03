class Display
{
  public:
    static void Init();
    static void ClearScreen(uint16_t color);
    static void FillRect(byte x, byte y, byte w, byte h, uint16_t color);
    static void DrawSprite(byte x, byte y, byte w, byte h, const byte* sprite, uint16_t frontColor, uint16_t backColor);
    static void DrawTransparentSprite(byte x, byte y, byte w, byte h, const byte* sprite, uint16_t color);
    static void DrawText(byte x, byte y, const char* text, uint16_t color);
};
