#ifndef PARSING_ERRORS_H
#define PARSING_ERRORS_H
#include "parser/parse_terminals.hh"
#include <variant>
#include <memory>
#include "parser/parse_instructions.hh"
using common_parsing::VariantError;
using common_parsing::TerminalNoParseInfo;
using common_parsing::NotParseNode;
using common_parsing::Pos;
using common_parsing::Text;
using common_parsing::ParseNode;
struct NoParseSintaxInfo{
    struct ParseVariationSintaxError:common_parsing::VariantError<NoParseSintaxInfo>{};
    std::shared_ptr<std::variant<TerminalNoParseInfo,ParseVariationSintaxError>> error;
    NoParseSintaxInfo(TerminalNoParseInfo info){
        error=make_shared<std::variant<TerminalNoParseInfo,ParseVariationSintaxError>>(info);
    }
    NoParseSintaxInfo(ParseVariationSintaxError info){
        error=make_shared<std::variant<TerminalNoParseInfo,ParseVariationSintaxError>>(info);
    }
};
template<typename Res, typename Context>
struct NotParseToEndError{
    ParseNode<Res,Context> node;
    Pos end_parse;
};
template<typename Res, typename Context>
struct ParseSyntaxError{
    std::variant<NotParseNode<NoParseSintaxInfo>,NotParseToEndError<Res,Context>> error;
    ParseSyntaxError(NotParseNode<TerminalNoParseInfo> node):
        error{NotParseNode<NoParseSintaxInfo>{node.start,node.target,NoParseSintaxInfo{node.info}}}{}
    ParseSyntaxError(NotParseNode<NoParseSintaxInfo::ParseVariationSintaxError> node):
        error{NotParseNode<NoParseSintaxInfo>{node.start,node.target,NoParseSintaxInfo{node.info}}}{}
    ParseSyntaxError(NotParseToEndError<Res,Context> info):error{info}{};
};


#endif // PARSING_ERRORS_H
