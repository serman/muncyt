import processing.core.*; 
import processing.data.*; 
import processing.event.*; 
import processing.opengl.*; 

import oscP5.*; 
import netP5.*; 
import processing.video.*; 
import java.net.*; 
import java.util.Arrays; 
import oscP5.*; 
import netP5.*; 

import java.util.HashMap; 
import java.util.ArrayList; 
import java.io.File; 
import java.io.BufferedReader; 
import java.io.PrintWriter; 
import java.io.InputStream; 
import java.io.OutputStream; 
import java.io.IOException; 

public class lab_f_LEDS extends PApplet {

OPC opc;
PImage dot;
int NUMPARTIC = 1;
int separacion =10;
int separacionV =3;
int OPC_counter=0;
int INTERTIRA=50;
int initX=50;
int initY=50; //esquina superior izquierda

// Map one 64-LED strip to the center of the window
//PANELES 1 2 3 4 
int NUM_TIRAS=10*2+3*2; // consideramos las tiras individuales como caso especial de la tira doble
int endX= (NUM_TIRAS-1)*separacion+initX;
int lengthTramo=separacionV*8;           //lontigud de un tramo de la tira 
int lengthTira=separacionV*24+INTERTIRA;  //longitud de una tira en el eje y 
int lengthPared=endX-initX;            //longitud de la "pared completa" en pixels
int endY=lengthTira+initY;

int initYTira2=initY+INTERTIRA+lengthTramo*2;
int initYTira1=initY+INTERTIRA+lengthTramo;

int screen1Pos=initX+separacion*5;
int screen2Pos=initX+separacion*13;
int screen3Pos=initX+separacion*21;




boolean debugOSC=true;
Movie movieExp;
char fade=0; // -1 0 o 1
boolean bdrawNoise=false;
boolean   bdrawWaves=false;
boolean bDrawRandomParticles=false;
boolean bDrawMovie=false;
boolean bDrawGridImage=false;
boolean bDrawGridCircles, bDrawLine,bExchange, bdrawOff,bdrawExplosionAngle=false;
boolean bDrawFading;
randomParticle randomP[]=new randomParticle[NUMPARTIC];
circles mcircle;
PImage gridImg;
PImage maskImage;
PImage maskImage2;
PImage maskImagelong;

PImage maskImageVert;
PImage maskImageHlong;
PImage maskImageVertSmall;
float exchangeTime; int exchangeSection;
int exchangeC1, exchangeC2;
public void setup()
{
  size(400, 400, P2D);
  
  //frame.setLocation(900,768);
  
  // Load a sample image
  dot = loadImage("color-dot.png");
  maskImage=loadImage("mask.png");
  maskImagelong=loadImage("masklong.png");
  maskImageVert=loadImage("maskVertical.png");
  maskImageVertSmall=loadImage("maskVerticalSmall.png");
  maskImage2=loadImage("mask2.png");
  maskImageHlong=loadImage("masHLong.png");
  frameRate(25);
  // Connect to the local instance of fcserver
  opc = new OPC(this, "127.0.0.1", 7890);
  int i=0;
  // void ledStrip(int index, int count, float x, float y, float spacing, float angle, boolean reversed)
  for (i=0; i<NUM_TIRAS; i++) {
    opc.ledStrip(OPC_counter, 8, initX+separacion*i, initY+lengthTramo/2 +INTERTIRA*2, separacionV, -PI/2, false);
    OPC_counter+=8;
    opc.ledStrip(OPC_counter, 8, initX+separacion*i, initY+lengthTramo/2 +INTERTIRA, separacionV, -PI/2, false);
    OPC_counter+=8;
    opc.ledStrip(OPC_counter, 8, initX+separacion*i, initY+lengthTramo/2, separacionV, -PI/2, false);
    OPC_counter+=8;  

    i++;
    if(i==5 || i==13  || i==21) continue;
    opc.ledStrip(OPC_counter, 8, initX+separacion*i, initY+lengthTramo/2, separacionV, -PI/2, true);
    OPC_counter+=8;
    opc.ledStrip(OPC_counter, 8, initX+separacion*i, initY+lengthTramo/2+INTERTIRA, separacionV, -PI/2, true);
    OPC_counter+=8;
    opc.ledStrip(OPC_counter, 8, initX+separacion*i, initY+lengthTramo/2+INTERTIRA*2, separacionV, -PI/2, true);
    OPC_counter+=8;
  } 
  println(i);
  setupOSC();   
  movieExp = new Movie(this, "explosion.mov");
  movieExp.play();
  movieExp.stop();

  gridImg=loadImage("grid.png");
  //class setup
  // smooth();
  myBall = new Ball[ballAmount];
  for (int ii = 0; ii < ballAmount; ii++) {
    myBall[ii] = new Ball();
    myBall[ii].setup();
  }

  for (int jj=0; jj< NUMPARTIC; jj++) {
    randomP[jj]=new randomParticle();
    randomP[jj].setup();
  }
  mcircle=new circles();
  /****/
  
}

public void draw()
{
  frameRate(25);
  background(0);
  /* stroke(255,0,0);
   fill(0,0,200);
   line(endX,initY,endX,endY);
   rect(initY,initX,2,2);*/

  //  rotateDraw(1);
  if (bdrawNoise) noiseDraw();
  //randomParticleDraw();
  if (bdrawWaves) waveDraw();
  if (bDrawRandomParticles) drawRandomParticles();

  //breath();
  if (bDrawMovie)  drawMovie();
  if (bDrawGridImage) drawGridImage();
  if (bDrawGridCircles) mcircle.draw();
  // drawWeird1();
  //drawTest();
  if (bdrawOff) off();
  if (bDrawLine) drawLine();
  if(bExchange)  exchangeDraw(exchangeTime, exchangeSection, exchangeC1, exchangeC2);
  if (bDrawFading) drawFading();
  if (bdrawExplosionAngle) drawExplosionAngle();
  drawScreens();
  //drawRandomParticles();
 // nWaves=2;
 // waveDraw();
  //drawTest();
  //rotateDraw(5);
  //mcircle.draw();
  //drawFading();
  //drawBottomTop();
  //noiseDraw();
  //text(frameRate,width-40,height-40);
  
}


public void movieEvent(Movie m) {
  m.read();
}

public void allOff() {
  bdrawExplosionAngle=false;
  bdrawNoise=false;
  bdrawWaves=false;
  bDrawRandomParticles=false;
  bDrawMovie=false;
  bDrawGridImage=false;
  bDrawGridCircles=false;
  bDrawLine=false; 
  bdrawOff=false;
  bExchange=false;
  bDrawFading=false;
}

public void keyPressed() {
  if (key == 'd') opc.setDithering(false);
  if (key == 'i') opc.setInterpolation(false);
  if (key == 'l') opc.setStatusLed(true);
}

public void keyReleased() {
  if (key == 'd') opc.setDithering(true);
  if (key == 'i') opc.setInterpolation(true);
  if (key == 'l') opc.setStatusLed(false);
}  

/*
 * Simple Open Pixel Control client for Processing,
 * designed to sample each LED's color from some point on the canvas.
 *
 * Micah Elizabeth Scott, 2013
 * This file is released into the public domain.
 */




public class OPC
{
  Socket socket;
  OutputStream output;
  String host;
  int port;

