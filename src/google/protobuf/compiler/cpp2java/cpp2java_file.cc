#include <google/protobuf/compiler/cpp2java/cpp2java_file.h>


#include <regex>

#include <google/protobuf/compiler/java/java_helpers.h>
#include <google/protobuf/compiler/java/java_name_resolver.h>


namespace google {
namespace protobuf {
namespace compiler {
namespace cpp2java {

FileGenerator::FileGenerator() {}
FileGenerator::~FileGenerator() {}

void FileGenerator::GenerateFile(const FileDescriptor* file_descriptor, io::Printer& printer) {
	std::map<std::string, std::string> variables;
	variables["cpp_header"] = file_descriptor->name() + ".pb.h";

	printer.Print(variables, "#include <$cpp_header$>\n");
	printer.Print(variables, "#include \"jni.h\"\n\n");

	for (int i = 0; i < file_descriptor->message_type_count(); i++) {
		const Descriptor* descriptor = file_descriptor->message_type(i);
		GenerateMessage(file_descriptor, descriptor, printer);
	}
}

void FileGenerator::GenerateMessage(const FileDescriptor* file_descriptor, const Descriptor* descriptor, io::Printer& printer) {
	std::map<std::string, std::string> variables;
	variables["cpp_class_name"] = "STUB_cpp_class_name";
	variables["java_binaray_class_name"] = "STUB_java/lang/Object";

	printer.Print(variables, "jobject cpp2java(JNIEnv* env, $cpp_class_name$* cpp_obj) {\n");
	printer.Indent();
	printer.Print(variables, "jclass cls = env->FindClass(env, $java_binaray_class_name$);\n");
	printer.Print(variables, "jmethodID constructor = env->GetMethodID(env, cls, \"<init>\", \"()V\");\n");
	printer.Print(variables, "jobject obj = env->NewObject(env, cls, constructor);\n");

	printer.Print(variables, "\n");
	for (int i = 0; i < descriptor->field_count(); i++) {
		const FieldDescriptor* field_descriptor = descriptor->field(i);
		GenerateField(file_descriptor, field_descriptor, printer);
	}
	printer.Print(variables, "\n");

	printer.Print(variables, "return obj;\n");
	printer.Outdent();
	printer.Print(variables, "}\n\n");
}

void FileGenerator::GenerateField(const FileDescriptor* file_descriptor, const FieldDescriptor* field_descriptor, io::Printer& printer) {
	std::map<std::string, std::string> variables;
	variables["field_name"] = field_descriptor->name();
	variables["capital_field_name"] = java::UnderscoresToCapitalizedCamelCase(field_descriptor);

	FieldDescriptor::Type type = field_descriptor->type();
	if (type == FieldDescriptor::Type::TYPE_INT32 ||
			type == FieldDescriptor::Type::TYPE_UINT32 ||
			type == FieldDescriptor::Type::TYPE_SINT32 ||
			type == FieldDescriptor::Type::TYPE_FIXED32 ||
			type == FieldDescriptor::Type::TYPE_SFIXED32) {
		printer.Print(variables, "jint $field_name$_cpp = cpp_obj->$field_name$();\n");
		printer.Print(variables, "jfieldID $field_name$_field = env->GetFieldID(cls, m$capital_field_name$), \"I\";\n");
		printer.Print(variables, "SetIntField(env, obj, $field_name$_field, $field_name$_cpp);\n\n");
	} else if (type == FieldDescriptor::Type::TYPE_INT64 ||
			type == FieldDescriptor::Type::TYPE_UINT64 ||
			type == FieldDescriptor::Type::TYPE_SINT64 ||
			type == FieldDescriptor::Type::TYPE_FIXED64 ||
			type == FieldDescriptor::Type::TYPE_SFIXED64) {
		printer.Print(variables, "jlong $field_name$_cpp = cpp_obj->$field_name$();\n");
		printer.Print(variables, "jfieldID $field_name$_field = env->GetFieldID(cls, m$capital_field_name$), \"J\";\n");
		printer.Print(variables, "SetLongField(env, obj, $field_name$_field, $field_name$_cpp);\n\n");
	} else if (type == FieldDescriptor::Type::TYPE_FLOAT) {
		printer.Print(variables, "jfloat $field_name$_cpp = cpp_obj->$field_name$();\n");
		printer.Print(variables, "jfieldID $field_name$_field = env->GetFieldID(cls, m$capital_field_name$), \"F\";\n");
		printer.Print(variables, "SetFloatField(env, obj, $field_name$_field, $field_name$_cpp);\n\n");
	} else if (type == FieldDescriptor::Type::TYPE_DOUBLE) {
		printer.Print(variables, "jdouble $field_name$_cpp = cpp_obj->$field_name$();\n");
		printer.Print(variables, "jfieldID $field_name$_field = env->GetFieldID(cls, m$capital_field_name$), \"D\";\n");
		printer.Print(variables, "SetDoubleField(env, obj, $field_name$_field, $field_name$_cpp);\n\n");
	} else if (type == FieldDescriptor::Type::TYPE_BOOL) {
		printer.Print(variables, "jboolean $field_name$_cpp = cpp_obj->$field_name$();\n");
		printer.Print(variables, "jfieldID $field_name$_field = env->GetFieldID(cls, m$capital_field_name$), \"Z\";\n");
		printer.Print(variables, "SetBooleanField(env, obj, $field_name$_field, $field_name$_cpp);\n\n");
	} else if (type == FieldDescriptor::Type::TYPE_STRING) {
		printer.Print(variables, "const jchar* $field_name$_cpp = cpp_obj->$field_name$().c_str();\n");
		printer.Print(variables, "jfieldID $field_name$_field = env->GetFieldID(cls, m$capital_field_name$), \"java/lang/String\";\n");
		printer.Print(variables, "jstring $field_name$_str = env->NewStringUTF(env, $field_name$_cpp)\n");
		printer.Print(variables, "SetObjectField(env, obj, $field_name$_field, $field_name$_str);\n\n");
	} else if (type == FieldDescriptor::Type::TYPE_ENUM) {
		const EnumDescriptor* enum_type = field_descriptor->enum_type();
		variables["java_binaray_type_name"] = GetBinarayClassName(file_descriptor, enum_type);

		printer.Print(variables, "jint $field_name$_cpp = cpp2java(env, cpp_obj->$field_name$());\n");
		printer.Print(variables, "jfieldID $field_name$_field = env->GetFieldID(cls, m$capital_field_name$), \"$java_binaray_type_name$\";\n");
		printer.Print(variables, "SetObjectField(env, obj, $field_name$_field, $field_name$_cpp);\n\n");
	} else if (type == FieldDescriptor::Type::TYPE_GROUP ||
						 type == FieldDescriptor::Type::TYPE_MESSAGE) {
		const Descriptor* message_type = field_descriptor->message_type();
		variables["java_binaray_type_name"] = GetBinarayClassName(file_descriptor, message_type);

		printer.Print(variables, "jobject $field_name$_cpp = cpp2java(env, cpp_obj->$field_name$());\n");
		printer.Print(variables, "jfieldID $field_name$_field = env->GetFieldID(cls, m$capital_field_name$), \"$java_binaray_type_name$\";\n");
		printer.Print(variables, "SetObjectField(env, obj, $field_name$_field, $field_name$_cpp);\n\n");
	}

}

const std::string FileGenerator::GetBinarayClassName(const FileDescriptor* file_descriptor, const Descriptor* descriptor) {
	java::ClassNameResolver java_name_resolver;
	std::regex pattern_dot("\\.");
	const std::string outer_class_name = std::regex_replace(java_name_resolver.GetClassName(file_descriptor, true), pattern_dot, "/");
	const std::string class_name = std::regex_replace(descriptor->full_name(), pattern_dot, "$");
	return outer_class_name + "$" + class_name;
}

const std::string FileGenerator::GetBinarayClassName(const FileDescriptor* file_descriptor, const EnumDescriptor* enum_descriptor) {
	java::ClassNameResolver java_name_resolver;
	std::regex pattern_dot("\\.");
	const std::string outer_class_name = std::regex_replace(java_name_resolver.GetClassName(file_descriptor, true), pattern_dot, "/");
	const std::string class_name = std::regex_replace(enum_descriptor->full_name(), pattern_dot, "$");
	return outer_class_name + "$" + class_name;
}

}  // namespace cpp2java
}  // namespace compiler
}  // namespace protobuf
}  // namespace google
