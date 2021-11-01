#ifndef GOOGLE_PROTOBUF_COMPILER_CPP2JAVA_HELPERS_H__
#define GOOGLE_PROTOBUF_COMPILER_CPP2JAVA_HELPERS_H__


#include <string>

#include <google/protobuf/descriptor.h>

namespace google {
namespace protobuf {
namespace compiler {
namespace cpp2java {

std::string GetFileName(const FileDescriptor* file_descriptor, std::string suffix);
const std::string GetJavaBinaryClassName(const FileDescriptor* file_descriptor, const Descriptor* descriptor);
const std::string GetJavaBinaryClassName(const FileDescriptor* file_descriptor, const EnumDescriptor* enum_descriptor);
const std::string GetJavaClassName(const FileDescriptor* file_descriptor, const Descriptor* descriptor);
const std::string GetJavaClassName(const FileDescriptor* file_descriptor, const EnumDescriptor* enum_descriptor);
const std::string GetHeaderFileProguardName(const FileDescriptor* file_descriptor);

}  // namespace cpp2java
}  // namespace compiler
}  // namespace protobuf
}  // namespace google

#endif  // GOOGLE_PROTOBUF_COMPILER_CPP2JAVA_HELPERS_H__
