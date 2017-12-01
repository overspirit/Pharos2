#pragma once

/**
对类型简单包装之后，通过类名生成类的实例
*/

//根类型的声明宏，放在类声明中，
//------------------------------------------------------------------------------
#define DECLARE_ROOT_CLASS(type) \
public: \
    static Utils::TypeWrapper<type> typeWrapper; \
	static bool RegisterWithFactory(); \
private:
//------------------------------------------------------------------------------

//子类型的声明宏，放在类声明中，
//------------------------------------------------------------------------------
#define DECLARE_CLASS(type, rootType) \
public: \
    static Utils::TypeWrapper<type, rootType> typeWrapper; \
	static bool RegisterWithFactory(); \
private:
//------------------------------------------------------------------------------

//根类型的实现宏，放在CPP文件中生成
//------------------------------------------------------------------------------
#define IMPLEMENT_ROOT_CLASS(type, factory) \
    Utils::TypeWrapper<type> type::typeWrapper(#type); \
    bool type::RegisterWithFactory(){return factory.Register(&type::typeWrapper);} \
	static const bool type##_registered = type::RegisterWithFactory();
//------------------------------------------------------------------------------

//子类型的实现宏，放在CPP文件中生成
//------------------------------------------------------------------------------
#define IMPLEMENT_CLASS(type, rootType, factory) \
    Utils::TypeWrapper<type, rootType> type::typeWrapper(#type); \
	bool type::RegisterWithFactory() { return factory.Register(&type::typeWrapper); } \
	static const bool type##_registered = type::RegisterWithFactory();
//------------------------------------------------------------------------------


namespace Pharos
{
	namespace Utils
	{
		//根类型的包装
		template<typename T>
		class ITypeWrapper
		{
		public:
			virtual ~ITypeWrapper() {}

		public:
			virtual const string& GetName() const = 0;

			virtual std::shared_ptr<T> CreateObject() const = 0;
		};

		//子类型的包装，需要传递当前类型和根类型
		template<typename T, typename R = T>
		class TypeWrapper : public ITypeWrapper<R>
		{
		public:
			TypeWrapper(const string& className)
			{
				m_name = className;
			}

		private:
			string	m_name;

		public:
			virtual const string& GetName() const { return this->m_name; }

			virtual std::shared_ptr<R> CreateObject() const { return MakeSharedPtr<T>(); }
		};

		//类型工厂，需要传递根类型
		template<typename T>
		class Factory
		{
		public:
			Factory() {}
			~Factory() {}

		private:
			map<string, ITypeWrapper<T>*>	m_nameTable;

		public:
			//可以注册一个接口类型
			bool Register(ITypeWrapper<T>* type, const string& className)
			{
				if (this->ClassExists(className)) return false;
				m_nameTable[className] = type;
				return true;
			}

			bool Register(ITypeWrapper<T>* type)
			{
				const string& className = type->GetName();
				if (this->ClassExists(className)) return false;
				m_nameTable[className] = type;
				return true;
			}

			bool ClassExists(const string& className) const
			{
				return (m_nameTable.find(className) != m_nameTable.end());
			}

			ITypeWrapper<T>* GetClassRuntimeType(const string& className) const
			{
				auto iter = m_nameTable.find(className);
				if (iter != m_nameTable.end()) return iter->second;
				return NULL;
			}

			std::shared_ptr<T> CreateObject(const string& className) const
			{
				auto iter = m_nameTable.find(className);
				if (iter != m_nameTable.end())
				{
					ITypeWrapper<T>* typeWrapper = iter->second;
					return typeWrapper->CreateObject();
				}

				return NULL;
			}
		};
	}
}

