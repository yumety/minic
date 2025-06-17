
// Generated from MiniC.g4 by ANTLR 4.12.0


#include "MiniCVisitor.h"

#include "MiniCParser.h"


using namespace antlrcpp;

using namespace antlr4;

namespace {

struct MiniCParserStaticData final {
  MiniCParserStaticData(std::vector<std::string> ruleNames,
                        std::vector<std::string> literalNames,
                        std::vector<std::string> symbolicNames)
      : ruleNames(std::move(ruleNames)), literalNames(std::move(literalNames)),
        symbolicNames(std::move(symbolicNames)),
        vocabulary(this->literalNames, this->symbolicNames) {}

  MiniCParserStaticData(const MiniCParserStaticData&) = delete;
  MiniCParserStaticData(MiniCParserStaticData&&) = delete;
  MiniCParserStaticData& operator=(const MiniCParserStaticData&) = delete;
  MiniCParserStaticData& operator=(MiniCParserStaticData&&) = delete;

  std::vector<antlr4::dfa::DFA> decisionToDFA;
  antlr4::atn::PredictionContextCache sharedContextCache;
  const std::vector<std::string> ruleNames;
  const std::vector<std::string> literalNames;
  const std::vector<std::string> symbolicNames;
  const antlr4::dfa::Vocabulary vocabulary;
  antlr4::atn::SerializedATNView serializedATN;
  std::unique_ptr<antlr4::atn::ATN> atn;
};

::antlr4::internal::OnceFlag minicParserOnceFlag;
MiniCParserStaticData *minicParserStaticData = nullptr;

void minicParserInitialize() {
  assert(minicParserStaticData == nullptr);
  auto staticData = std::make_unique<MiniCParserStaticData>(
    std::vector<std::string>{
      "compileUnit", "funcDef", "returnType", "formalParamList", "formalParam", 
      "block", "blockItemList", "blockItem", "varDecl", "basicType", "varDef", 
      "statement", "expr", "orExp", "andExp", "equalExp", "relationExp", 
      "addExp", "mulExp", "equalOp", "relationOp", "addOp", "mulOp", "unaryExp", 
      "primaryExp", "realParamList", "lVal"
    },
    std::vector<std::string>{
      "", "'('", "')'", "';'", "'['", "']'", "'{'", "'}'", "'='", "','", 
      "'+'", "'-'", "'*'", "'/'", "'%'", "'<'", "'>'", "'<='", "'>='", "'=='", 
      "'!='", "'&&'", "'||'", "'!'", "'return'", "'int'", "'void'", "'if'", 
      "'else'", "'while'", "'break'", "'continue'"
    },
    std::vector<std::string>{
      "", "T_L_PAREN", "T_R_PAREN", "T_SEMICOLON", "T_L_BRACKET", "T_R_BRACKET", 
      "T_L_BRACE", "T_R_BRACE", "T_ASSIGN", "T_COMMA", "T_ADD", "T_SUB", 
      "T_MUL", "T_DIV", "T_MOD", "T_LT", "T_GT", "T_LE", "T_GE", "T_EQ", 
      "T_NE", "T_AND", "T_OR", "T_NOT", "T_RETURN", "T_INT", "T_VOID", "T_IF", 
      "T_ELSE", "T_WHILE", "T_BREAK", "T_CONTINUE", "T_ID", "T_DIGIT", "WS", 
      "LINE_COMMENT", "BLOCK_COMMENT"
    }
  );
  static const int32_t serializedATNSegment[] = {
  	4,1,36,289,2,0,7,0,2,1,7,1,2,2,7,2,2,3,7,3,2,4,7,4,2,5,7,5,2,6,7,6,2,
  	7,7,7,2,8,7,8,2,9,7,9,2,10,7,10,2,11,7,11,2,12,7,12,2,13,7,13,2,14,7,
  	14,2,15,7,15,2,16,7,16,2,17,7,17,2,18,7,18,2,19,7,19,2,20,7,20,2,21,7,
  	21,2,22,7,22,2,23,7,23,2,24,7,24,2,25,7,25,2,26,7,26,1,0,1,0,1,0,5,0,
  	58,8,0,10,0,12,0,61,9,0,1,0,1,0,1,1,1,1,1,1,1,1,3,1,69,8,1,1,1,1,1,1,
  	1,1,2,1,2,3,2,76,8,2,1,3,1,3,1,3,1,3,1,3,1,3,5,3,84,8,3,10,3,12,3,87,
  	9,3,1,4,1,4,1,4,3,4,92,8,4,1,4,1,4,1,4,1,4,1,4,1,4,5,4,100,8,4,10,4,12,
  	4,103,9,4,3,4,105,8,4,1,5,1,5,3,5,109,8,5,1,5,1,5,1,6,4,6,114,8,6,11,
  	6,12,6,115,1,7,1,7,3,7,120,8,7,1,8,1,8,1,8,1,8,5,8,126,8,8,10,8,12,8,
  	129,9,8,1,8,1,8,1,9,1,9,1,10,1,10,1,10,3,10,138,8,10,1,10,1,10,1,10,1,
  	10,1,10,5,10,145,8,10,10,10,12,10,148,9,10,3,10,150,8,10,1,11,1,11,3,
  	11,154,8,11,1,11,1,11,1,11,1,11,1,11,1,11,1,11,1,11,3,11,164,8,11,1,11,
  	1,11,1,11,1,11,1,11,1,11,1,11,1,11,3,11,174,8,11,1,11,1,11,1,11,1,11,
  	1,11,1,11,1,11,1,11,1,11,1,11,3,11,186,8,11,1,12,1,12,1,13,1,13,1,13,
  	5,13,193,8,13,10,13,12,13,196,9,13,1,14,1,14,1,14,5,14,201,8,14,10,14,
  	12,14,204,9,14,1,15,1,15,1,15,1,15,5,15,210,8,15,10,15,12,15,213,9,15,
  	1,16,1,16,1,16,1,16,5,16,219,8,16,10,16,12,16,222,9,16,1,17,1,17,1,17,
  	1,17,5,17,228,8,17,10,17,12,17,231,9,17,1,18,1,18,1,18,1,18,5,18,237,
  	8,18,10,18,12,18,240,9,18,1,19,1,19,1,20,1,20,1,21,1,21,1,22,1,22,1,23,
  	1,23,1,23,1,23,1,23,1,23,1,23,1,23,3,23,258,8,23,1,23,3,23,261,8,23,1,
  	24,1,24,1,24,1,24,1,24,1,24,3,24,269,8,24,1,25,1,25,1,25,5,25,274,8,25,
  	10,25,12,25,277,9,25,1,26,1,26,1,26,1,26,1,26,5,26,284,8,26,10,26,12,
  	26,287,9,26,1,26,0,0,27,0,2,4,6,8,10,12,14,16,18,20,22,24,26,28,30,32,
  	34,36,38,40,42,44,46,48,50,52,0,4,1,0,19,20,1,0,15,18,1,0,10,11,1,0,12,
  	14,301,0,59,1,0,0,0,2,64,1,0,0,0,4,75,1,0,0,0,6,77,1,0,0,0,8,88,1,0,0,
  	0,10,106,1,0,0,0,12,113,1,0,0,0,14,119,1,0,0,0,16,121,1,0,0,0,18,132,
  	1,0,0,0,20,149,1,0,0,0,22,185,1,0,0,0,24,187,1,0,0,0,26,189,1,0,0,0,28,
  	197,1,0,0,0,30,205,1,0,0,0,32,214,1,0,0,0,34,223,1,0,0,0,36,232,1,0,0,
  	0,38,241,1,0,0,0,40,243,1,0,0,0,42,245,1,0,0,0,44,247,1,0,0,0,46,260,
  	1,0,0,0,48,268,1,0,0,0,50,270,1,0,0,0,52,278,1,0,0,0,54,58,3,2,1,0,55,
  	58,3,16,8,0,56,58,3,22,11,0,57,54,1,0,0,0,57,55,1,0,0,0,57,56,1,0,0,0,
  	58,61,1,0,0,0,59,57,1,0,0,0,59,60,1,0,0,0,60,62,1,0,0,0,61,59,1,0,0,0,
  	62,63,5,0,0,1,63,1,1,0,0,0,64,65,3,4,2,0,65,66,5,32,0,0,66,68,5,1,0,0,
  	67,69,3,6,3,0,68,67,1,0,0,0,68,69,1,0,0,0,69,70,1,0,0,0,70,71,5,2,0,0,
  	71,72,3,10,5,0,72,3,1,0,0,0,73,76,3,18,9,0,74,76,5,26,0,0,75,73,1,0,0,
  	0,75,74,1,0,0,0,76,5,1,0,0,0,77,78,3,18,9,0,78,85,3,8,4,0,79,80,5,9,0,
  	0,80,81,3,18,9,0,81,82,3,8,4,0,82,84,1,0,0,0,83,79,1,0,0,0,84,87,1,0,
  	0,0,85,83,1,0,0,0,85,86,1,0,0,0,86,7,1,0,0,0,87,85,1,0,0,0,88,104,5,32,
  	0,0,89,91,5,4,0,0,90,92,3,24,12,0,91,90,1,0,0,0,91,92,1,0,0,0,92,93,1,
  	0,0,0,93,94,5,5,0,0,94,101,1,0,0,0,95,96,5,4,0,0,96,97,3,24,12,0,97,98,
  	5,5,0,0,98,100,1,0,0,0,99,95,1,0,0,0,100,103,1,0,0,0,101,99,1,0,0,0,101,
  	102,1,0,0,0,102,105,1,0,0,0,103,101,1,0,0,0,104,89,1,0,0,0,104,105,1,
  	0,0,0,105,9,1,0,0,0,106,108,5,6,0,0,107,109,3,12,6,0,108,107,1,0,0,0,
  	108,109,1,0,0,0,109,110,1,0,0,0,110,111,5,7,0,0,111,11,1,0,0,0,112,114,
  	3,14,7,0,113,112,1,0,0,0,114,115,1,0,0,0,115,113,1,0,0,0,115,116,1,0,
  	0,0,116,13,1,0,0,0,117,120,3,22,11,0,118,120,3,16,8,0,119,117,1,0,0,0,
  	119,118,1,0,0,0,120,15,1,0,0,0,121,122,3,18,9,0,122,127,3,20,10,0,123,
  	124,5,9,0,0,124,126,3,20,10,0,125,123,1,0,0,0,126,129,1,0,0,0,127,125,
  	1,0,0,0,127,128,1,0,0,0,128,130,1,0,0,0,129,127,1,0,0,0,130,131,5,3,0,
  	0,131,17,1,0,0,0,132,133,5,25,0,0,133,19,1,0,0,0,134,137,5,32,0,0,135,
  	136,5,8,0,0,136,138,3,24,12,0,137,135,1,0,0,0,137,138,1,0,0,0,138,150,
  	1,0,0,0,139,146,5,32,0,0,140,141,5,4,0,0,141,142,3,24,12,0,142,143,5,
  	5,0,0,143,145,1,0,0,0,144,140,1,0,0,0,145,148,1,0,0,0,146,144,1,0,0,0,
  	146,147,1,0,0,0,147,150,1,0,0,0,148,146,1,0,0,0,149,134,1,0,0,0,149,139,
  	1,0,0,0,150,21,1,0,0,0,151,153,5,24,0,0,152,154,3,24,12,0,153,152,1,0,
  	0,0,153,154,1,0,0,0,154,155,1,0,0,0,155,186,5,3,0,0,156,157,3,52,26,0,
  	157,158,5,8,0,0,158,159,3,24,12,0,159,160,5,3,0,0,160,186,1,0,0,0,161,
  	186,3,10,5,0,162,164,3,24,12,0,163,162,1,0,0,0,163,164,1,0,0,0,164,165,
  	1,0,0,0,165,186,5,3,0,0,166,167,5,27,0,0,167,168,5,1,0,0,168,169,3,24,
  	12,0,169,170,5,2,0,0,170,173,3,22,11,0,171,172,5,28,0,0,172,174,3,22,
  	11,0,173,171,1,0,0,0,173,174,1,0,0,0,174,186,1,0,0,0,175,176,5,29,0,0,
  	176,177,5,1,0,0,177,178,3,24,12,0,178,179,5,2,0,0,179,180,3,22,11,0,180,
  	186,1,0,0,0,181,182,5,30,0,0,182,186,5,3,0,0,183,184,5,31,0,0,184,186,
  	5,3,0,0,185,151,1,0,0,0,185,156,1,0,0,0,185,161,1,0,0,0,185,163,1,0,0,
  	0,185,166,1,0,0,0,185,175,1,0,0,0,185,181,1,0,0,0,185,183,1,0,0,0,186,
  	23,1,0,0,0,187,188,3,26,13,0,188,25,1,0,0,0,189,194,3,28,14,0,190,191,
  	5,22,0,0,191,193,3,28,14,0,192,190,1,0,0,0,193,196,1,0,0,0,194,192,1,
  	0,0,0,194,195,1,0,0,0,195,27,1,0,0,0,196,194,1,0,0,0,197,202,3,30,15,
  	0,198,199,5,21,0,0,199,201,3,30,15,0,200,198,1,0,0,0,201,204,1,0,0,0,
  	202,200,1,0,0,0,202,203,1,0,0,0,203,29,1,0,0,0,204,202,1,0,0,0,205,211,
  	3,32,16,0,206,207,3,38,19,0,207,208,3,32,16,0,208,210,1,0,0,0,209,206,
  	1,0,0,0,210,213,1,0,0,0,211,209,1,0,0,0,211,212,1,0,0,0,212,31,1,0,0,
  	0,213,211,1,0,0,0,214,220,3,34,17,0,215,216,3,40,20,0,216,217,3,34,17,
  	0,217,219,1,0,0,0,218,215,1,0,0,0,219,222,1,0,0,0,220,218,1,0,0,0,220,
  	221,1,0,0,0,221,33,1,0,0,0,222,220,1,0,0,0,223,229,3,36,18,0,224,225,
  	3,42,21,0,225,226,3,36,18,0,226,228,1,0,0,0,227,224,1,0,0,0,228,231,1,
  	0,0,0,229,227,1,0,0,0,229,230,1,0,0,0,230,35,1,0,0,0,231,229,1,0,0,0,
  	232,238,3,46,23,0,233,234,3,44,22,0,234,235,3,46,23,0,235,237,1,0,0,0,
  	236,233,1,0,0,0,237,240,1,0,0,0,238,236,1,0,0,0,238,239,1,0,0,0,239,37,
  	1,0,0,0,240,238,1,0,0,0,241,242,7,0,0,0,242,39,1,0,0,0,243,244,7,1,0,
  	0,244,41,1,0,0,0,245,246,7,2,0,0,246,43,1,0,0,0,247,248,7,3,0,0,248,45,
  	1,0,0,0,249,261,3,48,24,0,250,251,5,11,0,0,251,261,3,46,23,0,252,253,
  	5,23,0,0,253,261,3,46,23,0,254,255,5,32,0,0,255,257,5,1,0,0,256,258,3,
  	50,25,0,257,256,1,0,0,0,257,258,1,0,0,0,258,259,1,0,0,0,259,261,5,2,0,
  	0,260,249,1,0,0,0,260,250,1,0,0,0,260,252,1,0,0,0,260,254,1,0,0,0,261,
  	47,1,0,0,0,262,263,5,1,0,0,263,264,3,24,12,0,264,265,5,2,0,0,265,269,
  	1,0,0,0,266,269,5,33,0,0,267,269,3,52,26,0,268,262,1,0,0,0,268,266,1,
  	0,0,0,268,267,1,0,0,0,269,49,1,0,0,0,270,275,3,24,12,0,271,272,5,9,0,
  	0,272,274,3,24,12,0,273,271,1,0,0,0,274,277,1,0,0,0,275,273,1,0,0,0,275,
  	276,1,0,0,0,276,51,1,0,0,0,277,275,1,0,0,0,278,285,5,32,0,0,279,280,5,
  	4,0,0,280,281,3,24,12,0,281,282,5,5,0,0,282,284,1,0,0,0,283,279,1,0,0,
  	0,284,287,1,0,0,0,285,283,1,0,0,0,285,286,1,0,0,0,286,53,1,0,0,0,287,
  	285,1,0,0,0,30,57,59,68,75,85,91,101,104,108,115,119,127,137,146,149,
  	153,163,173,185,194,202,211,220,229,238,257,260,268,275,285
  };
  staticData->serializedATN = antlr4::atn::SerializedATNView(serializedATNSegment, sizeof(serializedATNSegment) / sizeof(serializedATNSegment[0]));

  antlr4::atn::ATNDeserializer deserializer;
  staticData->atn = deserializer.deserialize(staticData->serializedATN);

  const size_t count = staticData->atn->getNumberOfDecisions();
  staticData->decisionToDFA.reserve(count);
  for (size_t i = 0; i < count; i++) { 
    staticData->decisionToDFA.emplace_back(staticData->atn->getDecisionState(i), i);
  }
  minicParserStaticData = staticData.release();
}

}

