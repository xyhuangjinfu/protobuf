#include <google/protobuf/compiler/javabean/javabean_generator.h>


#include <google/protobuf/compiler/javabean/javabean_file.h>
#include <google/protobuf/compiler/javabean/javabean_helpers.h>

namespace google {
namespace protobuf {
namespace compiler {
namespace javabean {

JavaBeanGenerator::JavaBeanGenerator() {}
JavaBeanGenerator::~JavaBeanGenerator() {}

uint64_t JavaBeanGenerator::GetSupportedFeatures() const {
	return CodeGenerator::Feature::FEATURE_PROTO3_OPTIONAL;
}

bool JavaBeanGenerator::Generate(const FileDescriptor* file_descriptor,
																 const std::string& parameter,
																 GeneratorContext* context,
																 std::string* error) const {
	// -----------------------------------------------------------------
	// parse generator options

	std::vector<std::pair<std::string, std::string> > options;
	ParseGeneratorParameter(parameter, &options);
	Options file_options;

	for (int i = 0; i < options.size(); i++) {
		if (options[i].first == "output_list_file") {
			file_options.output_list_file = options[i].second;
		} else if (options[i].first == "immutable") {
			file_options.generate_immutable_code = true;
		} else if (options[i].first == "mutable") {
			file_options.generate_mutable_code = true;
		} else if (options[i].first == "shared") {
			file_options.generate_shared_code = true;
		} else if (options[i].first == "lite") {
			// Note: Java Lite does not guarantee API/ABI stability. We may choose to
			// break existing API in order to boost performance / reduce code size.
			file_options.enforce_lite = true;
		} else if (options[i].first == "annotate_code") {
			file_options.annotate_code = true;
		} else if (options[i].first == "annotation_list_file") {
			file_options.annotation_list_file = options[i].second;
		} else {
			*error = "Unknown generator option: " + options[i].first;
			return false;
		}
	}

	if (file_options.enforce_lite && file_options.generate_mutable_code) {
		*error = "lite runtime generator option cannot be used with mutable API.";
		return false;
	}

	// By default we generate immutable code and shared code for immutable API.
	if (!file_options.generate_immutable_code &&
			!file_options.generate_mutable_code &&
			!file_options.generate_shared_code) {
		file_options.generate_immutable_code = true;
		file_options.generate_shared_code = true;
	}

	// -----------------------------------------------------------------

	if (!ValidateFile(file_descriptor, file_options, error)) {
		return false;
	}

	std::string package_dir = JavaPackageToDir(GetPackageName(file_descriptor));
	std::string java_filename = package_dir;
	java_filename += GetClassName(file_descriptor);
	java_filename += ".java";

	std::unique_ptr<io::ZeroCopyOutputStream> output(context->Open(java_filename));
	GeneratedCodeInfo annotations;
	io::AnnotationProtoCollector<GeneratedCodeInfo> annotation_collector(&annotations);
	io::Printer printer(
		output.get(), '$',
		file_options.annotate_code ? &annotation_collector : NULL);

	GenerateJavaBeanFile(file_descriptor, &printer);

	return true;
}

}  // namespace javabean
}  // namespace compiler
}  // namespace protobuf
}  // namespace google
