import processing.core.*; 
import processing.data.*; 
import processing.event.*; 
import processing.opengl.*; 

import processing.event.KeyEvent; 
import processing.event.KeyEvent; 
import gab.opencv.*; 
import java.awt.Rectangle; 
import controlP5.*; 
import java.util.*; 
import java.io.BufferedReader; 
import java.io.FileReader; 
import java.io.IOException; 

import java.util.HashMap; 
import java.util.ArrayList; 
import java.io.File; 
import java.io.BufferedReader; 
import java.io.PrintWriter; 
import java.io.InputStream; 
import java.io.OutputStream; 
import java.io.IOException; 

public class HaarcascadeVisualizer extends PApplet {







OpenCV opencv;

// ------------------------------------------------------------------------
// START Editable Parameters
// Cascade file
String cascadeFile = "haarcascade_frontalface_default.xml";

// Choose amount of white space around the image
int margin = 0;
// Color of the image background
int imgBgColor = color(200);



ControlP5 cp5;
// Interface elements

PFont font1, font2;
PFont courier;
// Logic

// GUI

int marginL = 40;
int marginT = 60;

// Graphics
int[] sampleSize = new int[2];
int minSize = 20;
PImage img, resizedImg;
//int rows, cols;
int imgSize;

// File I/O
ArrayList stages;
float stageScale;
String[] xmlFiles;
String[] fotoFiles={"test640.jpg","test640_2.jpg","test640_3.jpg"};
int currentPhoto=0;
String path; // sketch path

PGraphics mgraphics; //haar features are drawn here

Rectangle[] faces; //faces detected in the "real algorightm"
ArrayList<Rectangle> fakeDetectedfaces; //faces detected with this fake algorithm
/******statuses ****/
boolean fileLoaded=false; //is xml haar file loaded
boolean isDebug=true;
boolean doProcess; //controls reproduction

boolean showInterface;

String statusMsg = "";
int idCur;




int windowsize=100; //haar window size
int xposition=0; 
int yposition=0;
Rectangle mrect;
Rectangle faceRectangle;
int stagesCounter=0; //This variable is the counter used to know in which stage of the algorithm we are (0 to 26 for xxxfacexxx.xml)
int featuresCounter=0; //Each stage has a different number of features
//FAKE STUFF
int stopStage=20; //The algorithm takes too long for a full visualization. so we include a stop stage
int xAxeMovement=100; 
int yAxeMovement=100;
int slowOneOfEach= 1; 
float areaInterseccion=0;
PImage img2; //the image we represent on the right side
miniImages minimages;
PImage imagebg;
boolean amIOveraFace=false;
public void setup() { 
  size(1280, 1024,P2D);
  cp5 = new ControlP5(this);
  /*** OPENCV STUFF ***/
  loadNewPhoto("test640.jpg");
  setInitPositions();
  mrect.x=xposition;
  mrect.y=yposition;
  
  if (isDebug){  
    println("[main] using cascade: " + cascadeFile);
    cp5.addButton("bAll")
        .setPosition(marginL, marginT+50)
          .setSize(200, 19).setCaptionLabel("RENDER ALL STAGES")
            ;
    cp5.addButton("refresh")
        .setPosition(marginL, marginT+100)
          .setSize(200, 19).setCaptionLabel("refresh")
            ;
  }
  cp5.hide();
  path = sketchPath; // an environment variable
  
  fakeDetectedfaces=new ArrayList<Rectangle>();
  smooth();

  font1 = createFont("Helvetica-Light", 21);
  font2 = createFont("Helvetica", 11);
  courier= createFont("courier",15);
  mgraphics= createGraphics(100,100,P2D);
  minimages= new miniImages();
  minimages.setup();
  setupCurrentIteration();
  frameRate(60);
  
  if (isDebug==false) doProcess=true;
  else{
    yposition+=yAxeMovement;yposition+=yAxeMovement;//yposition+=yAxeMovement;yposition+=yAxeMovement;
    xposition+=xAxeMovement;
  }
  imagebg=loadImage("background.jpg");
  doProcess=true;
} //end setup



public void draw() {

 // println(xposition);
  //actualizacion del rectangulo
    if(stagesCounter>stopStage){ //fin de una etapa toca mover cuadradito
      stagesCounter=0;
      featuresCounter=0;
      xposition+=xAxeMovement;
      if(xposition>img.width-windowsize){ //row jump needed
        xposition=0;
        yposition+=yAxeMovement;
      }
      if(yposition>img.height-windowsize){ //current image has finished
        refresh(); 
        setInitPositions();
      }      
      mrect.x=xposition;
      mrect.y=yposition;
      setupCurrentIteration();

    }

  // Draw
  background(255);
  image(imagebg,0,0,width, height);

  if(frameCount%40==0) slowMotion(false );
   if (doProcess && (frameCount% slowOneOfEach)==0) {
     renderStages(windowsize);
   }
  /****DRAW MAIN PHOTO****/
  pushMatrix();
    translate(740,80);
    image(img, 0, 0);
    noFill();
    stroke(0, 255, 0,10);
    strokeWeight(2);
    for (int i = 0; i < faces.length; i++) {
      rect(faces[i].x, faces[i].y, faces[i].width, faces[i].height);
    }

  //if(frameCount%30==0)
    stroke(0, 255, 0,255);
    strokeWeight(2);
    for (int i = 0; i< fakeDetectedfaces.size(); i++) {
      rect(fakeDetectedfaces.get(i).x, fakeDetectedfaces.get(i).y, fakeDetectedfaces.get(i).width, fakeDetectedfaces.get(i).height);
    }
    
    image(mgraphics,xposition,yposition); //dibuja el canvas con las features haar
  
  
  
    stroke(255,0,0);
    rect(mrect.x,mrect.y,mrect.width,mrect.height);
        

  popMatrix();
 /************/

// Draw zoomed version   
  pushMatrix();  
    translate(55,391);
    img2.filter(GRAY);
    image(img2,0,0,min(faceRectangle.width*2,250),min(faceRectangle.height*2,250));    
    image(mgraphics,0,0,min(faceRectangle.width*2,250),min(faceRectangle.height*2,250));
  popMatrix();
  
if(showInterface){
  int yPos = marginT;
  int xPos = marginL;
  yPos+= 20;
  textFont(font2);
  fill(60);
  text("\"" + cascadeFile + "\"", xPos, yPos);
  fill(255, 0, 0 );
  yPos += 15;
 
}
  textFont(courier);
  fill(255,255,255);
  text(statusMsg, 376, 428);
 /* textFont(font1);
  fill(20);
  text(" El sistema recorre toda la imagen \n buscando patrones de luminosidad \n cada vez en mas detalle ",xPos,yPos+250);
  text(" Compara la intensidad de color bajo las zonas \n blancas y negras y busca que la diferencia \n supere cierto umbral",xPos,yPos+350);
  text(" Para un ordenador, una cara humana s\u00f3lo es un conjunto \n de contrastes dentro de una imagen",xPos,yPos+460);
 */
  if(amIOveraFace){ 
    if( stagesCounter==10 && (featuresCounter==2 || featuresCounter==6)){
      minimages.addImage(img2,mgraphics);
    }
    if( stagesCounter==11 && (featuresCounter==6 ||  featuresCounter==37) ) {
      minimages.addImage(img2,mgraphics); 
    }
  } 
  
  
  pushMatrix();
  translate(746,836);
  minimages.draw();
  popMatrix();
  fill(80);
  if (isDebug==true && showInterface)  debugInfo();

 
}
public void debugInfo(){
  textFont(font2);
  pushMatrix();
  translate(100,20);
   text("framerater+ " + frameRate, 0, 00);
  text("area interseccion" + areaInterseccion,0,20);
  text("faceRectangle size" + windowsize + " face on screen width:" + faces[0].width ,0,40);  
  popMatrix();
}

public void loadNewPhoto(String name){
  opencv = new OpenCV(this, name);  
  opencv.loadCascade(cascadeFile); 
  faces = opencv.detect(1.4f,3,0,100,250);
  faceRectangle=faces[0];
  faces = opencv.detect(1.4f,0,0,100,250);
  mrect=new Rectangle(0,0,100,100);
  img = loadImage(name);

}

//loads new image and define how fast the rectangles will move
public void refresh(){
  currentPhoto++; 
  if(currentPhoto>=fotoFiles.length){
  currentPhoto=0;
  }
  loadNewPhoto(fotoFiles[currentPhoto]); //TODO TAKE NEW PHOTO FROM LISTS OR WHATEVER
  fakeDetectedfaces.clear();
  xposition=0;
  yposition=0;
  //setupCurrentIteration();
  minimages.reset();
  
}

public void setInitPositions(){
  if(faceRectangle.y >100) {
    xAxeMovement=faceRectangle.x/2;
    yAxeMovement=faceRectangle.y/2;
  }
  else{  
      yAxeMovement=100;  
      yposition=faceRectangle.y; 
      xposition=0;
      xAxeMovement=faceRectangle.x/2; 
  }
}

//set what happens when the finding window is over one position of the screen.
public void setupCurrentIteration(){    
  windowsize=faceRectangle.width;
  if(mgraphics.width!=windowsize)  mgraphics = createGraphics(windowsize,windowsize,P2D);
  mrect.width=windowsize;
  mrect.height=windowsize;
  
  
  if(mrect.intersects(faceRectangle)){
    Rectangle interseccion=mrect.intersection(faceRectangle);
    areaInterseccion=(float) (interseccion.width*interseccion.height/2) / (float)(windowsize*windowsize/2); //normalized intersection area
  } 
  else{
    areaInterseccion=0;
  }
  /*** stop  Stage ***/
  stopStage= ceil(map(areaInterseccion,0,1,1,9));
  if(areaInterseccion>0.94f) { //This iteration is over a face
    stopStage=16;
    fakeDetectedfaces.add(new Rectangle( mrect));
    amIOveraFace=true;
  }
  else amIOveraFace=false;
  
  if(stopStage>10) stagesCounter=stopStage-10;   
   img2=createImage(mrect.width,mrect.height,RGB); 
   img2=img.get( mrect.x, mrect.y, mrect.width,mrect.height);
   img2.updatePixels();
}

public void  bAll(){
  if(doProcess)
  doProcess = false;
   else doProcess = true;
}

public void renderStages(int imgSize) {  
 
  if(fileLoaded==false)
    loadFile();
 // imgSize = img.width;//calucated from XML file
  minSize = sampleSize[0];
  stageScale = (float)imgSize/(float)minSize;
     int i=stagesCounter;
    Stage stage = (Stage) stages.get(i);
   
    //println("trees in this stage" + stage.getNumItems() );
    // Render stage
      int j=featuresCounter; 
      Tree tree = stage.getTree(j);
     // println("[main] Processing tree # " + j + " in stage " + (i+1));
      //statusMsg = "[main] Processing tree # " + j + " in stage " + (i+1);
      statusMsg = "ETAPA " + (i+1) + " TEST: " + j + "/"+stage.getNumItems();
     // redraw();
        //mgraphics.clear();
        mgraphics.beginDraw();
        mgraphics.clear();
     //  mgraphics.rect(0,0,100,100);
       mgraphics.endDraw();
       tree.render(imgSize, stageScale, 1,mgraphics);
      
    
    if(featuresCounter<stage.getNumItems() -1) {
      featuresCounter++;
      //println (featuresCounter);
    }
    else{
      featuresCounter=0;      
      if(stagesCounter<=stages.size()-1){ 
        stagesCounter++;      
       // println("cambio a la etapa "+ stagesCounter);
     }
      else{
          //TODO final del proceso
          exit();
      }
    }
}

boolean slowmoDOWN=false;
public void slowMotion(boolean start){
//  println("startslowmo" + slowOneOfEach);
  if(start) slowOneOfEach=2;
  
  if (slowOneOfEach > 1 ){    
    if (slowmoDOWN==false){
      if(slowOneOfEach<7)      slowOneOfEach++;
      else {slowmoDOWN=true;}
    }  else if(slowmoDOWN==true){
      slowOneOfEach--;
    }
  }
  
  if(slowmoDOWN==true){
     if( slowOneOfEach <=1) { 
       slowmoDOWN=false;
     println("end slowmodown");
     }
  }
} 


public void loadFile() {
  String name = cascadeFile;
  String path = sketchPath +"/";
  println(path+name);
  stages = doLoadXML(path+name);
  String[] pieces = split(name, '.');
  name = pieces[0];

 fileLoaded=true;
}

public void keyPressed( ){
    
  if(key== ' '  ){
    println("key pressed");
    showInterface =! showInterface;
    if(showInterface) cp5.setAutoDraw(true);
    else cp5.setAutoDraw(false);
  }if(key== 's' ){
   slowMotion(true); 
  }
  
}
class HaarRect{

