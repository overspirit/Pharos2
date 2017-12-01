#pragma once

namespace Pharos
{
	namespace Math
	{
		//! 3D线
		class Line3D
		{
		public:
			//! 默认构造函数
			Line3D()
			{
				start.Set(0, 0, 0);

				end.Set(1, 1, 1);
			}

			//! 用两点初始化值
			Line3D(float32 xa, float32 ya, float32 za, float32 xb, float32 yb, float32 zb)
			{
				start.Set(xa, ya, za);

				end.Set(xb, yb, zb);
			}

			//! 用两点初始化值
			Line3D(const Vector3Df& nstart, const Vector3Df& nend)
			{
				start = nstart;
				end = nend;
			}

			Line3D(const Line3D& other)
			{
				start = other.start;
				end = other.end;
			}

			// 运算符重载

			Line3D operator+(const Vector3Df& point) const { return Line3D(start + point, end + point); }
			Line3D& operator+=(const Vector3Df& point) { start += point; end += point; return *this; }

			Line3D operator-(const Vector3Df& point) const { return Line3D(start - point, end - point); }
			Line3D& operator-=(const Vector3Df& point) { start -= point; end -= point; return *this; }

			bool operator==(const Line3D& other) const
			{
				return (start == other.start && end == other.end) || (end == other.start && start == other.end);
			}

			bool operator!=(const Line3D& other) const
			{
				return !((start == other.start && end == other.end) || (end == other.start && start == other.end));
			}

			// functions

			//! 初始化新值
			void SetLine(float32 xa, float32 ya, float32 za, float32 xb, float32 yb, float32 zb)
			{
				start.Set(xa, ya, za); end.Set(xb, yb, zb);
			}

			//! Set this line to a new line going through the two points.
			void SetLine(const Vector3Df& nstart, const Vector3Df& nend)
			{
				start.Set(nstart); end.Set(nend);
			}

			//! Set this line to new line given as parameter.
			void SetLine(const Line3D& line)
			{
				start.Set(line.start); end.Set(line.end);
			}

			//! 获得线段长度
			float32 GetLength() const { return start.GetDistanceFrom(end); }

			//! 获得线段长度的平方
			float32 GetLengthSQ() const { return start.GetDistanceFromSQ(end); }

			//! 获得线段中点
			Vector3Df GetMiddle() const
			{
				return (start + end) / (float32)2;
			}

			//! 获得直线方向向量,没有归一化
			Vector3Df GetVector() const
			{
				return (end - start);
			}

			//! 检测给定点是否在线段的两点之间
			bool IsPointBetweenStartAndEnd(const Vector3Df& point) const
			{
				return point.IsBetweenPoints(start, end);
			}

			//! 获得线段上距指定点最近的点
			Vector3Df GetClosestPoint(const Vector3Df& point) const
			{
				Vector3Df c = point - start;
				Vector3Df v = end - start;
				float32 d = (float32)v.GetLength();
				v /= d;
				float32 t = v.DotProduct(c);

				if (t < (float32)0.0)
					return start;

				if (t > d)
					return end;

				v *= t;
				return start + v;
			}

			//检测直线是否和球碰撞,交点可能落在线段外
			bool GetIntersectionWithSphere(const Vector3Df& sorigin, float32 sradius, float64& outdistance) const
			{
				const Vector3Df q = sorigin - start;
				Vector3Df dir = GetVector().Normalize();

				float32 v = q.DotProduct(dir);
				float32 d = sradius * sradius - (q.GetLengthSQ() - v*v);

				if (d < 0.0) return false;

				outdistance = v - sqrt(d);

				return true;
			}

			//检测直线是否和球碰撞，无返回交点距离，所以速度比较快
			bool GetIntersectionWithSphere(const Vector3Df& sorigin, float32 sradius) const
			{
				const Vector3Df q = sorigin - start;
				Vector3Df dir = GetVector().Normalize();

				float32 v = q.DotProduct(dir);
				float32 d = sradius * sradius - (q.GetLengthSQ() - v*v);

				if (d < 0.0) return false;

				return true;
			}

			//! 获得指定点到直线的距离
			float32 GetDistToPoint(const Vector3Df& pos) const
			{
				Vector3Df d = pos - start;
				Vector3Df normal = GetVector().Normalize();

				float32 fa1 = normal.DotProduct(d);
				float32 fa2 = d.GetLengthSQ();

				return (float32)sqrt(fa2 - fa1*fa1);
			}

			// member variables

		public:
			Vector3Df start;
			Vector3Df end;
		};
	}

	typedef Math::Line3D Line3Df;
}