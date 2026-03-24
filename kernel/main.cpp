#include <stdint.h>
#include <stddef.h> 
// #include <cstdint>
// #include <cstddef>
#include "frame_buffer_config.hpp"
#include "graphics.hpp"
#include "font.cpp"

// グローバル変数
char pixel_writer_buf[sizeof(RGBResv8BitPerColorPixelWriter)];
PixelWriter* pixel_witer;

// 演算子の定義
void* operator new(size_t size, void* buf){
    return buf;
}
void operator delete(void* ojb) noexcept{
}

extern "C" void KernelMain(const FrameBufferConfig& frame_buffer_config){
    switch (frame_buffer_config.pixel_format){
        case kPixelRGBResv8BitPerColor:
            pixel_witer = new(pixel_writer_buf)
                RGBResv8BitPerColorPixelWriter{frame_buffer_config};
            break;
        case kPixelBGRResv8BitPerColor:
            pixel_witer = new(pixel_writer_buf)
                BGRResv8BitPerColorPixelWriter{frame_buffer_config};
            break;
    }

    for(int x = 0;x < frame_buffer_config.horizontal_resolution; ++x){
        for(int y = 0; y < frame_buffer_config.vertical_resolution; ++y){
            pixel_witer->Write(x,y,{255,255,255}); // かわいい紫色
        }
    }

    // 格子模様にしたいので格子のサイズを固定する
    const int squareSize = 50;
    for(int x = 0;x < frame_buffer_config.horizontal_resolution; ++x){
        for(int y = 0; y < frame_buffer_config.vertical_resolution; ++y){
            if (x % squareSize == 0 || y % squareSize == 0 ){
               for(int s = 0 ; s < squareSize/2 ; s++){
                   pixel_witer->Write(x + s ,y + s ,{255,204,229}); // パステルなピンク色！
               }
            }
        }
    }
    WriteAscii(*pixel_witer, 50, 50, 'A', {0,0,0});
    WriteAscii(*pixel_witer, 58, 50, 'A', {0,0,0});    
    while (1) __asm__("hlt"); // __asm__はアセンブリ言語を書くための関数。hltはCPUを停止させて省電力にする命令。
}