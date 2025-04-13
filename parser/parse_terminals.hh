/*
 * parse_terminals.hh
 *
 *  Created on: Mar 31, 2025
 *      Author: aleksandr
 */

#ifndef PARSE_TERMINALS_HH_
#define PARSE_TERMINALS_HH_

#include <vector>
#include "function_parse_architect.hh"
#include "outparse.hh"
namespace common_parsing{
struct TerminalNoParseInfo{
	Text terminal;
};

ParseResult<None,None,TerminalNoParseInfo> do_parse_terminal(Text target, Pos start, Text terminal){
	if(target->substr(start,terminal->length())==*terminal){
		return ParseNode<None,None>{start,start+terminal->length(),target,[](None){return None{};}};
	}else{
		return NotParseNode<TerminalNoParseInfo>{start,target,TerminalNoParseInfo{terminal}};
	}
}
Parse<None,None,TerminalNoParseInfo> parse_terminal(Text terminal){
	return [=](Text target, Pos start){
		return do_parse_terminal(target,start,terminal);
	};
}
Parse<None,None,TerminalNoParseInfo> parse_terminal(string s){
    return parse_terminal(make_shared<string>(s));
}
static const outparse skip_spaces=outparse_rep(
		outparse_variants({wrap_skip(parse_terminal(make_unique<string>(" "))),wrap_skip(parse_terminal(make_unique<string>("\t")))})
		);
outparse skip_seq(vector<outparse> seq,outparse skip=skip_spaces){
    vector<outparse> sp_seq;
    for(auto i:seq){
        sp_seq.push_back(skip);
        sp_seq.push_back(i);
        sp_seq.push_back(skip);
    }
    return outparse_seq(sp_seq);
}
}
#endif /* PARSE_TERMINALS_HH_ */
