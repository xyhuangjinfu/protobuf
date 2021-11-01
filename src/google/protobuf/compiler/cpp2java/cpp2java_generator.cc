#include <google/protobuf/compiler/cpp2java/cpp2java_generator.h>


#include <google/protobuf/io/printer.h>
#include <google/protobuf/io/zero_copy_stream.h>
#include <google/protobuf/descriptor.pb.h>

#include <google/protobuf/compiler/cpp2java/cpp2java_file.h>
#include <google/protobuf/compiler/cpp2java/cpp2java_helpers.h>

namespace google {
namespace protobuf {
namespace compiler {
namespace cpp2java {


Cpp2JavaGenerator::Cpp2JavaGenerator() {
	file_generator_ = new FileGenerator();
}
Cpp2JavaGenerator::~Cpp2JavaGenerator() {
	delete file_generator_;
}

uint64_t Cpp2JavaGenerator::GetSupportedFeatures() const {
	return CodeGenerator::Feature::FEATURE_PROTO3_OPTIONAL;
}


bool Cpp2JavaGenerator::Generate(const FileDescriptor* file_descriptor,
														 const std::string& parameter,
														 GeneratorContext* generator_context,
														 std::string* error) const {
	const FileOptions& file_options = file_descriptor->options();
	if (!file_options.has_java_package()) {
		*error = "option java_package not exist!";
		return false;
	}

	// ----------------------------------------------------------------------


	std::unique_ptr<io::ZeroCopyOutputStream> header_output(generator_context->Open(GetFileName(file_descriptor, ".cpp2java.h")));
	io::Printer header_printer(header_output.get(), '$');
	file_generator_->GenerateHeaderFile(file_descriptor, header_printer);

	std::unique_ptr<io::ZeroCopyOutputStream> cpp_output(generator_context->Open(GetFileName(file_descriptor, ".cpp2java.cc")));
	io::Printer cpp_printer(cpp_output.get(), '$');
	file_generator_->GenerateCppFile(file_descriptor, cpp_printer);

	if (!file_generator_->GlobalFileGenerated()) {
		std::unique_ptr<io::ZeroCopyOutputStream> global_header_output(generator_context->Open("cpp2java.h"));
		io::Printer global_header_printer(global_header_output.get(), '$');
		file_generator_->GenerateGlobalHeaderFile(file_descriptor, global_header_printer);

		std::unique_ptr<io::ZeroCopyOutputStream> global_cpp_output(generator_context->Open("cpp2java.cc"));
		io::Printer global_cpp_printer(global_cpp_output.get(), '$');
		file_generator_->GenerateGlobalCppFile(file_descriptor, global_cpp_printer);
	}

	return true;
}

}  // namespace cpp2java
}  // namespace compiler
}  // namespace protobuf
}  // namespace google
