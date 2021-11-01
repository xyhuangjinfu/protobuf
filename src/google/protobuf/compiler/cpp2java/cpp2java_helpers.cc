#include <google/protobuf/compiler/cpp2java/cpp2java_helpers.h>


#include <regex>
#include <algorithm>

#include <google/protobuf/compiler/java/java_helpers.h>
#include <google/protobuf/compiler/java/java_name_resolver.h>

namespace google {
namespace protobuf {
namespace compiler {
namespace cpp2java {

std::string GetFileName(const FileDescriptor* file_descriptor, std::string suffix) {
	std::regex pattern_proto_file_suffix("\\.proto");
	std::string str = std::regex_replace(file_descriptor->name(), pattern_proto_file_suffix, "") + suffix;
	return str;
}

const std::string GetJavaBinaryClassName(const FileDescriptor* file_descriptor, const Descriptor* descriptor) {
	java::ClassNameResolver java_name_resolver;
	std::regex pattern_dot("\\.");
	const std::string outer_class_name = std::regex_replace(java_name_resolver.GetClassName(file_descriptor, true), pattern_dot, "/");

	std::string full_name = descriptor->full_name();
	if (!file_descriptor->package().empty()) {
		full_name = full_name.substr(file_descriptor->package().size() + 1);
	}

	const std::string class_name = std::regex_replace(full_name, pattern_dot, "$");
	return outer_class_name + "$" + class_name;
}

const std::string GetJavaBinaryClassName(const FileDescriptor* file_descriptor, const EnumDescriptor* enum_descriptor) {
	java::ClassNameResolver java_name_resolver;
	std::regex pattern_dot("\\.");
	const std::string outer_class_name = std::regex_replace(java_name_resolver.GetClassName(file_descriptor, true), pattern_dot, "/");

	std::string full_name = enum_descriptor->full_name();
	if (!file_descriptor->package().empty()) {
		full_name = full_name.substr(file_descriptor->package().size() + 1);
	}

	const std::string class_name = std::regex_replace(full_name, pattern_dot, "$");
	return outer_class_name + "$" + class_name;
}

const std::string GetJavaClassName(const FileDescriptor* file_descriptor, const Descriptor* descriptor) {
	java::ClassNameResolver java_name_resolver;
	std::regex pattern_dot("\\.");
	const std::string outer_class_name = java_name_resolver.GetClassName(file_descriptor, true);

	std::string full_name = descriptor->full_name();
	if (!file_descriptor->package().empty()) {
		full_name = full_name.substr(file_descriptor->package().size() + 1);
	}

	const std::string class_name = std::regex_replace(full_name, pattern_dot, "$");
	return outer_class_name + "$" + class_name;
}

const std::string GetJavaClassName(const FileDescriptor* file_descriptor, const EnumDescriptor* enum_descriptor) {
	java::ClassNameResolver java_name_resolver;
	std::regex pattern_dot("\\.");
	const std::string outer_class_name = java_name_resolver.GetClassName(file_descriptor, true);

	std::string full_name = enum_descriptor->full_name();
	if (!file_descriptor->package().empty()) {
		full_name = full_name.substr(file_descriptor->package().size() + 1);
	}

	const std::string class_name = std::regex_replace(full_name, pattern_dot, "$");
	return outer_class_name + "$" + class_name;
}

const std::string GetHeaderFileProguardName(const FileDescriptor* file_descriptor) {
	std::string file_name = file_descriptor->name();
	std::transform(file_name.begin(), file_name.end(), file_name.begin(), ::toupper);
	std::regex pattern_dot("\\.");
	const std::string header_proguard_name = std::regex_replace(file_name, pattern_dot, "_");
	return header_proguard_name + "__";
}

}  // namespace cpp2java
}  // namespace compiler
}  // namespace protobuf
}  // namespace google