  int dx,dy,dw,dh,dweight,sx,sy,sw,sh,sweight;

  HaarRect(int[] d){
    dx = d[0];
    dy = d[1];
    dw = d[2];
    dh = d[3];
    dweight = d[4];
  } 

  public int getArea(){
    return sw*sw; 
  }

  public void setScale(int s){
    sx = PApplet.parseInt( dx * s );
    sy = PApplet.parseInt( dy * s );
    sw = PApplet.parseInt( dw * s );
    sh = PApplet.parseInt( dh * s );
  }

  public void setScaleWeight(int s){
    sweight = dweight * s;
  }

  public Rectangle getRectangle(){
    return new Rectangle(dx,dy,dw,dh); 
  }


}






public class HaarRectSizeComparator implements Comparator {

  public int compare(Object h1, Object h2) {

    int a1 = ((HaarRect) h1).getArea();

    int a2 = ((HaarRect) h2).getArea();

    if (a1 > a2)

      return 1;

    else if (a1 < a2)

      return -1;

    else
      return 0;

  }

}





class Node {
  HaarRect[] haarRects;
  boolean tilted;
  float leftVal, rightVal, threshold;
  int leftNode, rightNode;


  Node(HaarRect[] haarRects, boolean tilted, float threshold, float leftVal, float rightVal, int leftNode, int rightNode) {
    this.haarRects = haarRects;
    this.threshold = threshold;
    this.tilted = tilted;
    this.leftVal = leftVal;
    this.rightVal = rightVal;
    this.leftNode = leftNode;
    this.rightNode = rightNode;
  }

