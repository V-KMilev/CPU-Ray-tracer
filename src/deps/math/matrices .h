#pragma once

#include <iostream>
#include <math.h>

typedef float* matrix_ptr;

class matrix {
	public:
		matrix(matrix_ptr* matrix) : matrix_ptr(matrix[0]) {}

		matrix(matrix_ptr* matrix, float angle) 
		: matrix_ptr(matrix[0]), angle(angle) {
			cos_angle = cos(angle);
			sin_angle = sin(angle);
		}

		matrix(matrix_ptr* matrix, float t_value[3]) 
		: matrix_ptr(matrix[0]), translation_value{t_value[0], t_value[1], t_value[2]} {}

		matrix(matrix_ptr* matrix, float s_value[3]) 
		: matrix_ptr(matrix[0]), scaling_value{ s_value[0], s_value[1], s_value[2]} {}

		~matrix() {}

	matrix_ptr reversed_matrix(matrixType type = t_indentity) {

		if(type == t_indentity) {
			return indentity_4[0];
		}

		if(type == t_translation) {
			translation_value[0] *= -1;
			translation_value[1] *= -1;
			translation_value[2] *= -1;
			return translation[0];
		}

		if(type == t_scaling) {
			scaling_value[0] = 1 / scaling_value[0];
			scaling_value[1] = 1 / scaling_value[1];
			scaling_value[2] = 1 / scaling_value[2];
			return scaling[0];
		}

		if(type == t_rotation) {
			angle *= -1;
			return scaling[0];
		}

		return nullptr;
	}

	private:
		/* Matrix types:
		 * 0 = indentity (no type set) [4][4]
		 * 1 = translation             [4][4]
		 * 2 = scaling                 [4][4]
		 * 3 = rotation                [4][4]
		 * 
		 * TODO: [3][3] support
		 * 4 = indentity               [3][3]
		 * 5 = rotation                [3][3]
		 */
		matrix_ptr matrix_ptr;

		float angle;
		float scaling_value[3];
		float translation_value[3];

		float cos_angle;
		float sin_angle;

///////////////////////////////////////////////////////////////////////////////////

		/* indentity_3:
		* | 1 0 0 |
		* | 0 1 0 |
		* | 0 0 1 |
		*/

		float indentity_3[3][3] = {
			{ 1.0f, 0.0f, 0.0f },
			{ 0.0f, 1.0f, 0.0f },
			{ 0.0f, 0.0f, 1.0f }
		};

///////////////////////////////////////////////////////////////////////////////////

		/* indentity_4:
		* | 1 0 0 0 |
		* | 0 1 0 0 |
		* | 0 0 1 0 |
		* | 0 0 0 1 |
		*/

		float indentity_4[4][4] = {
			{ 1.0f, 0.0f, 0.0f, 0.0f },
			{ 0.0f, 1.0f, 0.0f, 0.0f },
			{ 0.0f, 0.0f, 1.0f, 0.0f },
			{ 0.0f, 0.0f, 0.0f, 1.0f }
		};

///////////////////////////////////////////////////////////////////////////////////

		/* rotation_x_4:
		* | 1 0   0     |
		* | 0 +cos -sin |
		* | 0 +sin +cos |
		*/

		float rotation_x_3[3][3] = {
			{ 1.0f, 0.0f, 0.0f },
			{ 0.0f, cos_angle, -sin_angle },
			{ 0.0f, sin_angle, cos_angle }
		};

		/* rotation_y_4:
		* | +cos 0 +sin |
		* | 0    1 0    |
		* | -sin 0 +cos |
		*/

		float rotation_y_3[3][3] = {
			{ cos_angle, 0.0f, sin_angle },
			{ 0.0f, 1.0f, 0.0f },
			{ -sin_angle, 0.0f, cos_angle }
		};

		/* rotation_z_4:
		* | +cos -sin 0 |
		* | +sin +cos 0 |
		* | 0    0    1 |
		*/

