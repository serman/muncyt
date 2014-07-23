/**
 * Getting Started with Capture.
 * 
 * Reading and displaying an image from an attached Capture device. 
 */

import processing.video.*;

Capture cam;
PFont courier;

PImage zoom_img1;
PImage zoom_img2;
PImage bg;
int l1_no_zoom, l1_zoom,scale1;
int l2_no_zoom, l2_zoom,scale2;

int cols1, rows1;
int cols2, rows2;

int p1_x, p1_y;
PImage zoom_overlay1, zoom_overlay2, zoom_overlay3;

void setup() {
  size(1280, 1024);

  String[] cameras = Capture.list();

  if (cameras == null) {
    println("Failed to retrieve the list of available cameras, will try the default...");
    cam = new Capture(this, 640, 480);
  } if (cameras.length == 0) {
    println("There are no cameras available for capture.");
    exit();
  } else {
    println("Available cameras:");
   /* for (int i = 0; i < cameras.length; i++) {
      println(cameras[i]);
    }*/

    // The camera can be initialized directly using an element
    // from the array returned by list():
    cam = new Capture(this, cameras[0]);
    // Or, the settings can be defined based on the text in the list
    //cam = new Capture(this, 640, 480, "Built-in iSight", 30);
    
    // Start capturing the images from the camera
    cam.start();
  }
  l1_no_zoom=50;
  l1_zoom=200;
  
  
  l2_no_zoom=4;
  l2_zoom=160;
  courier= createFont("courier",14);
  zoom_img1=createImage(l1_no_zoom,l1_no_zoom,RGB);
  zoom_img2=createImage(l2_no_zoom,l2_no_zoom,RGB);
  scale1=l1_zoom/l1_no_zoom;
  scale2=l2_zoom/l2_no_zoom;

  bg=loadImage("bg.jpg");
  zoom_overlay1=loadImage("zoom_img1.png");
  zoom_overlay2=loadImage("zoom_img2.png");
 zoom_overlay3=loadImage("zoom_img3.png");  
}

void draw() {
  background(20);
  image(bg,0,0);
  if (cam.available() == true) {
    cam.read();
  }
  image(cam, 54, 228);
  zoom_img1=cam.get(477,335,l1_no_zoom,l1_no_zoom);
  zoom_img1.updatePixels();
  
  zoom_img2=zoom_img1.get(155/4,88/4,l2_no_zoom,l2_no_zoom);
  zoom_img2.updatePixels();
  
  //image(zoom_img1,700,100,l1_zoom,l1_zoom);
  //image(zoom_img2,800,350,l2_zoom,l2_zoom);
  
  
  pushMatrix();
  translate(820,362);
    // Begin loop for columns
  for (int i = 0; i < l1_no_zoom; i++) {
    // Begin loop for rows
    for (int j = 0; j < l1_no_zoom; j++) {      
      // Scaling up to draw a rectangle at (x,y)
      int x = i*scale1;
      int y = j*scale1;
      //noFill();
      color c= zoom_img1.get(i,j);
      fill(c);
      stroke(0);
      // For every column and row, a rectangle is drawn at an (x,y) location scaled and sized by videoScale.
      rect(x,y,scale1,scale1); 
      
    }
  }
  
  popMatrix();
  
  pushMatrix();
  
  translate(1068,385);
  color c=0;
   for (int i = 0; i < l2_no_zoom; i++) {
    // Begin loop for rows
    for (int j = 0; j < l2_no_zoom; j++) {      
      // Scaling up to draw a rectangle at (x,y)
      int x = i*scale2;
      int y = j*scale2;
      c= zoom_img2.get(i,j);
      fill(c);
      //noFill();
      stroke(0);
      if(i==l2_no_zoom-1 && j ==l2_no_zoom-1) stroke(255);
      // For every column and row, a rectangle is drawn at an (x,y) location scaled and sized by videoScale.
      rect(x,y,scale2,scale2); 
    }
  }
  //translate(150,0);
  popMatrix();
  textFont(courier);
  textAlign(LEFT,TOP);
  translate(0,1);
//textSize(15);
  fill(255,0,0);
  text("rojo= " + red(c) +" de 255",1080,580);
  fill(0,255,0);
  text("verde= " + green(c)+" de 255",1070,607);
  fill(0,0,255);
  text("azul= " + blue(c)+" de 255",1078,634);
 image(zoom_overlay1,531,361);
 image(zoom_overlay2,970,385);
 image(zoom_overlay3,1070,508);
}

