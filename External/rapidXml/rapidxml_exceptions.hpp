#ifndef RAPIDXML_EXCEPTIONS_INCLUDED
#define RAPIDXML_EXCEPTIONS_INCLUDED

///////////////////////////////////////////////////////////////////////////
// RAPIDXML_PARSE_ERROR

#if defined(RAPIDXML_NO_EXCEPTIONS)

#define RAPIDXML_PARSE_ERROR(what, where) { parse_error_handler(what, where); assert(0); }

namespace rapidxml
{
	//! When exceptions are disabled by defining RAPIDXML_NO_EXCEPTIONS, 
	//! this function is called to notify user about the error.
	//! It must be defined by the user.
	//! <br><br>
	//! This function cannot return. If it does, the results are undefined.
	//! <br><br>
	//! A very simple definition might look like that:
	//! <pre>
	//! void %rapidxml::%parse_error_handler(const char *what, void *where)
	//! {
	//!     std::cout << "Parse error: " << what << "\n";
	//!     std::abort();
	//! }
	//! </pre>
	//! \param what Human readable description of the error.
	//! \param where Pointer to character data where error was detected.
	void parse_error_handler(const char *what, void *where);
}

#else

#include <exception>    // For std::exception

#define RAPIDXML_PARSE_ERROR(what, where) throw parse_error(what, where)

namespace rapidxml
{

	//! Parse error exception. 
	//! This exception is thrown by the parser when an error occurs. 
	//! Use what() function to get human-readable error message. 
	//! Use where() function to get a pointer to position within source text where error was detected.
	//! <br><br>
	//! If throwing exceptions by the parser is undesirable, 
	//! it can be disabled by defining RAPIDXML_NO_EXCEPTIONS macro before rapidxml.hpp is included.
	//! This will cause the parser to call rapidxml::parse_error_handler() function instead of throwing an exception.
	//! This function must be defined by the user.
	//! <br><br>
	//! This class derives from <code>std::exception</code> class.
	class parse_error : public std::exception
	{

	public:
		//! Constructs parse error
		parse_error(const char *what, void *where)
			: m_what(what)
			, m_where(where)
		{
		}

		//! Gets human readable description of error.
		//! \return Pointer to null terminated description of the error.
		virtual const char *what() const throw()
		{
			return m_what;
		}

		//! Gets pointer to character data where error happened.
		//! Ch should be the same as char type of xml_document that produced the error.
		//! \return Pointer to location within the parsed string where error occured.
		template<class Ch>
		Ch *where() const
		{
			return reinterpret_cast<Ch *>(m_where);
		}

	private:

		const char *m_what;
		void *m_where;

	};
}

#endif

#endif