MiniCParser::MiniCParser(TokenStream *input) : MiniCParser(input, antlr4::atn::ParserATNSimulatorOptions()) {}

MiniCParser::MiniCParser(TokenStream *input, const antlr4::atn::ParserATNSimulatorOptions &options) : Parser(input) {
  MiniCParser::initialize();
  _interpreter = new atn::ParserATNSimulator(this, *minicParserStaticData->atn, minicParserStaticData->decisionToDFA, minicParserStaticData->sharedContextCache, options);
}

MiniCParser::~MiniCParser() {
  delete _interpreter;
}

const atn::ATN& MiniCParser::getATN() const {
  return *minicParserStaticData->atn;
}

std::string MiniCParser::getGrammarFileName() const {
  return "MiniC.g4";
}

const std::vector<std::string>& MiniCParser::getRuleNames() const {
  return minicParserStaticData->ruleNames;
}

const dfa::Vocabulary& MiniCParser::getVocabulary() const {
  return minicParserStaticData->vocabulary;
}

antlr4::atn::SerializedATNView MiniCParser::getSerializedATN() const {
  return minicParserStaticData->serializedATN;
}


//----------------- CompileUnitContext ------------------------------------------------------------------

MiniCParser::CompileUnitContext::CompileUnitContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MiniCParser::CompileUnitContext::EOF() {
  return getToken(MiniCParser::EOF, 0);
}

