#version 330 core

const float PI = 3.1415926535897932384626433832795;

uniform vec3 light_color;
uniform vec3 light_position;
uniform vec3 light_direction;
uniform vec3 spotlight_color;
uniform vec3 spotlight_position;
uniform vec3 spotlight_direction;
uniform vec3 radial_light_color;
uniform vec3 radial_light_position;
uniform vec3 radial_light_direction;

uniform float shading_ambient_strength;
uniform float shading_diffuse_strength;
uniform float shading_specular_strength;
uniform float specularAlpha;

uniform float light_near_plane;
uniform float light_far_plane;
uniform float light_cutoff_outer = 0.88;
uniform float light_cutoff_inner = 0.93;

uniform vec3 view_position;

uniform sampler2D shadow_map;
uniform sampler2D textureSampler;

uniform int useTexture;
uniform int isBlended;
uniform int useShadows;
uniform bool useDefaultLight = true;
uniform bool useSpotlight = false;
uniform bool useRadialLight = false;

uniform int isSkyBox;
uniform float blendAlpha;

in vec3 vertexColor;
in vec2 vertexUV;
in vec3 fragment_normal;
in vec3 fragment_position;
in vec4 fragment_position_light_space;

in vec4 gl_FragCoord;

out vec4 FragColor;

vec3 ambient_color(vec3 light_color_arg) {
    return shading_ambient_strength * light_color_arg;
}

vec3 diffuse_color(vec3 light_color_arg, vec3 light_position_arg) {
    vec3 light_direction = normalize(light_position_arg - fragment_position);
    return shading_diffuse_strength * light_color_arg * max(dot(normalize(fragment_normal), light_direction), 0.0f);
}

vec3 specular_color(vec3 light_color_arg, vec3 light_position_arg, float alpha_arg) {
    vec3 light_direction = normalize(light_position_arg - fragment_position);
    vec3 view_direction = normalize(view_position - fragment_position);
    vec3 reflect_light_direction = reflect(-light_direction, normalize(fragment_normal));
    float specular_intensity = pow(max(dot(reflect_light_direction, view_direction), 0.0f), alpha_arg);
    return shading_specular_strength * light_color_arg * specular_intensity;
}

float shadow_scalar() {
    vec3 ndc = fragment_position_light_space.xyz / fragment_position_light_space.w;
    ndc = ndc * 0.5 + 0.5;
    float closest_depth = texture(shadow_map, ndc.xy).r;
    float current_depth = ndc.z;
    float bias = 0.003;
    return ((current_depth - bias) < closest_depth) ? 1.0 : 0.0;
}

float spotlight_scalar(vec3 light_position_arg, vec3 light_direction_arg) {
    float theta = dot(normalize(fragment_position - light_position_arg), light_direction_arg);
    float epsilon = light_cutoff_inner - light_cutoff_outer;
    
    if (theta > light_cutoff_inner) {
        return 1.0;
    } else if (theta > light_cutoff_outer) {
        return clamp((theta - light_cutoff_outer) / epsilon, 0.0, 1.0);
    } else {
        return 0.0;
    }
}

void main()
{
    vec3 ambient = vec3(0.0f);
    vec3 diffuse = vec3(0.0f);
    vec3 specular = vec3(0.0f);
    vec3 spotlight_ambient = vec3(0.0f);
    vec3 spotlight_diffuse = vec3(0.0f);
    vec3 spotlight_specular = vec3(0.0f);
    vec3 radial_light_ambient = vec3(0.0f);
    vec3 radial_light_diffuse = vec3(0.0f);
    vec3 radial_light_specular = vec3(0.0f);
    vec3 lightColor = vec3(0.0f);
    
    ambient = ambient_color(light_color);

    if (useDefaultLight) {
        if (useShadows == 1) 
        {
            diffuse = shadow_scalar() * diffuse_color(light_color, light_position);
            specular = shadow_scalar() * specular_color(light_color, light_position, specularAlpha);
        }
        else 
        {
            diffuse = diffuse_color(light_color, light_position);
            specular = specular_color(light_color, light_position, specularAlpha);
        }

        lightColor = (specular + diffuse + ambient);
    }
    else {
        lightColor = ambient;
    }

    if (useSpotlight) {
        spotlight_ambient = ambient_color(spotlight_color) * 0.1;
        spotlight_diffuse = spotlight_scalar(spotlight_position, spotlight_direction) * diffuse_color(spotlight_color, spotlight_position);
        spotlight_specular = spotlight_scalar(spotlight_position, spotlight_direction) * specular_color(spotlight_color, spotlight_position, specularAlpha);

        lightColor += (spotlight_ambient + spotlight_diffuse + spotlight_specular);
    }

    if (useRadialLight) {
        radial_light_ambient = ambient_color(radial_light_color) * 0.1;
        radial_light_diffuse = spotlight_scalar(radial_light_position, radial_light_direction) * diffuse_color(radial_light_color, radial_light_position);
        radial_light_specular = spotlight_scalar(radial_light_position, radial_light_direction) * specular_color(radial_light_color, radial_light_position, specularAlpha);

        lightColor += (radial_light_ambient + radial_light_diffuse + radial_light_specular);
    }
    
    
    vec4 textureColor = texture(textureSampler, vertexUV);
    
    vec3 finalColor;

    if (useTexture == 1)
    {
        if (isSkyBox == 1) {
            finalColor = textureColor.rgb;
        }   
        else if (isBlended == 0)
        {
            finalColor = lightColor * textureColor.rgb;
        }
        else
        {
            FragColor = vec4(lightColor, 1.0) * (vec4(textureColor.rgb, blendAlpha) + vec4(vertexColor.rgb, 1.0) * (1.0 - blendAlpha));
            return;
        }
    }
    else
    {
        if (isSkyBox == 1) 
        {
            finalColor = vertexColor;
        } 
        else 
        {
            finalColor = lightColor * vertexColor;
        }
    }

    FragColor = vec4(finalColor, 1.0f);
}