  int[] pixelLocations;
  byte[] packetData;
  byte firmwareConfig;
  String colorCorrection;
  boolean enableShowLocations;

  OPC(PApplet parent, String host, int port)
  {
    this.host = host;
    this.port = port;
    this.enableShowLocations = true;
    parent.registerDraw(this);
  }

  // Set the location of a single LED
  public void led(int index, int x, int y)  
  {
    // For convenience, automatically grow the pixelLocations array. We do want this to be an array,
    // instead of a HashMap, to keep draw() as fast as it can be.
    if (pixelLocations == null) {
      pixelLocations = new int[index + 1];
    } else if (index >= pixelLocations.length) {
      pixelLocations = Arrays.copyOf(pixelLocations, index + 1);
    }

    pixelLocations[index] = x + width * y;
  }
  
  // Set the location of several LEDs arranged in a strip.
  // Angle is in radians, measured clockwise from +X.
  // (x,y) is the center of the strip.
  public void ledStrip(int index, int count, float x, float y, float spacing, float angle, boolean reversed)
  {
    float s = sin(angle);
    float c = cos(angle);
    for (int i = 0; i < count; i++) {
      led(reversed ? (index + count - 1 - i) : (index + i),
        (int)(x + (i - (count-1)/2.0f) * spacing * c + 0.5f),
        (int)(y + (i - (count-1)/2.0f) * spacing * s + 0.5f));
    }
  }

  // Set the location of several LEDs arranged in a grid. The first strip is
  // at 'angle', measured in radians clockwise from +X.
  // (x,y) is the center of the grid.
  public void ledGrid(int index, int stripLength, int numStrips, float x, float y,
               float ledSpacing, float stripSpacing, float angle, boolean zigzag)
  {
    float s = sin(angle + HALF_PI);
    float c = cos(angle + HALF_PI);
    for (int i = 0; i < numStrips; i++) {
      ledStrip(index + stripLength * i, stripLength,
        x + (i - (numStrips-1)/2.0f) * stripSpacing * c,
        y + (i - (numStrips-1)/2.0f) * stripSpacing * s, ledSpacing,
        angle, zigzag && (i % 2) == 1);
    }
  }

  // Set the location of 64 LEDs arranged in a uniform 8x8 grid.
  // (x,y) is the center of the grid.
  public void ledGrid8x8(int index, float x, float y, float spacing, float angle, boolean zigzag)
  {
    ledGrid(index, 8, 8, x, y, spacing, spacing, angle, zigzag);
  }

  // Should the pixel sampling locations be visible? This helps with debugging.
  // Showing locations is enabled by default. You might need to disable it if our drawing
  // is interfering with your processing sketch, or if you'd simply like the screen to be
  // less cluttered.
  public void showLocations(boolean enabled)
  {
    enableShowLocations = enabled;
  }
  
