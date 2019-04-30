#version 460

void main( void )
{
    // Comment to force recompile
    // sdfjkahsdfkddjhzddskdfadssaaadhdzzdsd
    
    const vec4 vertices[] = vec4[]( vec4( 0.95,-0.933, 0.6, 1.0 ),
                                    vec4(-0.95,-0.933, 0.6, 1.0 ),
                                    vec4( 0.95, 0.933, 0.6, 1.0 ) ) ;

    gl_Position = vertices[gl_VertexID] ;
}
