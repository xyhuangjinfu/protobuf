#ifndef GOOGLE_PROTOBUF_COMPILER_JAVABEAN_GENERATOR_H__
#define GOOGLE_PROTOBUF_COMPILER_JAVABEAN_GENERATOR_H__

#include <string>
#include <google/protobuf/compiler/code_generator.h>

#include <google/protobuf/port_def.inc>

namespace google {
namespace protobuf {
namespace compiler {
namespace javabean {

// CodeGenerator implementation which generates Java Bean code.  If you create your
// own protocol compiler binary and you want it to support Java Bean output, you
// can do so by registering an instance of this CodeGenerator with the
// CommandLineInterface in your main() function.
class PROTOC_EXPORT JavaBeanGenerator : public CodeGenerator {
public:
	JavaBeanGenerator();
	~JavaBeanGenerator();

	bool Generate(const FileDescriptor* file,
								const std::string& parameter,
								GeneratorContext* context,
								std::string* error) const override;

	uint64_t GetSupportedFeatures() const override;

private:
	GOOGLE_DISALLOW_EVIL_CONSTRUCTORS(JavaBeanGenerator);
};

}  // namespace javabean
}  // namespace compiler
}  // namespace protobuf
}  // namespace google

#include <google/protobuf/port_undef.inc>

#endif  // GOOGLE_PROTOBUF_COMPILER_JAVABEAN_GENERATOR_H__
