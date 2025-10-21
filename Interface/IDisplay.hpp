#include <cstdint>
#include <stdint.h>
class IDisplay
{
    
public:
    virtual void Clear() = 0;
    virtual void DrawPixel(int x, int y, bool value) = 0;
    virtual void Render() = 0;
    virtual void UpdateFromMemory(uint8_t* memoryDisplay) = 0;
    
};


