import oscP5.*;
import netP5.*;
  
OscP5 oscP5;
NetAddress myRemoteLocation;
String  IPEnvio = "localhost";
int     puertoEnvio = 9000;

int     puertoEscucha = 9000;

void setupOSC() {
  oscP5 = new OscP5(this,puertoEscucha);
//EM  
  oscP5.plug(this,"startEM","/sync/electromagnetism/start_event"); //direccionamiento con más sentido  
  oscP5.plug(this,"drawWaves","/sync/electromagnetism/number_waves"); //direccionamiento con más sentido

//MENU
 
oscP5.plug(this,"drawMenu","/sync/menu/start_event"); 
//Gravedad
  oscP5.plug(this,"startGravity","/sync/gravity/start_event");
  oscP5.plug(this,"blackHole","/sync/gravity/collapse_start_event");
  oscP5.plug(this,"drawOff","/sync/gravity/collapse_stop_event");

// Nuclear Fuerte
  oscP5.plug(this,"exchangeColors","/sync/strong_nuclear/hand_on_event");

//Nuclear Debil
  oscP5.plug(this,"Explosion","/sync/weak_nuclear/chain_reaction_event"); //
  oscP5.plug(this,"drawRotating","/sync/weak_nuclear/ball"); 
}

// si llga un evento test con dos parametros i1 e i2 se vendría a esta función por el plug de antes
void startEM(){
  bdrawNoise=true;
  fade=1;  
}

void drawWaves(float f){
    if(debugOSC) println("Function: drawWaves: " + f);
    bdrawNoise=false;
    bdrawWaves=true;
    nWaves=(int)f;
}

void drawMenu(){
    if(debugOSC) println("Function: drawMenu: ");
    bDrawRandomParticles=true;
}

void Explosion(){  
  movieExp.play();
  bDrawMovie=true;
}

void startGravity(){
 bDrawGridImage=true; 
}

void blackHole(){
bDrawGridCircles=true;
}

void drawOff(){
  bdrawOff=true;
}

void drawRotating(float angle, float speed){
   println ("draw rotating" + angle + " " + speed);
   rotatePos=round(map(angle,0,2*PI,0,lengthPared));
   bDrawLine=true;
}

void exchangeColors(float time, float section, float color1, float color2){
  println ("draw exchangeColors" + time + " " + section);
  exchangeTime= time;
  exchangeSection=(int)section; 
  setupExchange((int)section);
  color mcolor1;
  if(color1==1) mcolor1=color(150,0,0);
  if(color1==2) mcolor1=color(0,150,0);
  else mcolor1=color(0,0,150);
  color mcolor2;
  if(color2==1) mcolor2=color(150,0,0);
  if(color2==2) mcolor2=color(0,150,0);
  else mcolor2=color(0,0,150);
  
  exchangeC1=(int) mcolor1; 
  exchangeC2=(int) mcolor2;

  bExchange=true;
}
/* incoming osc message are forwarded to the oscEvent method. */
void oscEvent(OscMessage oscMsg) {
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

void scene1(){
  
}

void scene2(){

}