std::vector<MiniCParser::FuncDefContext *> MiniCParser::CompileUnitContext::funcDef() {
  return getRuleContexts<MiniCParser::FuncDefContext>();
}

MiniCParser::FuncDefContext* MiniCParser::CompileUnitContext::funcDef(size_t i) {
  return getRuleContext<MiniCParser::FuncDefContext>(i);
}

std::vector<MiniCParser::VarDeclContext *> MiniCParser::CompileUnitContext::varDecl() {
  return getRuleContexts<MiniCParser::VarDeclContext>();
}

MiniCParser::VarDeclContext* MiniCParser::CompileUnitContext::varDecl(size_t i) {
  return getRuleContext<MiniCParser::VarDeclContext>(i);
}

std::vector<MiniCParser::StatementContext *> MiniCParser::CompileUnitContext::statement() {
  return getRuleContexts<MiniCParser::StatementContext>();
}

MiniCParser::StatementContext* MiniCParser::CompileUnitContext::statement(size_t i) {
  return getRuleContext<MiniCParser::StatementContext>(i);
}


size_t MiniCParser::CompileUnitContext::getRuleIndex() const {
  return MiniCParser::RuleCompileUnit;
}


std::any MiniCParser::CompileUnitContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniCVisitor*>(visitor))
    return parserVisitor->visitCompileUnit(this);
  else
    return visitor->visitChildren(this);
}

MiniCParser::CompileUnitContext* MiniCParser::compileUnit() {
  CompileUnitContext *_localctx = _tracker.createInstance<CompileUnitContext>(_ctx, getState());
  enterRule(_localctx, 0, MiniCParser::RuleCompileUnit);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(59);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 16903047242) != 0)) {
      setState(57);
      _errHandler->sync(this);
      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 0, _ctx)) {
      case 1: {
        setState(54);
        funcDef();
        break;
      }

      case 2: {
        setState(55);
        varDecl();
        break;
      }

      case 3: {
        setState(56);
        statement();
        break;
      }

      default:
        break;
      }
      setState(61);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(62);
    match(MiniCParser::EOF);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- FuncDefContext ------------------------------------------------------------------

MiniCParser::FuncDefContext::FuncDefContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

MiniCParser::ReturnTypeContext* MiniCParser::FuncDefContext::returnType() {
  return getRuleContext<MiniCParser::ReturnTypeContext>(0);
}

tree::TerminalNode* MiniCParser::FuncDefContext::T_ID() {
  return getToken(MiniCParser::T_ID, 0);
}

tree::TerminalNode* MiniCParser::FuncDefContext::T_L_PAREN() {
  return getToken(MiniCParser::T_L_PAREN, 0);
}

tree::TerminalNode* MiniCParser::FuncDefContext::T_R_PAREN() {
  return getToken(MiniCParser::T_R_PAREN, 0);
}

MiniCParser::BlockContext* MiniCParser::FuncDefContext::block() {
  return getRuleContext<MiniCParser::BlockContext>(0);
}

MiniCParser::FormalParamListContext* MiniCParser::FuncDefContext::formalParamList() {
  return getRuleContext<MiniCParser::FormalParamListContext>(0);
}


size_t MiniCParser::FuncDefContext::getRuleIndex() const {
  return MiniCParser::RuleFuncDef;
}


std::any MiniCParser::FuncDefContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniCVisitor*>(visitor))
    return parserVisitor->visitFuncDef(this);
  else
    return visitor->visitChildren(this);
}

MiniCParser::FuncDefContext* MiniCParser::funcDef() {
  FuncDefContext *_localctx = _tracker.createInstance<FuncDefContext>(_ctx, getState());
  enterRule(_localctx, 2, MiniCParser::RuleFuncDef);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(64);
    returnType();
    setState(65);
    match(MiniCParser::T_ID);
    setState(66);
    match(MiniCParser::T_L_PAREN);
    setState(68);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == MiniCParser::T_INT) {
      setState(67);
      formalParamList();
    }
    setState(70);
    match(MiniCParser::T_R_PAREN);
    setState(71);
    block();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ReturnTypeContext ------------------------------------------------------------------

MiniCParser::ReturnTypeContext::ReturnTypeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

MiniCParser::BasicTypeContext* MiniCParser::ReturnTypeContext::basicType() {
  return getRuleContext<MiniCParser::BasicTypeContext>(0);
}

tree::TerminalNode* MiniCParser::ReturnTypeContext::T_VOID() {
  return getToken(MiniCParser::T_VOID, 0);
}


size_t MiniCParser::ReturnTypeContext::getRuleIndex() const {
  return MiniCParser::RuleReturnType;
}


std::any MiniCParser::ReturnTypeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniCVisitor*>(visitor))
    return parserVisitor->visitReturnType(this);
  else
    return visitor->visitChildren(this);
}

MiniCParser::ReturnTypeContext* MiniCParser::returnType() {
  ReturnTypeContext *_localctx = _tracker.createInstance<ReturnTypeContext>(_ctx, getState());
  enterRule(_localctx, 4, MiniCParser::RuleReturnType);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(75);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case MiniCParser::T_INT: {
        enterOuterAlt(_localctx, 1);
        setState(73);
        basicType();
        break;
      }

      case MiniCParser::T_VOID: {
        enterOuterAlt(_localctx, 2);
        setState(74);
        match(MiniCParser::T_VOID);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- FormalParamListContext ------------------------------------------------------------------

MiniCParser::FormalParamListContext::FormalParamListContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<MiniCParser::BasicTypeContext *> MiniCParser::FormalParamListContext::basicType() {
  return getRuleContexts<MiniCParser::BasicTypeContext>();
}

MiniCParser::BasicTypeContext* MiniCParser::FormalParamListContext::basicType(size_t i) {
  return getRuleContext<MiniCParser::BasicTypeContext>(i);
}

std::vector<MiniCParser::FormalParamContext *> MiniCParser::FormalParamListContext::formalParam() {
  return getRuleContexts<MiniCParser::FormalParamContext>();
}

MiniCParser::FormalParamContext* MiniCParser::FormalParamListContext::formalParam(size_t i) {
  return getRuleContext<MiniCParser::FormalParamContext>(i);
}

std::vector<tree::TerminalNode *> MiniCParser::FormalParamListContext::T_COMMA() {
  return getTokens(MiniCParser::T_COMMA);
}

tree::TerminalNode* MiniCParser::FormalParamListContext::T_COMMA(size_t i) {
  return getToken(MiniCParser::T_COMMA, i);
}


size_t MiniCParser::FormalParamListContext::getRuleIndex() const {
  return MiniCParser::RuleFormalParamList;
}


std::any MiniCParser::FormalParamListContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniCVisitor*>(visitor))
    return parserVisitor->visitFormalParamList(this);
  else
    return visitor->visitChildren(this);
}

MiniCParser::FormalParamListContext* MiniCParser::formalParamList() {
  FormalParamListContext *_localctx = _tracker.createInstance<FormalParamListContext>(_ctx, getState());
  enterRule(_localctx, 6, MiniCParser::RuleFormalParamList);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(77);
    basicType();
    setState(78);
    formalParam();
    setState(85);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == MiniCParser::T_COMMA) {
      setState(79);
      match(MiniCParser::T_COMMA);
      setState(80);
      basicType();
      setState(81);
      formalParam();
      setState(87);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- FormalParamContext ------------------------------------------------------------------

MiniCParser::FormalParamContext::FormalParamContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MiniCParser::FormalParamContext::T_ID() {
  return getToken(MiniCParser::T_ID, 0);
}

std::vector<tree::TerminalNode *> MiniCParser::FormalParamContext::T_L_BRACKET() {
  return getTokens(MiniCParser::T_L_BRACKET);
}

tree::TerminalNode* MiniCParser::FormalParamContext::T_L_BRACKET(size_t i) {
  return getToken(MiniCParser::T_L_BRACKET, i);
}

std::vector<tree::TerminalNode *> MiniCParser::FormalParamContext::T_R_BRACKET() {
  return getTokens(MiniCParser::T_R_BRACKET);
}

tree::TerminalNode* MiniCParser::FormalParamContext::T_R_BRACKET(size_t i) {
  return getToken(MiniCParser::T_R_BRACKET, i);
}

std::vector<MiniCParser::ExprContext *> MiniCParser::FormalParamContext::expr() {
  return getRuleContexts<MiniCParser::ExprContext>();
}

MiniCParser::ExprContext* MiniCParser::FormalParamContext::expr(size_t i) {
  return getRuleContext<MiniCParser::ExprContext>(i);
}


size_t MiniCParser::FormalParamContext::getRuleIndex() const {
  return MiniCParser::RuleFormalParam;
}


std::any MiniCParser::FormalParamContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniCVisitor*>(visitor))
    return parserVisitor->visitFormalParam(this);
  else
    return visitor->visitChildren(this);
}

