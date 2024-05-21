#version 330 core

uniform sampler3D volume;
uniform vec3 camera;
uniform vec3 ray;
uniform float step;

out vec4 FragColor;

float getCT(vec3 pos)
{
	return texture(volume, pos).r;
}

void main()
{
	vec3 start = vec3(gl_FragCoord.x/512, gl_FragCoord.y/512, 0.5);
	vec3 now = start;
	float color = 0;
	float alpha = 0;

	for(int i=0;i<271;i++)
	{
		float CT = getCT(now);
			color = CT;
			alpha = CT;
			break;
		//color += CT;
		//alpha += CT;
		//if(alpha >= 1) break;
		now += step*ray;
	}
	FragColor = vec4(color, color, color, alpha);
}