  public Rectangle[] getRectangles() {
    Rectangle[] rects = new Rectangle[haarRects.length];
    for (int i=0;i<haarRects.length;i++) {
      rects[i] = new Rectangle(haarRects[i].dx, haarRects[i].dy, haarRects[i].dw, haarRects[i].dh);
    }
    return rects;
  }

  public HaarRect[] getHaarRectangles() {
    return haarRects;
  }

  public HaarRect[] getHaarRectanglesSorted() {
    HaarRect[] sorted = haarRects;
    Arrays.sort(sorted, new HaarRectSizeComparator());
    return sorted;
  }
}


class Stage{
  ArrayList items;

  Stage(ArrayList items){
    this.items = items;
  }

  public void addItem(Tree tree){
    items.add(tree);
  }
  
  public int getNumItems(){
    return items.size();
  }
 
  public Tree getTree(int id){
    Tree t = (Tree) items.get(id);
    return t;
  }
  
}


class Tree{

  ArrayList nodes;
  float fillAlpha = 1;
  int styleParam;
  int method = 2;

  Tree(ArrayList items){
    nodes = items;//nodes
  }

  public void addItem(ArrayList node){
    nodes.add(node);
  }

  public int getNumItems(){
    return nodes.size();
  }

  public void setAlpha(float f){
    fillAlpha = f;
  }

