#include <google/protobuf/compiler/javabean/javabean_field.h>


#include <google/protobuf/compiler/java/java_helpers.h>
#include <google/protobuf/compiler/java/java_name_resolver.h>

#include <google/protobuf/compiler/javabean/javabean_helpers.h>

namespace google {
namespace protobuf {
namespace compiler {
namespace javabean {

void GenerateStringField(const FieldDescriptor* field_descriptor, io::Printer* printer) {
	std::map<std::string, std::string> variables;
	variables["capitalized_name"] = java::UnderscoresToCapitalizedCamelCase(field_descriptor);
	variables["debug_string"] = field_descriptor->DebugString();

	printer->Print(variables, "/** $debug_string$ */\n");
	if (java::HasHazzer(field_descriptor)) {
		printer->Print(variables, "private boolean mHas$capitalized_name$;\n");
		printer->Print(variables, "public boolean has$capitalized_name$() {\n");
		printer->Indent();
		printer->Print(variables, "return mHas$capitalized_name$;\n");
		printer->Outdent();
		printer->Print(variables, "}\n");
	}
	printer->Print(variables, "private java.lang.String m$capitalized_name$;\n");
	printer->Print(variables, "public java.lang.String get$capitalized_name$() {\n");
	printer->Indent();
	printer->Print(variables, "return m$capitalized_name$;\n");
	printer->Outdent();
	printer->Print(variables, "}\n");
}

void GenerateRepeatedStringField(const FieldDescriptor* field_descriptor, io::Printer* printer) {
	std::map<std::string, std::string> variables;
	variables["capitalized_name"] = java::UnderscoresToCapitalizedCamelCase(field_descriptor);
	variables["debug_string"] = field_descriptor->DebugString();

	printer->Print(variables, "/** $debug_string$ */\n");
	printer->Print(variables, "private java.util.List<java.lang.String> m$capitalized_name$ = new java.util.ArrayList<>(0);\n");
	printer->Print(variables, "public java.util.List<java.lang.String> get$capitalized_name$() {\n");
	printer->Indent();
	printer->Print(variables, "return m$capitalized_name$;\n");
	printer->Outdent();
	printer->Print(variables, "}\n");
}

void GeneratePrimitiveField(const FieldDescriptor* field_descriptor, io::Printer* printer) {
	std::map<std::string, std::string> variables;
	variables["type"] = GetPrimitiveTypeName(field_descriptor);
	variables["capitalized_name"] = java::UnderscoresToCapitalizedCamelCase(field_descriptor);
	variables["debug_string"] = field_descriptor->DebugString();

	printer->Print(variables, "/** $debug_string$ */\n");
	if (java::HasHazzer(field_descriptor)) {
		printer->Print(variables, "private boolean mHas$capitalized_name$;\n");
		printer->Print(variables, "public boolean has$capitalized_name$() {\n");
		printer->Indent();
		printer->Print(variables, "return mHas$capitalized_name$;\n");
		printer->Outdent();
		printer->Print(variables, "}\n");
	}
	printer->Print(variables, "private $type$ m$capitalized_name$;\n");
	printer->Print(variables, "public $type$ get$capitalized_name$() {\n");
	printer->Indent();
	printer->Print(variables, "return m$capitalized_name$;\n");
	printer->Outdent();
	printer->Print(variables, "}\n");
}

void GenerateRepeatedPrimitiveField(const FieldDescriptor* field_descriptor, io::Printer* printer) {
	std::map<std::string, std::string> variables;
	variables["type"] = GetPrimitiveTypeName(field_descriptor);
	variables["boxed_type"] = GetBoxedPrimitiveTypeName(field_descriptor);
	variables["capitalized_name"] = java::UnderscoresToCapitalizedCamelCase(field_descriptor);
	variables["debug_string"] = field_descriptor->DebugString();

	printer->Print(variables, "/** $debug_string$ */\n");
	printer->Print(variables, "private java.util.List<$boxed_type$> m$capitalized_name$ = new java.util.ArrayList<>(0);\n");
	printer->Print(variables, "public java.util.List<$boxed_type$> get$capitalized_name$() {\n");
	printer->Indent();
	printer->Print(variables, "return m$capitalized_name$;\n");
	printer->Outdent();
	printer->Print(variables, "}\n");
}

void GenerateMessageField(const FieldDescriptor* field_descriptor, io::Printer* printer) {
	std::map<std::string, std::string> variables;
	variables["type"] = GetClassName(field_descriptor->message_type());
	variables["capitalized_name"] = java::UnderscoresToCapitalizedCamelCase(field_descriptor);
	variables["debug_string"] = field_descriptor->DebugString();

	printer->Print(variables, "/** $debug_string$ */\n");
	if (java::HasHasbit(field_descriptor)) {
		printer->Print(variables, "private boolean mHas$capitalized_name$;\n");
		printer->Print(variables, "public boolean has$capitalized_name$() {\n");
		printer->Indent();
		printer->Print(variables, "return mHas$capitalized_name$;\n");
		printer->Outdent();
		printer->Print(variables, "}\n");
	}
	printer->Print(variables, "private $type$ m$capitalized_name$;\n");
	printer->Print(variables, "public $type$ get$capitalized_name$() {\n");
	printer->Indent();
	printer->Print(variables, "return m$capitalized_name$;\n");
	printer->Outdent();
	printer->Print(variables, "}\n");

}

void GenerateRepeatedMessageField(const FieldDescriptor* field_descriptor, io::Printer* printer) {
	std::map<std::string, std::string> variables;
	variables["type"] = GetClassName(field_descriptor->message_type());
	variables["capitalized_name"] = java::UnderscoresToCapitalizedCamelCase(field_descriptor);
	variables["debug_string"] = field_descriptor->DebugString();

	printer->Print(variables, "/** $debug_string$ */\n");
	printer->Print(variables, "private java.util.List<$type$> m$capitalized_name$ = new java.util.ArrayList<>(0);\n");
	printer->Print(variables, "public java.util.List<$type$> get$capitalized_name$() {\n");
	printer->Indent();
	printer->Print(variables, "return m$capitalized_name$;\n");
	printer->Outdent();
	printer->Print(variables, "}\n");
}

void GenerateEnumField(const FieldDescriptor* field_descriptor, io::Printer* printer) {
	std::map<std::string, std::string> variables;
	variables["type"] = GetClassName(field_descriptor->enum_type());
	variables["capitalized_name"] = java::UnderscoresToCapitalizedCamelCase(field_descriptor);
	variables["debug_string"] = field_descriptor->DebugString();

	printer->Print(variables, "/** $debug_string$ */\n");
	if (java::HasHazzer(field_descriptor)) {
		printer->Print(variables, "private boolean mHas$capitalized_name$;\n");
		printer->Print(variables, "public boolean has$capitalized_name$() {\n");
		printer->Indent();
		printer->Print(variables, "return mHas$capitalized_name$;\n");
		printer->Outdent();
		printer->Print(variables, "}\n");
	}
	printer->Print(variables, "private $type$ m$capitalized_name$;\n");
	printer->Print(variables, "public $type$ get$capitalized_name$() {\n");
	printer->Indent();
	printer->Print(variables, "return m$capitalized_name$;\n");
	printer->Outdent();
	printer->Print(variables, "}\n");
}

void GenerateRepeatedEnumField(const FieldDescriptor* field_descriptor, io::Printer* printer) {
	std::map<std::string, std::string> variables;
	variables["type"] = GetClassName(field_descriptor->enum_type());
	variables["capitalized_name"] = java::UnderscoresToCapitalizedCamelCase(field_descriptor);
	variables["debug_string"] = field_descriptor->DebugString();

	printer->Print(variables, "/** $debug_string$ */\n");
	printer->Print(variables, "private java.util.List<$type$> m$capitalized_name$ = new java.util.ArrayList<>(0);\n");
	printer->Print(variables, "public java.util.List<$type$> get$capitalized_name$() {\n");
	printer->Indent();
	printer->Print(variables, "return m$capitalized_name$;\n");
	printer->Outdent();
	printer->Print(variables, "}\n");
}

std::string TypeName1(const FieldDescriptor* field) {
	if (java::GetJavaType(field) == java::JAVATYPE_MESSAGE) {
		return GetClassName(field->message_type());
	} else if (java::GetJavaType(field) == java::JAVATYPE_ENUM) {
		return GetClassName(field->enum_type());
	} else {
		return GetBoxedPrimitiveTypeName(field);
	}
}

void GenerateMapField(const FieldDescriptor* field_descriptor, io::Printer* printer) {
	std::map<std::string, std::string> variables;
	variables["capitalized_name"] = java::UnderscoresToCapitalizedCamelCase(field_descriptor);
	variables["debug_string"] = field_descriptor->DebugString();

	const FieldDescriptor* key_field_descriptor = field_descriptor->message_type()->map_key();
	if (java::GetJavaType(key_field_descriptor) == java::JAVATYPE_MESSAGE) {
		variables["boxed_key_type"] = GetClassName(key_field_descriptor->message_type());
	} else if (java::GetJavaType(key_field_descriptor) == java::JAVATYPE_ENUM) {
		variables["boxed_key_type"] = GetClassName(key_field_descriptor->enum_type());
	} else {
		variables["boxed_key_type"] = GetBoxedPrimitiveTypeName(key_field_descriptor);
	}

	const FieldDescriptor* value_field_descriptor = field_descriptor->message_type()->map_value();
	if (java::GetJavaType(value_field_descriptor) == java::JAVATYPE_MESSAGE) {
		variables["boxed_value_type"] = GetClassName(value_field_descriptor->message_type());
	} else if (java::GetJavaType(value_field_descriptor) == java::JAVATYPE_ENUM) {
		variables["boxed_value_type"] = GetClassName(value_field_descriptor->enum_type());
	} else {
		variables["boxed_value_type"] = GetBoxedPrimitiveTypeName(value_field_descriptor);
	}

	printer->Print(variables, "/** $debug_string$ */\n");
	printer->Print(variables, "private java.util.Map<$boxed_key_type$, $boxed_value_type$> m$capitalized_name$ = new java.util.HashMap<>(0);\n");
	printer->Print(variables, "public java.util.Map<$boxed_key_type$, $boxed_value_type$> get$capitalized_name$() {\n");
	printer->Indent();
	printer->Print(variables, "return m$capitalized_name$;\n");
	printer->Outdent();
	printer->Print(variables, "}\n");
}

}  // namespace javabean
}  // namespace compiler
}  // namespace protobuf
}  // namespace google
