#pragma once

#include <iostream>
#include <math.h>

#include "Vec.h"

enum matrixType : unsigned int {
	t_none          = 0,
	t_indentity_3   = 1,
	t_scaling_3     = 2,
	t_rotation_3    = 3,
	t_rotation_x_3  = 4,
	t_rotation_y_3  = 5,
	t_rotation_z_3  = 6
};

class Matrix {
	public:
		Matrix(float matrix[9] = 0, matrixType type = t_none)
		: my_matrix{ matrix[0], matrix[1], matrix[2],
					 matrix[3], matrix[4], matrix[5],
					 matrix[6], matrix[7], matrix[8] } {}

		Matrix operator *= (Matrix &t0_matrix) {
			/* Matrix multiplication 3x3 with 3x3:
			 * this Matrix:
			 * | m00 m01 m02 |
			 * | m03 m04 m05 |
			 * | m06 m07 m08 |
			 * t1_Matrix:
			 * | m10 m11 m12 |
			 * | m13 m14 m15 |
			 * | m16 m17 m18 |
			 * 
			 * Mult_Matrix:
			 * | (m00 * m10 + m01 * m13 + m02 * m16) (m00 * m11 + m01 * m14 + m02 * m17) (m00 * m12 + m01 * m15 + m02 * m18) |
			 * | (m03 * m10 + m04 * m13 + m05 * m16) (m03 * m11 + m04 * m14 + m05 * m17) (m03 * m12 + m04 * m15 + m05 * m18) |
			 * | (m06 * m10 + m07 * m13 + m08 * m16) (m06 * m11 + m07 * m14 + m08 * m17) (m06 * m12 + m07 * m15 + m08 * m18) |
			 */

			float mult[9];

			mult[0] = my_matrix[0] * t0_matrix.my_matrix[0] + my_matrix[1] * t0_matrix.my_matrix[3] +  my_matrix[2] * t0_matrix.my_matrix[6];
			mult[1] = my_matrix[0] * t0_matrix.my_matrix[1] + my_matrix[1] * t0_matrix.my_matrix[4] +  my_matrix[2] * t0_matrix.my_matrix[7];
			mult[2] = my_matrix[0] * t0_matrix.my_matrix[2] + my_matrix[1] * t0_matrix.my_matrix[5] +  my_matrix[2] * t0_matrix.my_matrix[8];

			mult[3] = my_matrix[3] * t0_matrix.my_matrix[0] + my_matrix[4] * t0_matrix.my_matrix[3] +  my_matrix[5] * t0_matrix.my_matrix[6];
			mult[4] = my_matrix[3] * t0_matrix.my_matrix[1] + my_matrix[4] * t0_matrix.my_matrix[4] +  my_matrix[5] * t0_matrix.my_matrix[7];
			mult[5] = my_matrix[3] * t0_matrix.my_matrix[2] + my_matrix[4] * t0_matrix.my_matrix[5] +  my_matrix[5] * t0_matrix.my_matrix[8];

			mult[6] = my_matrix[6] * t0_matrix.my_matrix[0] + my_matrix[7] * t0_matrix.my_matrix[3] +  my_matrix[8] * t0_matrix.my_matrix[6];
			mult[7] = my_matrix[6] * t0_matrix.my_matrix[1] + my_matrix[7] * t0_matrix.my_matrix[4] +  my_matrix[8] * t0_matrix.my_matrix[7];
			mult[8] = my_matrix[6] * t0_matrix.my_matrix[2] + my_matrix[7] * t0_matrix.my_matrix[5] +  my_matrix[8] * t0_matrix.my_matrix[8];

			my_matrix[0] = mult[0];
			my_matrix[1] = mult[1];
			my_matrix[2] = mult[2];

			my_matrix[3] = mult[3];
			my_matrix[4] = mult[4];
			my_matrix[5] = mult[5];

			my_matrix[6] = mult[6];
			my_matrix[7] = mult[7];
			my_matrix[8] = mult[8];

			return *this;
		}

