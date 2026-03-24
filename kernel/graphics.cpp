#include "graphics.hpp"

void RGBResv8BitPerColorPixelWriter::Write(int x, int y, const PixelColor& c) {
    auto p = PixelAt(x,y);
    p[0] = c.b;
    p[1] = c.g;
    p[2] = c.r;
};

void BGRResv8BitPerColorPixelWriter:: Write(int x,int y,const PixelColor& c) {
    auto p = PixelAt(x,y);
    p[0] = c.b;
    p[1] = c.g;
    p[2] = c.r;
};