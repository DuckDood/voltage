#version 330 core

in vec2 v_texcoord;
out vec4 fragColor;
uniform sampler2D tex;

mat4 bayer = mat4(
	0.,8.,2.,10.,
	12.,4.,14.,6.,
	3.,11.,1.,9.,
	15.,7.,13.,5.
);

void main() {
	float reduction = 1.;

    vec4 pixColor = texture2D(tex, v_texcoord);

	//pixColor = vec4(vec3(pixColor.x * 0.3 + pixColor.y * 0.6 + pixColor.z * 0.1), 1.);

	vec2 pixelPos = vec2(gl_FragCoord);
	float diff = 0.5-((1./16.) * bayer[int(pixelPos.y)%4][int(pixelPos.x)%4]);
	
	pixColor += vec4(vec3(diff/reduction), 0.);

	pixColor *= reduction;
	pixColor = round(pixColor);
	pixColor /= reduction;

	/*if(pixColor.x == 0.) {
		pixColor.x = 1./255.;
		pixColor.y = 33./255.;
		pixColor.z = 8./255.;
	} else
	if(pixColor.x == 0.5) {
		pixColor.x = 42./255.;
		pixColor.y = 103./255.;
		pixColor.z = 34./255.;
		
	} else
	if(pixColor.x == 1.) {
		pixColor.x = 105./255.;
		pixColor.y = 236./255.;
		pixColor.z = 88./255.;
		
	}*/

	pixColor.a = 1.;

	fragColor = pixColor;
}

