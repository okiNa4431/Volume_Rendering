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
	vec3 start = vec3(gl_FragCoord.x/512+camera.x, gl_FragCoord.y/512+camera.y, 0.0);
	vec3 now = start;
	float totalcolor = 0;
	float totalalpha = 0;

	for(int i=0;i<271;i++)
	{
		if(now.x < 0.0 || now.x > 1.0 || now.y < 0.0 || now.y > 1.0 || now.z < 0.0 || now.z > 1.0) continue;
		if(totalalpha >= 1.0) break;

		float CT = getCT(now);
		float alpha = CT*0.01;
		totalcolor += CT*alpha*(1.0-totalalpha);
		totalalpha += alpha*(1.0-totalalpha);
		now += step*ray;
	}
	FragColor = vec4(1);
	//if(totalalpha > 0.0) FragColor = vec4(totalcolor, totalcolor, totalcolor, totalalpha);
	//else FragColor = vec4(0, 0, 0, 1);
}