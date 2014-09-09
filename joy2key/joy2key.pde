/**
 * oscP5sendreceive by andreas schlegel
 * example shows how to send and receive osc messages.
 * oscP5 website at http://www.sojamo.de/oscP5
 */
 
import oscP5.*;
import netP5.*;
  int mx,my;
OscP5 oscP5;
NetAddress myRemoteLocation;
      int previousval0=0;
    int previousval1=0;
void setup() {
  size(400,400);
  frameRate(25);
  /* start oscP5, listening for incoming messages at port 12000 */
  oscP5 = new OscP5(this,12000);
  
  /* myRemoteLocation is a NetAddress. a NetAddress takes 2 parameters,
   * an ip address and a port number. myRemoteLocation is used as parameter in
   * oscP5.send() when sending osc packets to another computer, device, 
   * application. usage see below. for testing purposes the listening port
   * and the port of the remote location address are the same, hence you will
   * send messages back to this sketch.
   */
  myRemoteLocation = new NetAddress("127.0.0.1",33333);

}

void draw() {
  background(0);  
  fill(255,0,0);
  ellipse(mx,my,5,5);
}

void mousePressed() {
  /* in the following different ways of creating osc messages are shown by example */
  OscMessage myMessage = new OscMessage("/test");
  
  myMessage.add(123); /* add an int to the osc message */

  /* send the message */
  oscP5.send(myMessage, myRemoteLocation); 
}

void keyPressed(){
  println("pressd: "+ key);
if(key=='w')
  my--;
else if(key=='s')
my++;
else if(key=='a')
mx--;
else if(key=='d')
mx++;



}
void keyReleased(){
}
/* incoming osc message are forwarded to the oscEvent method. */
void oscEvent(OscMessage m) {
  /* print the address pattern and the typetag of the received OscMessage */
 // print("### received an osc message.");
 // println(" addrpattern: "+m.addrPattern());
  //println(" typetag: "+m.typetag());
  int value;  
      // check for mouse moved message            
      if(m.checkAddrPattern("/joystick/0/axis/0")==true){
        //println("axis 0");
          value = (int)m.get(0).floatValue();
          //println(value);
          if(value==1){
              key='d';
              keyPressed();                  
          }
          else if(value==0){                    
              if(previousval0>0){
                  key='d';
                  keyReleased();
                  key='a';
                   keyReleased();
              }
          }
          if(value==-1){
              key='a';
              keyPressed();      
          }
          if(value!=0 && previousval0<2) previousval0++;
          if(value==0 && previousval0>0) previousval0--;
      }
      else if(m.checkAddrPattern( "/joystick/0/axis/1")==true){
          value = (int)m.get(0).floatValue();
          if(value==1){
              key='s';
              keyPressed();
          }
          else if(value==0){
              if(previousval1>0){
                 key='w';
                  keyReleased();
                  key='s';
                   keyReleased();
              }
          }
          if(value==-1){
              key='w';
              keyPressed();
          }
          if(value!=0 && previousval1<2) previousval1++;
          if(value==0 && previousval1>0) previousval1--;
      }
      else if(m.checkAddrPattern( "/joystick/0/JOYBUTTONDOWN")==true){ 
           value = (int)m.get(0).intValue();
          if(value==0){
              key='e';
              keyPressed();
          }
          else if(value==1){
              key='r';
              keyPressed();
          }
          else if(value==2){
              key='t';
              keyPressed();
          }
          else if(value==3){
              key='y';
              keyPressed();
          }
      }
      else if(m.checkAddrPattern( "/joystick/0/JOYBUTTONUP")==true){ 
          value = (int)m.get(0).intValue();
          if(value==0){
               key='e';
                   keyReleased();
          }
          else if(value==1){
               key='r';
                   keyReleased();
          }
          else if(value==2){
               key='t';
                   keyReleased();
          }
          else if(value==3){
               key='y';
                   keyReleased();
          }
      }
      else{
        println("no hay patron");
      }       
      
}
