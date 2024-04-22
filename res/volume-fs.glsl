#version 400

uniform mat4 modelViewProjectionMatrix;
uniform mat4 inverseModelViewProjectionMatrix;

uniform sampler3D volumeTexture;
uniform sampler3D gradientTexture;

uniform float transferFunctionMidValue;
uniform float transferFunctionValueRange;
uniform float marchDistance;
uniform float densityMultiplier;

in vec2 fragPosition;
out vec4 fragColor;

float calcDepth(vec3 pos)
{
	// using calcDepth, you can convert a ray position to an OpenGL z-value, so that intersections/occlusions with the
	// model geometry are handled correctly, e.g.: gl_FragDepth = calcDepth(nearestHit);
	// in case there is no intersection, you should get gl_FragDepth to 1.0, i.e., the output of the shader will be ignored

	float far = gl_DepthRange.far; 
	float near = gl_DepthRange.near;
	vec4 clip_space_pos = modelViewProjectionMatrix * vec4(pos, 1.0);
	float ndc_depth = clip_space_pos.z / clip_space_pos.w;
	return (((far - near) * ndc_depth) + near + far) / 2.0;
}

// BEZIER INTERPOLATION

float tfInterpolateBezierCubic(float valueToMidLength, float totalLength)
{
	float t = 1.0 - (valueToMidLength / totalLength);
	return pow(t, 3.0) - (3.0 * pow(t, 2.0)) / 2.0 + (3.0 * t) / 2.0;

}

// TRANSFER FUNCTION

vec4 transferFunction(float dataValue)
{
	// SETUP

	// Need to divide by size of Uint (I dont remember exactly why. But, it will return a fractional number)
	float sizeofUint = 65536.0;

	float midValue = transferFunctionMidValue;
	midValue = midValue / sizeofUint;

	float valueRange = transferFunctionValueRange;
	valueRange = valueRange / sizeofUint;

	float valueToMidLength = abs(dataValue - midValue);
	float totalRangeLength = valueRange / 2.0;

	// return blank (or context color value) if outside range
	if (valueToMidLength > totalRangeLength)
	{
		return vec4(0.0, 0.0, 0.0, 0.0);
	}

	// Return a focus color value if inside range
	else
	{
		// To get a smoother transfer between context and focus, we can use a bezier curve
		float totalInfluence = tfInterpolateBezierCubic(valueToMidLength, totalRangeLength);

		totalInfluence *= densityMultiplier;

		// Return a color using the interpolated value
		vec3 returnColor = vec3(totalInfluence, totalInfluence, totalInfluence);
		float returnAlpha = totalInfluence;
		return vec4(returnColor, returnAlpha);
	}
}

// Direct volume rendering with alpha blending
vec4 directVolumeRender(vec3 rayOrigin, vec3 rayDirection, vec3 enterPoint, vec3 exitPoint)
{
	float maxDistance = length(exitPoint - enterPoint);
	vec3 currentPosition;
	float totalDistanceTraveled = 0.0;

	vec3 currentColor = vec3(0.0);
	float currentAlpha = 0.0;

	vec3 accumulatedColor = vec3(0.0);
	float accumulatedAlpha = 0.0;

	// DIRECT VOLUME RENDERING LOOP
	while (totalDistanceTraveled < maxDistance)
	{
		// CURRENT POSITION
		currentPosition = enterPoint + totalDistanceTraveled * rayDirection;

		// GET VALUE AT CURRENT MARCH POINT
		float volumeTextureValue = texture(volumeTexture, currentPosition).r;

		// TRANSFER FUNCTION
		vec4 currentSample = transferFunction(volumeTextureValue);
		currentColor = currentSample.xyz;
		currentAlpha = currentSample.w;

		// COMPOSITE
		float colorAlpha = currentAlpha - (currentAlpha * accumulatedAlpha);
		colorAlpha *= densityMultiplier;
		accumulatedAlpha += colorAlpha;
		accumulatedColor += currentColor * colorAlpha;

		// Early ray termination
		if (accumulatedAlpha > 0.99) {
			break;
		}
		// MARCH ONE STEP FORWARD
		totalDistanceTraveled += marchDistance;
	}
	// RETURN RESULT
	return vec4(accumulatedColor, accumulatedAlpha);
}

void main()
{
	// RAY SETUP
	vec4 near = inverseModelViewProjectionMatrix * vec4(fragPosition, -1.0, 1.0);
	near /= near.w;
	vec4 far = inverseModelViewProjectionMatrix * vec4(fragPosition, 1.0, 1.0);
	far /= far.w;
	vec3 rayOrigin = near.xyz;
	vec3 rayDirection = normalize((far - near).xyz);

	// Start/End of ray segment
	// To segment the volume, use implicitly defined surfaces to find new entry- and exit points, 
	// then do individual direct volume rendering over each segments and composite
	vec3 enterPoint = rayOrigin;
	vec3 exitPoint = rayOrigin + (rayDirection * 5);

	// OUTPUT
	fragColor = directVolumeRender(rayOrigin, rayDirection, enterPoint, exitPoint);

	// Use FragDepth if you need to utilize depth buffer (read information on top)
	//gl_FragDepth = 1.0;
}
