#version 310 es
precision mediump float;

out vec4 FragColor;
in float Height;
out float h;

void main()
{
     h = (Height + 16.0f)/32.0f;	// shift and scale the height into a grayscale value
    
    vec4 color;
    if (Height < 0.1) {
        color = vec4(0.2, 0.8, 0.2, 0.8); // blue for lower terrain
    } else if (Height < 0.2) {
        color = vec4(0.5, 0.5, 0.5, 1.0); // gray for mid-level terrain
    } else {
        color = vec4(0.0, 1.0, 0.2, 1.0); // green for higher terrain
    }
    
    FragColor = color;
}