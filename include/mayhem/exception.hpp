#ifndef ___BYTENOL__EXCEPTION_MAYHEM__CPP__HPP__
#define ___BYTENOL__EXCEPTION_MAYHEM__CPP__HPP__

#include <stdexcept>
#include <string>


namespace mhy {

    class _MayhemCppExceptionBase: public std::logic_error {
        public:
            _MayhemCppExceptionBase(const std::string& msg): 
                std::logic_error(msg + " in FILE " + __FILE__ + 
                " at line " + std::to_string(__LINE__)) {}
    };

    class NotImplementedException: public _MayhemCppExceptionBase {
        public:
            NotImplementedException(): _MayhemCppExceptionBase("Function not yet implemented") {}
    };


    class GlFailureException: public _MayhemCppExceptionBase {
        public:
            GlFailureException(const std::string& msg): _MayhemCppExceptionBase(msg) { }
    };


    class ShaderFailureException: public _MayhemCppExceptionBase {
        public:
            ShaderFailureException(const std::string& msg): _MayhemCppExceptionBase(msg) { }
    };


    class InvalidBufferException: public _MayhemCppExceptionBase {
        public:
            InvalidBufferException(const std::string& msg): _MayhemCppExceptionBase(msg) { }
    };

}


#endif 