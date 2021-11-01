#include <google/protobuf/compiler/javabean/javabean_enum.h>


namespace google {
namespace protobuf {
namespace compiler {
namespace javabean {

void GenerateEnumClass(const EnumDescriptor* enum_descriptor, io::Printer* printer) {
	std::map<std::string, std::string> variables;
	variables["class_name"] = enum_descriptor->name();

	printer->Print(variables,
								 "public enum $class_name$ {\n");

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
	printer->Print("this.mNumber = number;\n");
	printer->Outdent();
	printer->Print("}\n");
	printer->Print("public final int getNumber() {\n");
	printer->Indent();
	printer->Print("return mNumber;\n");
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

	printer->Print("}\n\n");
}

}  // namespace javabean
}  // namespace compiler
}  // namespace protobuf
}  // namespace google
