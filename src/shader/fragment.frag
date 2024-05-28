#version 330 core

uniform sampler3D volume;
uniform vec3 camera;
uniform vec3 ray;
uniform vec3 up;
uniform vec3 right;
uniform vec3 source;
uniform float step;

out vec4 FragColor;

float getCT(vec3 pos)
{
	vec3 sample = pos-source+vec3(0.5);
	sample.z = (sample.z-0.5+0.26464)/0.26464*0.5;
	return texture(volume, sample).r;
}

void main()
{
	float distance = abs(dot(camera-source, ray));
	vec3 now = camera+distance*right*(gl_FragCoord.x/512-0.5)+distance*up*(gl_FragCoord.y/512-0.5);
	float totalcolor = 0;
	float totalalpha = 0;

	for(int i=0;i<1000;i++)
	{
		now += step*ray;
		if(now.x < source.x-0.5 || now.x > source.x+0.5 || now.y < source.y-0.5 || now.y > source.y+0.5 || now.z < source.z-0.26464 || now.z > source.z+0.26464) continue;
		if(totalalpha >= 1.0) break;

		float CT = getCT(now);
		float alpha = CT*0.1;
		totalcolor += CT*alpha*(1.0-totalalpha);
		totalalpha += alpha*(1.0-totalalpha);
	}
	//FragColor = vec4(start,1);
	FragColor = vec4(totalcolor, totalcolor, totalcolor, 1.0);
}