		Matrix operator *= (float t) {
			/* Matrix multiplication 3x3 with float:
			 * this Matrix:
			 * | m00 m01 m02 |
			 * | m03 m04 m05 |
			 * | m06 m07 m08 |
			 * 
			 * Mult_Matrix:
			 * | (m00 * t) (m01 * t) (m02 * t) |
			 * | (m03 * t) (m04 * t) (m05 * t) |
			 * | (m06 * t) (m07 * t) (m08 * t) |
			 */

			my_matrix[0] *= t;
			my_matrix[1] *= t;
			my_matrix[2] *= t;

			my_matrix[3] *= t;
			my_matrix[4] *= t;
			my_matrix[5] *= t;

			my_matrix[6] *= t;
			my_matrix[7] *= t;
			my_matrix[8] *= t;

			return *this;
		}

		Matrix inverse_matrix() {
			float det = get_det();

			if(det == 0) {
				std::cerr << "Matrix: The determinant is equal to zero!\n";
				return *this;
			}

			float* adjoint_matrix = get_adjoint();

			my_matrix[0] = adjoint_matrix[0] * (1 / det);
			my_matrix[1] = adjoint_matrix[1] * (1 / det);
			my_matrix[2] = adjoint_matrix[2] * (1 / det);

			my_matrix[3] = adjoint_matrix[3] * (1 / det);
			my_matrix[4] = adjoint_matrix[4] * (1 / det);
			my_matrix[5] = adjoint_matrix[5] * (1 / det);

			my_matrix[6] = adjoint_matrix[6] * (1 / det);
			my_matrix[7] = adjoint_matrix[7] * (1 / det);
			my_matrix[8] = adjoint_matrix[8] * (1 / det);

			return *this;
		}

		float* get_adjoint() {
			/* adjoint for 3x3:
			 * Matrix:
			 * | 0 1 2 |
			 * | 3 4 5 |
			 * | 6 7 8 |
			 * 
			 * adjoint Matrix:
			 * | +| 4 5 | - | 3 5 | +| 3 4 | |
			 * |  | 7 8 |   | 6 8 |  | 6 7 | |
			 * | -| 1 2 | + | 0 2 | -| 0 1 | |
			 * |  | 7 8 |   | 6 8 |  | 6 7 | |
			 * | +| 1 2 | - | 0 2 | -| 0 1 | |
			 * |  | 4 5 |   | 3 5 |  | 3 4 | |
			 * 
			 * Matrix cofactor 2x2:
			 * cofactor Matrix:
			 * | 0 1 |
			 * | 2 3 |
			 * cofactor = 0 * 3 - 1 * 2;
			 */

			my_adjoint_matrix[0] = my_matrix[4] * my_matrix[8] - my_matrix[5] * my_matrix[7];
			my_adjoint_matrix[1] = -(my_matrix[3] * my_matrix[8] - my_matrix[5] * my_matrix[6]);
			my_adjoint_matrix[2] = my_matrix[3] * my_matrix[7] - my_matrix[4] * my_matrix[6];

			my_adjoint_matrix[3] = -(my_matrix[1] * my_matrix[8] - my_matrix[2] * my_matrix[7]);
			my_adjoint_matrix[4] = my_matrix[0] * my_matrix[8] - my_matrix[2] * my_matrix[6];
			my_adjoint_matrix[5] = -(my_matrix[0] * my_matrix[7] - my_matrix[1] * my_matrix[6]);

			my_adjoint_matrix[6] = my_matrix[1] * my_matrix[5] - my_matrix[2] * my_matrix[4];
			my_adjoint_matrix[7] = -(my_matrix[0] * my_matrix[5] - my_matrix[2] * my_matrix[3]);
			my_adjoint_matrix[8] = my_matrix[0] * my_matrix[4] - my_matrix[1] * my_matrix[3];

			return my_adjoint_matrix;
		}

