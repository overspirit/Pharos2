 #pragma once

#define COLOR_A(c) (((c) >> 24) & 0xFF)
#define COLOR_R(c) (((c) >> 16) & 0xFF)
#define COLOR_G(c) (((c) >> 8) & 0xFF)
#define COLOR_B(c) ((c) & 0xFF)

#define COLOR_RGBA(r,g,b,a) ((Color4)((((a)&0xff)<<24)|(((b)&0xff)<<16)|(((g)&0xff)<<8)|((r)&0xff)))
#define COLOR_ARGB(a,r,g,b) COLOR_RGBA(r,g,b,a)
#define COLOR_XRGB(r,g,b)   COLOR_ARGB(0xff,r,g,b)

#define COLOR_ARGB_TO_RGBA(c) (COLOR_RGBA(COLOR_R(c), COLOR_G(c), COLOR_B(c), COLOR_A(c)))

namespace Pharos
{
	typedef uint32	Color4;

	namespace Math
	{
		//由4个浮点数组成的颜色
		class CColor4f
		{
		public:
			//构造函数
			CColor4f();
			CColor4f(float32 r, float32 g, float32 b, float32 a = 1.0f);
			CColor4f(uint8 r, uint8 g, uint8 b, uint8 a = 255);
			CColor4f(Color4 color);

			//运算符重载
			operator Color4 () const;
			operator float32* ();
			operator const float32* () const;

			bool operator == (const CColor4f& other) const;
			bool operator != (const CColor4f& other) const;

			const CColor4f& operator + (float32 r);
			const CColor4f& operator + (const CColor4f& c);
			const CColor4f& operator += (float32 r);
			const CColor4f& operator += (const CColor4f& c);

			const CColor4f& operator - (float32 r);
			const CColor4f& operator - (const CColor4f& c);
			const CColor4f& operator -= (float32 r);
			const CColor4f& operator -= (const CColor4f& c);

			const CColor4f& operator * (float32 r);
			const CColor4f& operator * (const CColor4f& c);
			const CColor4f& operator *= (float32 r);
			const CColor4f& operator *= (const CColor4f& c);

			const CColor4f& operator / (float32 r);
			const CColor4f& operator / (const CColor4f& c);
			const CColor4f& operator /= (float32 r);
			const CColor4f& operator /= (const CColor4f& c);

			//颜色反转
			void Negative();

			//使用color1和color2插值，s为插值比例数
			void Lerp(const CColor4f& color1, const CColor4f& color2, float32 s);
			
			//调整颜色饱和度，S为饱和度，S为0时，该颜色就是灰色
			void AdjustSaturation(float32 s);

			float32 r, g, b, a;
		};

		inline CColor4f::CColor4f()
		{
			this->a = 1.0f;
			this->r = 0.0f;
			this->g = 0.0f;
			this->b = 0.0f;
		}

		inline CColor4f::CColor4f(float32 r, float32 g, float32 b, float32 a)
		{
			this->r = r;
			this->g = g;
			this->b = b;
			this->a = a;
		}

		inline CColor4f::CColor4f(uint8 r, uint8 g, uint8 b, uint8 a)
		{
			this->a = a / 255.0f;
			this->r = r / 255.0f;
			this->g = g / 255.0f;
			this->b = b / 255.0f;
		}

		inline CColor4f::CColor4f(Color4 color)
		{
			this->a = ((color >> 24) & 0xFF) / 255.0f;
			this->r = ((color >> 16) & 0xFF) / 255.0f;
			this->g = ((color >> 8) & 0xFF) / 255.0f;
			this->b = ((color)& 0xFF) / 255.0f;
		}

		inline CColor4f::operator Color4 () const
		{
			return ((uint8)(a * 255) << 24) | ((uint8)(r * 255) << 16) | ((uint8)(g * 255) << 8) | (uint8)(b * 255);
		}

		inline CColor4f::operator float32* ()
		{
			return (float32*)&r;
		}

		inline CColor4f::operator const float32* () const
		{
			return (const float32*)&r;
		}

