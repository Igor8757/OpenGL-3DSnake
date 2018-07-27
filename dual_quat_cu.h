#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

class Dual_quat_cu {
public:

	// -------------------------------------------------------------------------
	/// @name Constructors
	// -------------------------------------------------------------------------

	/// Default constructor generates a dual quaternion with no translation
	/// and no rotation either
	Dual_quat_cu()
	{
		Dual_quat_cu res = dual_quat_from(glm::quat(1.f, glm::vec3(0)), glm::vec3(0.f, 0.f, 0.f));
		*this = res;
	}

	/// Fill directly the dual quaternion with two quaternion for the non-dual
	/// and dual part
	Dual_quat_cu(const glm::quat& q0, const glm::quat& qe)
	{
		_quat_0 = q0;
		_quat_e = qe;
	}

	/// Construct a dual quaternion with a quaternion 'q' which express the
	/// rotation and a translation vector
	Dual_quat_cu(const glm::quat& q, const glm::vec3& t)
	{
		Dual_quat_cu res = dual_quat_from(q, t);
		*this = res;
	}

	/// Construct from rigid transformation 't'
	Dual_quat_cu(const glm::mat4& t)
	{
		glm::quat q(t);

		// 4th column is translation
		glm::vec3 translation(t[3]);
		Dual_quat_cu res = dual_quat_from(q, translation);
		*this = res;
	}


	// -------------------------------------------------------------------------
	/// @name Methods
	// -------------------------------------------------------------------------

	void normalize()
	{
		float norm = glm::length(_quat_0); // .norm();
		_quat_0 = _quat_0 / norm;
		_quat_e = _quat_e / norm;
	}

	/// Transformation of point p with the dual quaternion
	glm::vec3 transform(const glm::vec3& p) const
	{
		// As the dual quaternions may be the results from a
		// linear blending we have to normalize it :
		float norm = glm::length(_quat_0); // .norm();
		glm::quat qblend_0 = _quat_0 / norm;
		glm::quat qblend_e = _quat_e  / norm;

		// Translation from the normalized dual quaternion equals :
		// 2.f * qblend_e * conjugate(qblend_0)
		glm::vec3 v0 = glm::vec3(qblend_0.x, qblend_0.y, qblend_0.z);//.get_vec_part();
		glm::vec3 ve = glm::vec3(qblend_e.x, qblend_0.y, qblend_0.z); //.get_vec_part();
		glm::vec3 trans = (ve * qblend_0.w - v0 * qblend_e.w + 2.f * glm::cross(v0,ve));

		// Rotate
		return glm::rotate(qblend_0, p) + trans; //.rotate(p) + trans;
	}

	/// Rotate a vector with the dual quaternion
	glm::vec3 rotate(const glm::vec3& v) const
	{
		glm::quat tmp = _quat_0;
		glm::normalize(tmp); //.normalize();
		return glm::rotate(tmp, v); // .rotate(v);
	}

	Dual_quat_cu dual_quat_from(const glm::quat& q, const glm::vec3& t) const
	{
		float w = -0.5f*(t.x * q.x + t.y * q.y + t.z * q.z);
		float i = 0.5f*(t.x * q.w + t.y * q.z - t.z * q.y);
		float j = 0.5f*(-t.x * q.z + t.y * q.w + t.z * q.x);
		float k = 0.5f*(t.x * q.y - t.y * q.x + t.z * q.w);

		return Dual_quat_cu(q, glm::quat(w, i, j, k));
	}

	inline glm::mat4 from3(const glm::mat3& x, const glm::vec3& v) {
		// Column major
		glm::mat4 res(x[0][0], x[0][1], x[0][2], 0.f,
						x[1][0], x[1][1], x[1][2], 0.f,
						x[2][0], x[2][1], x[2][2], 0.f,
						v.x, v.y, v.z, 1.f);

		return res;
	}

	/// Convert the dual quaternion to a homogenous matrix
	/// N.B: Dual quaternion is normalized before conversion
	glm::mat4 to_transformation()
	{
		glm::vec3 t;
		float norm = glm::length(_quat_0); //.norm();

		// Rotation matrix from non-dual quaternion part
		glm::mat3 m = glm::mat3_cast(_quat_0 / norm); //.to_matrix3();

		// translation vector from dual quaternion part:
		t.x = 2.f*(-_quat_e.w*_quat_0.x + _quat_e.x*_quat_0.w - _quat_e.y*_quat_0.z + _quat_e.z*_quat_0.y) / norm;
		t.y = 2.f*(-_quat_e.w*_quat_0.y + _quat_e.x*_quat_0.z + _quat_e.y*_quat_0.w - _quat_e.z*_quat_0.x) / norm;
		t.z = 2.f*(-_quat_e.w*_quat_0.z - _quat_e.x*_quat_0.y + _quat_e.y*_quat_0.x + _quat_e.z*_quat_0.w) / norm;

		return from3(m, t);
	}

	// -------------------------------------------------------------------------
	/// @name Operators
	// -------------------------------------------------------------------------

	Dual_quat_cu operator+(const Dual_quat_cu& dq) const
	{
		return Dual_quat_cu(_quat_0 + dq._quat_0, _quat_e + dq._quat_e);
	}

	Dual_quat_cu operator*(float scalar) const
	{
		return Dual_quat_cu(_quat_0 * scalar, _quat_e * scalar);
	}

	/// Return a dual quaternion with no translation and no rotation
	static Dual_quat_cu identity()
	{
		return Dual_quat_cu(glm::quat(1.f, 0.f, 0.f, 0.f),
			glm::vec3(0.f, 0.f, 0.f));
	}

	// -------------------------------------------------------------------------
	/// @name Getters
	// -------------------------------------------------------------------------

	glm::quat get_dual_part() const { return _quat_e; }

	glm::quat get_non_dual_part() const { return _quat_0; }

	glm::quat translation() const { return _quat_e; }

	glm::quat rotation() const { return _quat_0; }

	void set_rotation(const glm::quat& q) { _quat_0 = q; }

	// -------------------------------------------------------------------------
	/// @name Attributes
	// -------------------------------------------------------------------------

private:
	glm::quat _quat_0;
	glm::quat _quat_e;
};