		float get_det() {
			/* determinant for 3x3:
			 * Matrix:
			 * | 0 1 2 | 0 1
			 * | 3 4 5 | 3 4
			 * | 6 7 8 | 6 7
			 * Determinant:
			 * det = (0 * 4 * 8) + (1 * 5 * 6) + (2 * 3 * 7) 
			 *     - (2 * 4 * 6) - (0 * 5 * 7) - (1 * 3 * 8);
			 */

			my_determinant = my_matrix[0] * my_matrix[4] * my_matrix[8] +
							 my_matrix[1] * my_matrix[5] * my_matrix[6] +
							 my_matrix[2] * my_matrix[3] * my_matrix[7] -
							 my_matrix[2] * my_matrix[4] * my_matrix[6] -
							 my_matrix[0] * my_matrix[5] * my_matrix[7] -
							 my_matrix[1] * my_matrix[3] * my_matrix[8];

			return my_determinant;
		}

	public:
		float my_matrix[9];

		matrixType my_type;

	private:
		float my_adjoint_matrix[9];
		float my_determinant;
};

inline std::ostream& operator << (std::ostream &out, const Matrix &m) {
	return out << m.my_matrix[0] << ' ' << m.my_matrix[1] << ' ' << m.my_matrix[2] << '\n'
			   << m.my_matrix[3] << ' ' << m.my_matrix[4] << ' ' << m.my_matrix[5] << '\n'
			   << m.my_matrix[6] << ' ' << m.my_matrix[7] << ' ' << m.my_matrix[8] << '\n';
}

inline Matrix operator * (Matrix &t0_matrix, Matrix &t1_matrix) {
	/* Matrix multiplication 3x3 with 3x3:
	 * t0_Matrix:
	 * | m00 m01 m02 |
	 * | m03 m04 m05 |
	 * | m06 m07 m08 |
	 * t1_Matrix:
	 * | m10 m11 m12 |
	 * | m13 m14 m15 |
	 * | m16 m17 m18 |
	 * 
	 * Mult_Matrix:
	 * | (m00 * m10 + m01 * m13 + m02 * m16) (m00 * m11 + m01 * m14 + m02 * m17) (m00 * m12 + m01 * m15 + m02 * m18) |
	 * | (m03 * m10 + m04 * m13 + m05 * m16) (m03 * m11 + m04 * m14 + m05 * m17) (m03 * m12 + m04 * m15 + m05 * m18) |
	 * | (m06 * m10 + m07 * m13 + m08 * m16) (m06 * m11 + m07 * m14 + m08 * m17) (m06 * m12 + m07 * m15 + m08 * m18) |
	 */

	float mult[9];

	mult[0] = t0_matrix.my_matrix[0] * t1_matrix.my_matrix[0] + t0_matrix.my_matrix[1] * t1_matrix.my_matrix[3] +  t0_matrix.my_matrix[2] * t1_matrix.my_matrix[6];
	mult[1] = t0_matrix.my_matrix[0] * t1_matrix.my_matrix[1] + t0_matrix.my_matrix[1] * t1_matrix.my_matrix[4] +  t0_matrix.my_matrix[2] * t1_matrix.my_matrix[7];
	mult[2] = t0_matrix.my_matrix[0] * t1_matrix.my_matrix[2] + t0_matrix.my_matrix[1] * t1_matrix.my_matrix[5] +  t0_matrix.my_matrix[2] * t1_matrix.my_matrix[8];

	mult[3] = t0_matrix.my_matrix[3] * t1_matrix.my_matrix[0] + t0_matrix.my_matrix[4] * t1_matrix.my_matrix[3] +  t0_matrix.my_matrix[5] * t1_matrix.my_matrix[6];
	mult[4] = t0_matrix.my_matrix[3] * t1_matrix.my_matrix[1] + t0_matrix.my_matrix[4] * t1_matrix.my_matrix[4] +  t0_matrix.my_matrix[5] * t1_matrix.my_matrix[7];
	mult[5] = t0_matrix.my_matrix[3] * t1_matrix.my_matrix[2] + t0_matrix.my_matrix[4] * t1_matrix.my_matrix[5] +  t0_matrix.my_matrix[5] * t1_matrix.my_matrix[8];

	mult[6] = t0_matrix.my_matrix[6] * t1_matrix.my_matrix[0] + t0_matrix.my_matrix[7] * t1_matrix.my_matrix[3] +  t0_matrix.my_matrix[8] * t1_matrix.my_matrix[6];
	mult[7] = t0_matrix.my_matrix[6] * t1_matrix.my_matrix[1] + t0_matrix.my_matrix[7] * t1_matrix.my_matrix[4] +  t0_matrix.my_matrix[8] * t1_matrix.my_matrix[7];
	mult[8] = t0_matrix.my_matrix[6] * t1_matrix.my_matrix[2] + t0_matrix.my_matrix[7] * t1_matrix.my_matrix[5] +  t0_matrix.my_matrix[8] * t1_matrix.my_matrix[8];

	return Matrix(mult);
}

