#ifndef WFPOINT3D_H_INCLUDED
#define WFPOINT3D_H_INCLUDED

namespace fw
{
	namespace math
	{
		class Point3D
		{
			public:
				virtual ~Point3D() { }
				Point3D();
				Point3D(double dX, double dY, double dZ);

				void setX(double dX) { m_dX = dX;  }
				double getX() const { return m_dX; }
				void setY(double dY) { m_dY = dY;  }
				double getY() const { return m_dY; }
				double setZ(double dZ) { m_dZ = dZ; }
				double getZ() const { return m_dZ; }


			protected:

				double m_dX; 
				double m_dY;
				double m_dZ;
		};

	}

}

static inline fw::math::Point3D operator-(const fw::math::Point3D& pt1, const fw::math::Point3D& pt2)
{
	return fw::math::Point3D(pt1.getX() - pt2.getX(), pt1.getY() - pt2.getY(), pt1.getZ() - pt2.getZ());
}

#endif // WFPOINT3D_H_INCLUDED
