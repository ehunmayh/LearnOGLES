attribute vec4 position;
attribute vec4 color;
uniform mat4 size;
uniform mat4 U_ModeMatrix;
uniform mat4 U_ViewMatrix;
uniform mat4 U_ProjectionMatrix;
varying vec4 V_color;
void main(){
   V_color=color;
   gl_Position=U_ProjectionMatrix*U_ViewMatrix*U_ModeMatrix*position;
}