inline Vec operator * (Matrix &t0_matrix, Vec &t0_vec) {
	/* Matrix multiplication 3x3 with vector 3x1:
	 * t0_Matrix:
	 * | m00 m01 m02 |
	 * | m03 m04 m05 |
	 * | m06 m07 m08 |
	 * t0_Vector:
	 * | v0x v0y v0z |
	 * 
	 * Mult_Vec:
	 * | (m00 * v0x + m01 * v0y + m02 * v0z) (m03 * v0x + m04 * v0y + m05 * v0z) (m06 * v0x + m07 * v0y + m02 * v0z) |
	 */

	return Vec
	(
		t0_matrix.my_matrix[0] * t0_vec.getX() + t0_matrix.my_matrix[1] * t0_vec.getY() +  t0_matrix.my_matrix[2] * t0_vec.getZ(),
		t0_matrix.my_matrix[3] * t0_vec.getX() + t0_matrix.my_matrix[4] * t0_vec.getY() +  t0_matrix.my_matrix[5] * t0_vec.getZ(),
		t0_matrix.my_matrix[6] * t0_vec.getX() + t0_matrix.my_matrix[7] * t0_vec.getY() +  t0_matrix.my_matrix[8] * t0_vec.getZ()
	);
}


inline Matrix operator * (Matrix &t0_matrix, float t) {
	/* Matrix multiplication 3x3 with float:
	 * t0_Matrix:
	 * | m00 m01 m02 |
	 * | m03 m04 m05 |
	 * | m06 m07 m08 |
	 * 
	 * Mult_Matrix:
	 * | (m00 * t) (m01 * t) (m02 * t) |
	 * | (m03 * t) (m04 * t) (m05 * t) |
	 * | (m06 * t) (m07 * t) (m08 * t) |
	 */

	t0_matrix.my_matrix[0] *= t;
	t0_matrix.my_matrix[1] *= t;
	t0_matrix.my_matrix[2] *= t;

	t0_matrix.my_matrix[3] *= t;
	t0_matrix.my_matrix[4] *= t;
	t0_matrix.my_matrix[5] *= t;

	t0_matrix.my_matrix[6] *= t;
	t0_matrix.my_matrix[7] *= t;
	t0_matrix.my_matrix[8] *= t;

	return t0_matrix;
}

///////////////////////////////////////////////////////////////////////////////////

/* indentity_3:
 * | 1 0 0 |
 * | 0 1 0 |
 * | 0 0 1 |
 */

float indentity_3[9] = {
	1.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 1.0f
};

///////////////////////////////////////////////////////////////////////////////////

/* rotation_x_3:
* | 1 0   0     |
* | 0 +cos -sin |
* | 0 +sin +cos |
*/

/* rotation_y_3:
* | +cos 0 +sin |
* | 0    1 0    |
* | -sin 0 +cos |
*/

/* rotation_z_3:
* | +cos -sin 0 |
* | +sin +cos 0 |
* | 0    0    1 |
*/

/* Reversed rotation_3:
* Set angle to -angle.
* -angle;
*/

///////////////////////////////////////////////////////////////////////////////////

/* scaling_3:
* | Kx 0 0 |
* | 0 Ky 0 |
* | 0 0 Kz |
*/

/* Reversed scaling:
* Set scaling_value(s) to 1/scaling_value(s).
* 1 / scaling_value_x;
* 1 / scaling_value_y;
* 1 / scaling_value_z;
*/

// ///////////////////////////////////////////////////////////////////////////////////
