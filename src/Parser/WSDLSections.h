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
#define TAG_MESSAGE "message"
#define TAG_PORTTYPE "portType"
#define TAG_PART "part"
#define TAG_OPERATION "operation"
#define TAG_INPUT "input"
#define TAG_OUTPUT "output"

/////////////////////////////////
// XML Schema Part 1: Structures
/////////////////////////////////

#define TAG_SCHEMA "schema"
#define TAG_COMPLEX_TYPE "complexType"
#define TAG_COMPLEX_CONTENT "complexContent"
#define TAG_SIMPLE_CONTENT "simpleContent"
#define TAG_EXTENSION "extension"
#define TAG_ELEMENT "element"
#define TAG_GROUP "group"
#define TAG_ALL "all"
#define TAG_CHOICE "choice"
#define TAG_SEQUENCE "sequence"
#define TAG_ANY "any"
#define TAG_ATTRIBUTE "attribute"
#define TAG_ATTRIBUTE_GROUP "attributeGroup"
#define TAG_INCLUDE "include"
#define TAG_IMPORT "import"
#define TAG_REDEFINE "redefine"
#define TAG_OVERRIDE "override"

/////////////////////////////////
// XML Schema Part 2: Datatypes
/////////////////////////////////

// Simple type
#define TAG_SIMPLE_TYPE "simpleType"
#define TAG_RESTRICTION "restriction"
#define TAG_LIST "list"
#define TAG_UNION "union"
#define TAG_MAX_LENGTH "maxLength"
#define TAG_MIN_LENGTH "minLength"
#define TAG_ENUMERATION "enumeration"

class Section {
public:
	enum Name {
		Other,			// Needed to handle unused (for now) sections

		// WSDL
		Definitions,
		Types,
		Message,
		Part,
		Operation,
		Input,
		Output,

		// XMLSchema Part 1 : Structures
		Schema,
		ComplexType,
		Extension,
		Element,
		Choice,
		Sequence,
		Any,
		Attribute,
		Include,
		Import,

		// XMLSchema Part 2 : Datatypes
		SimpleType,
		Restriction,
		List,
		Union,
		MaxLength,
		MinLength,
		Enumeration,
	};
};

#endif /* SRC_PARSER_WSDLTAGS_H_ */