  // Enable or disable dithering. Dithering avoids the "stair-stepping" artifact and increases color
  // resolution by quickly jittering between adjacent 8-bit brightness levels about 400 times a second.
  // Dithering is on by default.
  public void setDithering(boolean enabled)
  {
    if (enabled)
      firmwareConfig &= ~0x01;
    else
      firmwareConfig |= 0x01;
    sendFirmwareConfigPacket();
  }

  // Enable or disable frame interpolation. Interpolation automatically blends between consecutive frames
  // in hardware, and it does so with 16-bit per channel resolution. Combined with dithering, this helps make
  // fades very smooth. Interpolation is on by default.
  public void setInterpolation(boolean enabled)
  {
    if (enabled)
      firmwareConfig &= ~0x02;
    else
      firmwareConfig |= 0x02;
    sendFirmwareConfigPacket();
  }

  // Put the Fadecandy onboard LED under automatic control. It blinks any time the firmware processes a packet.
  // This is the default configuration for the LED.
  public void statusLedAuto()
  {
    firmwareConfig &= 0x0C;
    sendFirmwareConfigPacket();
  }    

  // Manually turn the Fadecandy onboard LED on or off. This disables automatic LED control.
  public void setStatusLed(boolean on)
  {
    firmwareConfig |= 0x04;   // Manual LED control
    if (on)
      firmwareConfig |= 0x08;
    else
      firmwareConfig &= ~0x08;
    sendFirmwareConfigPacket();
  } 

  // Set the color correction parameters
  public void setColorCorrection(float gamma, float red, float green, float blue)
  {
    colorCorrection = "{ \"gamma\": " + gamma + ", \"whitepoint\": [" + red + "," + green + "," + blue + "]}";
    sendColorCorrectionPacket();
  }
  
  // Set custom color correction parameters from a string
  public void setColorCorrection(String s)
  {
    colorCorrection = s;
    sendColorCorrectionPacket();
  }

  // Send a packet with the current firmware configuration settings
  public void sendFirmwareConfigPacket()
  {
    if (output == null) {
      // We'll do this when we reconnect
      return;
    }
 
    byte[] packet = new byte[9];
    packet[0] = 0;          // Channel (reserved)
    packet[1] = (byte)0xFF; // Command (System Exclusive)
    packet[2] = 0;          // Length high byte
    packet[3] = 5;          // Length low byte
    packet[4] = 0x00;       // System ID high byte
    packet[5] = 0x01;       // System ID low byte
    packet[6] = 0x00;       // Command ID high byte
    packet[7] = 0x02;       // Command ID low byte
    packet[8] = firmwareConfig;

    try {
      output.write(packet);
    } catch (Exception e) {
      dispose();
    }
  }

  // Send a packet with the current color correction settings
  public void sendColorCorrectionPacket()
  {
    if (colorCorrection == null) {
      // No color correction defined
      return;
    }
    if (output == null) {
      // We'll do this when we reconnect
      return;
    }

    byte[] content = colorCorrection.getBytes();
    int packetLen = content.length + 4;
    byte[] header = new byte[8];
    header[0] = 0;          // Channel (reserved)
    header[1] = (byte)0xFF; // Command (System Exclusive)
    header[2] = (byte)(packetLen >> 8);
    header[3] = (byte)(packetLen & 0xFF);
    header[4] = 0x00;       // System ID high byte
    header[5] = 0x01;       // System ID low byte
    header[6] = 0x00;       // Command ID high byte
    header[7] = 0x01;       // Command ID low byte

    try {
      output.write(header);
      output.write(content);
    } catch (Exception e) {
      dispose();
    }
  }

  // Automatically called at the end of each draw().
  // This handles the automatic Pixel to LED mapping.
  // If you aren't using that mapping, this function has no effect.
  // In that case, you can call setPixelCount(), setPixel(), and writePixels()
  // separately.
  public void draw()
  {
    if (pixelLocations == null) {
      // No pixels defined yet
      return;
    }
 
    if (output == null) {
      // Try to (re)connect
      connect();
    }
    if (output == null) {
      return;
    }

    int numPixels = pixelLocations.length;
    int ledAddress = 4;

    setPixelCount(numPixels);
    loadPixels();

    for (int i = 0; i < numPixels; i++) {
      int pixelLocation = pixelLocations[i];
      int pixel = pixels[pixelLocation];

      packetData[ledAddress] = (byte)(pixel >> 16);
      packetData[ledAddress + 1] = (byte)(pixel >> 8);
      packetData[ledAddress + 2] = (byte)pixel;
      ledAddress += 3;

      if (enableShowLocations) {
        pixels[pixelLocation] = 0xFFFFFF ^ pixel;
      }
    }

    writePixels();

    if (enableShowLocations) {
      updatePixels();
    }
  }
  
  // Change the number of pixels in our output packet.
  // This is normally not needed; the output packet is automatically sized
  // by draw() and by setPixel().
  public void setPixelCount(int numPixels)
  {
    int numBytes = 3 * numPixels;
    int packetLen = 4 + numBytes;
    if (packetData == null || packetData.length != packetLen) {
      // Set up our packet buffer
      packetData = new byte[packetLen];
      packetData[0] = 0;  // Channel
      packetData[1] = 0;  // Command (Set pixel colors)
      packetData[2] = (byte)(numBytes >> 8);
      packetData[3] = (byte)(numBytes & 0xFF);
    }
  }
  
