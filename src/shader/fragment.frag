#version 330 core

uniform sampler3D volume;
uniform vec3 camera;
uniform vec3 ray;
uniform vec3 up;
uniform vec3 right;
uniform vec3 source;
uniform float step;
uniform vec2 window;
uniform vec3 box;

out vec4 FragColor;

float getCT(vec3 pos)
{
	vec3 sample = pos-source;
	sample.x = (sample.x+box[0])/box[0]*box[0];
	sample.y = (sample.y+box[1])/box[1]*box[0];
	sample.z = (sample.z+box[2])/box[2]*box[0];
	return texture(volume, sample).r;
}

void main()
{
	float distance = abs(dot(camera-source, ray));
	vec3 now = camera+distance*right*(gl_FragCoord.x/window.x-0.5)*window.x/window.y+distance*up*(gl_FragCoord.y/window.y-0.5);
	float totalcolor = 0;
	float totalalpha = 0;

	for(int i=0;i<1000;i++)
	{
		now += step*ray;
		if(now.x < source.x-box[0] || now.x > source.x+box[0] || now.y < source.y-box[1] || now.y > source.y+box[1] || now.z < source.z-box[2] || now.z > source.z+box[2]) continue;
		if(totalalpha >= 1.0) break;

		float CT = getCT(now);
		if(CT < 0.55) continue;
		float alpha = CT*0.1;
		totalcolor += CT*alpha*(1.0-totalalpha);
		totalalpha += alpha*(1.0-totalalpha);
	}
	FragColor = vec4(totalcolor, totalcolor, totalcolor, 1.0);
}