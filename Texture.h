#pragma once

#include "Utility_functions.h"
#include "Vec.h"

class Texture {
	public:
		virtual Color value(float u, float v, const Point &point) const = 0;
};

class Solid_Color : public Texture {
	public:
		Solid_Color() {}
		Solid_Color(Color color) : color_value(color) {}

		Solid_Color(float red, float green, float blue) : Solid_Color(Color(red,green,blue)) {}

		virtual Color value(float u, float v, const Point &point) const override {
			return color_value;
		}

	private:
		Color color_value;
};

class Checker_Texture : public Texture {
	public:
		Checker_Texture() {}

		Checker_Texture(shared_ptr<Texture> even, shared_ptr<Texture> odd)
			: even(even), odd(odd) {}

		Checker_Texture(Color color_0, Color color_1)
			: even(make_shared<Solid_Color>(color_0)) , odd(make_shared<Solid_Color>(color_1)) {}

		virtual Color value(float u, float v, const Point &point) const override {
			float sines = 
				sin(10 * point.getX()) *
				sin(10 * point.getY()) *
				sin(10 * point.getZ());
			
			if (sines < 0) { return odd->value(u, v, point); }
			else { return even->value(u, v, point); }
		}

	public:
		shared_ptr<Texture> odd;
		shared_ptr<Texture> even;
};