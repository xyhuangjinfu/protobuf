#ifndef GOOGLE_PROTOBUF_COMPILER_JAVABEAN_HELPERS_H__
#define GOOGLE_PROTOBUF_COMPILER_JAVABEAN_HELPERS_H__

#include <string>

#include <google/protobuf/descriptor.h>
#include <google/protobuf/compiler/java/java_options.h>
#include <google/protobuf/compiler/java/java_helpers.h>

namespace google {
namespace protobuf {
namespace compiler {
namespace javabean {

typedef java::JavaType JavaType;
typedef java::Options Options;

const std::string GetPrimitiveTypeName(const FieldDescriptor* field_descriptor);
const std::string GetBoxedPrimitiveTypeName(const FieldDescriptor* field_descriptor);

const std::string GetPackageName(const FileDescriptor* file_descriptor);
const std::string GetClassName(const FileDescriptor* file_descriptor);
const std::string GetClassName(const Descriptor* descriptor);
const std::string GetClassName(const EnumDescriptor* enum_descriptor);

bool ValidateFile(const FileDescriptor* file_descriptor, const java::Options& options, std::string* error);

std::string JavaPackageToDir(std::string package_name);

JavaType GetJavaType(const FieldDescriptor* file_descriptor);

}  // namespace javabean
}  // namespace compiler
}  // namespace protobuf
}  // namespace google

#endif  // GOOGLE_PROTOBUF_COMPILER_JAVABEAN_HELPERS_H__
