// vertex shader
uniform vec3      iResolution;           // viewport resolution (in pixels)
uniform float     iGlobalTime;           // shader playback time (in seconds)
uniform vec4      iMouse;  
uniform sampler2D tex0;          // input channel. XX = 2D/Cube

// https://www.shadertoy.com/view/4dlGW2

// Tileable noise, for creating useful textures. By David Hoskins, Sept. 2013.
// It can be extrapolated to other types of randomised texture.

#define SHOW_TILING
#define TILES 2.0 // Use 1.0 for normal tiling across whole texture.

//----------------------------------------------------------------------------------------
float Hash(in vec2 p, in float scale)
{
    // This is tiling part, adjusts with the scale...
    p = mod(p, scale);
    return fract(sin(dot(p, vec2(27.16898, 28.90563))) * 44549.5473453);

//  Another hash function...
//  p += vec2(47.0, 577.0);
//  p = p*p;
//  return fract( p.x * p.y * .0123983);
}

//----------------------------------------------------------------------------------------
float Noise(in vec2 p, in float scale )
{
    vec2 f;
    
    p *= scale;

    // Movement, just for tiling demonstation...
    p += vec2(sin(iGlobalTime * .3+scale), cos(iGlobalTime * .33+scale))*(scale*.1) + iMouse.xy*.1;
    
    f = fract(p);       // Separate integer from fractional
    p = floor(p);
    
    f = f*f*(3.0-2.0*f);    // Cosine interpolation approximation
    //f = (1.0-cos(f*3.1415927)) * .5;
    
    float res = mix(mix(Hash(p,                  scale),
                        Hash(p + vec2(1.0, 0.0), scale), f.x),
                    mix(Hash(p + vec2(0.0, 1.0), scale),
                        Hash(p + vec2(1.0, 1.0), scale), f.x), f.y);
    return res;
}

//----------------------------------------------------------------------------------------
float fBm(in vec2 p)
{
    float f = 0.0;
    // Change starting scale to any integer value...
    float scale = 14.0;
    float amp   = 0.57;
    
    for (int i = 0; i < 8; i++)
    {
        f += Noise(p, scale) * amp;
        amp *= .5;
        // Scale must be multiplied by an integer value...
        scale *= 2.0;
    }
    // Clamp it just in case....
    return min(f, 1.0);
}

//----------------------------------------------------------------------------------------
void main(void)
{
    vec2 uv = gl_FragCoord.xy / iResolution.xy;

    #ifdef SHOW_TILING
    uv *= TILES;
    #endif
    
    // Do the noise cloud (fractal Brownian motion)
    float bri = fBm(uv);
    
    bri = pow(bri, 1.4); // ...cranked up the contrast for demo.
    vec3 col = vec3(bri);
    
    #ifdef SHOW_TILING
    // Flash tile borders...
    vec2 pixel = TILES / iResolution.xy;
    if (mod(iGlobalTime-2.0, 4.0) < 2.0)
    {
        vec2 first      = step(pixel, uv);
        uv  = step(fract(uv), pixel);   // Only add one line of pixels per tile.
        col = mix(col, vec3(1.0, 1.0, 0.0), (uv.x + uv.y) * first.x * first.y);
    }
    #endif

    gl_FragColor = vec4(col,1.0);
}