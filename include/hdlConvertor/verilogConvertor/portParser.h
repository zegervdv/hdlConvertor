#pragma once

#include <vector>

#include <hdlConvertor/verilogConvertor/Verilog2001Parser/Verilog2001Parser.h>
#include <hdlConvertor/hdlObjects/expr.h>
#include <hdlConvertor/hdlObjects/variable.h>
#include <hdlConvertor/verilogConvertor/commentParser.h>

namespace hdlConvertor {
namespace verilog {

class PortParser {
	CommentParser & commentParser;
public:
	using Verilog2001Parser = Verilog2001_antlr::Verilog2001Parser;
	using Variable = hdlObjects::Variable;

	PortParser(CommentParser & commentParser);

	std::vector<Variable*>* addTypeSpecToPorts(hdlObjects::Direction direction,
			Verilog2001Parser::Net_typeContext * net_type, bool signed_,
			bool reg_, std::string doc,
			Verilog2001Parser::Range_Context * range_,
			std::vector<Variable*> * ports);
	std::vector<Variable*>* visitList_of_ports(
			Verilog2001Parser::List_of_portsContext* ctx);
	std::vector<Variable*> * visitPort(Verilog2001Parser::PortContext* ctx);
	std::vector<Variable*> *visitPort_expression(
			Verilog2001Parser::Port_expressionContext* ctx);
	Variable * visitPort_reference(
			Verilog2001Parser::Port_referenceContext* ctx);
	std::vector<Variable*>* visitList_of_port_declarations(
			Verilog2001Parser::List_of_port_declarationsContext* ctx);
	std::vector<Variable*> * visitPort_declaration(
			Verilog2001Parser::Port_declarationContext* ctx);
	std::vector<Variable*> * visitList_of_port_identifiers(
			Verilog2001Parser::List_of_port_identifiersContext* ctx);
	Variable * visitPort_identifier(
			Verilog2001Parser::Port_identifierContext * ctx);
	std::vector<Variable*> *visitList_of_variable_port_identifiers(
			Verilog2001Parser::List_of_variable_port_identifiersContext * ctx);
};

}
}
