#pragma once

#include "Utility_functions.h"
#include "Vec.h"

class Texture {
	public:
		virtual Color value(float u, float v, const Point &point) const = 0;
};

class solid_color : public Texture {
	public:
		solid_color() {}
		solid_color(Color color) : color_value(color) {}

		solid_color(float red, float green, float blue)
		  : solid_color(Color(red,green,blue)) {}

		virtual Color value(float u, float v, const Point &point) const override {
			return color_value;
		}

	private:
		Color color_value;
};

class Checker_Texture : public Texture {
	public:
		Checker_Texture() {}

		Checker_Texture(shared_ptr<Texture> _even, shared_ptr<Texture> _odd)
			: even(_even), odd(_odd) {}

		Checker_Texture(Color color_0, Color color_1)
			: even(make_shared<solid_color>(color_0)) , odd(make_shared<solid_color>(color_1)) {}

		virtual Color value(float u, float v, const Point &point) const override {
			auto sines = 
				sin(10*point.getX()) *
				sin(10*point.getY()) *
				sin(10*point.getZ());
			
			if (sines < 0) { return odd->value(u, v, point); }
			else { return even->value(u, v, point); }
		}

	public:
		shared_ptr<Texture> odd;
		shared_ptr<Texture> even;
};