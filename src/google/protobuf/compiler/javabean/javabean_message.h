#ifndef GOOGLE_PROTOBUF_COMPILER_JAVABEAN_MESSAGE_H__
#define GOOGLE_PROTOBUF_COMPILER_JAVABEAN_MESSAGE_H__

#include <map>
#include <string>
#include <google/protobuf/compiler/javabean/javabean_field.h>


namespace google {
namespace protobuf {
namespace compiler {
namespace javabean {

void GenerateMessageClass(const Descriptor* descriptor, io::Printer* printer);

}  // namespace javabean
}  // namespace compiler
}  // namespace protobuf
}  // namespace google

#endif  // GOOGLE_PROTOBUF_COMPILER_JAVABEAN_MESSAGE_H__
