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

ParseResult<None,None,TerminalNoParseInfo> do_parse_terminal(Text target, Pos start, Text terminal);
Parse<None,None,TerminalNoParseInfo> parse_terminal(Text terminal);
Parse<None,None,TerminalNoParseInfo> parse_terminal(string s);
static const outparse skip_spaces=outparse_rep(
		outparse_variants({wrap_skip(parse_terminal(make_unique<string>(" "))),wrap_skip(parse_terminal(make_unique<string>("\t")))})
		);
outparse skip_seq(vector<outparse> seq,outparse skip=skip_spaces);
Parse<None,None,TerminalNoParseInfo> wrap_skip_parse_tarminal(string s);
}
#endif /* PARSE_TERMINALS_HH_ */
