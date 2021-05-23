/*<!-- author:urays email:urays@foxmail.com date: 2021.05.23 -->*/
#ifndef LASM_SUP_UNIT_TEST_H
#define LASM_SUP_UNIT_TEST_H  
// lasm is one of my current projects, it's a assembly optimizer. I cannot open it now.

// c-plus-plus

#include <iostream>
#include <string>
#include <vector>
#include <typeinfo>

#ifdef __linux__
#include <sys/time.h>

// The argument now should be a double (not a pointer to a double)
#define GET_TIME(now) {                  \
   struct timeval t;                     \
   gettimeofday(&t, NULL);               \
   now = t.tv_sec + t.tv_usec/1000000.0; \
}

#elif _WIN32
#include <time.h>

#define GET_TIME(now) {     \
   clock_t t = clock();     \
   now = (double)t/CLK_TCK; \
}
#endif

namespace utest
{
#define UOUT             (*_o)   //o_o  ^_^  *_*  +_+
	class testcase {
	public:
		testcase() {}
		virtual ~testcase() {}

		virtual void body() = 0;
	private:
		void operator=(testcase const&) = delete;
		testcase(testcase const&) = delete;
	};

	class testinfo {
	public:
		testinfo(const std::string& _tcn, const std::string& _tn, const std::string& _file, int _line)
			:test_case_name(_tcn), test_name(_tn), loc_file(_file), loc_line(_line)
		{
		}

	private:
		friend class unittest;

		std::string test_case_name;
		std::string test_name;

		std::string loc_file; // code location
		int loc_line;
	};

	template <class testclass>
	testinfo* regtest(const char*, const char*, const char*, int);

	template <typename T1, typename T2>
	void dispone(const int, const char*, const char*, const T1&, const T2&);

	//#define JUSTPRINT   ("JUSTPRINT")

	class unittest {
	public:
		static unittest* addr()
		{
			static unittest _a;
			return &_a;
		}

	private:
		template <class testclass>
		friend testinfo* regtest(const char*, const char*, const char*, int);

		friend void RUN_ALL(std::ostream&);

		template <typename T1, typename T2>
		friend void dispone(const int, const char*, const char*, const T1&, const T2&);

		void _sto(std::ostream& x = std::cout) { _o = &x; }

		void _run()
		{
			for (unsigned i = 0; i < _cupi.size(); ++i)
			{
				UOUT << "::" << _cupi[i]->test_case_name << "::" << _cupi[i]->test_name << " - ";
				UOUT << _cupi[i]->loc_file << ", " << _cupi[i]->loc_line << std::endl;

				UOUT << "{\n";
				_ridx = i;
				_cupc[i]->body();
				UOUT << "}\n\n";
			}
		}

		template <typename T1, typename T2>
		void _dispone(const int _l, const char* _v1e, const char* _v2e, const T1& _v1, const T2& _v2)
		{
			if (_displn != -1 && (_l - _displn > 1 || _l <= _displn)) { UOUT << "\n"; }
			_displn = _l;

			bool _eqmark = true;
			{
				_eqmark = (_v1 == _v2);
			}

			std::string ln = std::to_string(_l);

			for (int i = 5 - ln.size(); i >= 0; --i) { UOUT << " "; }
			UOUT << ln << "|" << (_eqmark ? "   " : "¡Á ") << _v2e << " ";

			int _v2elen = 0;
			for (const char* p = _v2e; *p++ != '\0'; ++_v2elen) {}
			for (int i = 50 - _v2elen; i >= 0; --i) { UOUT << (_eqmark ? " " : "-"); }

			UOUT << "{";
			if (_eqmark) {
				if (typeid(_v2) == typeid(bool)) { UOUT << "(bool, " << _v2 << ")"; }
				else { UOUT << _v2; }
			}
			else {
				if (typeid(_v2) == typeid(bool)) { UOUT << "(bool, " << _v2 << ")"; }
				else { UOUT << _v2; }

				UOUT << "}, Expected Value:{";// << _v1e << ", ";

				if (typeid(_v1) == typeid(bool)) { UOUT << "(bool, " << _v1 << ")"; }
				else { UOUT << _v1; }
				_xcases.push_back({ _ridx, _l });  //record the failed case.
			}
			UOUT << "}\n";
		}

