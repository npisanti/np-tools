
#ifdef GL_ES
precision highp float;
#endif

uniform vec2 u_resolution;

uniform sampler2D u_tex0;

uniform float u_low;
uniform float u_high;
uniform float u_border;

void main(){
    
    vec2 st = gl_FragCoord.xy/u_resolution;
    vec4 source = texture2D( u_tex0, st ); // for texture access
    
    vec3 color = source.rgb;
    
    float luminance = color.r*0.299 + color.g*0.587 + color.b*0.114;	

    float alpha = smoothstep( 0.6, 0.9, luminance );
    
    alpha = floor( alpha * 3.1 ) * 0.25;
    
    gl_FragColor = vec4( 1.0, 1.0, 1.0, alpha );
  
}