		float rotation_z_3[3][3] = {
			{ cos_angle, -sin_angle, 0.0f },
			{ sin_angle, cos_angle , 0.0f },
			{ 0.0f , 0.0f, 1.0f }
		};

		/* Reversed rotation_3:
		* Set angle to -angle.
		* -angle;
		*/

///////////////////////////////////////////////////////////////////////////////////

		/* rotation_x_4:
		* | 1 0   0     0 |
		* | 0 +cos -sin 0 |
		* | 0 +sin +cos 0 |
		* | 0 0   0     1 |
		*/

		float rotation_x_4[4][4] = {
			{ 1.0f, 0.0f, 0.0f, 0.0f },
			{ 0.0f, cos_angle, -sin_angle, 0.0f },
			{ 0.0f, sin_angle, cos_angle, 0.0f },
			{ 0.0f, 0.0f, 0.0f, 1.0f },
		};

		/* rotation_y_4:
		* | +cos 0, +sin 0 |
		* | 0    1, 0    0 |
		* | -sin 0, +cos 0 |
		* | 0    0, 0    1 |
		*/

		float rotation_y_4[4][4] = {
			{ cos_angle, 0.0f, sin_angle, 0.0f },
			{ 0.0f, 1.0f, 0.0f, 0.0f },
			{ -sin_angle, 0.0f, cos_angle, 0.0f },
			{ 0.0f, 0.0f, 0.0f, 1.0f }
		};

		/* rotation_z_4:
		* | +cos -sin 0 0 |
		* | +sin +cos 0 0 |
		* | 0   0     1 0 |
		* | 0   0     0 1 |
		*/

		float rotation_z_4[4][4] = {
			{ cos_angle, -sin_angle, 0.0f, 0.0f },
			{ sin_angle, cos_angle , 0.0f, 0.0f },
			{ 0.0f , 0.0f, 1.0f, 0.0f },
			{ 0.0f, 0.0f, 0.0f, 1.0f }
		};

		/* Reversed rotation_4:
		* Set angle to -angle.
		* -angle;
		*/

///////////////////////////////////////////////////////////////////////////////////

		/* scaling_4:
		* | Kx 0 0 0 |
		* | 0 Ky 0 0 |
		* | 0 0 Kz 0 |
		* | 0 0 0  1 |
		*/

		float scaling[4][4] = {
			{ scaling_value[0], 0.0f, 0.0f, 0.0f },
			{ 0.0f, scaling_value[1], 0.0f, 0.0f },
			{ 0.0f, 0.0f, scaling_value[2], 0.0f },
			{ 0.0f, 0.0f, 0.0f, 1.0f }
		};

		/* Reversed scaling:
		* Set scaling_value(s) to 1/scaling_value(s).
		* 1 / scaling_value_x;
		* 1 / scaling_value_y;
		* 1 / scaling_value_z;
		*/

///////////////////////////////////////////////////////////////////////////////////

		/* translation_4:
		* | 0 0 0 Kx |
		* | 0 0 0 Ky |
		* | 0 0 0 Kz |
		* | 0 0 0  1 |
		*/

		float translation[4][4] = {
			{ 1.0f, 0.0f, 0.0f, translation_value[0] },
			{ 0.0f, 1.0f, 0.0f, translation_value[1] },
			{ 0.0f, 0.0f, 1.0f, translation_value[2] },
			{ 0.0f, 0.0f, 0.0f, 1.0f }
		};

		/* Reversed translation:
		* Set translation_value(s) to -translation_value(s).
		* -translation_value_x;
		* -translation_value_y;
		* -translation_value_z;
		*/

///////////////////////////////////////////////////////////////////////////////////
};

enum matrixType {
	t_indentity = 0,
	t_translation = 1,
	t_scaling = 2,
	t_rotation = 3,

	t_indentity3 = 4,
	t_rotation = 5
};