  public void setStyle(int styleParam){
    this.styleParam = styleParam; 
  }

  public void render(int imgSize,float stageScale,int maxItems, PGraphics pg){

    //PGraphics pg = createGraphics(imgSize,imgSize,P2D);

    for(int i = 0;i<nodes.size();i++){//for each node

      Node node = (Node) nodes.get(i);

      boolean invThreshold = (node.rightVal < node.leftVal) ? true : false;
      invThreshold = false;

      pg.beginDraw();
      pg.strokeWeight(2);
      pg.strokeCap(SQUARE);
      //pg.strokeJoin(MITER);

      pg.scale(stageScale);

      int fillColor = color(255);
      fillColor = color(20,200,20);
      //  HaarRect[] haarRects = node.getHaarRectangles();
      HaarRect[] haarRects = node.getHaarRectanglesSorted();

      for(int j=0;j<haarRects.length;j++){

        Rectangle r = haarRects[j].getRectangle();

        if(invThreshold == false){
          if(haarRects[j].dweight > 0){
            fillColor = color(255);
          } 
          else {
            fillColor = color(0);
          }
        } 
        else {
          if(haarRects[j].dweight > 0){
            fillColor = color(255);
          } 
          else {
            fillColor = color(0);
          }            
        }

        // Use outlines for double nodes
        if(true){
          pg.noStroke();
          pg.fill(fillColor);
        } 
        else {
          pg.stroke(fillColor);
          // pg.strokeCap(SQUARE);
          // pg.strokeJoin(MITER);
          pg.noFill();
        }
        pg.rect(r.x,r.y,r.width,r.height);
        //break;
      }

      pg.endDraw();
      
    }
    //return pg;
  }


}









































// XML Loading Routine



 
public ArrayList doLoadXML(String file){ 
  ArrayList returnList = new ArrayList();
  BufferedReader br;
  XML xml;
  xml=null;
  try{
     br = new BufferedReader(new FileReader(dataPath(file)));
     xml = new XML(br);
  } catch (IOException e) {
      e.printStackTrace();
    }   catch( Exception e){
      e.printStackTrace();
    } 

    xml=xml.getChild(1);
    //println(xml.getName());
  //xml=haarcascade_xxx_default
    String sampleSizeStr = (xml.getChildren("size")[0].getContent().trim());//size
    sampleSize = PApplet.parseInt(split(sampleSizeStr," "));
  //  println(sampleSize);
    XML returnListNode = xml.getChildren("stages")[0];//returnList es stages
    int numReturnList = returnListNode.getChildren("_").length;  
    
  //println("total:" + numReturnList);
  
  for(int i=0;i<numReturnList;i++){//20 returnList in this file //iterate within stages each stage starts with a <_> and a <trees>
    
    XML treesNode = returnListNode.getChildren("_")[i].getChild("trees"); //each tree within treess starts with <_>
    
    int numTrees = treesNode.getChildren("_").length;
    //println("numtrees " + numTrees);
    ArrayList trees = new ArrayList();// holder of nodes

    for(int j=0;j<numTrees;j++){ //iterate over each tree find the nodes inside
      
      XML currentTree=treesNode.getChildren("_")[j];
      //println(currentTree.toString());
      int numNodes = currentTree.getChildren("_").length;    
      //println("nodes inside each tree" + numNodes)  ;
      ArrayList nodes = new ArrayList();
      for(int n=0;n<numNodes;n++){
        XML node = currentTree.getChildren("_")[n];
        XML feature = node.getChild("feature");

        XML rectsNode = feature.getChild("rects");
        int numRects = rectsNode.getChildren("_").length;

        HaarRect[] haarRects = new HaarRect[numRects];
        int[] weights = new int[numRects];

        for(int k=0;k<numRects;k++){
          XML kid = rectsNode.getChildren("_")[k];

          String[] rStr = split(kid.getContent().trim()," ");

          int[] coords = new int[5];

          for(int l = 0;l<coords.length;l++){
            coords[l] = PApplet.parseInt(rStr[l]); 
          }
      //    println(coords);
          HaarRect hr = new HaarRect(coords);
          haarRects[k] = hr;

        }

        boolean tilted = PApplet.parseBoolean(feature.getChild("tilted").getContent());
        float leftVal = 0;
        float rightVal = 0;
        int rightNode = 0;
        int leftNode = 0;
        float threshold = PApplet.parseFloat(node.getChild("threshold").getContent().trim());

        for(int l=3;l<node.getChildCount();l++){
          String nName = node.getChild(l).getName();
          XML kid = node.getChild(l);

          if(node.getName().equals("left_val")){
            leftVal = PApplet.parseFloat(kid.getContent().trim());
          } 
          else if (nName.equals("left_node")){
            leftNode = PApplet.parseInt(kid.getContent().trim());
          } 
          else if(nName.equals("right_val")){
            rightVal = PApplet.parseFloat(kid.getContent().trim());
          } 
          else if(nName.equals("right_node")){
            rightNode = PApplet.parseInt(kid.getContent().trim());
          }
        }//end for rest of inside the node
        nodes.add(new Node(haarRects,tilted,threshold,leftVal,rightVal,leftNode,rightNode));
       // println("haarRects:  "+haarRects +  " tilted "+ tilted +" threshold: "+threshold+ " leftVal: " +leftVal+ " rightVal: " +rightVal+" leftNode: " +leftNode+" rightNode: "+rightNode);
      }// end nodes loop

      trees.add(new Tree(nodes));

    }//end trees loop
    returnList.add(new Stage(trees));
  }//end returnList loop


  println("");
  println("[cascadeLoader] Total stages: "+returnList.size());
  println("[cascadeLoader] Sample size: "+sampleSize[0] + ", "+sampleSize[1]);
  //println("[cascadeLoader] Rows: "+rows+", Columns: "+cols);
  return returnList;
}



class miniImages{
  int initX;
  int initY;
  int imgWidth;
  int imgHeight;
  int maxImages;
  ArrayList <PImage> images;
 
