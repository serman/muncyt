import oscP5.*;
import netP5.*;
  
OscP5 oscP5;
NetAddress myRemoteLocation;
String  IPEnvio = "localhost";
int     puertoEnvio = 9000;

int     puertoEscucha = 9000;

void setupOSC() {
  oscP5 = new OscP5(this,puertoEscucha);
  oscP5.plug(this,"test","/test"); //direccionamiento con más sentido
  
}

// si llga un evento test con dos parametros i1 e i2 se vendría a esta función por el plug de antes
void test(int i1, int i2){

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



