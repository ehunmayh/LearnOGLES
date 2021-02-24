attribute vec4 position;
attribute vec4 color;
uniform mat4 size;
uniform mat4 U_ModeMatrix;
uniform mat4 U_ViewMatrix;
uniform mat4 U_ProjectionMatrix;
void main(){
   gl_Position=U_ProjectionMatrix*U_ViewMatrix*U_ModeMatrix*position;
}