 miniImages(){
     initX=50;
     initY=620;
     imgWidth=100;
     imgHeight=imgWidth;
      maxImages=10;
 }
 
 public void setup(){
   images = new ArrayList<PImage> ();
 }
 
 
 public void reset(){
   images.clear();
   
 }
 
 public void addImage(PImage mImg, PGraphics mImg2){
   if(images.size()>maxImages) return;
   
   PGraphics mgraphic=createGraphics(imgWidth,imgHeight,P2D);
   mgraphic.beginDraw();
   mgraphic.image(mImg,0,0,imgWidth,imgHeight);
   mgraphic.image(mImg2,0,0,imgWidth,imgHeight);
   mgraphic.endDraw();
   images.add(mgraphic );
   //println("images size" + images.size());
 }
 
 public void draw(){
   for(int i=0; i< images.size(); i++){
    // image(images.get(i),50+ i*imgWidth,initY,imgWidth,imgHeight);   
     image(images.get(i), i*(imgWidth+25),0,imgWidth,imgHeight);   
   } 
  
  }

}
  static public void main(String[] passedArgs) {
    String[] appletArgs = new String[] { "--full-screen", "--bgcolor=#666666", "--hide-stop", "HaarcascadeVisualizer" };
    if (passedArgs != null) {
      PApplet.main(concat(appletArgs, passedArgs));
    } else {
      PApplet.main(appletArgs);
    }
  }
}
