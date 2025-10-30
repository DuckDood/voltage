#version 330 core

in vec2 v_texcoord;
out vec4 fragColor;
uniform sampler2D tex;
uniform float time;

float bayer[4*4] = float[4*4](
	0.,8.,2.,10.,
	12.,4.,14.,6.,
	3.,11.,1.,9.,
	15.,7.,13.,5.
);

float rand(vec2 seed) {
	return fract(sin(dot(seed.xy, vec2(14.1409, 82.1424))) * 44351.102318290490);
}

void main() {
	vec2 uv = v_texcoord;
	vec2 uv2 = uv;
	uv.x *= 320;
	uv.x = floor(uv.x);
	uv.x /= 320;

	uv.y *= 240;
	uv.y = floor(uv.y);
	uv.y /= 240;
	vec2 d = v_texcoord - 0.5;
	float rad = dot(d,d);
	uv+=d * rad * 0.3;
	uv2+=d * rad * 0.3;

	if(uv2.x > 1 || uv2.x < 0 || uv2.y > 1 || uv2.y < 0) {
		fragColor = vec4(0,0,0,1);
		return;
	}


	float scanline = max(sin(uv2.y * 400 + time * 1.45), 0);
	vec4 pixColor = texture(tex, uv);
	pixColor = mix(pixColor+0.03, vec4(0,0,0,1), scanline - 0.5);

	vec2 offSet = vec2(0.004, 0.002);
    vec4 pixColor2 = texture2D(tex, uv-offSet);

	scanline = max(sin((uv2.y - offSet.y) * 400 + time*1.45), 0);
	pixColor2 = mix(pixColor2 + 0.03, vec4(0,0,0,1), scanline - 0.5);
    vec4 pixColor3 = texture2D(tex, uv+offSet);

	scanline = max(sin((uv2.y + offSet.y) * 400 + time*1.45), 0);
	pixColor3 = mix(pixColor3 + 0.03, vec4(0,0,0,1), scanline - 0.5);

	pixColor.x = pixColor2.x;
	pixColor.z = pixColor3.z;

	pixColor += vec4(vec3(rand(uv2 + float(int(time)%100) + pixColor2.zy + rad)), 1) * 0.04;
	pixColor *= 8;
	pixColor = round(pixColor);
	pixColor /= 8;
	pixColor += vec4(vec3(rand(uv2 + float(int(time)%100) + pixColor2.zy + rad)), 1) * 0.02;


	fragColor = pixColor;
}