MiniCParser::FormalParamContext* MiniCParser::formalParam() {
  FormalParamContext *_localctx = _tracker.createInstance<FormalParamContext>(_ctx, getState());
  enterRule(_localctx, 8, MiniCParser::RuleFormalParam);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(88);
    match(MiniCParser::T_ID);
    setState(104);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == MiniCParser::T_L_BRACKET) {
      setState(89);
      match(MiniCParser::T_L_BRACKET);
      setState(91);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if ((((_la & ~ 0x3fULL) == 0) &&
        ((1ULL << _la) & 12893292546) != 0)) {
        setState(90);
        expr();
      }
      setState(93);
      match(MiniCParser::T_R_BRACKET);
      setState(101);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == MiniCParser::T_L_BRACKET) {
        setState(95);
        match(MiniCParser::T_L_BRACKET);
        setState(96);
        expr();
        setState(97);
        match(MiniCParser::T_R_BRACKET);
        setState(103);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- BlockContext ------------------------------------------------------------------

MiniCParser::BlockContext::BlockContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MiniCParser::BlockContext::T_L_BRACE() {
  return getToken(MiniCParser::T_L_BRACE, 0);
}

tree::TerminalNode* MiniCParser::BlockContext::T_R_BRACE() {
  return getToken(MiniCParser::T_R_BRACE, 0);
}

MiniCParser::BlockItemListContext* MiniCParser::BlockContext::blockItemList() {
  return getRuleContext<MiniCParser::BlockItemListContext>(0);
}


size_t MiniCParser::BlockContext::getRuleIndex() const {
  return MiniCParser::RuleBlock;
}


std::any MiniCParser::BlockContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniCVisitor*>(visitor))
    return parserVisitor->visitBlock(this);
  else
    return visitor->visitChildren(this);
}

MiniCParser::BlockContext* MiniCParser::block() {
  BlockContext *_localctx = _tracker.createInstance<BlockContext>(_ctx, getState());
  enterRule(_localctx, 10, MiniCParser::RuleBlock);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(106);
    match(MiniCParser::T_L_BRACE);
    setState(108);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 16835938378) != 0)) {
      setState(107);
      blockItemList();
    }
    setState(110);
    match(MiniCParser::T_R_BRACE);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- BlockItemListContext ------------------------------------------------------------------

MiniCParser::BlockItemListContext::BlockItemListContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<MiniCParser::BlockItemContext *> MiniCParser::BlockItemListContext::blockItem() {
  return getRuleContexts<MiniCParser::BlockItemContext>();
}

MiniCParser::BlockItemContext* MiniCParser::BlockItemListContext::blockItem(size_t i) {
  return getRuleContext<MiniCParser::BlockItemContext>(i);
}


size_t MiniCParser::BlockItemListContext::getRuleIndex() const {
  return MiniCParser::RuleBlockItemList;
}


std::any MiniCParser::BlockItemListContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniCVisitor*>(visitor))
    return parserVisitor->visitBlockItemList(this);
  else
    return visitor->visitChildren(this);
}

MiniCParser::BlockItemListContext* MiniCParser::blockItemList() {
  BlockItemListContext *_localctx = _tracker.createInstance<BlockItemListContext>(_ctx, getState());
  enterRule(_localctx, 12, MiniCParser::RuleBlockItemList);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(113); 
    _errHandler->sync(this);
    _la = _input->LA(1);
    do {
      setState(112);
      blockItem();
      setState(115); 
      _errHandler->sync(this);
      _la = _input->LA(1);
    } while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 16835938378) != 0));
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- BlockItemContext ------------------------------------------------------------------

MiniCParser::BlockItemContext::BlockItemContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

MiniCParser::StatementContext* MiniCParser::BlockItemContext::statement() {
  return getRuleContext<MiniCParser::StatementContext>(0);
}

MiniCParser::VarDeclContext* MiniCParser::BlockItemContext::varDecl() {
  return getRuleContext<MiniCParser::VarDeclContext>(0);
}


size_t MiniCParser::BlockItemContext::getRuleIndex() const {
  return MiniCParser::RuleBlockItem;
}


std::any MiniCParser::BlockItemContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniCVisitor*>(visitor))
    return parserVisitor->visitBlockItem(this);
  else
    return visitor->visitChildren(this);
}

MiniCParser::BlockItemContext* MiniCParser::blockItem() {
  BlockItemContext *_localctx = _tracker.createInstance<BlockItemContext>(_ctx, getState());
  enterRule(_localctx, 14, MiniCParser::RuleBlockItem);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(119);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case MiniCParser::T_L_PAREN:
      case MiniCParser::T_SEMICOLON:
      case MiniCParser::T_L_BRACE:
      case MiniCParser::T_SUB:
      case MiniCParser::T_NOT:
      case MiniCParser::T_RETURN:
      case MiniCParser::T_IF:
      case MiniCParser::T_WHILE:
      case MiniCParser::T_BREAK:
      case MiniCParser::T_CONTINUE:
      case MiniCParser::T_ID:
      case MiniCParser::T_DIGIT: {
        enterOuterAlt(_localctx, 1);
        setState(117);
        statement();
        break;
      }

      case MiniCParser::T_INT: {
        enterOuterAlt(_localctx, 2);
        setState(118);
        varDecl();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- VarDeclContext ------------------------------------------------------------------

MiniCParser::VarDeclContext::VarDeclContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

MiniCParser::BasicTypeContext* MiniCParser::VarDeclContext::basicType() {
  return getRuleContext<MiniCParser::BasicTypeContext>(0);
}

std::vector<MiniCParser::VarDefContext *> MiniCParser::VarDeclContext::varDef() {
  return getRuleContexts<MiniCParser::VarDefContext>();
}

MiniCParser::VarDefContext* MiniCParser::VarDeclContext::varDef(size_t i) {
  return getRuleContext<MiniCParser::VarDefContext>(i);
}

tree::TerminalNode* MiniCParser::VarDeclContext::T_SEMICOLON() {
  return getToken(MiniCParser::T_SEMICOLON, 0);
}

std::vector<tree::TerminalNode *> MiniCParser::VarDeclContext::T_COMMA() {
  return getTokens(MiniCParser::T_COMMA);
}

tree::TerminalNode* MiniCParser::VarDeclContext::T_COMMA(size_t i) {
  return getToken(MiniCParser::T_COMMA, i);
}


size_t MiniCParser::VarDeclContext::getRuleIndex() const {
  return MiniCParser::RuleVarDecl;
}


std::any MiniCParser::VarDeclContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniCVisitor*>(visitor))
    return parserVisitor->visitVarDecl(this);
  else
    return visitor->visitChildren(this);
}

MiniCParser::VarDeclContext* MiniCParser::varDecl() {
  VarDeclContext *_localctx = _tracker.createInstance<VarDeclContext>(_ctx, getState());
  enterRule(_localctx, 16, MiniCParser::RuleVarDecl);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(121);
    basicType();
    setState(122);
    varDef();
    setState(127);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == MiniCParser::T_COMMA) {
      setState(123);
      match(MiniCParser::T_COMMA);
      setState(124);
      varDef();
      setState(129);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(130);
    match(MiniCParser::T_SEMICOLON);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- BasicTypeContext ------------------------------------------------------------------

MiniCParser::BasicTypeContext::BasicTypeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MiniCParser::BasicTypeContext::T_INT() {
  return getToken(MiniCParser::T_INT, 0);
}


size_t MiniCParser::BasicTypeContext::getRuleIndex() const {
  return MiniCParser::RuleBasicType;
}


std::any MiniCParser::BasicTypeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniCVisitor*>(visitor))
    return parserVisitor->visitBasicType(this);
  else
    return visitor->visitChildren(this);
}

