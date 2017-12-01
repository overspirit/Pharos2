#pragma once

namespace Pharos
{
	namespace Math
	{
		//! 2D线		
		class Line2D
		{
		public:
			//! 默认构造函数
			Line2D()
			{
				start.Set(0, 0);

				end.Set(1, 1);
			}

			//! 使用两点的值初始化线
			Line2D(float32 xa, float32 ya, float32 xb, float32 yb)
			{
				start.Set(xa, ya);

				end.Set(xb, yb);
			}

			//! 使用两点初始化线
			Line2D(const Vector2D& nstart, const Vector2D& nend)
			{
				start = nstart;
				end = nend;
			}

			//! 拷贝构造函数
			Line2D(const Line2D& other)
			{
				start = other.start;
				end = other.end;
			}

			// 运算符重载

			Line2D operator+(const Vector2D& point) const { return Line2D(start + point, end + point); }
			Line2D& operator+=(const Vector2D& point) { start += point; end += point; return *this; }

			Line2D operator-(const Vector2D& point) const { return Line2D(start - point, end - point); }
			Line2D& operator-=(const Vector2D& point) { start -= point; end -= point; return *this; }

			bool operator==(const Line2D& other) const
			{
				return (start == other.start && end == other.start) || (end == other.start && start == other.end);
			}

			bool operator!=(const Line2D& other) const
			{
				return !((start == other.start && end == other.start) || (end == other.start && start == other.end));
			}

			// functions
			//! 设置新值
			void SetLine(float32 xa, float32 ya, float32 xb, float32 yb)
			{
				start.Set(xa, ya); end.Set(xb, yb);
			}

			//! 设置新值
			void SetLine(const Vector2D& nstart, const Vector2D& nend)
			{
				start.Set(nstart); end.Set(nend);
			}

			//! 设置新值
			void SetLine(const Line2D& line)
			{
				start.Set(line.start); end.Set(line.end);
			}

			//! 获得线段长度
			float32 GetLength() const { return start.GetDistanceFrom(end); }

			//! 获得线段长度的平方
			float32 GetLengthSQ() const { return start.GetDistanceFromSQ(end); }

			//! 获得线段的中心点，没有归一化
			Vector2D GetMiddle() const
			{
				return (start + end) / (float32)2;
			}

			//! 获得直线的方向向量
			Vector2D GetVector() const
			{
				return (end - start);
			}

