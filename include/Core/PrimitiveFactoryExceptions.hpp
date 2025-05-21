/*
**
**
**
**
*/

#pragma once
#include <stdexcept>
#include <string>

namespace Core {

    class PrimitiveFactoryException : public std::runtime_error {
        public:
            explicit PrimitiveFactoryException(const std::string& msg)
            : std::runtime_error("PrimitiveFactory error: " + msg) {}
    };

    
    class PluginLoadException : public PrimitiveFactoryException {
        public:
            explicit PluginLoadException(const std::string& pluginPath,
                                        const std::string& reason)
            : PrimitiveFactoryException(
                "plugin \"" + pluginPath + "\" load failed: " + reason) {}
    };

    class UnknownPrimitiveTypeException : public PrimitiveFactoryException {
        public:
            explicit UnknownPrimitiveTypeException(const std::string& type)
            : PrimitiveFactoryException("unknown primitive type \"" + type + "\"") {}
    };

}


