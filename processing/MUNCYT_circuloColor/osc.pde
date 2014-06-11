import oscP5.*;
import netP5.*;
  
OscP5 oscP5;
NetAddress myRemoteLocation;
String  IPEnvio = "localhost";
int     puertoEnvio = 9000;

int     puertoEscucha = 9000;

void setupOSC() {
  oscP5 = new OscP5(this,puertoEscucha);
  
}

/* incoming osc message are forwarded to the oscEvent method. */
void oscEvent(OscMessage oscMsg) {
  /* print the address pattern and the typetag of the received OscMessage */
  print("### received an osc message.");
  print(" addrpattern: "+oscMsg.addrPattern());
  println(" typetag: "+oscMsg.typetag());
  
  // coger valores segun el tag
  for(int i=0; i<oscMsg.arguments().length; i++) {
    String valor = "";
    switch(oscMsg.typetag().charAt(i)) {
      case 's':
        valor = oscMsg.get(i).stringValue();
        break;
      case 'f':
        valor = ""+oscMsg.get(i).floatValue();
        break;
      case 'i':
        valor = ""+oscMsg.get(i).intValue();
        break;
    }
    println("valor en pos: " + i + " --> " + valor);
  }  
  
  if(oscMsg.addrPattern().equals("/position") ){
    float ang = oscMsg.get(0).floatValue();
    wAng = oscMsg.get(1).floatValue();
  } 
  else 
  if(oscMsg.addrPattern().equals("/explosion") ){
    // Llega bang (sin datos de interés)
    // Ejemplo de animacion:
    // - Cambiar color fondo a uno con brillo y saturacion a 100
  }
 
  if(oscMsg.addrPattern().equals("/modo")) {
    int modo_Rcv = oscMsg.get(0).intValue();
  }  
  
}

