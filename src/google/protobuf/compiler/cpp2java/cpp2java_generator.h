#ifndef GOOGLE_PROTOBUF_COMPILER_CPP2JAVA_GENERATOR_H__
#define GOOGLE_PROTOBUF_COMPILER_CPP2JAVA_GENERATOR_H__

#include <string>
#include <google/protobuf/compiler/code_generator.h>

#include <google/protobuf/descriptor.h>
#include <google/protobuf/port_def.inc>

namespace google {
namespace protobuf {
namespace compiler {
namespace cpp2java {

// CodeGenerator implementation which generates Java code.  If you create your
// own protocol compiler binary and you want it to support Java output, you
// can do so by registering an instance of this CodeGenerator with the
// CommandLineInterface in your main() function.
class PROTOC_EXPORT Cpp2JavaGenerator : public CodeGenerator {
public:
	Cpp2JavaGenerator();
	~Cpp2JavaGenerator();

	// implements CodeGenerator ----------------------------------------
	bool Generate(const FileDescriptor* file_descriptor,
								const std::string& parameter,
								GeneratorContext* generator_context,
								std::string* error) const override;

	uint64_t GetSupportedFeatures() const override;

private:
	GOOGLE_DISALLOW_EVIL_CONSTRUCTORS(Cpp2JavaGenerator);
};

}  // namespace cpp2java
}  // namespace compiler
}  // namespace protobuf
}  // namespace google

#include <google/protobuf/port_undef.inc>

#endif  // GOOGLE_PROTOBUF_COMPILER_CPP2JAVA_GENERATOR_H__
