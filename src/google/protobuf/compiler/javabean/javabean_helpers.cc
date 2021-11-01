#include <google/protobuf/compiler/javabean/javabean_helpers.h>

#include <string>

#include <google/protobuf/stubs/strutil.h>

#include <google/protobuf/compiler/java/java_name_resolver.h>
#include <google/protobuf/compiler/java/java_helpers.h>
#include <google/protobuf/compiler/java/java_file.h>

namespace google {
namespace protobuf {
namespace compiler {
namespace javabean {

const std::string GetPrimitiveTypeName(const FieldDescriptor* field_descriptor) {
	const char* java_helper_output = java::PrimitiveTypeName(java::GetJavaType(field_descriptor));
	std::string str = std::string(java_helper_output);
	if (str == "com.google.protobuf.ByteString") {
		return "byte[]";
	}
	return java_helper_output;
}

const std::string GetBoxedPrimitiveTypeName(const FieldDescriptor* field_descriptor) {
	const char* java_helper_output = java::BoxedPrimitiveTypeName(java::GetJavaType(field_descriptor));
	std::string str = std::string(java_helper_output);
	if (str == "com.google.protobuf.ByteString") {
		return "byte[]";
	}
	return java_helper_output;
}

const std::string GetPackageName(const FileDescriptor* file_descriptor) {
	return java::FileJavaPackage(file_descriptor, true);
}

const std::string GetClassName(const FileDescriptor* file_descriptor) {
	java::ClassNameResolver class_name_resolver;
	return class_name_resolver.GetFileClassName(file_descriptor, true);
}

const std::string GetClassName(const Descriptor* descriptor) {
	java::ClassNameResolver class_name_resolver;
	return class_name_resolver.GetClassName(descriptor, true);
}

const std::string GetClassName(const EnumDescriptor* enum_descriptor) {
	java::ClassNameResolver class_name_resolver;
	return class_name_resolver.GetClassName(enum_descriptor, true);
}

bool ValidateFile(const FileDescriptor* file_descriptor, const java::Options& options, std::string* error) {
	java::FileGenerator file_generator(file_descriptor, options);
	return file_generator.Validate(error);
}

std::string JavaPackageToDir(std::string package_name) {
	std::string package_dir = StringReplace(package_name, ".", "/", true);
	if (!package_dir.empty()) package_dir += "/";
	return package_dir;
}

JavaType GetJavaType(const FieldDescriptor* field) {
  switch (field->type()) {
  case FieldDescriptor::TYPE_INT32:
  case FieldDescriptor::TYPE_UINT32:
  case FieldDescriptor::TYPE_SINT32:
  case FieldDescriptor::TYPE_FIXED32:
  case FieldDescriptor::TYPE_SFIXED32:
    return JavaType::JAVATYPE_INT;

  case FieldDescriptor::TYPE_INT64:
  case FieldDescriptor::TYPE_UINT64:
  case FieldDescriptor::TYPE_SINT64:
  case FieldDescriptor::TYPE_FIXED64:
  case FieldDescriptor::TYPE_SFIXED64:
    return JavaType::JAVATYPE_LONG;

  case FieldDescriptor::TYPE_FLOAT:
    return JavaType::JAVATYPE_FLOAT;

  case FieldDescriptor::TYPE_DOUBLE:
    return JavaType::JAVATYPE_DOUBLE;

  case FieldDescriptor::TYPE_BOOL:
    return JavaType::JAVATYPE_BOOLEAN;

  case FieldDescriptor::TYPE_STRING:
    return JavaType::JAVATYPE_STRING;

  case FieldDescriptor::TYPE_BYTES:
    return JavaType::JAVATYPE_BYTES;

  case FieldDescriptor::TYPE_ENUM:
    return JavaType::JAVATYPE_ENUM;

  case FieldDescriptor::TYPE_GROUP:
  case FieldDescriptor::TYPE_MESSAGE:
    return JavaType::JAVATYPE_MESSAGE;

    // No default because we want the compiler to complain if any new
    // types are added.
  }

  GOOGLE_LOG(FATAL) << "Can't get here.";
  return JavaType::JAVATYPE_INT;
}

}  // namespace javabean
}  // namespace compiler
}  // namespace protobuf
}  // namespace google
