#ifndef PARSE_ERRORS_VER_2_H
#define PARSE_ERRORS_VER_2_H
#include <string>
#include "parser/parse_instructions.hh"
#include "parser/parse_terminals.hh"
#include <variant>
#include <memory>
using common_parsing::VariantError;
using common_parsing::TerminalNoParseInfo;
using common_parsing::NotParseNode;
using common_parsing::Pos;
using common_parsing::Text;
using common_parsing::ParseNode;
using common_parsing::Maybe;
using std::string;
namespace concrete_parsing_ver_2 {
struct NoParseSintaxInfo{
    typedef std::variant<
        NotParseNode<NoParseSintaxInfo>,
        NotParseNode<VariantError<NoParseSintaxInfo>>,
        NotParseNode<TerminalNoParseInfo>,
        NoParseSintaxInfo,
        VariantError<NoParseSintaxInfo>,
        TerminalNoParseInfo
        > InnerType;
    string info;
    Maybe<
        std::shared_ptr<
            InnerType
        >
    >
        inner;
    NoParseSintaxInfo();
    NoParseSintaxInfo(string message);
    NoParseSintaxInfo(NotParseNode<NoParseSintaxInfo> node);
    NoParseSintaxInfo(NotParseNode<VariantError<NoParseSintaxInfo>> node);
    NoParseSintaxInfo(NotParseNode<TerminalNoParseInfo> node);
    NoParseSintaxInfo(VariantError<NoParseSintaxInfo> error);
    NoParseSintaxInfo(TerminalNoParseInfo info);
    static NoParseSintaxInfo from(NoParseSintaxInfo info);
    virtual string short_message();
    virtual string long_message();
    template<typename T>
    static NotParseNode<NoParseSintaxInfo> wrap(string message, NotParseNode<T> node){
        NoParseSintaxInfo info{message};
        info.inner=Maybe{make_shared<InnerType>(node)};
        return NotParseNode<T>{node.start, node.target, info};
    }
};
}
#endif // PARSE_ERRORS_VER_2_H
