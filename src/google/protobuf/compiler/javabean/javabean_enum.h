#ifndef GOOGLE_PROTOBUF_COMPILER_JAVABEAN_ENUM_H__
#define GOOGLE_PROTOBUF_COMPILER_JAVABEAN_ENUM_H__

#include <map>
#include <string>

#include <google/protobuf/descriptor.h>
#include <google/protobuf/io/printer.h>


namespace google {
namespace protobuf {
namespace compiler {
namespace javabean {

void GenerateEnumClass(const EnumDescriptor* descriptor, io::Printer* printer);

}  // namespace javabean
}  // namespace compiler
}  // namespace protobuf
}  // namespace google

#endif  // GOOGLE_PROTOBUF_COMPILER_JAVABEAN_ENUM_H__
