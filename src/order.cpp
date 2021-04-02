#include "order.h"

#include <pgen/default.h>

namespace pgen
{

void order_t::load(grammar_t &grammar)
{
	ORDER = grammar.rules.size();
	grammar.rules.push_back(rule_t(ORDER, "order::order", true, true));
	_ = grammar.rules.size();
	grammar.rules.push_back(rule_t(_, "order::_", false, true));
	RESOURCE = grammar.rules.size();
	grammar.rules.push_back(rule_t(RESOURCE, "order::resource", true, true));
	VARIABLE = grammar.rules.size();
	grammar.rules.push_back(rule_t(VARIABLE, "order::variable", true, true));
	SELECT = grammar.rules.size();
	grammar.rules.push_back(rule_t(SELECT, "order::select", true, true));
	CONSTRAINT = grammar.rules.size();
	grammar.rules.push_back(rule_t(CONSTRAINT, "order::constraint", true, true));
	TASK = grammar.rules.size();
	grammar.rules.push_back(rule_t(TASK, "order::task", true, true));
	EXPRESSION = grammar.rules.size();
	grammar.rules.push_back(rule_t(EXPRESSION, "order::expression", true, true));
	INSTANCE = grammar.rules.size();
	grammar.rules.push_back(rule_t(INSTANCE, "order::instance", true, true));
	TEXT = grammar.rules.size();
	grammar.rules.push_back(rule_t(TEXT, "order::text", true, true));
	EFFECT = grammar.rules.size();
	grammar.rules.push_back(rule_t(EFFECT, "order::effect", true, true));
	EXPRESSION0 = grammar.rules.size();
	grammar.rules.push_back(rule_t(EXPRESSION0, "order::expression0", true, true));
	EXPRESSION1 = grammar.rules.size();
	grammar.rules.push_back(rule_t(EXPRESSION1, "order::expression1", true, true));
	EXPRESSION2 = grammar.rules.size();
	grammar.rules.push_back(rule_t(EXPRESSION2, "order::expression2", true, true));
	EXPRESSION3 = grammar.rules.size();
	grammar.rules.push_back(rule_t(EXPRESSION3, "order::expression3", true, true));
	EXPRESSION4 = grammar.rules.size();
	grammar.rules.push_back(rule_t(EXPRESSION4, "order::expression4", true, true));
	EXPRESSION5 = grammar.rules.size();
	grammar.rules.push_back(rule_t(EXPRESSION5, "order::expression5", true, true));
	INTEGER = grammar.rules.size();
	grammar.rules.push_back(rule_t(INTEGER, "order::integer", true, true));
	FUNCTION = grammar.rules.size();
	grammar.rules.push_back(rule_t(FUNCTION, "order::function", true, true));

	symbol_t *n[139];
	n[0] = grammar.insert(new regular_expression("[ \\t\\n\\r]*", true));
	n[1] = grammar.insert(new regular_expression("[_a-zA-Z][_a-zA-Z0-9]*", true));
	n[2] = grammar.insert(new regular_expression("\\\"([^\\\"\\\\]|\\\\.)*\\\"", true));
	n[3] = grammar.insert(new regular_expression("[0-9]+|\\+inf", true));
	n[4] = grammar.insert(new regular_expression("\\)", true));
	n[5] = grammar.insert(new stem(1, true));
	n[6] = grammar.insert(new stem(7, true));
	n[7] = grammar.insert(new stem(1, true));
	n[8] = grammar.insert(new regular_expression(",", true));
	n[9] = grammar.insert(new stem(1, true));
	n[10] = grammar.insert(new stem(7, true));
	n[11] = grammar.insert(new stem(1, true));
	n[12] = grammar.insert(new regular_expression("\\(", true));
	n[13] = grammar.insert(new stem(1, true));
	n[14] = grammar.insert(new regular_expression("RelativeSum", true));
	n[15] = grammar.insert(new regular_expression("\\)", true));
	n[16] = grammar.insert(new stem(1, true));
	n[17] = grammar.insert(new stem(7, true));
	n[18] = grammar.insert(new stem(1, true));
	n[19] = grammar.insert(new regular_expression("\\(", true));
	n[20] = grammar.insert(new stem(1, true));
	n[21] = grammar.insert(new regular_expression("ParallelSum", true));
	n[22] = grammar.insert(new regular_expression("\\)", true));
	n[23] = grammar.insert(new stem(1, true));
	n[24] = grammar.insert(new stem(7, true));
	n[25] = grammar.insert(new stem(1, true));
	n[26] = grammar.insert(new regular_expression("\\(", true));
	n[27] = grammar.insert(new stem(1, true));
	n[28] = grammar.insert(new regular_expression("Sum", true));
	n[29] = grammar.insert(new stem(8, true));
	n[30] = grammar.insert(new stem(18, true));
	n[31] = grammar.insert(new stem(17, true));
	n[32] = grammar.insert(new regular_expression("\\)", false));
	n[33] = grammar.insert(new stem(1, true));
	n[34] = grammar.insert(new stem(7, true));
	n[35] = grammar.insert(new stem(1, true));
	n[36] = grammar.insert(new regular_expression("\\(", false));
	n[37] = grammar.insert(new stem(16, true));
	n[38] = grammar.insert(new stem(1, true));
	n[39] = grammar.insert(new regular_expression("-", true));
	n[40] = grammar.insert(new stem(15, true));
	n[41] = grammar.insert(new stem(1, true));
	n[42] = grammar.insert(new regular_expression("not", true));
	n[43] = grammar.insert(new stem(14, true));
	n[44] = grammar.insert(new stem(1, true));
	n[45] = grammar.insert(new regular_expression("%", true));
	n[46] = grammar.insert(new regular_expression("/", true));
	n[47] = grammar.insert(new regular_expression("\\*", true));
	n[48] = grammar.insert(new stem(1, true));
	n[49] = grammar.insert(new stem(14, true));
	n[50] = grammar.insert(new stem(13, true));
	n[51] = grammar.insert(new stem(1, true));
	n[52] = grammar.insert(new regular_expression("-", true));
	n[53] = grammar.insert(new regular_expression("\\+", true));
	n[54] = grammar.insert(new stem(1, true));
	n[55] = grammar.insert(new stem(13, true));
	n[56] = grammar.insert(new stem(12, true));
	n[57] = grammar.insert(new stem(1, true));
	n[58] = grammar.insert(new regular_expression("!=", true));
	n[59] = grammar.insert(new regular_expression("==", true));
	n[60] = grammar.insert(new regular_expression(">=", true));
	n[61] = grammar.insert(new regular_expression("<=", true));
	n[62] = grammar.insert(new regular_expression(">", true));
	n[63] = grammar.insert(new regular_expression("<", true));
	n[64] = grammar.insert(new stem(1, true));
	n[65] = grammar.insert(new stem(12, true));
	n[66] = grammar.insert(new stem(11, true));
	n[67] = grammar.insert(new stem(1, true));
	n[68] = grammar.insert(new regular_expression("or", true));
	n[69] = grammar.insert(new regular_expression("and", true));
	n[70] = grammar.insert(new stem(1, true));
	n[71] = grammar.insert(new stem(11, true));
	n[72] = grammar.insert(new regular_expression(";", false));
	n[73] = grammar.insert(new stem(1, true));
	n[74] = grammar.insert(new stem(8, true));
	n[75] = grammar.insert(new stem(1, true));
	n[76] = grammar.insert(new stem(7, true));
	n[77] = grammar.insert(new stem(1, true));
	n[78] = grammar.insert(new regular_expression(",", false));
	n[79] = grammar.insert(new stem(1, true));
	n[80] = grammar.insert(new stem(8, true));
	n[81] = grammar.insert(new stem(1, true));
	n[82] = grammar.insert(new stem(7, true));
	n[83] = grammar.insert(new stem(1, true));
	n[84] = grammar.insert(new regular_expression("produces", true));
	n[85] = grammar.insert(new regular_expression("consumes", true));
	n[86] = grammar.insert(new regular_expression("locks", true));
	n[87] = grammar.insert(new regular_expression("uses", true));
	n[88] = grammar.insert(new regular_expression("}", false));
	n[89] = grammar.insert(new stem(1, true));
	n[90] = grammar.insert(new stem(10, true));
	n[91] = grammar.insert(new stem(1, true));
	n[92] = grammar.insert(new regular_expression("{", false));
	n[93] = grammar.insert(new stem(1, true));
	n[94] = grammar.insert(new stem(9, true));
	n[95] = grammar.insert(new stem(1, true));
	n[96] = grammar.insert(new regular_expression("task", false));
	n[97] = grammar.insert(new regular_expression(";", false));
	n[98] = grammar.insert(new stem(1, true));
	n[99] = grammar.insert(new stem(7, true));
	n[100] = grammar.insert(new stem(1, true));
	n[101] = grammar.insert(new regular_expression("keep", false));
	n[102] = grammar.insert(new regular_expression(";", false));
	n[103] = grammar.insert(new stem(1, true));
	n[104] = grammar.insert(new stem(7, true));
	n[105] = grammar.insert(new stem(1, true));
	n[106] = grammar.insert(new regular_expression("max", true));
	n[107] = grammar.insert(new regular_expression("min", true));
	n[108] = grammar.insert(new regular_expression(";", false));
	n[109] = grammar.insert(new stem(1, true));
	n[110] = grammar.insert(new stem(7, true));
	n[111] = grammar.insert(new stem(1, true));
	n[112] = grammar.insert(new regular_expression("=", false));
	n[113] = grammar.insert(new stem(1, true));
	n[114] = grammar.insert(new stem(8, true));
	n[115] = grammar.insert(new stem(1, true));
	n[116] = grammar.insert(new regular_expression("let", false));
	n[117] = grammar.insert(new regular_expression(";", false));
	n[118] = grammar.insert(new stem(1, true));
	n[119] = grammar.insert(new stem(8, true));
	n[120] = grammar.insert(new stem(1, true));
	n[121] = grammar.insert(new stem(7, true));
	n[122] = grammar.insert(new stem(1, true));
	n[123] = grammar.insert(new regular_expression(",", false));
	n[124] = grammar.insert(new stem(1, true));
	n[125] = grammar.insert(new stem(8, true));
	n[126] = grammar.insert(new stem(1, true));
	n[127] = grammar.insert(new stem(7, true));
	n[128] = grammar.insert(new stem(1, true));
	n[129] = grammar.insert(new regular_expression("need", true));
	n[130] = grammar.insert(new regular_expression("have", true));
	n[131] = grammar.insert(new regular_expression("\\0", false));
	n[132] = grammar.insert(new stem(1, true));
	n[133] = grammar.insert(new stem(6, true));
	n[134] = grammar.insert(new stem(5, true));
	n[135] = grammar.insert(new stem(4, true));
	n[136] = grammar.insert(new stem(3, true));
	n[137] = grammar.insert(new stem(2, true));
	n[138] = grammar.insert(new stem(1, true));

	n[0]->next.push_back(NULL);
	n[1]->next.push_back(NULL);
	n[2]->next.push_back(NULL);
	n[3]->next.push_back(NULL);
	n[4]->next.push_back(NULL);
	n[5]->next.push_back(n[4]);
	n[6]->next.push_back(n[5]);
	n[7]->next.push_back(n[6]);
	n[8]->next.push_back(n[7]);
	n[9]->next.push_back(n[8]);
	n[10]->next.push_back(n[9]);
	n[11]->next.push_back(n[10]);
	n[12]->next.push_back(n[11]);
	n[13]->next.push_back(n[12]);
	n[14]->next.push_back(n[13]);
	n[15]->next.push_back(NULL);
	n[16]->next.push_back(n[15]);
	n[17]->next.push_back(n[16]);
	n[18]->next.push_back(n[17]);
	n[19]->next.push_back(n[18]);
	n[20]->next.push_back(n[19]);
	n[21]->next.push_back(n[20]);
	n[22]->next.push_back(NULL);
	n[23]->next.push_back(n[22]);
	n[24]->next.push_back(n[23]);
	n[25]->next.push_back(n[24]);
	n[26]->next.push_back(n[25]);
	n[27]->next.push_back(n[26]);
	n[28]->next.push_back(n[27]);
	n[29]->next.push_back(NULL);
	n[30]->next.push_back(NULL);
	n[31]->next.push_back(NULL);
	n[32]->next.push_back(NULL);
	n[33]->next.push_back(n[32]);
	n[34]->next.push_back(n[33]);
	n[35]->next.push_back(n[34]);
	n[36]->next.push_back(n[35]);
	n[37]->next.push_back(NULL);
	n[38]->next.push_back(n[37]);
	n[39]->next.push_back(n[38]);
	n[40]->next.push_back(NULL);
	n[41]->next.push_back(n[40]);
	n[42]->next.push_back(n[41]);
	n[43]->next.push_back(n[48]);
	n[43]->next.push_back(NULL);
	n[44]->next.push_back(n[43]);
	n[45]->next.push_back(n[44]);
	n[46]->next.push_back(n[44]);
	n[47]->next.push_back(n[44]);
	n[48]->next.push_back(n[47]);
	n[48]->next.push_back(n[46]);
	n[48]->next.push_back(n[45]);
	n[49]->next.push_back(n[48]);
	n[49]->next.push_back(NULL);
	n[50]->next.push_back(n[54]);
	n[50]->next.push_back(NULL);
	n[51]->next.push_back(n[50]);
	n[52]->next.push_back(n[51]);
	n[53]->next.push_back(n[51]);
	n[54]->next.push_back(n[53]);
	n[54]->next.push_back(n[52]);
	n[55]->next.push_back(n[54]);
	n[55]->next.push_back(NULL);
	n[56]->next.push_back(NULL);
	n[57]->next.push_back(n[56]);
	n[58]->next.push_back(n[57]);
	n[59]->next.push_back(n[57]);
	n[60]->next.push_back(n[57]);
	n[61]->next.push_back(n[57]);
	n[62]->next.push_back(n[57]);
	n[63]->next.push_back(n[57]);
	n[64]->next.push_back(n[63]);
	n[64]->next.push_back(n[62]);
	n[64]->next.push_back(n[61]);
	n[64]->next.push_back(n[60]);
	n[64]->next.push_back(n[59]);
	n[64]->next.push_back(n[58]);
	n[65]->next.push_back(n[64]);
	n[65]->next.push_back(NULL);
	n[66]->next.push_back(n[70]);
	n[66]->next.push_back(NULL);
	n[67]->next.push_back(n[66]);
	n[68]->next.push_back(n[67]);
	n[69]->next.push_back(n[67]);
	n[70]->next.push_back(n[69]);
	n[70]->next.push_back(n[68]);
	n[71]->next.push_back(n[70]);
	n[71]->next.push_back(NULL);
	n[72]->next.push_back(NULL);
	n[73]->next.push_back(n[72]);
	n[74]->next.push_back(n[79]);
	n[74]->next.push_back(n[73]);
	n[75]->next.push_back(n[74]);
	n[76]->next.push_back(n[75]);
	n[77]->next.push_back(n[76]);
	n[78]->next.push_back(n[77]);
	n[79]->next.push_back(n[78]);
	n[80]->next.push_back(n[79]);
	n[80]->next.push_back(n[73]);
	n[81]->next.push_back(n[80]);
	n[82]->next.push_back(n[81]);
	n[83]->next.push_back(n[82]);
	n[84]->next.push_back(n[83]);
	n[85]->next.push_back(n[83]);
	n[86]->next.push_back(n[83]);
	n[87]->next.push_back(n[83]);
	n[88]->next.push_back(NULL);
	n[89]->next.push_back(n[88]);
	n[90]->next.push_back(n[91]);
	n[90]->next.push_back(n[89]);
	n[91]->next.push_back(n[90]);
	n[92]->next.push_back(n[91]);
	n[93]->next.push_back(n[92]);
	n[94]->next.push_back(n[93]);
	n[95]->next.push_back(n[94]);
	n[96]->next.push_back(n[95]);
	n[97]->next.push_back(NULL);
	n[98]->next.push_back(n[97]);
	n[99]->next.push_back(n[98]);
	n[100]->next.push_back(n[99]);
	n[101]->next.push_back(n[100]);
	n[102]->next.push_back(NULL);
	n[103]->next.push_back(n[102]);
	n[104]->next.push_back(n[103]);
	n[105]->next.push_back(n[104]);
	n[106]->next.push_back(n[105]);
	n[107]->next.push_back(n[105]);
	n[108]->next.push_back(NULL);
	n[109]->next.push_back(n[108]);
	n[110]->next.push_back(n[109]);
	n[111]->next.push_back(n[110]);
	n[112]->next.push_back(n[111]);
	n[113]->next.push_back(n[112]);
	n[114]->next.push_back(n[113]);
	n[115]->next.push_back(n[114]);
	n[116]->next.push_back(n[115]);
	n[117]->next.push_back(NULL);
	n[118]->next.push_back(n[117]);
	n[119]->next.push_back(n[124]);
	n[119]->next.push_back(n[118]);
	n[120]->next.push_back(n[119]);
	n[121]->next.push_back(n[120]);
	n[122]->next.push_back(n[121]);
	n[123]->next.push_back(n[122]);
	n[124]->next.push_back(n[123]);
	n[125]->next.push_back(n[124]);
	n[125]->next.push_back(n[118]);
	n[126]->next.push_back(n[125]);
	n[127]->next.push_back(n[126]);
	n[128]->next.push_back(n[127]);
	n[129]->next.push_back(n[128]);
	n[130]->next.push_back(n[128]);
	n[131]->next.push_back(NULL);
	n[132]->next.push_back(n[131]);
	n[133]->next.push_back(n[138]);
	n[133]->next.push_back(n[132]);
	n[134]->next.push_back(n[138]);
	n[134]->next.push_back(n[132]);
	n[135]->next.push_back(n[138]);
	n[135]->next.push_back(n[132]);
	n[136]->next.push_back(n[138]);
	n[136]->next.push_back(n[132]);
	n[137]->next.push_back(n[138]);
	n[137]->next.push_back(n[132]);
	n[138]->next.push_back(n[137]);
	n[138]->next.push_back(n[136]);
	n[138]->next.push_back(n[135]);
	n[138]->next.push_back(n[134]);
	n[138]->next.push_back(n[133]);

	grammar.rules[0].start.push_back(n[138]);
	grammar.rules[1].start.push_back(n[0]);
	grammar.rules[2].start.push_back(n[130]);
	grammar.rules[2].start.push_back(n[129]);
	grammar.rules[3].start.push_back(n[116]);
	grammar.rules[4].start.push_back(n[107]);
	grammar.rules[4].start.push_back(n[106]);
	grammar.rules[5].start.push_back(n[101]);
	grammar.rules[6].start.push_back(n[96]);
	grammar.rules[7].start.push_back(n[71]);
	grammar.rules[8].start.push_back(n[1]);
	grammar.rules[9].start.push_back(n[2]);
	grammar.rules[10].start.push_back(n[87]);
	grammar.rules[10].start.push_back(n[86]);
	grammar.rules[10].start.push_back(n[85]);
	grammar.rules[10].start.push_back(n[84]);
	grammar.rules[11].start.push_back(n[65]);
	grammar.rules[12].start.push_back(n[55]);
	grammar.rules[13].start.push_back(n[49]);
	grammar.rules[14].start.push_back(n[42]);
	grammar.rules[14].start.push_back(n[40]);
	grammar.rules[15].start.push_back(n[39]);
	grammar.rules[15].start.push_back(n[37]);
	grammar.rules[16].start.push_back(n[36]);
	grammar.rules[16].start.push_back(n[31]);
	grammar.rules[16].start.push_back(n[30]);
	grammar.rules[16].start.push_back(n[29]);
	grammar.rules[17].start.push_back(n[3]);
	grammar.rules[18].start.push_back(n[28]);
	grammar.rules[18].start.push_back(n[21]);
	grammar.rules[18].start.push_back(n[14]);
}

}