MiniCParser::BasicTypeContext* MiniCParser::basicType() {
  BasicTypeContext *_localctx = _tracker.createInstance<BasicTypeContext>(_ctx, getState());
  enterRule(_localctx, 18, MiniCParser::RuleBasicType);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(132);
    match(MiniCParser::T_INT);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- VarDefContext ------------------------------------------------------------------

MiniCParser::VarDefContext::VarDefContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MiniCParser::VarDefContext::T_ID() {
  return getToken(MiniCParser::T_ID, 0);
}

tree::TerminalNode* MiniCParser::VarDefContext::T_ASSIGN() {
  return getToken(MiniCParser::T_ASSIGN, 0);
}

std::vector<MiniCParser::ExprContext *> MiniCParser::VarDefContext::expr() {
  return getRuleContexts<MiniCParser::ExprContext>();
}

MiniCParser::ExprContext* MiniCParser::VarDefContext::expr(size_t i) {
  return getRuleContext<MiniCParser::ExprContext>(i);
}

std::vector<tree::TerminalNode *> MiniCParser::VarDefContext::T_L_BRACKET() {
  return getTokens(MiniCParser::T_L_BRACKET);
}

tree::TerminalNode* MiniCParser::VarDefContext::T_L_BRACKET(size_t i) {
  return getToken(MiniCParser::T_L_BRACKET, i);
}

std::vector<tree::TerminalNode *> MiniCParser::VarDefContext::T_R_BRACKET() {
  return getTokens(MiniCParser::T_R_BRACKET);
}

tree::TerminalNode* MiniCParser::VarDefContext::T_R_BRACKET(size_t i) {
  return getToken(MiniCParser::T_R_BRACKET, i);
}


size_t MiniCParser::VarDefContext::getRuleIndex() const {
  return MiniCParser::RuleVarDef;
}


std::any MiniCParser::VarDefContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniCVisitor*>(visitor))
    return parserVisitor->visitVarDef(this);
  else
    return visitor->visitChildren(this);
}

MiniCParser::VarDefContext* MiniCParser::varDef() {
  VarDefContext *_localctx = _tracker.createInstance<VarDefContext>(_ctx, getState());
  enterRule(_localctx, 20, MiniCParser::RuleVarDef);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(149);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 14, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(134);
      match(MiniCParser::T_ID);
      setState(137);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == MiniCParser::T_ASSIGN) {
        setState(135);
        match(MiniCParser::T_ASSIGN);
        setState(136);
        expr();
      }
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(139);
      match(MiniCParser::T_ID);
      setState(146);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == MiniCParser::T_L_BRACKET) {
        setState(140);
        match(MiniCParser::T_L_BRACKET);
        setState(141);
        expr();
        setState(142);
        match(MiniCParser::T_R_BRACKET);
        setState(148);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      break;
    }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- StatementContext ------------------------------------------------------------------

MiniCParser::StatementContext::StatementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t MiniCParser::StatementContext::getRuleIndex() const {
  return MiniCParser::RuleStatement;
}

void MiniCParser::StatementContext::copyFrom(StatementContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- WhileStatementContext ------------------------------------------------------------------

tree::TerminalNode* MiniCParser::WhileStatementContext::T_WHILE() {
  return getToken(MiniCParser::T_WHILE, 0);
}

tree::TerminalNode* MiniCParser::WhileStatementContext::T_L_PAREN() {
  return getToken(MiniCParser::T_L_PAREN, 0);
}

MiniCParser::ExprContext* MiniCParser::WhileStatementContext::expr() {
  return getRuleContext<MiniCParser::ExprContext>(0);
}

tree::TerminalNode* MiniCParser::WhileStatementContext::T_R_PAREN() {
  return getToken(MiniCParser::T_R_PAREN, 0);
}

MiniCParser::StatementContext* MiniCParser::WhileStatementContext::statement() {
  return getRuleContext<MiniCParser::StatementContext>(0);
}

MiniCParser::WhileStatementContext::WhileStatementContext(StatementContext *ctx) { copyFrom(ctx); }


std::any MiniCParser::WhileStatementContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniCVisitor*>(visitor))
    return parserVisitor->visitWhileStatement(this);
  else
    return visitor->visitChildren(this);
}
//----------------- BlockStatementContext ------------------------------------------------------------------

MiniCParser::BlockContext* MiniCParser::BlockStatementContext::block() {
  return getRuleContext<MiniCParser::BlockContext>(0);
}

MiniCParser::BlockStatementContext::BlockStatementContext(StatementContext *ctx) { copyFrom(ctx); }


std::any MiniCParser::BlockStatementContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniCVisitor*>(visitor))
    return parserVisitor->visitBlockStatement(this);
  else
    return visitor->visitChildren(this);
}
//----------------- AssignStatementContext ------------------------------------------------------------------

MiniCParser::LValContext* MiniCParser::AssignStatementContext::lVal() {
  return getRuleContext<MiniCParser::LValContext>(0);
}

tree::TerminalNode* MiniCParser::AssignStatementContext::T_ASSIGN() {
  return getToken(MiniCParser::T_ASSIGN, 0);
}

MiniCParser::ExprContext* MiniCParser::AssignStatementContext::expr() {
  return getRuleContext<MiniCParser::ExprContext>(0);
}

tree::TerminalNode* MiniCParser::AssignStatementContext::T_SEMICOLON() {
  return getToken(MiniCParser::T_SEMICOLON, 0);
}

MiniCParser::AssignStatementContext::AssignStatementContext(StatementContext *ctx) { copyFrom(ctx); }


std::any MiniCParser::AssignStatementContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniCVisitor*>(visitor))
    return parserVisitor->visitAssignStatement(this);
  else
    return visitor->visitChildren(this);
}
//----------------- BreakStatementContext ------------------------------------------------------------------

tree::TerminalNode* MiniCParser::BreakStatementContext::T_BREAK() {
  return getToken(MiniCParser::T_BREAK, 0);
}

tree::TerminalNode* MiniCParser::BreakStatementContext::T_SEMICOLON() {
  return getToken(MiniCParser::T_SEMICOLON, 0);
}

MiniCParser::BreakStatementContext::BreakStatementContext(StatementContext *ctx) { copyFrom(ctx); }


std::any MiniCParser::BreakStatementContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniCVisitor*>(visitor))
    return parserVisitor->visitBreakStatement(this);
  else
    return visitor->visitChildren(this);
}
//----------------- ExpressionStatementContext ------------------------------------------------------------------

tree::TerminalNode* MiniCParser::ExpressionStatementContext::T_SEMICOLON() {
  return getToken(MiniCParser::T_SEMICOLON, 0);
}

MiniCParser::ExprContext* MiniCParser::ExpressionStatementContext::expr() {
  return getRuleContext<MiniCParser::ExprContext>(0);
}

MiniCParser::ExpressionStatementContext::ExpressionStatementContext(StatementContext *ctx) { copyFrom(ctx); }


std::any MiniCParser::ExpressionStatementContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniCVisitor*>(visitor))
    return parserVisitor->visitExpressionStatement(this);
  else
    return visitor->visitChildren(this);
}
//----------------- ContinueStatementContext ------------------------------------------------------------------

tree::TerminalNode* MiniCParser::ContinueStatementContext::T_CONTINUE() {
  return getToken(MiniCParser::T_CONTINUE, 0);
}

tree::TerminalNode* MiniCParser::ContinueStatementContext::T_SEMICOLON() {
  return getToken(MiniCParser::T_SEMICOLON, 0);
}

MiniCParser::ContinueStatementContext::ContinueStatementContext(StatementContext *ctx) { copyFrom(ctx); }


std::any MiniCParser::ContinueStatementContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniCVisitor*>(visitor))
    return parserVisitor->visitContinueStatement(this);
  else
    return visitor->visitChildren(this);
}
//----------------- ReturnStatementContext ------------------------------------------------------------------

tree::TerminalNode* MiniCParser::ReturnStatementContext::T_RETURN() {
  return getToken(MiniCParser::T_RETURN, 0);
}

tree::TerminalNode* MiniCParser::ReturnStatementContext::T_SEMICOLON() {
  return getToken(MiniCParser::T_SEMICOLON, 0);
}

MiniCParser::ExprContext* MiniCParser::ReturnStatementContext::expr() {
  return getRuleContext<MiniCParser::ExprContext>(0);
}

MiniCParser::ReturnStatementContext::ReturnStatementContext(StatementContext *ctx) { copyFrom(ctx); }


std::any MiniCParser::ReturnStatementContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniCVisitor*>(visitor))
    return parserVisitor->visitReturnStatement(this);
  else
    return visitor->visitChildren(this);
}
//----------------- IfStatementContext ------------------------------------------------------------------

tree::TerminalNode* MiniCParser::IfStatementContext::T_IF() {
  return getToken(MiniCParser::T_IF, 0);
}

tree::TerminalNode* MiniCParser::IfStatementContext::T_L_PAREN() {
  return getToken(MiniCParser::T_L_PAREN, 0);
}

MiniCParser::ExprContext* MiniCParser::IfStatementContext::expr() {
  return getRuleContext<MiniCParser::ExprContext>(0);
}

tree::TerminalNode* MiniCParser::IfStatementContext::T_R_PAREN() {
  return getToken(MiniCParser::T_R_PAREN, 0);
}

std::vector<MiniCParser::StatementContext *> MiniCParser::IfStatementContext::statement() {
  return getRuleContexts<MiniCParser::StatementContext>();
}

