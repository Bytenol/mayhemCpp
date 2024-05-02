#ifndef ___BYTENOL__OPENGL_COOKLIB_MAYHEM__EXCEPTION__HPP__
#define ___BYTENOL__OPENGL_COOKLIB_MAYHEM__EXCEPTION__HPP__

#include <stdexcept>
#include <string>


namespace mhy {

    class NotImplementedException: public std::logic_error {
        public:
            NotImplementedException(): std::logic_error("Function not yet implemented") {}
    };


    class GlFailureException: public std::logic_error {
        public:
            GlFailureException(const std::string& msg): std::logic_error(msg) { }
    };


    class ShaderFailureException: public std::logic_error {
        public:
            ShaderFailureException(const std::string& msg): std::logic_error(msg) { }
    };

}


#endif 