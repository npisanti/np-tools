
#ifdef GL_ES
precision highp float;
#endif

uniform vec2 u_resolution;

uniform sampler2D u_tex0;

uniform float u_border;

void main(){
    
    vec2 st = gl_FragCoord.xy/u_resolution;
    vec4 source = texture2D( u_tex0, st ); // for texture access
    
    float framing = (smoothstep( 0.0, u_border, st.x ) 
                  - smoothstep( 1.0-u_border, 1.0, st.x ) )
                  * (smoothstep( 0.0, u_border, st.y ) 
                  - smoothstep( 1.0-u_border, 1.0, st.y ));

    gl_FragColor = vec4( source.rgb, source.a * framing );
  
}
