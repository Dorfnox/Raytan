#include "Sampler.hpp"

Sampler::Sampler(const std::string& filepath)
{
	unsigned error = lodepng::decode(_imageData, _width, _height, filepath.c_str());

	if (error)
	{
		std::cout << filepath << std::endl;
		std::cout << "error: " << lodepng_error_text(error) << std::endl;
	}
}

glm::dvec4	Sampler::Color(double x, double y) const
{
	glm::dvec4 out;
	unsigned xIndex = glm::round((_width - 1) * glm::clamp(x, 0.0, 1.0));
	unsigned yIndex = glm::round((_height - 1) * glm::clamp(y, 0.0, 1.0));

	out.r = _imageData[(xIndex + _width * yIndex) * 4 + 0];
	out.g =	_imageData[(xIndex + _width * yIndex) * 4 + 1];	
	out.b =	_imageData[(xIndex + _width * yIndex) * 4 + 2];
	out.a = _imageData[(xIndex + _width * yIndex) * 4 + 3];

	return out / 255.0;
}