  // Directly manipulate a pixel in the output buffer. This isn't needed
  // for pixels that are mapped to the screen.
  public void setPixel(int number, int c)
  {
    int offset = 4 + number * 3;
    if (packetData == null || packetData.length < offset + 3) {
      setPixelCount(number + 1);
    }

    packetData[offset] = (byte) (c >> 16);
    packetData[offset + 1] = (byte) (c >> 8);
    packetData[offset + 2] = (byte) c;
  }
  
  // Read a pixel from the output buffer. If the pixel was mapped to the display,
  // this returns the value we captured on the previous frame.
  public int getPixel(int number)
  {
    int offset = 4 + number * 3;
    if (packetData == null || packetData.length < offset + 3) {
      return 0;
    }
    return (packetData[offset] << 16) | (packetData[offset + 1] << 8) | packetData[offset + 2];
  }

  // Transmit our current buffer of pixel values to the OPC server. This is handled
  // automatically in draw() if any pixels are mapped to the screen, but if you haven't
  // mapped any pixels to the screen you'll want to call this directly.
  public void writePixels()
  {
    if (packetData == null || packetData.length == 0) {
      // No pixel buffer
      return;
    }
    if (output == null) {
      // Try to (re)connect
      connect();
    }
    if (output == null) {
      return;
    }

    try {
      output.write(packetData);
    } catch (Exception e) {
      dispose();
    }
  }

  public void dispose()
  {
    // Destroy the socket. Called internally when we've disconnected.
    if (output != null) {
      println("Disconnected from OPC server");
    }
    socket = null;
    output = null;
  }

  public void connect()
  {
    // Try to connect to the OPC server. This normally happens automatically in draw()
    try {
      socket = new Socket(host, port);
      socket.setTcpNoDelay(true);
      output = socket.getOutputStream();
      println("Connected to OPC server");
    } catch (ConnectException e) {
      dispose();
    } catch (IOException e) {
      dispose();
    }
    
    sendColorCorrectionPacket();
    sendFirmwareConfigPacket();
  }
}

//THE CLASS
class Ball {
  float x;
  float y;
  float z;
  float incX;
  float incY;
  float incZ;
 
  public void setup() {
    x = random(width);
    y = random(height);
    z = random(width);
    setInc();
  }
 
