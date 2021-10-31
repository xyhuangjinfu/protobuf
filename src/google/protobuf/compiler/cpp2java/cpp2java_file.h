#ifndef GOOGLE_PROTOBUF_COMPILER_CPP2JAVA_FILE_H__
#define GOOGLE_PROTOBUF_COMPILER_CPP2JAVA_FILE_H__

#include <google/protobuf/io/printer.h>
#include <google/protobuf/descriptor.h>

namespace google {
namespace protobuf {
namespace compiler {
namespace cpp2java {

class FileGenerator {
public:
	FileGenerator();
	~FileGenerator();

	void GenerateFile(const FileDescriptor* file_descriptor, io::Printer& printer);
	void GenerateMessage(const FileDescriptor* file_descriptor, const Descriptor* descriptor, io::Printer& printer);
	void GenerateField(const FileDescriptor* file_descriptor, const FieldDescriptor* field_descriptor, io::Printer& printer);

private:
	const std::string GetBinarayClassName(const FileDescriptor* file_descriptor, const Descriptor* descriptor);

	GOOGLE_DISALLOW_EVIL_CONSTRUCTORS(FileGenerator);
};

}  // namespace cpp2java
}  // namespace compiler
}  // namespace protobuf
}  // namespace google

#endif  // GOOGLE_PROTOBUF_COMPILER_CPP2JAVA_FILE_H__