MiniCParser::StatementContext* MiniCParser::IfStatementContext::statement(size_t i) {
  return getRuleContext<MiniCParser::StatementContext>(i);
}

tree::TerminalNode* MiniCParser::IfStatementContext::T_ELSE() {
  return getToken(MiniCParser::T_ELSE, 0);
}

MiniCParser::IfStatementContext::IfStatementContext(StatementContext *ctx) { copyFrom(ctx); }


std::any MiniCParser::IfStatementContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniCVisitor*>(visitor))
    return parserVisitor->visitIfStatement(this);
  else
    return visitor->visitChildren(this);
}
MiniCParser::StatementContext* MiniCParser::statement() {
  StatementContext *_localctx = _tracker.createInstance<StatementContext>(_ctx, getState());
  enterRule(_localctx, 22, MiniCParser::RuleStatement);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(185);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 18, _ctx)) {
    case 1: {
      _localctx = _tracker.createInstance<MiniCParser::ReturnStatementContext>(_localctx);
      enterOuterAlt(_localctx, 1);
      setState(151);
      match(MiniCParser::T_RETURN);
      setState(153);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if ((((_la & ~ 0x3fULL) == 0) &&
        ((1ULL << _la) & 12893292546) != 0)) {
        setState(152);
        expr();
      }
      setState(155);
      match(MiniCParser::T_SEMICOLON);
      break;
    }

    case 2: {
      _localctx = _tracker.createInstance<MiniCParser::AssignStatementContext>(_localctx);
      enterOuterAlt(_localctx, 2);
      setState(156);
      lVal();
      setState(157);
      match(MiniCParser::T_ASSIGN);
      setState(158);
      expr();
      setState(159);
      match(MiniCParser::T_SEMICOLON);
      break;
    }

    case 3: {
      _localctx = _tracker.createInstance<MiniCParser::BlockStatementContext>(_localctx);
      enterOuterAlt(_localctx, 3);
      setState(161);
      block();
      break;
    }

    case 4: {
      _localctx = _tracker.createInstance<MiniCParser::ExpressionStatementContext>(_localctx);
      enterOuterAlt(_localctx, 4);
      setState(163);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if ((((_la & ~ 0x3fULL) == 0) &&
        ((1ULL << _la) & 12893292546) != 0)) {
        setState(162);
        expr();
      }
      setState(165);
      match(MiniCParser::T_SEMICOLON);
      break;
    }

    case 5: {
      _localctx = _tracker.createInstance<MiniCParser::IfStatementContext>(_localctx);
      enterOuterAlt(_localctx, 5);
      setState(166);
      match(MiniCParser::T_IF);
      setState(167);
      match(MiniCParser::T_L_PAREN);
      setState(168);
      expr();
      setState(169);
      match(MiniCParser::T_R_PAREN);
      setState(170);
      statement();
      setState(173);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 17, _ctx)) {
      case 1: {
        setState(171);
        match(MiniCParser::T_ELSE);
        setState(172);
        statement();
        break;
      }

      default:
        break;
      }
      break;
    }

    case 6: {
      _localctx = _tracker.createInstance<MiniCParser::WhileStatementContext>(_localctx);
      enterOuterAlt(_localctx, 6);
      setState(175);
      match(MiniCParser::T_WHILE);
      setState(176);
      match(MiniCParser::T_L_PAREN);
      setState(177);
      expr();
      setState(178);
      match(MiniCParser::T_R_PAREN);
      setState(179);
      statement();
      break;
    }

    case 7: {
      _localctx = _tracker.createInstance<MiniCParser::BreakStatementContext>(_localctx);
      enterOuterAlt(_localctx, 7);
      setState(181);
      match(MiniCParser::T_BREAK);
      setState(182);
      match(MiniCParser::T_SEMICOLON);
      break;
    }

    case 8: {
      _localctx = _tracker.createInstance<MiniCParser::ContinueStatementContext>(_localctx);
      enterOuterAlt(_localctx, 8);
      setState(183);
      match(MiniCParser::T_CONTINUE);
      setState(184);
      match(MiniCParser::T_SEMICOLON);
      break;
    }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ExprContext ------------------------------------------------------------------

MiniCParser::ExprContext::ExprContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

MiniCParser::OrExpContext* MiniCParser::ExprContext::orExp() {
  return getRuleContext<MiniCParser::OrExpContext>(0);
}


size_t MiniCParser::ExprContext::getRuleIndex() const {
  return MiniCParser::RuleExpr;
}


std::any MiniCParser::ExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniCVisitor*>(visitor))
    return parserVisitor->visitExpr(this);
  else
    return visitor->visitChildren(this);
}

MiniCParser::ExprContext* MiniCParser::expr() {
  ExprContext *_localctx = _tracker.createInstance<ExprContext>(_ctx, getState());
  enterRule(_localctx, 24, MiniCParser::RuleExpr);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(187);
    orExp();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- OrExpContext ------------------------------------------------------------------

MiniCParser::OrExpContext::OrExpContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<MiniCParser::AndExpContext *> MiniCParser::OrExpContext::andExp() {
  return getRuleContexts<MiniCParser::AndExpContext>();
}

MiniCParser::AndExpContext* MiniCParser::OrExpContext::andExp(size_t i) {
  return getRuleContext<MiniCParser::AndExpContext>(i);
}

std::vector<tree::TerminalNode *> MiniCParser::OrExpContext::T_OR() {
  return getTokens(MiniCParser::T_OR);
}

tree::TerminalNode* MiniCParser::OrExpContext::T_OR(size_t i) {
  return getToken(MiniCParser::T_OR, i);
}


size_t MiniCParser::OrExpContext::getRuleIndex() const {
  return MiniCParser::RuleOrExp;
}


std::any MiniCParser::OrExpContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniCVisitor*>(visitor))
    return parserVisitor->visitOrExp(this);
  else
    return visitor->visitChildren(this);
}

MiniCParser::OrExpContext* MiniCParser::orExp() {
  OrExpContext *_localctx = _tracker.createInstance<OrExpContext>(_ctx, getState());
  enterRule(_localctx, 26, MiniCParser::RuleOrExp);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(189);
    andExp();
    setState(194);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == MiniCParser::T_OR) {
      setState(190);
      match(MiniCParser::T_OR);
      setState(191);
      andExp();
      setState(196);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- AndExpContext ------------------------------------------------------------------

MiniCParser::AndExpContext::AndExpContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<MiniCParser::EqualExpContext *> MiniCParser::AndExpContext::equalExp() {
  return getRuleContexts<MiniCParser::EqualExpContext>();
}

MiniCParser::EqualExpContext* MiniCParser::AndExpContext::equalExp(size_t i) {
  return getRuleContext<MiniCParser::EqualExpContext>(i);
}

std::vector<tree::TerminalNode *> MiniCParser::AndExpContext::T_AND() {
  return getTokens(MiniCParser::T_AND);
}

tree::TerminalNode* MiniCParser::AndExpContext::T_AND(size_t i) {
  return getToken(MiniCParser::T_AND, i);
}


size_t MiniCParser::AndExpContext::getRuleIndex() const {
  return MiniCParser::RuleAndExp;
}


std::any MiniCParser::AndExpContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniCVisitor*>(visitor))
    return parserVisitor->visitAndExp(this);
  else
    return visitor->visitChildren(this);
}

MiniCParser::AndExpContext* MiniCParser::andExp() {
  AndExpContext *_localctx = _tracker.createInstance<AndExpContext>(_ctx, getState());
  enterRule(_localctx, 28, MiniCParser::RuleAndExp);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(197);
    equalExp();
    setState(202);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == MiniCParser::T_AND) {
      setState(198);
      match(MiniCParser::T_AND);
      setState(199);
      equalExp();
      setState(204);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- EqualExpContext ------------------------------------------------------------------

MiniCParser::EqualExpContext::EqualExpContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<MiniCParser::RelationExpContext *> MiniCParser::EqualExpContext::relationExp() {
  return getRuleContexts<MiniCParser::RelationExpContext>();
}

MiniCParser::RelationExpContext* MiniCParser::EqualExpContext::relationExp(size_t i) {
  return getRuleContext<MiniCParser::RelationExpContext>(i);
}

std::vector<MiniCParser::EqualOpContext *> MiniCParser::EqualExpContext::equalOp() {
  return getRuleContexts<MiniCParser::EqualOpContext>();
}

MiniCParser::EqualOpContext* MiniCParser::EqualExpContext::equalOp(size_t i) {
  return getRuleContext<MiniCParser::EqualOpContext>(i);
}


size_t MiniCParser::EqualExpContext::getRuleIndex() const {
  return MiniCParser::RuleEqualExp;
}


std::any MiniCParser::EqualExpContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniCVisitor*>(visitor))
    return parserVisitor->visitEqualExp(this);
  else
    return visitor->visitChildren(this);
}

