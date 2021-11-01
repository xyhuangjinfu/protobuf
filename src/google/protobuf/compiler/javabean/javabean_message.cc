#include <google/protobuf/compiler/javabean/javabean_message.h>


#include <google/protobuf/compiler/javabean/javabean_helpers.h>
#include <google/protobuf/compiler/javabean/javabean_enum.h>

namespace google {
namespace protobuf {
namespace compiler {
namespace javabean {

void GenerateMessageClass(const Descriptor* descriptor, io::Printer* printer) {
	std::map<std::string, std::string> variables;
	variables["class_name"] = descriptor->name();

	printer->Print(variables,
								 "public static final class $class_name$ {\n");

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
	printer->Print("}\n\n");

}

}  // namespace javabean
}  // namespace compiler
}  // namespace protobuf
}  // namespace google
