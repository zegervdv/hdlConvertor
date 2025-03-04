#pragma once

#include <string>
#include <map>
#include <sys/stat.h>
#include <typeinfo>

#include <hdlConvertor/verilogPreproc/verilogPreprocParser/verilogPreprocLexer.h>
#include <hdlConvertor/verilogPreproc/verilogPreprocParser/verilogPreprocParser.h>
#include <hdlConvertor/verilogPreproc/verilogPreprocParser/verilogPreprocParserBaseVisitor.h>

#include <hdlConvertor/verilogPreproc/macroSymbol.h>
#include <hdlConvertor/syntaxErrorLogger.h>
#include <hdlConvertor/conversion_exception.h>
#include <hdlConvertor/language.h>
#include <filesystem>

namespace hdlConvertor {
namespace verilog_pp {

/*
 * Verilog preprocessor
 *
 * :ivar _defineDB: database of defines
 * :ivar _incdir: directories where to search for included files
 * 		    (last will be searched first, unique items)
 * :ivar added_incdir: true if the new incdir was added exactly for this file
 * 			and last incdir should be removed before processing of new file
 * :ivar _stack_incfile: stack of include files which are
 * 			currently parsed (used for detection of cycle in includes)
 * :ivar did_added_incdir: the flag which tells if the directory of this folder
 * 			was added to include directories and thus should be removed after parser ends
 **/
class vPreprocessor: public verilogPreproc_antlr::verilogPreprocParserBaseVisitor {
	macroSymbol & _defineDB;
	antlr4::CommonTokenStream & _tokens;
	std::vector<std::filesystem::path> _incdir;
	bool added_incdir;
	std::vector<std::filesystem::path> & _stack_incfile;
	Language _mode;

	void remove_comment(antlr4::Token * start, antlr4::Token * end,
			std::string * str);
	std::string genBlank(size_t n);
	void replace_context_by_bank(antlr4::ParserRuleContext * ctx);

public:
	using verilogPreprocParser = verilogPreproc_antlr::verilogPreprocParser;
	size_t include_depth_limit;
	antlr4::TokenStreamRewriter _rewriter;

	vPreprocessor(antlr4::TokenStream & tokens,
			std::vector<std::filesystem::path> &incdir, bool added_incdir,
			macroSymbol & defineDB,
			std::vector<std::filesystem::path> &stack_incfile, Language mode =
					Language::VERILOG2001, size_t include_depth_limit = 100);

	virtual ~vPreprocessor();

	virtual antlrcpp::Any visitResetall(
			verilogPreprocParser::ResetallContext * ctx);
	virtual antlrcpp::Any visitCelldefine(
			verilogPreprocParser::CelldefineContext * ctx);
	virtual antlrcpp::Any visitEndcelldefine(
			verilogPreprocParser::EndcelldefineContext * ctx);

	virtual antlrcpp::Any visitTiming_spec(
			verilogPreprocParser::Timing_specContext * ctx);
	virtual antlrcpp::Any visitDefault_nettype(
			verilogPreprocParser::Default_nettypeContext * ctx);
	virtual antlrcpp::Any visitUnconnected_drive(
			verilogPreprocParser::Unconnected_driveContext * ctx);
	virtual antlrcpp::Any visitNounconnected_drive(
			verilogPreprocParser::Nounconnected_driveContext * ctx);
	virtual antlrcpp::Any visitLine_directive(
			verilogPreprocParser::Line_directiveContext * ctx);

	virtual antlrcpp::Any visitDefine(
			verilogPreprocParser::DefineContext * ctx);
	virtual antlrcpp::Any visitUndef(verilogPreprocParser::UndefContext * ctx);

	virtual antlrcpp::Any visitToken_id(
			verilogPreprocParser::Token_idContext * ctx);

	virtual antlrcpp::Any visitIfdef_directive(
			verilogPreprocParser::Ifdef_directiveContext * ctx);
	virtual antlrcpp::Any visitIfndef_directive(
			verilogPreprocParser::Ifndef_directiveContext * ctx);

	virtual antlrcpp::Any visitStringLiteral(
			verilogPreprocParser::StringLiteralContext *ctx);
	virtual antlrcpp::Any visitInclude(
			verilogPreprocParser::IncludeContext * ctx);

	virtual antlrcpp::Any visitKeywords_directive(
			verilogPreprocParser::Keywords_directiveContext * ctx);
	virtual antlrcpp::Any visitEndkeywords_directive(
			verilogPreprocParser::Endkeywords_directiveContext * ctx);

	virtual antlrcpp::Any visitPragma(
			verilogPreprocParser::PragmaContext * ctx);

	virtual antlrcpp::Any visitUndefineall(
			verilogPreprocParser::UndefineallContext * ctx);

	virtual antlrcpp::Any visitFile_nb(
			verilogPreprocParser::File_nbContext * ctx);
	virtual antlrcpp::Any visitLine_nb(
			verilogPreprocParser::Line_nbContext * ctx);

};

std::string run_verilog_preproc(antlr4::ANTLRInputStream & input,
		std::vector<std::filesystem::path> &incdir, bool added_incdir,
		macroSymbol & defineDB,
		std::vector<std::filesystem::path> & stack_incfile, Language mode);
std::string run_verilog_preproc_str(const std::string& verilog_str,
		std::vector<std::filesystem::path> &incdir, macroSymbol & defineDB,
		std::vector<std::filesystem::path> & stack_incfile, Language mode);

std::string run_verilog_preproc_file(const std::filesystem::path & filename,
		std::vector<std::filesystem::path> &incdir, macroSymbol & defineDB,
		std::vector<std::filesystem::path> & stack_incfile, Language mode);

std::string& rtrim(std::string& str, const std::string& chars = " \t\n\r");
std::string& ltrim(std::string& str, const std::string& chars = " \t\n\r");
std::string& trim(std::string& str, const std::string& chars = " \t\n\r");

}
}
