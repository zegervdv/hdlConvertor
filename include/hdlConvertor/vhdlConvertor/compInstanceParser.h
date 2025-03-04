#pragma once

#include <vector>
#include <hdlConvertor/vhdlConvertor/vhdlParser/vhdlParser.h>
#include <hdlConvertor/hdlObjects/expr.h>
#include <hdlConvertor/hdlObjects/compInstance.h>

namespace hdlConvertor {
namespace vhdl {

class CompInstanceParser {
public:
	using vhdlParser = vhdl_antlr::vhdlParser;
	using CompInstance = hdlObjects::CompInstance;
	using Expr = hdlObjects::Expr;

	static CompInstance * visitComponent_instantiation_statement(
			vhdlParser::Component_instantiation_statementContext* ctx);
	static std::vector<Expr*> * visitPort_map_aspect(
			vhdlParser::Port_map_aspectContext* ctx);
	static CompInstance * visitInstantiated_unit(
			vhdlParser::Instantiated_unitContext* ctx);
	static std::vector<Expr*> * visitGeneric_map_aspect(
			vhdlParser::Generic_map_aspectContext* ctx);
};

}
}
