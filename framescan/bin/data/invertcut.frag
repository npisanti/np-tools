
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
    
    vec3 color = vec3( 1.0 - source.r, 1.0 - source.g, 1.0 - source.b );
    
    float luminance = color.r*0.299 + color.g*0.587 + color.b*0.114;	
    
    float alpha = smoothstep( u_low, u_high, luminance );
    
    gl_FragColor = vec4( vec3(1.0), alpha );
  
}