MiniCParser::EqualExpContext* MiniCParser::equalExp() {
  EqualExpContext *_localctx = _tracker.createInstance<EqualExpContext>(_ctx, getState());
  enterRule(_localctx, 30, MiniCParser::RuleEqualExp);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(205);
    relationExp();
    setState(211);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == MiniCParser::T_EQ

    || _la == MiniCParser::T_NE) {
      setState(206);
      equalOp();
      setState(207);
      relationExp();
      setState(213);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- RelationExpContext ------------------------------------------------------------------

MiniCParser::RelationExpContext::RelationExpContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<MiniCParser::AddExpContext *> MiniCParser::RelationExpContext::addExp() {
  return getRuleContexts<MiniCParser::AddExpContext>();
}

MiniCParser::AddExpContext* MiniCParser::RelationExpContext::addExp(size_t i) {
  return getRuleContext<MiniCParser::AddExpContext>(i);
}

std::vector<MiniCParser::RelationOpContext *> MiniCParser::RelationExpContext::relationOp() {
  return getRuleContexts<MiniCParser::RelationOpContext>();
}

MiniCParser::RelationOpContext* MiniCParser::RelationExpContext::relationOp(size_t i) {
  return getRuleContext<MiniCParser::RelationOpContext>(i);
}


size_t MiniCParser::RelationExpContext::getRuleIndex() const {
  return MiniCParser::RuleRelationExp;
}


std::any MiniCParser::RelationExpContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniCVisitor*>(visitor))
    return parserVisitor->visitRelationExp(this);
  else
    return visitor->visitChildren(this);
}

MiniCParser::RelationExpContext* MiniCParser::relationExp() {
  RelationExpContext *_localctx = _tracker.createInstance<RelationExpContext>(_ctx, getState());
  enterRule(_localctx, 32, MiniCParser::RuleRelationExp);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(214);
    addExp();
    setState(220);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 491520) != 0)) {
      setState(215);
      relationOp();
      setState(216);
      addExp();
      setState(222);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- AddExpContext ------------------------------------------------------------------

MiniCParser::AddExpContext::AddExpContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<MiniCParser::MulExpContext *> MiniCParser::AddExpContext::mulExp() {
  return getRuleContexts<MiniCParser::MulExpContext>();
}

MiniCParser::MulExpContext* MiniCParser::AddExpContext::mulExp(size_t i) {
  return getRuleContext<MiniCParser::MulExpContext>(i);
}

std::vector<MiniCParser::AddOpContext *> MiniCParser::AddExpContext::addOp() {
  return getRuleContexts<MiniCParser::AddOpContext>();
}

MiniCParser::AddOpContext* MiniCParser::AddExpContext::addOp(size_t i) {
  return getRuleContext<MiniCParser::AddOpContext>(i);
}


size_t MiniCParser::AddExpContext::getRuleIndex() const {
  return MiniCParser::RuleAddExp;
}


std::any MiniCParser::AddExpContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniCVisitor*>(visitor))
    return parserVisitor->visitAddExp(this);
  else
    return visitor->visitChildren(this);
}

MiniCParser::AddExpContext* MiniCParser::addExp() {
  AddExpContext *_localctx = _tracker.createInstance<AddExpContext>(_ctx, getState());
  enterRule(_localctx, 34, MiniCParser::RuleAddExp);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(223);
    mulExp();
    setState(229);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == MiniCParser::T_ADD

    || _la == MiniCParser::T_SUB) {
      setState(224);
      addOp();
      setState(225);
      mulExp();
      setState(231);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- MulExpContext ------------------------------------------------------------------

MiniCParser::MulExpContext::MulExpContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<MiniCParser::UnaryExpContext *> MiniCParser::MulExpContext::unaryExp() {
  return getRuleContexts<MiniCParser::UnaryExpContext>();
}

MiniCParser::UnaryExpContext* MiniCParser::MulExpContext::unaryExp(size_t i) {
  return getRuleContext<MiniCParser::UnaryExpContext>(i);
}

std::vector<MiniCParser::MulOpContext *> MiniCParser::MulExpContext::mulOp() {
  return getRuleContexts<MiniCParser::MulOpContext>();
}

MiniCParser::MulOpContext* MiniCParser::MulExpContext::mulOp(size_t i) {
  return getRuleContext<MiniCParser::MulOpContext>(i);
}


size_t MiniCParser::MulExpContext::getRuleIndex() const {
  return MiniCParser::RuleMulExp;
}


std::any MiniCParser::MulExpContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniCVisitor*>(visitor))
    return parserVisitor->visitMulExp(this);
  else
    return visitor->visitChildren(this);
}

MiniCParser::MulExpContext* MiniCParser::mulExp() {
  MulExpContext *_localctx = _tracker.createInstance<MulExpContext>(_ctx, getState());
  enterRule(_localctx, 36, MiniCParser::RuleMulExp);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(232);
    unaryExp();
    setState(238);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 28672) != 0)) {
      setState(233);
      mulOp();
      setState(234);
      unaryExp();
      setState(240);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- EqualOpContext ------------------------------------------------------------------

MiniCParser::EqualOpContext::EqualOpContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MiniCParser::EqualOpContext::T_EQ() {
  return getToken(MiniCParser::T_EQ, 0);
}

tree::TerminalNode* MiniCParser::EqualOpContext::T_NE() {
  return getToken(MiniCParser::T_NE, 0);
}


size_t MiniCParser::EqualOpContext::getRuleIndex() const {
  return MiniCParser::RuleEqualOp;
}


std::any MiniCParser::EqualOpContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniCVisitor*>(visitor))
    return parserVisitor->visitEqualOp(this);
  else
    return visitor->visitChildren(this);
}

MiniCParser::EqualOpContext* MiniCParser::equalOp() {
  EqualOpContext *_localctx = _tracker.createInstance<EqualOpContext>(_ctx, getState());
  enterRule(_localctx, 38, MiniCParser::RuleEqualOp);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(241);
    _la = _input->LA(1);
    if (!(_la == MiniCParser::T_EQ

    || _la == MiniCParser::T_NE)) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- RelationOpContext ------------------------------------------------------------------

MiniCParser::RelationOpContext::RelationOpContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MiniCParser::RelationOpContext::T_LT() {
  return getToken(MiniCParser::T_LT, 0);
}

tree::TerminalNode* MiniCParser::RelationOpContext::T_GT() {
  return getToken(MiniCParser::T_GT, 0);
}

tree::TerminalNode* MiniCParser::RelationOpContext::T_LE() {
  return getToken(MiniCParser::T_LE, 0);
}

tree::TerminalNode* MiniCParser::RelationOpContext::T_GE() {
  return getToken(MiniCParser::T_GE, 0);
}


size_t MiniCParser::RelationOpContext::getRuleIndex() const {
  return MiniCParser::RuleRelationOp;
}


std::any MiniCParser::RelationOpContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniCVisitor*>(visitor))
    return parserVisitor->visitRelationOp(this);
  else
    return visitor->visitChildren(this);
}

MiniCParser::RelationOpContext* MiniCParser::relationOp() {
  RelationOpContext *_localctx = _tracker.createInstance<RelationOpContext>(_ctx, getState());
  enterRule(_localctx, 40, MiniCParser::RuleRelationOp);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(243);
    _la = _input->LA(1);
    if (!((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 491520) != 0))) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- AddOpContext ------------------------------------------------------------------

MiniCParser::AddOpContext::AddOpContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MiniCParser::AddOpContext::T_ADD() {
  return getToken(MiniCParser::T_ADD, 0);
}

tree::TerminalNode* MiniCParser::AddOpContext::T_SUB() {
  return getToken(MiniCParser::T_SUB, 0);
}


size_t MiniCParser::AddOpContext::getRuleIndex() const {
  return MiniCParser::RuleAddOp;
}


std::any MiniCParser::AddOpContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniCVisitor*>(visitor))
    return parserVisitor->visitAddOp(this);
  else
    return visitor->visitChildren(this);
}

MiniCParser::AddOpContext* MiniCParser::addOp() {
  AddOpContext *_localctx = _tracker.createInstance<AddOpContext>(_ctx, getState());
  enterRule(_localctx, 42, MiniCParser::RuleAddOp);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(245);
    _la = _input->LA(1);
    if (!(_la == MiniCParser::T_ADD

    || _la == MiniCParser::T_SUB)) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- MulOpContext ------------------------------------------------------------------

MiniCParser::MulOpContext::MulOpContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MiniCParser::MulOpContext::T_MUL() {
  return getToken(MiniCParser::T_MUL, 0);
}

tree::TerminalNode* MiniCParser::MulOpContext::T_DIV() {
  return getToken(MiniCParser::T_DIV, 0);
}

tree::TerminalNode* MiniCParser::MulOpContext::T_MOD() {
  return getToken(MiniCParser::T_MOD, 0);
}


size_t MiniCParser::MulOpContext::getRuleIndex() const {
  return MiniCParser::RuleMulOp;
}


std::any MiniCParser::MulOpContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniCVisitor*>(visitor))
    return parserVisitor->visitMulOp(this);
  else
    return visitor->visitChildren(this);
}

