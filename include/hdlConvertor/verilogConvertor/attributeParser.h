#pragma once

#include <vector>

#include <hdlConvertor/verilogConvertor/Verilog2001Parser/Verilog2001Parser.h>
#include <hdlConvertor/hdlObjects/expr.h>

namespace hdlConvertor {
namespace verilog {

class AttributeParser {
public:
	using Verilog2001Parser = Verilog2001_antlr::Verilog2001Parser;

	static std::vector<hdlObjects::Expr*>* visitAttribute_instance(
			Verilog2001Parser::Attribute_instanceContext * ctx);
};

}
}
