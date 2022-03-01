#pragma once

#include <iostream>
#include <math.h>

#include "Vec.h"

typedef float (*matrix_ptr)[3];

enum matrixType : unsigned int {
	t_indentity_3   = 0,
	t_translation_3 = 1,
	t_scaling_3     = 2,
	t_rotation_3    = 3,
	t_rotation_x_3  = 4,
	t_rotation_y_3  = 5,
	t_rotation_z_3  = 6,

	t_indentity_4   = 7,
	t_translation_4 = 8,
	t_scaling_4     = 9,
	t_rotation_4    = 10,
	t_rotation_x_4  = 11,
	t_rotation_y_4  = 12,
	t_rotation_z_4  = 13
};

class matrix {
	public:
		matrix(matrixType type = t_indentity_3) 
		: my_type(type), my_matrix_ptr(indentity_3) {}

		matrix(float angle  = 0, matrixType type = t_rotation_3) 
		: my_type(type), my_matrix_ptr(set_rotation_matrix_ptr()), angle(angle), cos_angle(cos(angle)), sin_angle(sin(angle)) {}

		matrix(Vec translation = Vec(0,0,0), matrixType type = t_translation_3) 
		: my_type(type), my_matrix_ptr(nullptr), translation(translation) {}

		matrix(float scaling[3] = 0, matrixType type = t_scaling_3) 
		: my_type(type), my_matrix_ptr(scaling_3), scaling{scaling[0], scaling[1], scaling[2]} {}

		~matrix() {}

		matrix_ptr get_matrix() const { return my_matrix_ptr; }

		void set_matrix_ptr(matrixType type) {
			if(my_type == t_indentity_3)
				my_matrix_ptr = indentity_3;

			if(my_type == t_translation_3)
				my_matrix_ptr = indentity_3;

			if(my_type == t_scaling_3)
				my_matrix_ptr = scaling_3;
			
			if(my_type == t_rotation_x_3)
				my_matrix_ptr = rotation_x_3;

			if(my_type == t_rotation_y_3)
				my_matrix_ptr = rotation_y_3;

			if(my_type == t_rotation_z_3)
				my_matrix_ptr = rotation_z_3;

			if(my_type > 6U)
				my_matrix_ptr = nullptr;
		}

		// TODO: Creates new matrix and gives a pointer
		matrix_ptr inverse_matrix() {

			// if(get_det() == 0)
			// 	return nullptr;

			// if(my_type == t_indentity_3) {
			// 	return indentity_3;
			// }

			// if(my_type == t_translation_3) {
			// 	translation *= -1;
			// 	// return (indentity_3 * translation);
			// }

			// if(my_type == t_scaling_3) {
			// 	scaling[0] = 1 / scaling[0];
			// 	scaling[1] = 1 / scaling[1];
			// 	scaling[2] = 1 / scaling[2];
			// 	return scaling_3;
			// }

			// if(my_type == t_rotation_x_3) {
			// 	angle *= -1;
			// 	return rotation_x_3;
			// }

			// if(my_type == t_rotation_y_3) {
			// 	angle *= -1;
			// 	return rotation_y_3;
			// }

			// if(my_type == t_rotation_z_3) {
			// 	angle *= -1;
			// 	return rotation_z_3;
			// }

			return nullptr;
		}

	private:
		matrix_ptr set_rotation_matrix_ptr() { 
			if(my_type == t_rotation_x_3)
				return rotation_x_3;

			if(my_type == t_rotation_y_3)
				return rotation_y_3;

			if(my_type == t_rotation_z_3)
				return rotation_z_3;

			return nullptr;
		}

		float get_det() {
			matrix_ptr matrix = get_matrix();

			/* determinant for 3x3:
			 * Matrix:
			 * | 00 01 02 | 00 01
			 * | 10 11 12 | 10 11
			 * | 20 21 22 | 20 21
			 * Determinant:
			 * det = (00 * 11 * 22) + (01 * 12 * 20) + (02 * 10 * 21) 
			 *     - (02 * 11 * 20) - (00 * 12 * 21) - (01 * 10 * 22);
			 */

			return matrix[0][0] * matrix[1][1] * matrix[2][2] +
				   matrix[0][1] * matrix[1][2] * matrix[2][0] +
				   matrix[0][2] * matrix[1][0] * matrix[2][1] -
				   matrix[0][2] * matrix[1][1] * matrix[2][0] -
				   matrix[0][0] * matrix[1][2] * matrix[2][1] -
				   matrix[0][0] * matrix[1][0] * matrix[2][2];
		}

	private:
		/* Matrix types:
		 * 0 = indentity (no type set) [3][3]
		 * 1 = translation             [3][3]
		 * 2 = scaling                 [3][3]
		 * 3 = rotation_3              [3][3]
		 * 4 = rotation_x              [3][3]
		 * 5 = rotation_y              [3][3]
		 * 6 = rotation_z              [3][3]
		 * 
		 * TODO: [4][4] support
		 */
		matrixType my_type;

		matrix_ptr my_matrix_ptr;

		Vec translation;
		float scaling[3];

		float angle;
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

		/* rotation_x_3:
		* | 1 0   0     |
		* | 0 +cos -sin |
		* | 0 +sin +cos |
		*/

		float rotation_x_3[3][3] = {
			{ 1.0f, 0.0f, 0.0f },
			{ 0.0f, cos_angle, -sin_angle },
			{ 0.0f, sin_angle, cos_angle }
		};

		/* rotation_y_3:
		* | +cos 0 +sin |
		* | 0    1 0    |
		* | -sin 0 +cos |
		*/

		float rotation_y_3[3][3] = {
			{ cos_angle, 0.0f, sin_angle },
			{ 0.0f, 1.0f, 0.0f },
			{ -sin_angle, 0.0f, cos_angle }
		};

		/* rotation_z_3:
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

		/* scaling_3:
		* | Kx 0 0 |
		* | 0 Ky 0 |
		* | 0 0 Kz |
		*/

		float scaling_3[3][3] = {
			{ scaling[0], 0.0f, 0.0f },
			{ 0.0f, scaling[1], 0.0f },
			{ 0.0f, 0.0f, scaling[2] }
		};

		/* Reversed scaling:
		* Set scaling_value(s) to 1/scaling_value(s).
		* 1 / scaling_value_x;
		* 1 / scaling_value_y;
		* 1 / scaling_value_z;
		*/

///////////////////////////////////////////////////////////////////////////////////

		/* scaling_4:
		* | Kx 0 0 0 |
		* | 0 Ky 0 0 |
		* | 0 0 Kz 0 |
		* | 0 0 0  1 |
		*/

		float scaling_4[4][4] = {
			{ scaling[0], 0.0f, 0.0f, 0.0f },
			{ 0.0f, scaling[1], 0.0f, 0.0f },
			{ 0.0f, 0.0f, scaling[2], 0.0f },
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

		float translation_4[4][4] = {
			{ 1.0f, 0.0f, 0.0f, translation.getX() },
			{ 0.0f, 1.0f, 0.0f, translation.getY() },
			{ 0.0f, 0.0f, 1.0f, translation.getZ() },
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

inline matrix_ptr operator * (const matrix &t0_matrix, const matrix &t1_matrix) {
	// TODO:
}

inline matrix_ptr operator * (const matrix &t0_matrix, Vec &t0_vec) {
	// TODO:
}
