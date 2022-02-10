#pragma once

#include <fstream>

class Color {
	public:
		Color() : e{ 0.0f, 0.0f, 0.0f} {}
		Color(float red, float green, float blue) : e{red, green, blue} {}

		/*
		 * Functions - getRed, getGreen, getBlue
		 *
		 * Parameters:
		 * none
		 * 
		 * Return type:
		 * float
		 * 
		 * Use:
		 * By calling this funtion you get 
		 * in return color value.
		 */

		float getRed() const { return e[0]; }
		float getGreen() const { return e[1]; }
		float getBlue() const { return e[2]; }

		Color& color_gama(int samples_per_pixel) {
			/* Divide the color by the number of samples and gamma-correct for gamma=2.0. - GAMMA 2 */
			float scale = 1.0 / samples_per_pixel;

			e[0] = sqrt(scale * e[0]);
			e[1] = sqrt(scale * e[1]);
			e[2] = sqrt(scale * e[2]);

			return *this;
		}

		Color operator -() const { return Color(-e[0], -e[1], -e[2]); }

		float operator [] (int i) const { return e[i]; }
		float& operator [] (int i) { return e[i]; }

		Color& operator += (const Color &v) {
			e[0] += v.e[0];
			e[1] += v.e[1];
			e[2] += v.e[2];

			return *this;
		}

		Color& operator *= (const float number) {
			e[0] *= number;
			e[1] *= number;
			e[2] *= number;

			return *this;
		}

		Color& operator /= (const float number) {

			return *this *= 1 / number;
		}

	public:
		float e[3];
};

inline std::ostream& operator << (std::ostream &out, const Color &v) {
	return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

inline Color operator + (const Color &u, const Color &v) {
	return Color(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

inline Color operator - (const Color &u, const Color &v) {
	return Color(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

inline Color operator * (const Color &u, const Color &v) {
	return Color(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

inline Color operator * (float distance, const Color &v) {
	return Color(distance * v.e[0], distance * v.e[1], distance * v.e[2]);
}

inline Color operator * (const Color &v, float distance) {
	return distance * v;
}

inline Color operator / (Color v, float distance) {
	return (1 / distance) * v;
}
