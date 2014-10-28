//THE CLASS
class Ball {
  float x;
  float y;
  float z;
  float incX;
  float incY;
  float incZ;
 
  void setup() {
    x = random(width);
    y = random(height);
    z = random(width);
    setInc();
  }
 
  //speed
  void setInc() {
    incX = random(-6, 6);
    incY = random(-8, 8);
    incZ = random(-10,10);
  }
 

 
  //direction
  void step() {
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