		inline bool CColor4f::operator == (const CColor4f& other) const
		{
			return other.r == r && other.g == g && other.b == b && other.a == a;
		}

		inline bool CColor4f::operator != (const CColor4f& other) const
		{
			return other.r != r || other.g != g || other.b != b || other.a != a;
		}

		inline const CColor4f& CColor4f::operator + (float32 r)
		{
			r += r;	g += r;	b += r;	a += r;
			return *this;
		}

		inline const CColor4f& CColor4f::operator + (const CColor4f& c)
		{
			r += c.r;	g += c.g;	b += c.b;	a += c.a;
			return *this;
		}

		inline const CColor4f& CColor4f::operator += (float32 r)
		{
			r += r;	g += r;	b += r;	a += r;
			return *this;
		}

		inline const CColor4f& CColor4f::operator += (const CColor4f& c)
		{
			r += c.r;	g += c.g;	b += c.b;	a += c.a;
			return *this;
		}

		inline const CColor4f& CColor4f::operator - (float32 r)
		{
			r -= r;	g -= r;	b -= r;	a -= r;
			return *this;
		}

		inline const CColor4f& CColor4f::operator - (const CColor4f& c)
		{
			r -= c.r;	g -= c.g;	b -= c.b;	a -= c.a;
			return *this;
		}

		inline const CColor4f& CColor4f::operator -= (float32 r)
		{
			r -= r;	g -= r;	b -= r;	a -= r;
			return *this;
		}

		inline const CColor4f& CColor4f::operator -= (const CColor4f& c)
		{
			r -= c.r;	g -= c.g;	b -= c.b;	a -= c.a;
			return *this;
		}

		inline const CColor4f& CColor4f::operator * (float32 r)
		{
			r *= r;	g *= r;	b *= r;	a *= r;
			return *this;
		}

		inline const CColor4f& CColor4f::operator * (const CColor4f& c)
		{
			r *= c.r;	g *= c.g;	b *= c.b;	a *= c.a;
			return *this;
		}

		inline const CColor4f& CColor4f::operator *= (float32 r)
		{
			r *= r;	g *= r;	b *= r;	a *= r;
			return *this;
		}

		inline const CColor4f& CColor4f::operator *= (const CColor4f& c)
		{
			r *= c.r;	g *= c.g;	b *= c.b;	a *= c.a;
			return *this;
		}

		inline const CColor4f& CColor4f::operator / (float32 r)
		{
			r /= r;	g /= r;	b /= r;	a /= r;
			return *this;
		}

		inline const CColor4f& CColor4f::operator / (const CColor4f& c)
		{
			r /= c.r;	g /= c.g;	b /= c.b;	a /= c.a;
			return *this;
		}

		inline const CColor4f& CColor4f::operator /= (float32 r)
		{
			r /= r;	g /= r;	b /= r;	a /= r;
			return *this;
		}

		inline const CColor4f& CColor4f::operator /= (const CColor4f& c)
		{
			r /= c.r;	g /= c.g;	b /= c.b;	a /= c.a;
			return *this;
		}

		inline void CColor4f::Negative()
		{
			this->r = 1.0f - this->r;
			this->g = 1.0f - this->g;
			this->b = 1.0f - this->b;
		}

		inline void CColor4f::Lerp(const CColor4f& color1, const CColor4f& color2, float32 s)
		{
			this->r = color1.r + s * (color2.r - color1.r);
			this->g = color1.g + s * (color2.g - color1.g);
			this->b = color1.b + s * (color2.b - color1.b);
			this->a = color1.a + s * (color2.a - color1.a);
		}

		inline void CColor4f::AdjustSaturation(float32 s)
		{
			float32 grey = this->r * 0.2125f + this->g * 0.7154f + this->b * 0.0721f;
			this->r = grey + s * (this->r - grey);
			this->g = grey + s * (this->g - grey);
			this->b = grey + s * (this->b - grey);
		}
	}

	typedef Math::CColor4f Color4f;
}