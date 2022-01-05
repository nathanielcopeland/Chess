#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

// texture samplers
uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
	// linearly interpolate between both textures (80% container, 20% awesomeface)
	//FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.9);

	vec4 AlphaColor = texture(texture2, TexCoord);
    if(AlphaColor.a < 0.1) {
        FragColor = texture(texture1, TexCoord);
    } else {
        FragColor = mix(texture(texture1, TexCoord), AlphaColor, 0.9);
    }
}