#ifndef GOOGLE_PROTOBUF_COMPILER_JAVABEAN_FIELD_H__
#define GOOGLE_PROTOBUF_COMPILER_JAVABEAN_FIELD_H__

#include <google/protobuf/descriptor.h>
#include <google/protobuf/io/printer.h>

namespace google {
namespace protobuf {
namespace compiler {
namespace javabean {

void GenerateStringField(const FieldDescriptor* file_descriptor, io::Printer* printer);
void GenerateRepeatedStringField(const FieldDescriptor* file_descriptor, io::Printer* printer);

void GeneratePrimitiveField(const FieldDescriptor* file_descriptor, io::Printer* printer);
void GenerateRepeatedPrimitiveField(const FieldDescriptor* file_descriptor, io::Printer* printer);

void GenerateMessageField(const FieldDescriptor* file_descriptor, io::Printer* printer);
void GenerateRepeatedMessageField(const FieldDescriptor* file_descriptor, io::Printer* printer);

void GenerateEnumField(const FieldDescriptor* file_descriptor, io::Printer* printer);
void GenerateRepeatedEnumField(const FieldDescriptor* file_descriptor, io::Printer* printer);

void GenerateMapField(const FieldDescriptor* file_descriptor, io::Printer* printer);

}  // namespace javabean
}  // namespace compiler
}  // namespace protobuf
}  // namespace google

#endif  // GOOGLE_PROTOBUF_COMPILER_JAVABEAN_FIELD_H__
