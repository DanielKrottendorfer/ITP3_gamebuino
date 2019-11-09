float lerp(const float lo, const float hi, const float t) 
{ return lo * (1 - t) + hi * t; } 

class ValueNoise1D 
{ 
public: 
    ValueNoise1D(unsigned seed, int min, int max) 
    { 
        int range = max - min;
        srand(seed); 
        for (unsigned i = 0; i < kMaxVertices; ++i) { 
            r[i] = float( (rand()%range) + min);
        } 
    } 
 
    // Evaluate the noise function at position x
    float eval(const float &x) 
    { 
        int xi = (int)x; 
        int xMin = xi % (int)kMaxVertices; 
        float t = x - xi; 
        int xMax = (xMin == kMaxVertices - 1) ? 0 : xMin + 1; 
 
        return lerp(r[xMin], r[xMax], t); 
    } 
    static const unsigned kMaxVertices = 10; 
    float r[kMaxVertices]; 
}; 

#include <Gamebuino-Meta.h>

// CHECKER PATTERN
const uint8_t myImgBuf[] = {
  8, 8, // width, heigth
  1, 0, // frames
  0, // frame loop
  0xFF, // transparent color
  1, // color mode

  0x07, 0x07, 0x07, 0x07,
  0x70, 0x70, 0x70, 0x70,
  0x07, 0x07, 0x07, 0x07,
  0x70, 0x70, 0x70, 0x70,
  0x07, 0x07, 0x07, 0x07,
  0x70, 0x70, 0x70, 0x70,
  0x07, 0x07, 0x07, 0x07,
  0x70, 0x70, 0x70, 0x70,
};

#define HEIGHT 64
#define WIDTH 80

uint16_t imgBuf[(WIDTH*HEIGHT)+6];

//Image myImg(myImgBuf);
Image myImg;

void generateTerrain(){
  ValueNoise1D valueNoise1D(2222,30,54); 
  imgBuf[0] = WIDTH;
  imgBuf[1] = HEIGHT;
  imgBuf[2] = 1;
  imgBuf[3] = 0;
  imgBuf[4] = 0;
  imgBuf[5] = 0xF800;

  int offset = 6;
  bool t = true;
  for( int l=0 ; l<WIDTH ; l++ )
  {
    float x = l / float(WIDTH - 1) * 5; 
    for( int c=0 ; c<HEIGHT ; c++ )
    {
      if( valueNoise1D.eval( x ) < float(c) )
      {
        imgBuf[ (c*WIDTH) + l + offset ] = 0x0664;
      }else{
        imgBuf[ (c*WIDTH) + l + offset ] = 0x039F;
      }
    }
  }
  myImg = Image(imgBuf);
}

void setup() {
  gb.begin();
  generateTerrain();
}

void loop() {
  while(!gb.update());
  gb.display.clear();
  
  // draw the image
  gb.display.drawImage(0, 0, myImg);
}
