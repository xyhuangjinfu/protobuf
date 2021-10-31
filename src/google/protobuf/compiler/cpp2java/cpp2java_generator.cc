#include <google/protobuf/compiler/cpp2java/cpp2java_generator.h>


#include <google/protobuf/io/printer.h>
#include <google/protobuf/io/zero_copy_stream.h>

#include <google/protobuf/compiler/cpp2java/cpp2java_file.h>

namespace google {
namespace protobuf {
namespace compiler {
namespace cpp2java {


Cpp2JavaGenerator::Cpp2JavaGenerator() {}
Cpp2JavaGenerator::~Cpp2JavaGenerator() {}

uint64_t Cpp2JavaGenerator::GetSupportedFeatures() const {
	return CodeGenerator::Feature::FEATURE_PROTO3_OPTIONAL;
}


bool Cpp2JavaGenerator::Generate(const FileDescriptor* file_descriptor,
														 const std::string& parameter,
														 GeneratorContext* generator_context,
														 std::string* error) const {

	std::unique_ptr<io::ZeroCopyOutputStream> output(generator_context->Open(file_descriptor->name() + ".cpp2java.cc"));
	io::Printer printer(output.get(), '$');


	FileGenerator file_generator;
	file_generator.GenerateFile(file_descriptor, printer);

	return true;
}

}  // namespace cpp2java
}  // namespace compiler
}  // namespace protobuf
}  // namespace google
