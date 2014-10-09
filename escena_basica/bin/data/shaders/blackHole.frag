#version 120
#extension GL_ARB_texture_rectangle : enable
//---  Galaxy --- Fabrice NEYRET  august 2013
uniform vec3      iResolution;           // viewport resolution (in pixels) SI
uniform float     iGlobalTime;           // shader playback time (in seconds)
uniform sampler2DRect iChannel1;          // input channel. XX = 2D/Cube
uniform sampler2DRect tex0;
uniform float BULB_R ; //ratio del sol central

const float RETICULATION = 5.;  // strenght of dust texture
const float NB_ARMS = 1.;       // number of arms
//const float ARM = 3.;         // contrast in/out arms
const float COMPR = .2;         // compression in arms
const float SPEED = .1;
const float GALAXY_R = 1./2.;

const vec3 GALAXY_COL = vec3(.9,.9,1.); //(1.,.8,.5);
const vec3 BULB_COL   = vec3(0.,.0,.0);
const vec3 SKY_COL    = .5*vec3(.1,.3,.5);

#define Pi 3.1415927
float t = iGlobalTime;

// --- base noise
float tex(vec2 uv)
{
	float n = texture2DRect(iChannel1,uv*5).r;
	
#define MODE 3  // kind of noise texture
#if MODE==0         // unsigned
#define A 2.
	return n;
#elif MODE==1       // signed
#define A 3.
	return 2.*n-1.;
#elif MODE==2       // bulbs
#define A 3.
	return abs(2.*n-1.);
#elif MODE==3       // wires
#define A 1.5
	return 1.-abs(2.0*n-1.);
#endif
}


// --- perlin turbulent noise + rotation
float noise(vec2 uv)
{
	float v=0.;
	float a=-SPEED*t,	co=cos(a),si=sin(a);
	mat2 M = mat2(co,-si,si,co);
	const int L = 7;
	float s=1.;
	for (int i=0; i<L; i++)
	{
		uv = M*uv;
		float b = tex(uv*s);
		v += 1./s* pow(b,RETICULATION);
		s *= 2.;
	}
	
    return v/2.;
}

bool keyToggle(int ascii)
{
	return true; //(texture2DRect(iChannel2,vec2((.5+float(ascii))/256.,0.75)).x > 0.);
}

void main(void)
{
	vec2 uv = gl_FragCoord.xy/iResolution.y -vec2(.8,.5);
	vec3 col;
	
	// spiral stretching with distance
	float rho = length(uv); // polar coords
	float ang = atan(uv.y,uv.x);
	float shear = 2.*log(rho); // logarythmic spiral
	float c = cos(shear), s=sin(shear);
	mat2 R = mat2(c,-s,s,c);
    
	// galaxy profile
	float r; // disk
	r = rho/GALAXY_R; float dens = exp(-r*r);
	r = rho/BULB_R;	  float bulb = exp(-r*r);
	float phase = NB_ARMS*(ang-shear);
	// arms = spirals compression
	ang = ang-COMPR*cos(phase)+SPEED*t;
	uv = rho*vec2(cos(ang),sin(ang));
	// stretched texture must be darken by d(new_ang)/d(ang)
	float spires = 1.+NB_ARMS*COMPR*sin(phase);
	// pires = mix(1.,sin(phase),ARM);
	dens *= .9*spires;
	
	// gaz texture
	float gaz = noise(.09*1.2*R*uv);
	float gaz_trsp = pow((1.-gaz*dens),2.);
    
	// stars
	//float a=SPEED*t, co=cos(a),si=sin(a);
	//mat2 M = mat2(co,-si,si,co);
	// adapt stars size to display resolution
	float ratio = .8;//*iResolution.y;// /iChannelResolution[0].y;
	float stars1 = texture2DRect(iChannel1,ratio*uv+.5).r; // M*uv
    float stars2 = texture2DRect(iChannel1,ratio*uv+.5).r;
    float stars = pow(1.-(1.-stars1)*(1.-stars2),5.);
	
	//stars = pow(stars,5.);
	
	// keybord controls (numbers)
//	if (keyToggle(49))
  //      gaz_trsp = 1./1.7;
//	if (keyToggle(50))
 //       stars = 0.;
//	if (keyToggle(51))
  //      bulb = 0.;
//	if (keyToggle(52))
        dens = .3*spires;
	
	// mix all
	col = mix(SKY_COL,
			  gaz_trsp*(1.7*GALAXY_COL) + 1.2*stars,
			  dens);
	col = mix(col, 1.2*BULB_COL, bulb);
    
	gl_FragColor = vec4(col,1.);
}