			//! 检测和另一个直线是否有交点，out为返回的交点，checkOnlySegments为交点必须落在线段内
			bool IntersectWith(const Line2D& l, Vector2D& out, bool checkOnlySegments = true) const
			{
				// Uses the method given at:
				// http://local.wasp.uwa.edu.au/~pbourke/geometry/lineline2d/
				const float32 commonDenominator = (float32)(l.end.y - l.start.y)*(end.x - start.x) -
					(l.end.x - l.start.x)*(end.y - start.y);

				const float32 numeratorA = (float32)(l.end.x - l.start.x)*(start.y - l.start.y) -
					(l.end.y - l.start.y)*(start.x - l.start.x);

				const float32 numeratorB = (float32)(end.x - start.x)*(start.y - l.start.y) -
					(end.y - start.y)*(start.x - l.start.x);

				if (equals(commonDenominator, 0.f))
				{
					// The lines are either coincident or parallel
					// if both numerators are 0, the lines are coincident
					if (equals(numeratorA, 0.f) && equals(numeratorB, 0.f))
					{
						// Try and find a common endpoint
						if (l.start == start || l.end == start)
							out = start;
						else if (l.end == end || l.start == end)
							out = end;
						// now check if the two segments are disjunct
						else if (l.start.x > start.x && l.end.x > start.x && l.start.x > end.x && l.end.x > end.x)
							return false;
						else if (l.start.y > start.y && l.end.y > start.y && l.start.y > end.y && l.end.y > end.y)
							return false;
						else if (l.start.x < start.x && l.end.x < start.x && l.start.x < end.x && l.end.x < end.x)
							return false;
						else if (l.start.y < start.y && l.end.y < start.y && l.start.y < end.y && l.end.y < end.y)
							return false;
						// else the lines are overlapping to some extent
						else
						{
							// find the points which are not contributing to the
							// common part
							Vector2D maxp;
							Vector2D minp;
							if ((start.x > l.start.x && start.x > l.end.x && start.x > end.x) || (start.y > l.start.y && start.y > l.end.y && start.y > end.y))
								maxp = start;
							else if ((end.x > l.start.x && end.x > l.end.x && end.x > start.x) || (end.y > l.start.y && end.y > l.end.y && end.y > start.y))
								maxp = end;
							else if ((l.start.x > start.x && l.start.x > l.end.x && l.start.x > end.x) || (l.start.y > start.y && l.start.y > l.end.y && l.start.y > end.y))
								maxp = l.start;
							else
								maxp = l.end;
							if (maxp != start && ((start.x < l.start.x && start.x < l.end.x && start.x < end.x) || (start.y < l.start.y && start.y < l.end.y && start.y < end.y)))
								minp = start;
							else if (maxp != end && ((end.x < l.start.x && end.x < l.end.x && end.x < start.x) || (end.y < l.start.y && end.y < l.end.y && end.y < start.y)))
								minp = end;
							else if (maxp != l.start && ((l.start.x < start.x && l.start.x < l.end.x && l.start.x < end.x) || (l.start.y < start.y && l.start.y < l.end.y && l.start.y < end.y)))
								minp = l.start;
							else
								minp = l.end;

							// one line is contained in the other. Pick the center
							// of the remaining points, which overlap for sure
							out = Vector2D();
							if (start != maxp && start != minp)
								out += start;
							if (end != maxp && end != minp)
								out += end;
							if (l.start != maxp && l.start != minp)
								out += l.start;
							if (l.end != maxp && l.end != minp)
								out += l.end;
							out.x = (float32)(out.x / 2);
							out.y = (float32)(out.y / 2);
						}

						return true; // coincident
					}

					return false; // parallel
				}

				// Get the point of intersection on this line, checking that
				// it is within the line segment.
				const float32 uA = numeratorA / commonDenominator;
				if (checkOnlySegments && (uA < 0.f || uA > 1.f))
					return false; // Outside the line segment

				const float32 uB = numeratorB / commonDenominator;
				if (checkOnlySegments && (uB < 0.f || uB > 1.f))
					return false; // Outside the line segment

				// Calculate the intersection point.
				out.x = (float32)(start.x + uA * (end.x - start.x));
				out.y = (float32)(start.y + uA * (end.y - start.y));
				return true;
			}

			//! 获得和另一条直线的夹角，结果为弧度制
			float64 GetAngleWith(const Line2D& l) const
			{
				Vector2D vect = GetVector();
				Vector2D vect2 = l.GetVector();
				return vect.GetAngleWith(vect2);
			}

			//! 获取一个值，这个值小于0说明点在直线的左侧，大于0说明点在直线的右侧，等于0说明点在直线上
			float32 GetPointOrientation(const Vector2D& point) const
			{
				return ((end.x - start.x) * (point.y - start.y) -
					(point.x - start.x) * (end.y - start.y));
			}

			//! 检测点是否在线段上
			bool IsPointOnLine(const Vector2D& point) const
			{
				float32 d = GetPointOrientation(point);
				return (iszero(d) && point.IsBetweenPoints(start, end));
			}

			//! 检测点是否在线段两点之间
			bool IsPointBetweenStartAndEnd(const Vector2D& point) const
			{
				return point.IsBetweenPoints(start, end);
			}

			//! 获得直线上距给定点最近的点，checkOnlySegments为是否返回点必须在线段内
			Vector2D GetClosestPoint(const Vector2D& point, bool checkOnlySegments = true) const
			{
				Vector2D c((point.x - start.x), (point.y - start.y));
				Vector2D v((end.x - start.x), (end.y - start.y));
				float32 d = v.GetLength();
				if (iszero(d))	// can't tell much when the line is just a single point
					return start;
				v /= d;
				float32 t = v.DotProduct(c);

				if (checkOnlySegments)
				{
					if (t < 0) return Vector2D((float32)start.x, (float32)start.y);
					if (t > d) return Vector2D((float32)end.x, (float32)end.y);
				}

				v *= t;
				return Vector2D((float32)(start.x + v.x), (float32)(start.y + v.y));
			}

		public:
			Vector2D start;
			Vector2D end;
		};
	}

	typedef Math::Line2D Line2Df;
}