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


	bool GlobalFileGenerated() { return global_header_file_generated_ && global_cpp_file_generated_; };
	void GenerateGlobalHeaderFile(const FileDescriptor* file_descriptor, io::Printer& printer);
	void GenerateGlobalCppFile(const FileDescriptor* file_descriptor, io::Printer& printer);

	void GenerateHeaderFile(const FileDescriptor* file_descriptor, io::Printer& printer);
	void GenerateCppFile(const FileDescriptor* file_descriptor, io::Printer& printer);
	void GenerateMessageTransfer(const FileDescriptor* file_descriptor, const Descriptor* descriptor, io::Printer& printer);
	void GenerateMessageTransferDeclare(const FileDescriptor* file_descriptor, const Descriptor* descriptor, io::Printer& printer);
	void GenerateFieldTransfer(const FileDescriptor* file_descriptor, const FieldDescriptor* field_descriptor, io::Printer& printer);

private:
	bool global_header_file_generated_ = false;
	bool global_cpp_file_generated_ = false;

	GOOGLE_DISALLOW_EVIL_CONSTRUCTORS(FileGenerator);
};

}  // namespace cpp2java
}  // namespace compiler
}  // namespace protobuf
}  // namespace google

#endif  // GOOGLE_PROTOBUF_COMPILER_CPP2JAVA_FILE_H__