		void _statics()
		{
			uint64_t _sz = _xcases.size();

			UOUT << "[STATICS:FAILED:" << _sz << "]";
			if (_sz > 0)
			{
				unsigned _la = _xcases[0].first;
				for (unsigned i = 0; i < _sz; ++i)
				{
					if (i == 0 || _cupi[_la]->loc_file != _cupi[_xcases[i].first]->loc_file)
					{
						if (i > 0) { UOUT << "}"; }
						UOUT << "\n::" << _cupi[_xcases[i].first]->loc_file << "::{" << _xcases[i].second;
						_la = _xcases[i].first;
					}
					else {
						UOUT << ", " << _xcases[i].second;
					}
				}
				UOUT << "}";
			}
			UOUT << "\n\n";
		}

		unittest() {
			_o = nullptr;
			_ridx = 0;
			_displn = -1;
		}

		~unittest() {
			for (unsigned i = 0; i < _cupi.size(); ++i) { delete _cupi[i]; }
			for (unsigned i = 0; i < _cupc.size(); ++i) { delete _cupc[i]; }
		}

		void _add(testcase* _tc, testinfo* _ti)
		{
			_cupc.push_back(_tc);
			_cupi.push_back(_ti);
		}

		std::ostream* _o;

		std::vector<testcase*> _cupc;
		std::vector<testinfo*> _cupi;

		unsigned _ridx; // 
		std::vector<std::pair<unsigned, unsigned>> _xcases;

		int _displn;  // save the last __LINE__
	};

	template <class testclass>
	testinfo* regtest(const char* _tcn, const char* _tn, const char* _f, int _l)
	{
		testinfo* _a = new testinfo(_tcn, _tn, _f, _l);
		unittest::addr()->_add(new testclass, _a);
		return _a;
	}

	template <typename T1, typename T2>
	void dispone(const int _l, const char* _v1e, const char* _v2e, const T1& _v1, const T2& _v2)
	{
		unittest::addr()->_dispone(_l, _v1e, _v2e, _v1, _v2);
	}

	inline void RUN_ALL(std::ostream& o = std::cout)
	{
		unittest::addr()->_sto(o);
		unittest::addr()->_run();
		unittest::addr()->_statics();
	}
}

#define TEST_CLASS_NAME(test_case_name, test_name) test_case_name##_##test_name##_test

#define TEST(test_case_name, test_name)                                          \
class TEST_CLASS_NAME(test_case_name, test_name) : public ::utest::testcase      \
{                                                                                \
public:                                                                          \
    TEST_CLASS_NAME(test_case_name, test_name)() {}                              \
    virtual void body();                                                         \
    static ::utest::testinfo* const info;                                        \
private:                                                                         \
    void operator=(TEST_CLASS_NAME(test_case_name, test_name) const &) = delete; \
    TEST_CLASS_NAME(test_case_name, test_name)(                                  \
    TEST_CLASS_NAME(test_case_name, test_name) const&) = delete;                 \
};                                                                               \
::utest::testinfo* const TEST_CLASS_NAME(test_case_name, test_name)::info =      \
::utest::regtest<TEST_CLASS_NAME(test_case_name, test_name)>                     \
                   (#test_case_name, #test_name, __FILE__, __LINE__);            \
void TEST_CLASS_NAME(test_case_name, test_name)::body()

// recommend to use, DISP(true, ...) or DISP(false, ...)
#define DISP(exp, real)    \
::utest::dispone(__LINE__, #exp, #real, (exp), (real))

#define EPSINON            (1.192092896e-07F)
#define DOUBLE_EQUAL(a,b)  (((a) - (b) > -EPSINON) && ((a) - (b) < EPSINON))

//::tools for array
template <class T, std::size_t N>
constexpr inline size_t array_length_of(T(&)[N]) {
	return N;
}

#endif