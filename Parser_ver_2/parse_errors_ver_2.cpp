#include "parse_errors_ver_2.h"
namespace concrete_parsing_ver_2{

NoParseSintaxInfo::NoParseSintaxInfo()
{
    info="<no concrete info about this error>";
}

NoParseSintaxInfo::NoParseSintaxInfo(string message)
{
    info=message;
}

NoParseSintaxInfo::NoParseSintaxInfo(NotParseNode<NoParseSintaxInfo> node)
{
    info="can not parse node";
    inner=Maybe{make_shared<InnerType>(node)};
}

NoParseSintaxInfo::NoParseSintaxInfo(NotParseNode<VariantError<NoParseSintaxInfo> > node)
{
    info="can not parse variation node";
    inner=Maybe{make_shared<InnerType>(node)};
}

NoParseSintaxInfo::NoParseSintaxInfo(NotParseNode<TerminalNoParseInfo> node)
{
    info="can not parse terminal node";
    inner=Maybe{make_shared<InnerType>(node)};
}

NoParseSintaxInfo::NoParseSintaxInfo(VariantError<NoParseSintaxInfo> error)
{
    info="can not parse veriation";
    inner=Maybe{make_shared<InnerType>(error)};
}

NoParseSintaxInfo::NoParseSintaxInfo(TerminalNoParseInfo error)
{
    info="can not parse terminal";
    inner=Maybe{make_shared<InnerType>(error)};
}

NoParseSintaxInfo NoParseSintaxInfo::from(NoParseSintaxInfo error)
{
    NoParseSintaxInfo res;
    res.info="cant parse bacause of error";
    res.inner=Maybe{make_shared<InnerType>(error)};
    return res;
}

string NoParseSintaxInfo::short_message()
{
    return info;
}

string NoParseSintaxInfo::long_message()
{
    throw "я пока не сделал";
}

}
