

uniform vec3      iResolution;           // viewport resolution (in pixels)
uniform float     iGlobalTime;           // shader playback time (in seconds)
uniform vec4      iMouse;  
uniform sampler2D tex0;          // input channel. XX = 2D/Cube
uniform float     alpha; 
// vertex shader

float grayComponent;
 
float rand( vec2 co)
{
  // I got this technique online ... don't know if there is anything magical about the constants
    // or if it's just thrown in junk...
    float returnVal = fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
    return returnVal;

}
 
void main()
{
    grayComponent = rand(vec2(gl_FragCoord.x + iGlobalTime, gl_FragCoord.y+ iGlobalTime));
    gl_FragColor = vec4(grayComponent, grayComponent, grayComponent, alpha);
}