MiniCParser::MulOpContext* MiniCParser::mulOp() {
  MulOpContext *_localctx = _tracker.createInstance<MulOpContext>(_ctx, getState());
  enterRule(_localctx, 44, MiniCParser::RuleMulOp);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(247);
    _la = _input->LA(1);
    if (!((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 28672) != 0))) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- UnaryExpContext ------------------------------------------------------------------

MiniCParser::UnaryExpContext::UnaryExpContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

MiniCParser::PrimaryExpContext* MiniCParser::UnaryExpContext::primaryExp() {
  return getRuleContext<MiniCParser::PrimaryExpContext>(0);
}

tree::TerminalNode* MiniCParser::UnaryExpContext::T_SUB() {
  return getToken(MiniCParser::T_SUB, 0);
}

MiniCParser::UnaryExpContext* MiniCParser::UnaryExpContext::unaryExp() {
  return getRuleContext<MiniCParser::UnaryExpContext>(0);
}

tree::TerminalNode* MiniCParser::UnaryExpContext::T_NOT() {
  return getToken(MiniCParser::T_NOT, 0);
}

tree::TerminalNode* MiniCParser::UnaryExpContext::T_ID() {
  return getToken(MiniCParser::T_ID, 0);
}

tree::TerminalNode* MiniCParser::UnaryExpContext::T_L_PAREN() {
  return getToken(MiniCParser::T_L_PAREN, 0);
}

tree::TerminalNode* MiniCParser::UnaryExpContext::T_R_PAREN() {
  return getToken(MiniCParser::T_R_PAREN, 0);
}

MiniCParser::RealParamListContext* MiniCParser::UnaryExpContext::realParamList() {
  return getRuleContext<MiniCParser::RealParamListContext>(0);
}


size_t MiniCParser::UnaryExpContext::getRuleIndex() const {
  return MiniCParser::RuleUnaryExp;
}


std::any MiniCParser::UnaryExpContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniCVisitor*>(visitor))
    return parserVisitor->visitUnaryExp(this);
  else
    return visitor->visitChildren(this);
}

MiniCParser::UnaryExpContext* MiniCParser::unaryExp() {
  UnaryExpContext *_localctx = _tracker.createInstance<UnaryExpContext>(_ctx, getState());
  enterRule(_localctx, 46, MiniCParser::RuleUnaryExp);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(260);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 26, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(249);
      primaryExp();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(250);
      match(MiniCParser::T_SUB);
      setState(251);
      unaryExp();
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(252);
      match(MiniCParser::T_NOT);
      setState(253);
      unaryExp();
      break;
    }

    case 4: {
      enterOuterAlt(_localctx, 4);
      setState(254);
      match(MiniCParser::T_ID);
      setState(255);
      match(MiniCParser::T_L_PAREN);
      setState(257);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if ((((_la & ~ 0x3fULL) == 0) &&
        ((1ULL << _la) & 12893292546) != 0)) {
        setState(256);
        realParamList();
      }
      setState(259);
      match(MiniCParser::T_R_PAREN);
      break;
    }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- PrimaryExpContext ------------------------------------------------------------------

MiniCParser::PrimaryExpContext::PrimaryExpContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MiniCParser::PrimaryExpContext::T_L_PAREN() {
  return getToken(MiniCParser::T_L_PAREN, 0);
}

MiniCParser::ExprContext* MiniCParser::PrimaryExpContext::expr() {
  return getRuleContext<MiniCParser::ExprContext>(0);
}

tree::TerminalNode* MiniCParser::PrimaryExpContext::T_R_PAREN() {
  return getToken(MiniCParser::T_R_PAREN, 0);
}

tree::TerminalNode* MiniCParser::PrimaryExpContext::T_DIGIT() {
  return getToken(MiniCParser::T_DIGIT, 0);
}

MiniCParser::LValContext* MiniCParser::PrimaryExpContext::lVal() {
  return getRuleContext<MiniCParser::LValContext>(0);
}


size_t MiniCParser::PrimaryExpContext::getRuleIndex() const {
  return MiniCParser::RulePrimaryExp;
}


std::any MiniCParser::PrimaryExpContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniCVisitor*>(visitor))
    return parserVisitor->visitPrimaryExp(this);
  else
    return visitor->visitChildren(this);
}

MiniCParser::PrimaryExpContext* MiniCParser::primaryExp() {
  PrimaryExpContext *_localctx = _tracker.createInstance<PrimaryExpContext>(_ctx, getState());
  enterRule(_localctx, 48, MiniCParser::RulePrimaryExp);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(268);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case MiniCParser::T_L_PAREN: {
        enterOuterAlt(_localctx, 1);
        setState(262);
        match(MiniCParser::T_L_PAREN);
        setState(263);
        expr();
        setState(264);
        match(MiniCParser::T_R_PAREN);
        break;
      }

      case MiniCParser::T_DIGIT: {
        enterOuterAlt(_localctx, 2);
        setState(266);
        match(MiniCParser::T_DIGIT);
        break;
      }

      case MiniCParser::T_ID: {
        enterOuterAlt(_localctx, 3);
        setState(267);
        lVal();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- RealParamListContext ------------------------------------------------------------------

MiniCParser::RealParamListContext::RealParamListContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<MiniCParser::ExprContext *> MiniCParser::RealParamListContext::expr() {
  return getRuleContexts<MiniCParser::ExprContext>();
}

MiniCParser::ExprContext* MiniCParser::RealParamListContext::expr(size_t i) {
  return getRuleContext<MiniCParser::ExprContext>(i);
}

std::vector<tree::TerminalNode *> MiniCParser::RealParamListContext::T_COMMA() {
  return getTokens(MiniCParser::T_COMMA);
}

tree::TerminalNode* MiniCParser::RealParamListContext::T_COMMA(size_t i) {
  return getToken(MiniCParser::T_COMMA, i);
}


size_t MiniCParser::RealParamListContext::getRuleIndex() const {
  return MiniCParser::RuleRealParamList;
}


std::any MiniCParser::RealParamListContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniCVisitor*>(visitor))
    return parserVisitor->visitRealParamList(this);
  else
    return visitor->visitChildren(this);
}

MiniCParser::RealParamListContext* MiniCParser::realParamList() {
  RealParamListContext *_localctx = _tracker.createInstance<RealParamListContext>(_ctx, getState());
  enterRule(_localctx, 50, MiniCParser::RuleRealParamList);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(270);
    expr();
    setState(275);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == MiniCParser::T_COMMA) {
      setState(271);
      match(MiniCParser::T_COMMA);
      setState(272);
      expr();
      setState(277);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- LValContext ------------------------------------------------------------------

MiniCParser::LValContext::LValContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MiniCParser::LValContext::T_ID() {
  return getToken(MiniCParser::T_ID, 0);
}

std::vector<tree::TerminalNode *> MiniCParser::LValContext::T_L_BRACKET() {
  return getTokens(MiniCParser::T_L_BRACKET);
}

tree::TerminalNode* MiniCParser::LValContext::T_L_BRACKET(size_t i) {
  return getToken(MiniCParser::T_L_BRACKET, i);
}

std::vector<MiniCParser::ExprContext *> MiniCParser::LValContext::expr() {
  return getRuleContexts<MiniCParser::ExprContext>();
}

MiniCParser::ExprContext* MiniCParser::LValContext::expr(size_t i) {
  return getRuleContext<MiniCParser::ExprContext>(i);
}

std::vector<tree::TerminalNode *> MiniCParser::LValContext::T_R_BRACKET() {
  return getTokens(MiniCParser::T_R_BRACKET);
}

tree::TerminalNode* MiniCParser::LValContext::T_R_BRACKET(size_t i) {
  return getToken(MiniCParser::T_R_BRACKET, i);
}


size_t MiniCParser::LValContext::getRuleIndex() const {
  return MiniCParser::RuleLVal;
}


std::any MiniCParser::LValContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniCVisitor*>(visitor))
    return parserVisitor->visitLVal(this);
  else
    return visitor->visitChildren(this);
}

MiniCParser::LValContext* MiniCParser::lVal() {
  LValContext *_localctx = _tracker.createInstance<LValContext>(_ctx, getState());
  enterRule(_localctx, 52, MiniCParser::RuleLVal);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(278);
    match(MiniCParser::T_ID);
    setState(285);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == MiniCParser::T_L_BRACKET) {
      setState(279);
      match(MiniCParser::T_L_BRACKET);
      setState(280);
      expr();
      setState(281);
      match(MiniCParser::T_R_BRACKET);
      setState(287);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

void MiniCParser::initialize() {
  ::antlr4::internal::call_once(minicParserOnceFlag, minicParserInitialize);
}
