#include <google/protobuf/compiler/javabean/javabean_file.h>

#include <google/protobuf/compiler/javabean/javabean_helpers.h>
#include <google/protobuf/compiler/javabean/javabean_field.h>


namespace google {
namespace protobuf {
namespace compiler {
namespace javabean {

void GenerateJavaBeanFile(const FileDescriptor* file_descriptor, io::Printer* printer) {
	std::map<std::string, std::string> variables;
	variables["file_name"] = file_descriptor->name();
	variables["class_name"] = GetClassName(file_descriptor);
	variables["package_name"] = GetPackageName(file_descriptor);

	printer->Print(variables,
								 "// Generated by the protocol buffer compiler.  DO NOT EDIT!\n"
								 "// source: $file_name$\n\n");

	if (!GetPackageName(file_descriptor).empty()) {
		printer->Print(variables, "package $package_name$;\n\n");
	}

	printer->Print(variables, "public final class $class_name$ {\n\n");

	printer->Indent();
	printer->Indent();

	for (int i = 0; i < file_descriptor->message_type_count(); i++) {
		const Descriptor* d = file_descriptor->message_type(i);
		GenerateMessageClass(d, printer);
	}

	for (int i = 0; i < file_descriptor->enum_type_count(); i++) {
		const EnumDescriptor* d = file_descriptor->enum_type(i);
		GenerateEnumClass(d, printer);
	}

	printer->Outdent();
	printer->Outdent();
	printer->Print("}");
}

void GenerateMessageClass(const Descriptor* descriptor, io::Printer* printer) {
	std::map<std::string, std::string> variables;
	variables["class_name"] = descriptor->name();

	printer->Print(variables, "public static final class $class_name$ {\n");

	printer->Indent();
	printer->Indent();

	for (int i = 0; i < descriptor->field_count(); i++) {
		const FieldDescriptor* field_descriptor = descriptor->field(i);

		if (field_descriptor->is_repeated()) {
			switch (GetJavaType(field_descriptor)) {
			case JavaType::JAVATYPE_MESSAGE:
				if (field_descriptor->message_type()->options().map_entry()) {
					GenerateMapField(field_descriptor, printer);
				} else {
					GenerateRepeatedMessageField(field_descriptor, printer);
				}
				break;
			case JavaType::JAVATYPE_ENUM:
				GenerateRepeatedEnumField(field_descriptor, printer);
				break;
			case JavaType::JAVATYPE_STRING:
				GenerateRepeatedStringField(field_descriptor, printer);
				break;
			default:
				GenerateRepeatedPrimitiveField(field_descriptor, printer);
				break;
			}
		} else {
			switch (GetJavaType(field_descriptor)) {
			case JavaType::JAVATYPE_MESSAGE:
				GenerateMessageField(field_descriptor, printer);
				break;
			case JavaType::JAVATYPE_ENUM:
				GenerateEnumField(field_descriptor, printer);
				break;
			case JavaType::JAVATYPE_STRING:
				GenerateStringField(field_descriptor, printer);
				break;
			default:
				GeneratePrimitiveField(field_descriptor, printer);
				break;
			}
		}


	}

	printer->Print("\n");
	for (int i = 0; i < descriptor->nested_type_count(); i++) {
		if (descriptor->nested_type(i)->options().map_entry()) {
			continue;
		}
		const Descriptor* d = descriptor->nested_type(i);
		GenerateMessageClass(d, printer);
	}

	printer->Print("\n");
	for (int i = 0; i < descriptor->enum_type_count(); i++) {
		const EnumDescriptor* d = descriptor->enum_type(i);
		GenerateEnumClass(d, printer);
	}

	printer->Outdent();
	printer->Outdent();
	printer->Print("}\n\n");

}

void GenerateEnumClass(const EnumDescriptor* enum_descriptor, io::Printer* printer) {
	std::map<std::string, std::string> variables;
	variables["class_name"] = enum_descriptor->name();

	printer->Print(variables,
								 "public enum $class_name$ {\n");

	printer->Indent();
	printer->Indent();

	for (int i = 0; i < enum_descriptor->value_count(); i++) {
		const EnumValueDescriptor* d = enum_descriptor->value(i);
		variables["value_name"] = d->name();
		variables["value_number"] = std::to_string(d->number());

		printer->Print(variables, "$value_name$($value_number$),\n");
	}
	printer->Print(";\n");

	printer->Print("private final int mNumber;\n");
	printer->Print(variables, "$class_name$(int number) {\n");
	printer->Indent();
	printer->Indent();
	printer->Print("this.mNumber = number;\n");
	printer->Outdent();
	printer->Outdent();
	printer->Print("}\n");
	printer->Print("public final int getNumber() {\n");
	printer->Indent();
	printer->Indent();
	printer->Print("return mNumber;\n");
	printer->Outdent();
	printer->Outdent();
	printer->Print("}\n");


	printer->Print(variables, "public static $class_name$ fromNumber(int number) {\n");
	for (int i = 0; i < enum_descriptor->value_count(); i++) {
		const EnumValueDescriptor* d = enum_descriptor->value(i);
		variables["value_name"] = d->name();
		variables["value_number"] = std::to_string(d->number());

		printer->Print(variables, "if (number == $value_number$) { return $value_name$; }\n");
	}
	printer->Print(variables, "return null;\n");
	printer->Print(variables, "}\n");



	printer->Outdent();
	printer->Outdent();

	printer->Print("}\n\n");
}

}  // namespace javabean
}  // namespace compiler
}  // namespace protobuf
}  // namespace google
