#pragma once

namespace Pharos
{
	namespace Utils
	{
		//从此类继承将禁止类的拷贝构造函数和赋值构造函数
		class Noncopyable
		{
		protected:
			Noncopyable() {}
			virtual ~Noncopyable() {}

		private:
			Noncopyable(const Noncopyable&);
			const Noncopyable& operator=(const Noncopyable&);
		};

		template <typename T>
		class Singleton
		{
		protected:
			Singleton() {}
			virtual ~Singleton() {}

		private:
			Singleton(const Singleton& source) {}

		public:
			static T* Inst()
			{
				static T t;
				return &t;
			}
		};
	}
}