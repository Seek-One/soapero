/*
 * WSDLTags.h
 *
 *  Created on: 22 juin 2018
 *      Author: alavier
 */

#ifndef SRC_PARSER_WSDLTAGS_H_
#define SRC_PARSER_WSDLTAGS_H_

#define TAG_DEFINITIONS "definitions"
#define TAG_TYPES "types"
#define TAG_SCHEMA "schema"
#define TAG_IMPORT "import"
#define TAG_INCLUDE "include"
#define TAG_COMPLEX_TYPE "complexType"
#define TAG_SIMPLE_TYPE "simpleType"
#define TAG_ATTRIBUTE "attribute"
#define TAG_EXTENSION "extension"
#define TAG_ELEMENT "element"
#define TAG_PART "part"
#define TAG_INPUT "input"
#define TAG_OUTPUT "output"
#define TAG_MESSAGE "message"
#define TAG_OPERATION "operation"
#define TAG_RESTRICTION "restriction"
#define TAG_MAX_LENGTH "maxLength"
#define TAG_MIN_LENGTH "minLength"
#define TAG_ENUMERATION "enumeration"
#define TAG_LIST "list"
#define TAG_UNION "union"
#define TAG_ANY "any"

class Section {
public:
	enum Name {
		Other,			// Needed to handle unused (for now) sections

		Definitions,
		Types,
		Schema,
		Import,
		Include,
		ComplexType,
		SimpleType,
		Attribute,
		Extension,
		Element,
		Part,
		Input,
		Output,
		Message,
		Operation,
		Restriction,
		MaxLength,
		MinLength,
		Enumeration,
		List,
		Union,
		Any,
	};
};

#endif /* SRC_PARSER_WSDLTAGS_H_ */
