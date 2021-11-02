#include <google/protobuf/compiler/cpp2java/cpp2java_file.h>


#include <regex>

#include <google/protobuf/compiler/java/java_helpers.h>
#include <google/protobuf/compiler/cpp/cpp_helpers.h>

#include <google/protobuf/compiler/cpp2java/cpp2java_helpers.h>


namespace google {
namespace protobuf {
namespace compiler {
namespace cpp2java {

FileGenerator::FileGenerator() {}
FileGenerator::~FileGenerator() {}

void FileGenerator::GenerateGlobalHeaderFile(const FileDescriptor* file_descriptor, io::Printer& printer){
	global_header_file_generated_ = true;

	std::map<std::string, std::string> variables;

	printer.Print(variables,
		"// Generated by the protocol buffer compiler.  DO NOT EDIT!\n\n\n");


	printer.Print(variables, "#ifndef CPP2JAVA_H__\n");
	printer.Print(variables, "#define CPP2JAVA_H__\n\n\n");

	printer.Print(variables, "#include <jni.h>\n\n");

	printer.Print(variables, "/* init env */\n");
	printer.Print(variables, "void init(JavaVM *jvm);\n\n");
	printer.Print(variables, "/* get class */\n");
	printer.Print(variables, "jclass GetClass(const char* java_class_name);\n\n");
	printer.Print(variables, "/* attach current native thread to jvm */\n");
	printer.Print(variables, "void AttachCurrentThread(JNIEnv* env);\n\n");

	printer.Print(variables, "#endif // CPP2JAVA_H__\n\n");
}

void FileGenerator::GenerateGlobalCppFile(const FileDescriptor* file_descriptor, io::Printer& printer) {
	global_cpp_file_generated_ = true;

	std::map<std::string, std::string> variables;
	variables["file_name"] = file_descriptor->name();

	printer.Print(variables,
		"// Generated by the protocol buffer compiler.  DO NOT EDIT!\n\n\n");


	printer.Print(variables, "#include \"cpp2java.h\"\n\n\n");
	printer.Print(variables, "#include <string>\n");
	printer.Print(variables, "#include <map>\n\n\n");
	printer.Print(variables, "std::map<std::string, jclass> class_map;\n");
	printer.Print(variables, "JavaVM *global_jvm;\n");
	printer.Print(variables, "JNIEnv *env;\n");
	printer.Print(variables, "jclass class_loader_class;\n");
	printer.Print(variables, "jmethodID class_loader_load_class;\n");
	printer.Print(variables, "jobject class_loader;\n\n");
	printer.Print(variables, "/* init env */\n");
	printer.Print(variables, "void init(JavaVM *jvm) {\n");
	printer.Indent();
	printer.Print(variables, "global_jvm = jvm;\n");
	printer.Print(variables, "jvm->GetEnv((void **) &env, JNI_VERSION_1_4);\n");
	printer.Print(variables, "class_loader_class = (jclass) env->NewGlobalRef(env->FindClass(\"java/lang/ClassLoader\"));\n");
	printer.Print(variables, "class_loader_load_class = env->GetMethodID(class_loader_class, \"loadClass\", \"(Ljava/lang/String;)Ljava/lang/Class;\");\n");
	if (file_descriptor->message_type_count() > 0) {
		const Descriptor* descriptor = file_descriptor->message_type(0);
		variables["java_binary_type_name"] = GetJavaBinaryClassName(file_descriptor, descriptor);
		printer.Print(variables, "jclass custom_clsss = (jclass) env->NewGlobalRef(env->FindClass(\"$java_binary_type_name$\"));\n");
	} else if (file_descriptor->enum_type_count() > 0) {
		const EnumDescriptor* enum_descriptor = file_descriptor->enum_type(0);
		variables["java_binary_type_name"] = GetJavaBinaryClassName(file_descriptor, enum_descriptor);
		printer.Print(variables, "jclass custom_clsss = (jclass) env->NewGlobalRef(env->FindClass(\"$java_binary_type_name$\"));\n");
	} else {
		//TODO error
	}
	printer.Print(variables, "jclass class_class = env->FindClass(\"java/lang/Class\");\n");
	printer.Print(variables, "jmethodID get_class_loader = env->GetMethodID(class_class, \"getClassLoader\", \"()Ljava/lang/ClassLoader;\");\n");
	printer.Print(variables, "class_loader = env->NewGlobalRef(env->CallObjectMethod(custom_clsss, get_class_loader));\n");
	printer.Outdent();
	printer.Print(variables, "}\n\n");

	printer.Print(variables, "/* get class */\n");
	printer.Print(variables, "jclass GetClass(const char* java_class_name) {\n");
	printer.Indent();
	printer.Print(variables, "if (class_map.count(java_class_name) > 0) {\n");
	printer.Indent();
	printer.Print(variables, "return class_map[java_class_name];\n");
	printer.Outdent();
	printer.Print(variables, "} else {\n");
	printer.Indent();
	printer.Print(variables, "jclass clazz = (jclass)env->CallObjectMethod(class_loader, class_loader_load_class, env->NewStringUTF(java_class_name));\n");
	printer.Print(variables, "class_map[java_class_name] = clazz;\n");
	printer.Print(variables, "return clazz;\n");
	printer.Outdent();
	printer.Print(variables, "}\n");
	printer.Outdent();
	printer.Print(variables, "}\n\n");

	printer.Print(variables, "/* attach current native thread to jvm */\n");
	printer.Print(variables, "void AttachCurrentThread(JNIEnv* env) {\n");
	printer.Indent();
	printer.Print(variables, "global_jvm->AttachCurrentThread(&env, NULL);\n");
	printer.Outdent();
	printer.Print(variables, "}\n\n");
}

void FileGenerator::GenerateHeaderFile(const FileDescriptor* file_descriptor, io::Printer& printer) {
	std::map<std::string, std::string> variables;
	variables["cpp_proto_header"] = GetFileName(file_descriptor, ".pb.h");
	variables["file_name"] = file_descriptor->name();;
	variables["header_proguard_name"] = GetHeaderFileProguardName(file_descriptor);

	printer.Print(variables,
								 "// Generated by the protocol buffer compiler.  DO NOT EDIT!\n"
								 "// source: $file_name$\n\n");

	printer.Print(variables, "#ifndef $header_proguard_name$\n");
	printer.Print(variables, "#define $header_proguard_name$\n\n\n");

	printer.Print(variables, "#include \"$cpp_proto_header$\"\n");
	printer.Print(variables, "#include <jni.h>\n\n");
	printer.Print(variables, "#include \"cpp2java.h\"\n\n");

	for (int i = 0; i < file_descriptor->message_type_count(); i++) {
		const Descriptor* descriptor = file_descriptor->message_type(i);
		GenerateMessageTransferDeclare(file_descriptor, descriptor, printer);
	}

	printer.Print(variables, "#endif // $header_proguard_name$\n\n");
}

void FileGenerator::GenerateCppFile(const FileDescriptor* file_descriptor, io::Printer& printer) {
	std::map<std::string, std::string> variables;
	variables["cpp2java_header"] = GetFileName(file_descriptor, ".cpp2java.h");
	variables["file_name"] = file_descriptor->name();

	printer.Print(variables,
								 "// Generated by the protocol buffer compiler.  DO NOT EDIT!\n"
								 "// source: $file_name$\n\n");

	printer.Print(variables, "#include \"$cpp2java_header$\"\n\n\n");

	for (int i = 0; i < file_descriptor->public_dependency_count(); i++) {
		const FileDescriptor* dependency_file_descriptor = file_descriptor->public_dependency(i);
		variables["dependency_file_name"] = GetFileName(dependency_file_descriptor, ".cpp2java.h");
		printer.Print(variables, "#include \"$dependency_file_name$\"\n");
	}
	for (int i = 0; i < file_descriptor->dependency_count(); i++) {
		const FileDescriptor* dependency_file_descriptor = file_descriptor->dependency(i);
		variables["dependency_file_name"] = GetFileName(dependency_file_descriptor, ".cpp2java.h");
		printer.Print(variables, "#include \"$dependency_file_name$\"\n\n");
	}

	for (int i = 0; i < file_descriptor->message_type_count(); i++) {
		const Descriptor* descriptor = file_descriptor->message_type(i);
		GenerateMessageTransfer(file_descriptor, descriptor, printer);
	}
}

void FileGenerator::GenerateMessageTransfer(const FileDescriptor* file_descriptor, const Descriptor* descriptor, io::Printer& printer) {
	std::map<std::string, std::string> variables;
	variables["cpp_class_namespace"] = file_descriptor->package();
	variables["cpp_class_name"] = cpp::ClassName(descriptor);
	variables["java_binary_type_name"] = GetJavaBinaryClassName(file_descriptor, descriptor);
	variables["java_class_name"] = GetJavaClassName(file_descriptor, descriptor);
	variables["debug_string"] = descriptor->DebugString();

	printer.Print(variables, "/* $debug_string$ */\n");
	printer.Print(variables, "jobject cpp2java(JNIEnv* env, const $cpp_class_namespace$::$cpp_class_name$& cpp_obj) {\n");
	printer.Indent();
	printer.Print(variables, "AttachCurrentThread(env);\n");
	printer.Print(variables, "jclass cls = GetClass(\"$java_class_name$\");\n");
	printer.Print(variables, "jmethodID constructor = env->GetMethodID(cls, \"<init>\", \"()V\");\n");
	printer.Print(variables, "jobject obj = env->NewObject(cls, constructor);\n");

	printer.Print(variables, "\n");
	for (int i = 0; i < descriptor->field_count(); i++) {
		const FieldDescriptor* field_descriptor = descriptor->field(i);
		GenerateFieldTransfer(file_descriptor, field_descriptor, printer);
	}
	printer.Print(variables, "\n");

	printer.Print(variables, "return obj;\n");
	printer.Outdent();
	printer.Print(variables, "}\n\n");

	// nest type
	for (int i = 0; i < descriptor->nested_type_count(); i++) {
		const Descriptor* nest_type_descriptor = descriptor->nested_type(i);
		GenerateMessageTransfer(file_descriptor, nest_type_descriptor, printer);
	}
}

void FileGenerator::GenerateMessageTransferDeclare(const FileDescriptor* file_descriptor, const Descriptor* descriptor, io::Printer& printer) {
	std::map<std::string, std::string> variables;
	variables["cpp_class_namespace"] = file_descriptor->package();
	variables["cpp_class_name"] = cpp::ClassName(descriptor);
	variables["java_binary_type_name"] = GetJavaBinaryClassName(file_descriptor, descriptor);
	variables["debug_string"] = descriptor->DebugString();

	printer.Print(variables, "/* $debug_string$ */\n");
	printer.Print(variables, "jobject cpp2java(JNIEnv* env, const $cpp_class_namespace$::$cpp_class_name$& cpp_obj);\n\n");

	// nest type
	for (int i = 0; i < descriptor->nested_type_count(); i++) {
		const Descriptor* nest_type_descriptor = descriptor->nested_type(i);
		GenerateMessageTransferDeclare(file_descriptor, nest_type_descriptor, printer);
	}
}

void FileGenerator::GenerateFieldTransfer(const FileDescriptor* file_descriptor, const FieldDescriptor* field_descriptor, io::Printer& printer) {
	std::map<std::string, std::string> variables;
	variables["field_name"] = field_descriptor->name();
	variables["java_field_name"] = "m" + java::UnderscoresToCapitalizedCamelCase(field_descriptor);
	variables["java_has_field_name"] = "mHas" + java::UnderscoresToCapitalizedCamelCase(field_descriptor);
	variables["cpp_field_name"] = cpp::FieldName(field_descriptor);
	variables["debug_string"] = field_descriptor->DebugString();

	FieldDescriptor::Type type = field_descriptor->type();
	if (type == FieldDescriptor::Type::TYPE_INT32 ||
			type == FieldDescriptor::Type::TYPE_UINT32 ||
			type == FieldDescriptor::Type::TYPE_SINT32 ||
			type == FieldDescriptor::Type::TYPE_FIXED32 ||
			type == FieldDescriptor::Type::TYPE_SFIXED32) {
		printer.Print(variables, "/* $debug_string$ */\n");
		if (field_descriptor->is_repeated()) {
			printer.Print(variables, "jint $field_name$_cpp_size = cpp_obj.$cpp_field_name$_size();\n");
			printer.Print(variables, "jclass $field_name$_list_class = GetClass(\"java.util.ArrayList\");\n");
			printer.Print(variables, "jmethodID $field_name$_list_constructor = env->GetMethodID($field_name$_list_class, \"<init>\", \"(I)V\");\n");
			printer.Print(variables, "jobject $field_name$_list = env->NewObject($field_name$_list_class, $field_name$_list_constructor, $field_name$_cpp_size);\n");
			printer.Print(variables, "jmethodID $field_name$_list_add = env->GetMethodID($field_name$_list_class, \"add\", \"(Ljava/lang/Object;)Z\");\n");
			printer.Print(variables, "jclass $field_name$_integer_class = GetClass(\"java.lang.Integer\");\n");
			printer.Print(variables, "jmethodID $field_name$_integer_constructor = env->GetMethodID($field_name$_integer_class, \"<init>\", \"(I)V\");\n");
			printer.Print(variables, "for (int i = 0; i < $field_name$_cpp_size; i++) {\n");
			printer.Indent();
			printer.Print(variables, "jobject $field_name$_integer_obj = env->NewObject($field_name$_integer_class, $field_name$_integer_constructor, cpp_obj.$cpp_field_name$(i));\n");
			printer.Print(variables, "env->CallBooleanMethod($field_name$_list, $field_name$_list_add, $field_name$_integer_obj);\n");
			printer.Outdent();
			printer.Print(variables, "}\n");
			printer.Print(variables, "jfieldID $field_name$_field = env->GetFieldID(cls, \"$java_field_name$\", \"Ljava/util/List;\");\n");
			printer.Print(variables, "env->SetObjectField(obj, $field_name$_field, $field_name$_list);\n\n");
		} else {
			if (cpp::HasHasbit(field_descriptor)) {
				printer.Print(variables, "jboolean has_$field_name$_cpp = cpp_obj.has_$cpp_field_name$();\n");
				printer.Print(variables, "jfieldID has_$field_name$_field = env->GetFieldID(cls, \"$java_has_field_name$\", \"Z\");\n");
				printer.Print(variables, "env->SetBooleanField(obj, has_$field_name$_field, has_$field_name$_cpp);\n\n");
			}
			printer.Print(variables, "jint $field_name$_cpp = cpp_obj.$cpp_field_name$();\n");
			printer.Print(variables, "jfieldID $field_name$_field = env->GetFieldID(cls, \"$java_field_name$\", \"I\");\n");
			printer.Print(variables, "env->SetIntField(obj, $field_name$_field, $field_name$_cpp);\n\n");
		}
	} else if (type == FieldDescriptor::Type::TYPE_INT64 ||
			type == FieldDescriptor::Type::TYPE_UINT64 ||
			type == FieldDescriptor::Type::TYPE_SINT64 ||
			type == FieldDescriptor::Type::TYPE_FIXED64 ||
			type == FieldDescriptor::Type::TYPE_SFIXED64) {
		printer.Print(variables, "/* $debug_string$ */\n");
		if (field_descriptor->is_repeated()) {
			printer.Print(variables, "jint $field_name$_cpp_size = cpp_obj.$cpp_field_name$_size();\n");
			printer.Print(variables, "jclass $field_name$_list_class = GetClass(\"java.util.ArrayList\");\n");
			printer.Print(variables, "jmethodID $field_name$_list_constructor = env->GetMethodID($field_name$_list_class, \"<init>\", \"(I)V\");\n");
			printer.Print(variables, "jobject $field_name$_list = env->NewObject($field_name$_list_class, $field_name$_list_constructor, $field_name$_cpp_size);\n");
			printer.Print(variables, "jmethodID $field_name$_list_add = env->GetMethodID($field_name$_list_class, \"add\", \"(Ljava/lang/Object;)Z\");\n");
			printer.Print(variables, "jclass $field_name$_long_class = GetClass(\"java.lang.Long\");\n");
			printer.Print(variables, "jmethodID $field_name$_long_constructor = env->GetMethodID($field_name$_long_class, \"<init>\", \"(J)V\");\n");
			printer.Print(variables, "for (int i = 0; i < $field_name$_cpp_size; i++) {\n");
			printer.Indent();
			printer.Print(variables, "jobject $field_name$_long_obj = env->NewObject($field_name$_long_class, $field_name$_long_constructor, cpp_obj.$cpp_field_name$(i));\n");
			printer.Print(variables, "env->CallBooleanMethod($field_name$_list, $field_name$_list_add, $field_name$_long_obj);\n");
			printer.Outdent();
			printer.Print(variables, "}\n");
			printer.Print(variables, "jfieldID $field_name$_field = env->GetFieldID(cls, \"$java_field_name$\", \"Ljava/util/List;\");\n");
			printer.Print(variables, "env->SetObjectField(obj, $field_name$_field, $field_name$_list);\n\n");
		} else {
			if (cpp::HasHasbit(field_descriptor)) {
				printer.Print(variables, "jboolean has_$field_name$_cpp = cpp_obj.has_$cpp_field_name$();\n");
				printer.Print(variables, "jfieldID has_$field_name$_field = env->GetFieldID(cls, \"$java_has_field_name$\", \"Z\");\n");
				printer.Print(variables, "env->SetBooleanField(obj, has_$field_name$_field, has_$field_name$_cpp);\n\n");
			}
			printer.Print(variables, "jlong $field_name$_cpp = cpp_obj.$cpp_field_name$();\n");
			printer.Print(variables, "jfieldID $field_name$_field = env->GetFieldID(cls, \"$java_field_name$\", \"J\");\n");
			printer.Print(variables, "env->SetLongField(obj, $field_name$_field, $field_name$_cpp);\n\n");
		}
	} else if (type == FieldDescriptor::Type::TYPE_FLOAT) {
		printer.Print(variables, "/* $debug_string$ */\n");
		if (field_descriptor->is_repeated()) {
			printer.Print(variables, "jint $field_name$_cpp_size = cpp_obj.$cpp_field_name$_size();\n");
			printer.Print(variables, "jclass $field_name$_list_class = GetClass(\"java/util/ArrayList\");\n");
			printer.Print(variables, "jmethodID $field_name$_list_constructor = env->GetMethodID($field_name$_list_class, \"<init>\", \"(I)V\");\n");
			printer.Print(variables, "jobject $field_name$_list = env->NewObject($field_name$_list_class, $field_name$_list_constructor, $field_name$_cpp_size);\n");
			printer.Print(variables, "jmethodID $field_name$_list_add = env->GetMethodID($field_name$_list_class, \"add\", \"(Ljava/lang/Object;)Z\");\n");
			printer.Print(variables, "jclass $field_name$_float_class = GetClass(\"java.lang.Float\");\n");
			printer.Print(variables, "jmethodID $field_name$_float_constructor = env->GetMethodID($field_name$_float_class, \"<init>\", \"(F)V\");\n");
			printer.Print(variables, "for (int i = 0; i < $field_name$_cpp_size; i++) {\n");
			printer.Indent();
			printer.Print(variables, "jobject $field_name$_float_obj = env->NewObject($field_name$_float_class, $field_name$_float_constructor, cpp_obj.$cpp_field_name$(i));\n");
			printer.Print(variables, "env->CallBooleanMethod($field_name$_list, $field_name$_list_add, $field_name$_float_obj);\n");
			printer.Outdent();
			printer.Print(variables, "}\n");
			printer.Print(variables, "jfieldID $field_name$_field = env->GetFieldID(cls, \"$java_field_name$\", \"Ljava/util/List;\");\n");
			printer.Print(variables, "env->SetObjectField(obj, $field_name$_field, $field_name$_list);\n\n");
		} else {
			if (cpp::HasHasbit(field_descriptor)) {
				printer.Print(variables, "jboolean has_$field_name$_cpp = cpp_obj.has_$cpp_field_name$();\n");
				printer.Print(variables, "jfieldID has_$field_name$_field = env->GetFieldID(cls, \"$java_has_field_name$\", \"Z\");\n");
				printer.Print(variables, "env->SetBooleanField(obj, has_$field_name$_field, has_$field_name$_cpp);\n\n");
			}
			printer.Print(variables, "jfloat $field_name$_cpp = cpp_obj.$cpp_field_name$();\n");
			printer.Print(variables, "jfieldID $field_name$_field = env->GetFieldID(cls, \"$java_field_name$\", \"F\");\n");
			printer.Print(variables, "env->SetFloatField(obj, $field_name$_field, $field_name$_cpp);\n\n");
		}
	} else if (type == FieldDescriptor::Type::TYPE_DOUBLE) {
		printer.Print(variables, "/* $debug_string$ */\n");
		if (field_descriptor->is_repeated()) {
			printer.Print(variables, "jint $field_name$_cpp_size = cpp_obj.$cpp_field_name$_size();\n");
			printer.Print(variables, "jclass $field_name$_list_class = GetClass(\"java.util.ArrayList\");\n");
			printer.Print(variables, "jmethodID $field_name$_list_constructor = env->GetMethodID($field_name$_list_class, \"<init>\", \"(I)V\");\n");
			printer.Print(variables, "jobject $field_name$_list = env->NewObject($field_name$_list_class, $field_name$_list_constructor, $field_name$_cpp_size);\n");
			printer.Print(variables, "jmethodID $field_name$_list_add = env->GetMethodID($field_name$_list_class, \"add\", \"(Ljava/lang/Object;)Z\");\n");
			printer.Print(variables, "jclass $field_name$_double_class = GetClass(\"java.lang.Double\");\n");
			printer.Print(variables, "jmethodID $field_name$_double_constructor = env->GetMethodID($field_name$_double_class, \"<init>\", \"(D)V\");\n");
			printer.Print(variables, "for (int i = 0; i < $field_name$_cpp_size; i++) {\n");
			printer.Indent();
			printer.Print(variables, "jobject $field_name$_double_obj = env->NewObject($field_name$_double_class, $field_name$_double_constructor, cpp_obj.$cpp_field_name$(i));\n");
			printer.Print(variables, "env->CallBooleanMethod($field_name$_list, $field_name$_list_add, $field_name$_double_obj);\n");
			printer.Outdent();
			printer.Print(variables, "}\n");
			printer.Print(variables, "jfieldID $field_name$_field = env->GetFieldID(cls, \"$java_field_name$\", \"Ljava/util/List;\");\n");
			printer.Print(variables, "env->SetObjectField(obj, $field_name$_field, $field_name$_list);\n\n");
		} else {
			if (cpp::HasHasbit(field_descriptor)) {
				printer.Print(variables, "jboolean has_$field_name$_cpp = cpp_obj.has_$cpp_field_name$();\n");
				printer.Print(variables, "jfieldID has_$field_name$_field = env->GetFieldID(cls, \"$java_has_field_name$\", \"Z\");\n");
				printer.Print(variables, "env->SetBooleanField(obj, has_$field_name$_field, has_$field_name$_cpp);\n\n");
			}
			printer.Print(variables, "jdouble $field_name$_cpp = cpp_obj.$cpp_field_name$();\n");
			printer.Print(variables, "jfieldID $field_name$_field = env->GetFieldID(cls, \"$java_field_name$\", \"D\");\n");
			printer.Print(variables, "env->SetDoubleField(obj, $field_name$_field, $field_name$_cpp);\n\n");
		}
	} else if (type == FieldDescriptor::Type::TYPE_BOOL) {
		printer.Print(variables, "/* $debug_string$ */\n");
		if (field_descriptor->is_repeated()) {
			printer.Print(variables, "jint $field_name$_cpp_size = cpp_obj.$cpp_field_name$_size();\n");
			printer.Print(variables, "jclass $field_name$_list_class = GetClass(\"java.util.ArrayList\");\n");
			printer.Print(variables, "jmethodID $field_name$_list_constructor = env->GetMethodID($field_name$_list_class, \"<init>\", \"(I)V\");\n");
			printer.Print(variables, "jobject $field_name$_list = env->NewObject($field_name$_list_class, $field_name$_list_constructor, $field_name$_cpp_size);\n");
			printer.Print(variables, "jmethodID $field_name$_list_add = env->GetMethodID($field_name$_list_class, \"add\", \"(Ljava/lang/Object;)Z\");\n");
			printer.Print(variables, "jclass $field_name$_boolean_class = GetClass(\"java.lang.Boolean\");\n");
			printer.Print(variables, "jmethodID $field_name$_boolean_constructor = env->GetMethodID($field_name$_boolean_class, \"<init>\", \"(Z)V\");\n");
			printer.Print(variables, "for (int i = 0; i < $field_name$_cpp_size; i++) {\n");
			printer.Indent();
			printer.Print(variables, "jobject $field_name$_boolean_obj = env->NewObject($field_name$_boolean_class, $field_name$_boolean_constructor, cpp_obj.$cpp_field_name$(i));\n");
			printer.Print(variables, "env->CallBooleanMethod($field_name$_list, $field_name$_list_add, $field_name$_boolean_obj);\n");
			printer.Outdent();
			printer.Print(variables, "}\n");
			printer.Print(variables, "jfieldID $field_name$_field = env->GetFieldID(cls, \"$java_field_name$\", \"Ljava/util/List;\");\n");
			printer.Print(variables, "env->SetObjectField(obj, $field_name$_field, $field_name$_list);\n\n");
		} else {
			if (cpp::HasHasbit(field_descriptor)) {
				printer.Print(variables, "jboolean has_$field_name$_cpp = cpp_obj.has_$cpp_field_name$();\n");
				printer.Print(variables, "jfieldID has_$field_name$_field = env->GetFieldID(cls, \"$java_has_field_name$\", \"Z\");\n");
				printer.Print(variables, "env->SetBooleanField(obj, has_$field_name$_field, has_$field_name$_cpp);\n\n");
			}
			printer.Print(variables, "jboolean $field_name$_cpp = cpp_obj.$cpp_field_name$();\n");
			printer.Print(variables, "jfieldID $field_name$_field = env->GetFieldID(cls, \"$java_field_name$\", \"Z\");\n");
			printer.Print(variables, "env->SetBooleanField(obj, $field_name$_field, $field_name$_cpp);\n\n");
		}
	} else if (type == FieldDescriptor::Type::TYPE_STRING) {
		printer.Print(variables, "/* $debug_string$ */\n");
		if (field_descriptor->is_repeated()) {
			printer.Print(variables, "jint $field_name$_cpp_size = cpp_obj.$cpp_field_name$_size();\n");
			printer.Print(variables, "jclass $field_name$_list_class = GetClass(\"java.util.ArrayList\");\n");
			printer.Print(variables, "jmethodID $field_name$_list_constructor = env->GetMethodID($field_name$_list_class, \"<init>\", \"(I)V\");\n");
			printer.Print(variables, "jobject $field_name$_list = env->NewObject($field_name$_list_class, $field_name$_list_constructor, $field_name$_cpp_size);\n");
			printer.Print(variables, "jmethodID $field_name$_list_add = env->GetMethodID($field_name$_list_class, \"add\", \"(Ljava/lang/Object;)Z\");\n");
			printer.Print(variables, "for (int i = 0; i < $field_name$_cpp_size; i++) {\n");
			printer.Indent();
			printer.Print(variables, "const char* $field_name$_cpp = cpp_obj.$cpp_field_name$(i).c_str();\n");
			printer.Print(variables, "jstring $field_name$_str = env->NewStringUTF($field_name$_cpp);\n");
			printer.Print(variables, "env->CallBooleanMethod($field_name$_list, $field_name$_list_add, $field_name$_str);\n");
			printer.Outdent();
			printer.Print(variables, "}\n");
			printer.Print(variables, "jfieldID $field_name$_field = env->GetFieldID(cls, \"$java_field_name$\", \"Ljava/util/List;\");\n");
			printer.Print(variables, "env->SetObjectField(obj, $field_name$_field, $field_name$_list);\n\n");
		} else {
			if (cpp::HasHasbit(field_descriptor)) {
				printer.Print(variables, "jboolean has_$field_name$_cpp = cpp_obj.has_$cpp_field_name$();\n");
				printer.Print(variables, "jfieldID has_$field_name$_field = env->GetFieldID(cls, \"$java_has_field_name$\", \"Z\");\n");
				printer.Print(variables, "env->SetBooleanField(obj, has_$field_name$_field, has_$field_name$_cpp);\n\n");
			}
			printer.Print(variables, "const char* $field_name$_cpp = cpp_obj.$cpp_field_name$().c_str();\n");
			printer.Print(variables, "jfieldID $field_name$_field = env->GetFieldID(cls, \"$java_field_name$\", \"Ljava/lang/String;\");\n");
			printer.Print(variables, "jstring $field_name$_str = env->NewStringUTF($field_name$_cpp);\n");
			printer.Print(variables, "env->SetObjectField(obj, $field_name$_field, $field_name$_str);\n\n");
		}
	} else if (type == FieldDescriptor::Type::TYPE_ENUM) {
		const EnumDescriptor* enum_type = field_descriptor->enum_type();
		variables["java_binary_type_name"] = GetJavaBinaryClassName(enum_type->file(), enum_type);
		variables["java_class_name"] = GetJavaClassName(enum_type->file(), enum_type);

		printer.Print(variables, "/* $debug_string$ */\n");
		if (field_descriptor->is_repeated()) {
			printer.Print(variables, "jint $field_name$_cpp_size = cpp_obj.$cpp_field_name$_size();\n");
			printer.Print(variables, "jclass $field_name$_list_class = GetClass(\"java.util.ArrayList\");\n");
			printer.Print(variables, "jmethodID $field_name$_list_constructor = env->GetMethodID($field_name$_list_class, \"<init>\", \"(I)V\");\n");
			printer.Print(variables, "jobject $field_name$_list = env->NewObject($field_name$_list_class, $field_name$_list_constructor, $field_name$_cpp_size);\n");
			printer.Print(variables, "jmethodID $field_name$_list_add = env->GetMethodID($field_name$_list_class, \"add\", \"(Ljava/lang/Object;)Z\");\n");
			printer.Print(variables, "jclass $field_name$_enum_class = GetClass(\"$java_class_name$\");\n");
			printer.Print(variables, "for (int i = 0; i < $field_name$_cpp_size; i++) {\n");
			printer.Indent();
			printer.Print(variables, "jint $field_name$_cpp = cpp_obj.$cpp_field_name$(i);\n");
			printer.Print(variables, "jmethodID $field_name$_get_enum = env->GetStaticMethodID($field_name$_enum_class, \"fromNumber\", \"(I)L$java_binary_type_name$;\");\n");
			printer.Print(variables, "jobject $field_name$_enum = env->CallStaticObjectMethod($field_name$_enum_class, $field_name$_get_enum, $field_name$_cpp);\n");
			printer.Print(variables, "env->CallBooleanMethod($field_name$_list, $field_name$_list_add, $field_name$_enum);\n");
			printer.Outdent();
			printer.Print(variables, "}\n");
			printer.Print(variables, "jfieldID $field_name$_field = env->GetFieldID(cls, \"$java_field_name$\", \"Ljava/util/List;\");\n");
			printer.Print(variables, "env->SetObjectField(obj, $field_name$_field, $field_name$_list);\n\n");
		} else {
			if (cpp::HasHasbit(field_descriptor)) {
				printer.Print(variables, "jboolean has_$field_name$_cpp = cpp_obj.has_$cpp_field_name$();\n");
				printer.Print(variables, "jfieldID has_$field_name$_field = env->GetFieldID(cls, \"$java_has_field_name$\", \"Z\");\n");
				printer.Print(variables, "env->SetBooleanField(obj, has_$field_name$_field, has_$field_name$_cpp);\n\n");
			}
			printer.Print(variables, "jclass $field_name$_cls = GetClass(\"$java_class_name$\");\n");
			printer.Print(variables, "jint $field_name$_cpp = cpp_obj.$cpp_field_name$();\n");
			printer.Print(variables, "jfieldID $field_name$_field = env->GetFieldID(cls, \"$java_field_name$\", \"L$java_binary_type_name$;\");\n");
			printer.Print(variables, "jmethodID $field_name$_get_enum = env->GetStaticMethodID($field_name$_cls, \"fromNumber\", \"(I)L$java_binary_type_name$;\");\n");
			printer.Print(variables, "jobject $field_name$_enum = env->CallStaticObjectMethod($field_name$_cls, $field_name$_get_enum, $field_name$_cpp);\n");
			printer.Print(variables, "env->SetObjectField(obj, $field_name$_field, $field_name$_enum);\n\n");
		}
	} else if (type == FieldDescriptor::Type::TYPE_GROUP ||
						 type == FieldDescriptor::Type::TYPE_MESSAGE) {
		const Descriptor* message_type = field_descriptor->message_type();
		variables["java_binary_type_name"] = GetJavaBinaryClassName(message_type->file(), message_type);

		printer.Print(variables, "/* $debug_string$ */\n");
		if (field_descriptor->is_repeated()) {
			if (field_descriptor->message_type()->options().map_entry()) {
				// TODO
				//printer.Print(variables, "jint $field_name$_cpp_size = cpp_obj.$cpp_field_name$_size();\n");
				//printer.Print(variables, "jclass $field_name$_map_class = GetClass(\"java.util.HashMap\");\n");
				//printer.Print(variables, "jmethodID $field_name$_map_constructor = env->GetMethodID(cls, \"<init>\", \"(I)V\");\n");
				//printer.Print(variables, "jobject $field_name$_map = env->NewObject(cls, $field_name$_map_constructor, $field_name$_cpp_size);\n");
				//printer.Print(variables, "jmethodID $field_name$_map_put = env->GetMethodID(cls, \"put\", \"(java/lang/Object;java/lang/Object)java/lang/Object\");\n");
				//printer.Print(variables, "for (int i = 0; i < $field_name$_cpp_size; i++) {\n");
				//printer.Indent();
				//printer.Print(variables, "env->CallObjectMethod(cls, $field_name$_map, $field_name$_map_put, cpp_obj.$cpp_field_name$(i));\n");
				//printer.Outdent();
				//printer.Print(variables, "}\n");
				//printer.Print(variables, "jfieldID $field_name$_field = env->GetFieldID(cls, \"$java_field_name$\"), \"I\";\n");
				//printer.Print(variables, "env->SetObjectField(obj, $field_name$_field, $field_name$_map);\n\n");
			} else {
				printer.Print(variables, "jint $field_name$_cpp_size = cpp_obj.$cpp_field_name$_size();\n");
				printer.Print(variables, "jclass $field_name$_list_class = GetClass(\"java.util.ArrayList\");\n");
				printer.Print(variables, "jmethodID $field_name$_list_constructor = env->GetMethodID($field_name$_list_class, \"<init>\", \"(I)V\");\n");
				printer.Print(variables, "jobject $field_name$_list = env->NewObject($field_name$_list_class, $field_name$_list_constructor, $field_name$_cpp_size);\n");
				printer.Print(variables, "jmethodID $field_name$_list_add = env->GetMethodID($field_name$_list_class, \"add\", \"(Ljava/lang/Object;)Z\");\n");
				printer.Print(variables, "for (int i = 0; i < $field_name$_cpp_size; i++) {\n");
				printer.Indent();
				printer.Print(variables, "env->CallBooleanMethod($field_name$_list, $field_name$_list_add, cpp2java(env, cpp_obj.$cpp_field_name$(i)));\n");
				printer.Outdent();
				printer.Print(variables, "}\n");
				printer.Print(variables, "jfieldID $field_name$_field = env->GetFieldID(cls, \"$java_field_name$\", \"Ljava/util/List;\");\n");
				printer.Print(variables, "env->SetObjectField(obj, $field_name$_field, $field_name$_list);\n\n");
			}
		} else {
			if (cpp::HasHasbit(field_descriptor)) {
				printer.Print(variables, "jboolean has_$field_name$_cpp = cpp_obj.has_$cpp_field_name$();\n");
				printer.Print(variables, "jfieldID has_$field_name$_field = env->GetFieldID(cls, \"$java_has_field_name$\", \"Z\");\n");
				printer.Print(variables, "env->SetBooleanField(obj, has_$field_name$_field, has_$field_name$_cpp);\n\n");
			}
			printer.Print(variables, "jobject $field_name$_cpp = cpp2java(env, cpp_obj.$cpp_field_name$());\n");
			printer.Print(variables, "jfieldID $field_name$_field = env->GetFieldID(cls, \"$java_field_name$\", \"L$java_binary_type_name$;\");\n");
			printer.Print(variables, "env->SetObjectField(obj, $field_name$_field, $field_name$_cpp);\n\n");
		}
	}

}



}  // namespace cpp2java
}  // namespace compiler
}  // namespace protobuf
}  // namespace google
