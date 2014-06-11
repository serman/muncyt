// File con métodos para crear y dibujar animaciones 
// y para animaciones de background

// - - - Background anims - - - 

void updateBackground() {
  colorMode(HSB, 360,100,100);
  float hh = hue(cBackground);
  float ss = saturation(cBackground);
  float bb = brightness(cBackground);
  
//  cBackground = color(;
  
  
}

void setBackgrndTgt(float hh, float ss, float bb) {
  hTgt = hh;
  sTgt = ss;
  bTgt = bb; 
} 

//  - - - Patrones - - - 

void crearPatronConcentrico(float radMax, float nRads, float nAngs) {
  
  pushStyle();
  colorMode(HSB,360,10,100);
  
  psConcentrico = createShape();
  
  psConcentrico.beginShape(QUAD_STRIP);

  psConcentrico.stroke(255);
  psConcentrico.strokeWeight(3);
  
  psConcentrico.fill(255,0,0);
  
  // forma radial
  // centrada en (0,0)
  float dRads = radMax/nRads;
  float dAngs = TWO_PI/nAngs;

  for(int j=0; j<nRads; j++) {
    psConcentrico.beginShape(QUAD_STRIP);
    float rad = dRads*j; 
    for(int i=0; i<=nAngs; i++) {
      float ang=i*dAngs;
      psConcentrico.fill(map(rad,0,radMax,0,360), 100,100);
      psConcentrico.vertex(rad*cos(ang),rad*sin(ang)); 
      psConcentrico.fill(map(rad+dRads,0,radMax,0,360), 100,100);
      psConcentrico.vertex((rad+dRads)*cos(ang),(rad+dRads)*sin(ang)); 
    }
    psConcentrico.endShape(CLOSE);    
  }
  psConcentrico.endShape();
  
  popStyle();    // Fin modo HSB
  
}

void creaPatronRadial(float radMax, float angRango, int nAngs, float nRamas, color cFill) {
  // Dibuja quesitos
  pushStyle();
  colorMode(HSB,360,10,100);
  
  psRadial = createShape(GROUP);
  
  float dAngRama = TWO_PI / nRamas;
  for(int j=0; j<nRamas; j++) {
    PShape rama = createShape();
    rama.beginShape();
//    rama.stroke(255,0,100);
    rama.noStroke();
    rama.rotate(j*dAngRama);
    rama.fill(cFill);
    rama.vertex(0,0);
    for(int k=0; k<=nAngs; k++) {
      rama.vertex(radMax*cos(angRango*k/nAngs),radMax*sin((angRango*k/nAngs)));
    }
    rama.endShape(CLOSE);
    psRadial.addChild(rama);
  }     
//  psRadial.endShape();
  
  popStyle();    // Fin modo HSB
 
}

void creaPatronLineas(float radMax, float ancho, float sep) {
  // dibuja lineas horizontales y verticales que emanan del centro
  psLines = createShape();
  psLines.beginShape(QUADS);
  
  psLines.fill(255);
  
    psLines.noStroke();
    for(float i=0; i<radMax+sep+ancho; i+=sep+ancho) { 
      psLines.vertex(i,-radMax);
      psLines.vertex(i, radMax);
      psLines.vertex(i+ancho, radMax);
      psLines.vertex(i+ancho, -radMax);
    }
  
  
//  int n = 30;
//  for(int i=0; i<30; i++) {
//    float yy = 1200/n*i;
//    vertex(-1000, yy);  
//    vertex(width+1000, yy+ random(-4,4));
//    vertex(width+1000, yy+ random(-14,4) + height/n);
//    vertex(-1000, yy + random(-4,14) + height/n);
//  }
  
  psLines.endShape(CLOSE);
  
}

void creaPatronPuntos(float radMax, int nPts, float rPts) {
  // dibuja puntos
  psPuntos = createShape();
  psPuntos.beginShape(POINTS);
  
  pushStyle();
  psPuntos.stroke(0);
  psPuntos.fill(0);
  for(int i=0; i<nPts; i++) {
    float angt = random(TWO_PI);
    float roht = random(radMax);
    float xx = roht*cos(angt);
    float yy = roht*sin(angt);

    psPuntos.strokeWeight(2);    
    psPuntos.vertex(xx,yy);
  }
  popStyle();  
  psPuntos.endShape();
  
}









//
// ...En reserva...
// Dibujo directo.
// Ahora dibujo en PShape
//
void drawCirculoColor() {
  
  stroke(255);
  strokeWeight(4);
  
  fill(255,0,0);
  
  // forma radial
  // centrada en la pantalla
  float cx = width/2;
  float cy = height/2;
  int nRads = 10;
  int nAngs = 30;
  float radMax = width/2;
  float dRads = radMax/nRads;
  float dAngs = TWO_PI/nAngs;

  pushStyle();
  colorMode(HSB,360,10,100);
  pushMatrix();  
  translate(cx, cy);
  for(int j=0; j<nRads-1; j++) {
    beginShape(QUAD_STRIP);
    float rad = dRads*j; 
    for(int i=0; i<=nAngs; i++) {
      float ang=i*dAngs;
      vertex(rad*cos(ang),rad*sin(ang)); 
      fill(map(rad,0,radMax,0,360), 100,100);
      vertex((rad+dRads)*cos(ang),(rad+dRads)*sin(ang)); 
      fill(map(rad+dRads,0,radMax,0,360), 100,100);
    }
    endShape(CLOSE);    
  }
  popMatrix();  
  
  popStyle();

}

