uniform sampler2DRect tex1;
uniform float glowSize;

void main()
{
    vec4 sum = vec4(0);
    vec2 texcoord = vec2(gl_TexCoord[0]);
    int j;
    int i;
    
    for( i= -4 ;i < 4; i++)
    {
        for (j = -3; j < 3; j++)
        {
            sum += texture2DRect(tex1, texcoord + vec2(j, i)*0.4) * glowSize;
        }
    }
    if (texture2DRect(tex1, texcoord).r < 0.3)
    {
        gl_FragColor = sum*sum*0.12 + texture2DRect(tex1, texcoord);
    }
    else
    {
        if (texture2DRect(tex1, texcoord).r < 0.5)
        {
            gl_FragColor = sum*sum*0.9 + texture2DRect(tex1, texcoord);
        }
        else
        {
            gl_FragColor = sum*sum*0.75 + texture2DRect(tex1, texcoord);
        }
    }
}