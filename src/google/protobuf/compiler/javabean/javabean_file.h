#ifndef GOOGLE_PROTOBUF_COMPILER_JAVABEAN_FILE_H__
#define GOOGLE_PROTOBUF_COMPILER_JAVABEAN_FILE_H__


#include <google/protobuf/descriptor.h>
#include <google/protobuf/io/printer.h>

namespace google {
namespace protobuf {
namespace compiler {
namespace javabean {

void GenerateJavaBeanFile(const FileDescriptor* file_descriptor, io::Printer* printer);
void GenerateMessageClass(const Descriptor* descriptor, io::Printer* printer);
void GenerateEnumClass(const EnumDescriptor* descriptor, io::Printer* printer);

}  // namespace javabean
}  // namespace compiler
}  // namespace protobuf
}  // namespace google

#endif  // GOOGLE_PROTOBUF_COMPILER_JAVABEAN_FILE_H__