  //speed
  public void setInc() {
    incX = random(-6, 6);
    incY = random(-8, 8);
    incZ = random(-10,10);
  }
 

 
  //direction
  public void step() {
    if (x > width || x < 0) {
      incX = -incX;
    }
    if (y > height || y < 0) {
      incY = -incY;
    }
    if (z < width || z > 0) {
    }
    x = x + incX;
    y = y + incY;
    z = z + incZ;
 
    if (x > width) {
      x = width;
      setInc();
    }
    if (x < 0) {
      x = 0;
      setInc();
    }
    if (y > height) {
      y = height;
      setInc();
    }
    if (y < 0) {
      y = 0;
      setInc();
    }
    if (z > width) {
      z = width;
      setInc();
    }
    if (z < 0) {
      z = 0;
      setInc();
  }
}


}
int[] data = { 
  0, 2, 3, 4, 6, 7, 8, 9, 10, 11, 12, 17, 17, 24, 25, 27, 28, 29, 30, 31, 32, 36, 39, 40, 41, 44, 45, 47, 52, 53, 54, 57, 68, 70, 71, 72, 73, 74, 75, 79, 82, 85, 84, 83, 82, 81, 80, 79, 78, 77, 76, 75, 74, 73, 72, 71, 70, 69, 68, 67, 66, 65, 64, 63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 19, 18, 18, 17, 17, 16, 16, 15, 15, 14, 14, 13, 13, 12, 12, 11, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 2, 1, 1, 0, 0, 2, 3, 4, 6, 7, 10, 14, 17, 17, 18, 19, 20, 34, 42, 40, 50, 62, 64, 71, 81, 82, 83, 94, 96, 99, 104, 107, 108, 115, 120, 121, 122, 123, 126, 130, 131, 132, 140, 150, 151, 152, 153, 154, 155, 155, 159, 163, 159, 160, 160, 161, 162, 163, 163, 163, 164, 164, 165, 165, 166, 166, 167, 167, 168, 168, 170, 168, 167, 164, 164, 162, 155, 154, 153, 148, 148, 147, 145, 145, 142, 142, 136, 134, 125, 123, 123, 122, 119, 102, 102, 101, 101, 100, 100, 99, 99, 98, 97, 96, 95, 95, 94, 91, 91, 90, 88, 84, 83, 80, 80, 79, 78, 73, 73, 72, 72, 70, 66, 65, 64, 63, 62, 60, 60, 59, 59, 58, 58, 57, 56, 55, 53, 53, 52, 52, 51, 51, 50, 49, 48, 47, 43, 42, 41, 40, 40, 39, 39, 38, 37, 36, 36, 37, 38, 38, 40, 41, 42, 45, 46, 47, 48, 49, 51, 53, 56, 58, 58, 62, 63, 68, 69, 70, 71, 73, 74, 76, 78, 79, 80, 80, 83, 83, 84, 87, 88, 98, 100, 102, 111, 124, 129, 131, 133, 136, 136, 142, 143, 144, 145, 147, 150, 148, 148, 144, 143, 141, 141, 136, 136, 135, 135, 134, 131, 124, 124, 123, 120, 119, 119, 117, 115, 115, 109, 109, 107, 103, 102, 101, 100, 99, 99, 97, 96, 94, 93, 93, 92, 92, 91, 91, 90, 90, 88, 88, 87, 86, 86, 85, 85, 78, 77, 70, 70, 69, 65, 64, 63, 63, 62, 62, 61, 60, 59, 58, 57, 57, 56, 54, 53, 48, 46, 38, 36, 36, 30, 28, 27, 26, 25, 24, 24, 18, 17, 16, 15, 11, 9, 8, 7, 6, 0
};

public void drawTest() {
  background(0);

  float dotSize = width * 0.2f;
  //image(dot, mouseX - dotSize*5/2, mouseY - dotSize/2, dotSize*5, dotSize);
      // fill(color(0,255,0));
      tint(color(255,0,0));
      image(maskImage,mouseX, mouseY,100,10);
      //fill(50, 50, 50);
  //rect(mouseX, mouseY, 300, lengthTramo+10);
}

int rotatePos=initX;
public void rotateDraw(float T) {
  frameRate(25);
  background(0);
  
  //freq= rotaciones/seg;
  //T = periodo en segundos
  float pix_second=lengthPared/T;  
  int inc_pix=round( pix_second/frameRate ); 
  pushStyle();
  fill(255);
  tint(10, 100, 10);
  noStroke();
  image(maskImageVert,rotatePos-15, initY, 30, lengthTira+10);
  //rect(rotatePos, initY, 30, lengthTira+10);  
  if (rotatePos>endX) rotatePos=initX;
  rotatePos+=inc_pix;
  popStyle();
}



int ex1Pos;
int ex2Pos;
public void exchangeDraw(float T,int section,int colour1 , int colour2) {
  frameRate(25);
  background(0);

  float pix_second=(lengthPared/3)/T; 
  int inc_pix=round( pix_second/frameRate ); 
  //println ( section + " ex1Pos " + ex1Pos + "||| ex2Pos "+ ex2Pos +" ||| 1: "  +screen1Pos  +" |||| 2: "+  screen3Pos + "  incpix " + inc_pix + " pared: " +lengthPared);
  if(section==1){
    if((ex1Pos<screen2Pos) && (ex2Pos >screen1Pos)){
     // noStroke();
      //colour1=color(200,0,0);
     // fill(colour1);
     //movimiento izquierda a derecha
      tint(colour1);
      imageMode(CORNERS);
      image(maskImage,ex1Pos-10, initY-10,ex1Pos+10,initY+20);
      tint(colour2);
      image(maskImage,ex2Pos-10, initYTira2-10,ex2Pos+10,initYTira2+25);      
      ex1Pos+=inc_pix;
      ex2Pos-=inc_pix;
    }
  }
   if(section==2){
    if((ex1Pos<screen3Pos) && (ex2Pos >screen2Pos)){
       tint(colour1);
      imageMode(CORNERS);
      image(maskImage,ex1Pos-10, initY-10,ex1Pos+10,initY+20);
      tint(colour2);
      image(maskImage,ex2Pos-10, initYTira2-10,ex2Pos+10,initYTira2+25);     
      ex1Pos+=inc_pix;
      ex2Pos-=inc_pix;
    }
  }
  if(section==3){
    if( (ex1Pos>=screen3Pos && ex1Pos<=lengthPared+initX) || (ex1Pos>=0 && ex1Pos<screen1Pos) ){
       tint(colour1);
      imageMode(CORNERS);
      image(maskImage,ex1Pos-10, initY-10,ex1Pos+10,initY+20);
      tint(colour2);
      image(maskImage,ex2Pos-10, initYTira2-10,ex2Pos+10,initYTira2+25);   
      
      ex1Pos+=inc_pix;
      ex2Pos-=inc_pix;
      if(ex1Pos>=lengthPared+initX) ex1Pos=initX;
      if(ex2Pos<=initX) ex2Pos=lengthPared+initX;
    }
  }
  imageMode(CORNER);
  
}
public void setupExchange(int section){
  if(section==1){
    ex1Pos=screen1Pos;
    ex2Pos=screen2Pos;
  }
  if(section==2){
    ex1Pos=screen2Pos;
    ex2Pos=screen3Pos;
  }
  if(section==3){
    ex1Pos=screen3Pos;
    ex2Pos=screen1Pos;
  }
}

public void drawLine() {
  background(0);
  frameRate(25);
  pushStyle();
    fill(255);
    
    //rect(initX+rotatePos, initY-3, 10, lengthTira+10);
    tint(10, 100, 10);
    noStroke();
    image(maskImageVert,initX+rotatePos-10, initY-3, 20, lengthTira+10);
  popStyle();
}



public void noiseDraw() {
  frameRate(6);
  background(0);
  int psize = 4;
  int x_count = (lengthPared/psize); 
  int y_count = (lengthTira/psize);
  pushStyle();
  noStroke(); //commenting this out gives a neat effect

  //draw the "pixels"
  for (int i=0; i<=y_count; i++) {
    for (int j=0; j<=x_count; j++) {
      //pick a random grayscale value before drawing each rectangle. (thanks subpixel!)
      float ruido=random(120);
      if(ruido<100) fill(0);
      else fill(ruido);
      rect(initX+j*psize, initY+i*psize, psize, psize);
    }
  } 
  popStyle();
}
int explosionAngleInitValue=10;
public void drawExplosionAngle(){
  println(explosionAngleInitValue);
   frameRate(6);
  background(0);
  int psize = 2;
  int x_count = 16; 
  int y_count = (lengthTira/psize);
  
  pushStyle();
  noStroke(); //commenting this out gives a neat effect

  //draw the "pixels"
  for (int i=0; i<=y_count; i++) {
    for (int j=explosionAngleInitValue/psize; j<=explosionAngleInitValue/psize+x_count; j++) {
      //pick a random grayscale value before drawing each rectangle. (thanks subpixel!)
      float ruido=random(120);
      if(ruido<100) fill(0);
      else fill(ruido,random(0,80),0);
      rect(initX+j*psize, initY+i*psize, psize, psize);
    }
  } 
  popStyle();
}

public void drawRandomParticles(){
  frameRate(25);
  background(0);
 for (int i=0; i<NUMPARTIC; i++){
   randomP[i].draw();
 //  println ("random partc " + i );
 } 
}


float phi =  0; // the phase shift offset amount
float amplitude = lengthTramo/2; // the amplitude of our wave
float frequency = 1.0f/32; // slow down our wave so we can see it better
int numPoints = 30; // the granularity of our drawing
int nWaves=0;
int waveColor2, waveColor1, waveColor0=color(200,255,255);;
public void waveDraw() {
  /*stroke(0, 40, 100);
  fill(0, 200);
  rect(0, 0, width, height);
  noFill();
  smooth();*/
  background(0);

  
  float x = 0;
  float y = 0;

  // if we consider 1 pixel to be 1 radian, then this is the number
  // of radians between points
  //float angle = lengthPared+initX/numPoints;probar esto!!!!!!
  float angle = (lengthPared+initX)/numPoints;
  smooth();
  imageMode(CENTER);
  colorMode(HSB);
  strokeWeight(3);
  for (int n=1; n<=min (nWaves, 3); n++) {
     pushMatrix();
     fill(255);
    if (n==1){ translate(initX, initY+ lengthTira/2); tint(waveColor0); }
    if (n==2){ translate(initX, initY+ lengthTramo/2);  tint(waveColor1); }
    if (n==3){  translate(initX, initY+ lengthTramo*2+ INTERTIRA+ lengthTramo/2);  tint(waveColor2); }

    //beginShape();
noStroke();

    for (int i = 0; i < numPoints; i++) {     
      // calculate where we are along x
      x = angle*i;     
      // y is determined by our function
      y = sin(x*frequency+phi+PI*n)* amplitude;
      //vertex(x, y, i, y);
      image(maskImageVertSmall,x,y);
      
    }
    //endShape();    
    popMatrix();
  }
  phi -= .10f;
imageMode(CORNER);
colorMode(RGB);
}


public void moveColor() {
}

public void off() {
  background(0);
}

int loopCount=0;
public void breath() {
  background(0);
  int temp = round(map(data[loopCount], 0, 170, 0, 200));
  fill(temp);
  rect(initX, initY, lengthPared, lengthTira);
  if (loopCount > data.length-2) {
    loopCount = 0;
  } else {
    loopCount++;
  }
}

float angle=0;
public void drawGridImage(){
  frameRate(25);
  pushMatrix();
  translate(initX+lengthPared/2,initY+lengthTira/2);
  rotate(angle);
  //
  tint(255,100);
   image(gridImg,-gridImg.width/2,-gridImg.width/2); 
  popMatrix();
  angle+=0.02f;
   
}
public void drawMovie() {

  background(0);
  fill(255, 100);
  image(movieExp, initX-10, initY-10, lengthPared*1.5f, lengthTira*1.5f);

}

float lengthFading=0;
float initFadingX=0;
float initFadingY=0;
float HueFading=100;
float fadingSpeed=1;
public void drawFading(){
  frameRate(25);
  background(0);
  imageMode(CORNERS);
  colorMode(HSB,255);
  tint(HueFading,200,160);
  if(fadingSpeed>0)
    image(maskImage2,initFadingX,initY+initFadingY,initFadingX+lengthFading,initFadingY+lengthTramo*1.8f+initY);
  else{

    pushMatrix();
    scale(-1,1);
    translate(-400,0);
    //image(maskImage2,initFadingX-lengthFading,initY+initFadingY,lengthFading,lengthTramo*1.8);
    image(maskImage2,initFadingX,initY+initFadingY,initFadingX-lengthFading,initFadingY+lengthTramo*1.8f+initY);
    popMatrix();
  }
  lengthFading+=fadingSpeed;
  if(abs(lengthFading)>200){
    initFadingX=random(0,lengthPared-50);
    lengthFading=0;
    float signo=random(-2,2);
    if(signo>0) fadingSpeed=8; else fadingSpeed=-8;
    initFadingY=random(0,lengthTira-lengthTramo);
    HueFading=random(70,150);
  }
  imageMode(CORNER);
  colorMode(RGB);
}

Ball[] myBall;
int ballAmount = 115;
int distance = 115;
int radius = 0;
public void drawWeird1() {
  background(0);
  for (int i = 0; i < ballAmount; i++) {
    myBall[i].step();

    for (int j = 0; j < i; j++) {
      if (i != j) {
        float distBalls = dist(myBall[i].x, myBall[i].y, myBall[j].x, myBall[j].y);

        if (distBalls <= distance) {
          float lineWeight = 100/distBalls;
          stroke(lineWeight*100, lineWeight*100, lineWeight*100);
          strokeWeight(1);
          line(myBall[i].x, myBall[i].y, myBall[j].x, myBall[j].y);
        }

        if (distBalls <= radius) {
          myBall[i].setInc();
          myBall[j].setInc();
        }
      }
    }
  }
}
int bottomTopCounter=200;
public void drawBottomTop(){
  tint(30,200,100);
 image(maskImageHlong,initX-10,bottomTopCounter,300,30);
  bottomTopCounter-=3;
 if(bottomTopCounter<-20){
 bottomTopCounter=200;
 }
  
}

public void drawScreens(){
  stroke(0,255,0);
  line(screen1Pos,initY,screen1Pos,initY+lengthTira);
  line(screen2Pos,initY,screen2Pos,initY+lengthTira);
  line(screen3Pos,initY,screen3Pos,initY+lengthTira);
}

class randomParticle {
  float particlePosX;
  float particlePosY;
  int mcolor;
  float incr=0;
  float incr2=0;
  float tasa=0;
  float speed;
  public void setup() {
     particlePosX=initX+(int)random(0,endX-20);
     particlePosY=initY+(int)random(0,lengthTira);
    colorMode(HSB);  
     mcolor=color(random(90,210), random(90,210), random(90,210));
    colorMode(RGB);
    tasa=random(0.004f,0.01f);
    speed=random(5,10);
  }

  public void draw() {
    pushStyle();
    noStroke(); 
       colorMode(HSB); 
      fill(255);   
      tint(mcolor);
      //rect(particlePosX, particlePosY, 5, 5);
      image(maskImage,particlePosX, particlePosY, 80, 5);
      particlePosX +=speed;//initX+lengthPared*(noise(incr)-0.5);
      particlePosY=initY + lengthTira*2*(noise(incr2));
      incr+=tasa;
      incr2+=tasa;
      if (particlePosX>endX+100) { 
        particlePosX=initX; 
        //particlePosY=initY+(int)random(0,lengthTira);
        mcolor=color(random(100,200), 200, 200);
      }
      popStyle();
    }
}


class circles{
 int size=1;
 public void draw(){
   size+=3;
   ellipseMode(CENTER);
   fill(100);
   ellipse(initX+lengthPared/2, initY+lengthTira/2,size,size);
   
   if(size>40){
     fill(0);
     ellipse(initX+lengthPared/2, initY+lengthTira/2,size-40,size-40);
   }
   if(size>80){
     fill(100);
     ellipse(initX+lengthPared/2, initY+lengthTira/2,size-80,size-80);
   }
   if(size>110){
     fill(0);
     ellipse(initX+lengthPared/2, initY+lengthTira/2,size-110,size-110);
   }
   
      if(size>140){
     fill(100);
     ellipse(initX+lengthPared/2, initY+lengthTira/2,size-140,size-140);
   }
   
   if(size>170){
     fill(0);
     ellipse(initX+lengthPared/2, initY+lengthTira/2,size-170,size-170);
   }
   
   if(size>210){
     fill(100);
     ellipse(initX+lengthPared/2, initY+lengthTira/2,size-210,size-210);
   }
   
   if (size>300) size=0;
 }
  
}



  
OscP5 oscP5;
NetAddress myRemoteLocation;
String  IPEnvio = "localhost";
int     puertoEnvio = 9000;

int     puertoEscucha = 9000;

public void setupOSC() {
  oscP5 = new OscP5(this,puertoEscucha);
//EM  
  oscP5.plug(this,"startEM","/sync/electromagnetism/start_event"); //direccionamiento con m\u00e1s sentido  
  oscP5.plug(this,"drawWaves","/sync/electromagnetism/number_waves"); //direccionamiento con m\u00e1s sentido
    oscP5.plug(this,"setWavesOptions","/sync/electromagnetism/wave_length"); //direccionamiento con m\u00e1s sentido

//MENU
 
oscP5.plug(this,"drawMenu","/sync/menu/start_event"); 
//Gravedad
  oscP5.plug(this,"startGravity","/sync/gravity/start_event");
  oscP5.plug(this,"blackHole","/sync/gravity/collapse_start_event");
  oscP5.plug(this,"drawOff","/sync/gravity/collapse_stop_event");

// Nuclear Fuerte
oscP5.plug(this,"start_nuclear_f","/sync/strong_nuclear/start_event");
  oscP5.plug(this,"exchangeColors","/sync/strong_nuclear/hand_on_event");
  oscP5.plug(this,"strong_nuclear_beam","/sync/strong_nuclear/beam");
  oscP5.plug(this,"strong_nuclear_explosion","/sync/strong_nuclear/explosion");
//Nuclear Debil
  oscP5.plug(this,"Explosion","/sync/weak_nuclear/chain_reaction_event"); //
  oscP5.plug(this,"drawRotating","/sync/weak_nuclear/ball"); 
  oscP5.plug(this,"end_event","/sync/electromagnetism/end_event");
  oscP5.plug(this,"start_event","/sync/menu/start_event");
}

// si llga un evento test con dos parametros i1 e i2 se vendr\u00eda a esta funci\u00f3n por el plug de antes
public void startEM(){
  allOff();
  bdrawNoise=true;
  fade=1;  
}

public void drawWaves(float f){
  allOff();
    //if(debugOSC) println("Function: drawWaves: " + f);
    if((int) f==0) {
      bdrawNoise=true;
    bdrawWaves=false;
    } 
    else{
    bdrawNoise=false;
    bdrawWaves=true;
    nWaves=(int)f;
    }
}
public void setWavesOptions(float id, float dist){
  println( "waves options " +id + " dist" +dist);
  colorMode(HSB);
  if((int)id==0){
      waveColor0=color(map(dist,0,768,0,255),255,255);
    println("color0 opt" + waveColor0);
  }
  else if((int)id==1){
    waveColor1=color(map(dist,0,768,0,170),255,255);
  }
  else if((int)id==2){
    waveColor2=color(map(dist,0,768,0,170),255,255);
  }
  colorMode(RGB);
}
public void drawMenu(){
  allOff();
    if(debugOSC) println("Function: drawMenu: ");
    bDrawRandomParticles=true;
}

public void Explosion(){  
  allOff();
  movieExp.play();
  bDrawMovie=true;
}

public void startGravity(){
  allOff(); 
 bDrawFading=true;
}
public void start_nuclear_f(){
  allOff();

}
public void blackHole(){
  allOff();
bDrawGridCircles=true;
}

public void drawOff(){
  allOff();
  bdrawOff=true;
}

public void drawRotating(float angle, float speed, float direction){
  allOff();
   println ("draw rotating" + angle + " " + speed);
   rotatePos=round(map(angle,0,2*PI,0,lengthPared));
   bDrawLine=true;
}

public void exchangeColors(float time, float section, float color1, float color2){
  println ("draw exchangeColors" + time + " " + section);
  allOff();  
  exchangeTime= time*1.5f;
  exchangeSection=(int)section; 
  setupExchange((int)section);
  int mcolor1;
  if(color1==0) mcolor1=color(150,0,0);
  else if(color1==1) mcolor1=color(0,150,0);
  else mcolor1=color(0,0,150);
  
  int mcolor2;
  if(color2==0) mcolor2=color(150,0,0);
  else if(color2==1) mcolor2=color(0,150,0);
  else mcolor2=color(0,0,150);
  
  exchangeC2=(int) mcolor1; 
  exchangeC1=(int) mcolor2;

  bExchange=true;
}
/* incoming osc message are forwarded to the oscEvent method. */
public void oscEvent(OscMessage oscMsg) {
  /* print the address pattern and the typetag of the received OscMessage */  
    if(oscMsg.isPlugged()==true) { return;}
    
  print("### received an osc message.");
  print(" addrpattern: "+oscMsg.addrPattern());
  println(" typetag: "+oscMsg.typetag());
  
  // coger valores segun el tag
  for(int i=0; i<oscMsg.arguments().length; i++) {
    
  }  
  
  if(oscMsg.addrPattern().equals("/position") ){
    float ang = oscMsg.get(0).floatValue();
    float wAng = oscMsg.get(1).floatValue();
  } 
  else 
  if(oscMsg.addrPattern().equals("/explosion") ){

  }
 
  if(oscMsg.addrPattern().equals("/modo")) {
    int modo_Rcv = oscMsg.get(0).intValue();
  }    
  
}
public void end_event(){
  println("end Event");
  println("------------------");
  
}

public void start_event(){
  println("start Event");
  
}

public void scene1(){
  
}

public void scene2(){

}

public void strong_nuclear_beam(float angle){
  println(angle);
  allOff();
  explosionAngleInitValue= (int)map(angle,0,2*PI,0,lengthPared) ;
  bdrawExplosionAngle=true;
}

public void strong_nuclear_explosion(){
}



  static public void main(String[] passedArgs) {
    String[] appletArgs = new String[] { "lab_f_LEDS" };
    if (passedArgs != null) {
      PApplet.main(concat(appletArgs, passedArgs));
    } else {
      PApplet.main(appletArgs);
    }
  }
}
