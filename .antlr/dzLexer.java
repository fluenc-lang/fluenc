// Generated from /home/znurre/Program/Develop/C++/dz/dz.g4 by ANTLR 4.8
import org.antlr.v4.runtime.Lexer;
import org.antlr.v4.runtime.CharStream;
import org.antlr.v4.runtime.Token;
import org.antlr.v4.runtime.TokenStream;
import org.antlr.v4.runtime.*;
import org.antlr.v4.runtime.atn.*;
import org.antlr.v4.runtime.dfa.DFA;
import org.antlr.v4.runtime.misc.*;

@SuppressWarnings({"all", "warnings", "unchecked", "unused", "cast"})
public class dzLexer extends Lexer {
	static { RuntimeMetaData.checkVersion("4.8", RuntimeMetaData.VERSION); }

	protected static final DFA[] _decisionToDFA;
	protected static final PredictionContextCache _sharedContextCache =
		new PredictionContextCache();
	public static final int
		T__0=1, T__1=2, T__2=3, T__3=4, T__4=5, T__5=6, T__6=7, T__7=8, INT=9, 
		OP=10, WS=11, ID=12;
	public static String[] channelNames = {
		"DEFAULT_TOKEN_CHANNEL", "HIDDEN"
	};

	public static String[] modeNames = {
		"DEFAULT_MODE"
	};

	private static String[] makeRuleNames() {
		return new String[] {
			"T__0", "T__1", "T__2", "T__3", "T__4", "T__5", "T__6", "T__7", "INT", 
			"OP", "WS", "ID", "CHAR", "DIGIT"
		};
	}
	public static final String[] ruleNames = makeRuleNames();

	private static String[] makeLiteralNames() {
		return new String[] {
			null, "'('", "','", "')'", "'return'", "'='", "'if'", "'{'", "'}'"
		};
	}
	private static final String[] _LITERAL_NAMES = makeLiteralNames();
	private static String[] makeSymbolicNames() {
		return new String[] {
			null, null, null, null, null, null, null, null, null, "INT", "OP", "WS", 
			"ID"
		};
	}
	private static final String[] _SYMBOLIC_NAMES = makeSymbolicNames();
	public static final Vocabulary VOCABULARY = new VocabularyImpl(_LITERAL_NAMES, _SYMBOLIC_NAMES);

	/**
	 * @deprecated Use {@link #VOCABULARY} instead.
	 */
	@Deprecated
	public static final String[] tokenNames;
	static {
		tokenNames = new String[_SYMBOLIC_NAMES.length];
		for (int i = 0; i < tokenNames.length; i++) {
			tokenNames[i] = VOCABULARY.getLiteralName(i);
			if (tokenNames[i] == null) {
				tokenNames[i] = VOCABULARY.getSymbolicName(i);
			}

			if (tokenNames[i] == null) {
				tokenNames[i] = "<INVALID>";
			}
		}
	}

	@Override
	@Deprecated
	public String[] getTokenNames() {
		return tokenNames;
	}

	@Override

	public Vocabulary getVocabulary() {
		return VOCABULARY;
	}


	public dzLexer(CharStream input) {
		super(input);
		_interp = new LexerATNSimulator(this,_ATN,_decisionToDFA,_sharedContextCache);
	}

	@Override
	public String getGrammarFileName() { return "dz.g4"; }

	@Override
	public String[] getRuleNames() { return ruleNames; }

	@Override
	public String getSerializedATN() { return _serializedATN; }

	@Override
	public String[] getChannelNames() { return channelNames; }

	@Override
	public String[] getModeNames() { return modeNames; }

	@Override
	public ATN getATN() { return _ATN; }

	public static final String _serializedATN =
		"\3\u608b\ua72a\u8133\ub9ed\u417c\u3be7\u7786\u5964\2\16W\b\1\4\2\t\2\4"+
		"\3\t\3\4\4\t\4\4\5\t\5\4\6\t\6\4\7\t\7\4\b\t\b\4\t\t\t\4\n\t\n\4\13\t"+
		"\13\4\f\t\f\4\r\t\r\4\16\t\16\4\17\t\17\3\2\3\2\3\3\3\3\3\4\3\4\3\5\3"+
		"\5\3\5\3\5\3\5\3\5\3\5\3\6\3\6\3\7\3\7\3\7\3\b\3\b\3\t\3\t\3\n\6\n\67"+
		"\n\n\r\n\16\n8\3\13\3\13\3\13\3\13\3\13\3\13\3\13\3\13\5\13C\n\13\3\f"+
		"\6\fF\n\f\r\f\16\fG\3\f\3\f\3\r\3\r\3\r\7\rO\n\r\f\r\16\rR\13\r\3\16\3"+
		"\16\3\17\3\17\2\2\20\3\3\5\4\7\5\t\6\13\7\r\b\17\t\21\n\23\13\25\f\27"+
		"\r\31\16\33\2\35\2\3\2\7\3\2\62;\4\2>>@@\5\2,-//\61\61\6\2\13\f\17\17"+
		"\"\"==\5\2C\\aac|\2\\\2\3\3\2\2\2\2\5\3\2\2\2\2\7\3\2\2\2\2\t\3\2\2\2"+
		"\2\13\3\2\2\2\2\r\3\2\2\2\2\17\3\2\2\2\2\21\3\2\2\2\2\23\3\2\2\2\2\25"+
		"\3\2\2\2\2\27\3\2\2\2\2\31\3\2\2\2\3\37\3\2\2\2\5!\3\2\2\2\7#\3\2\2\2"+
		"\t%\3\2\2\2\13,\3\2\2\2\r.\3\2\2\2\17\61\3\2\2\2\21\63\3\2\2\2\23\66\3"+
		"\2\2\2\25B\3\2\2\2\27E\3\2\2\2\31K\3\2\2\2\33S\3\2\2\2\35U\3\2\2\2\37"+
		" \7*\2\2 \4\3\2\2\2!\"\7.\2\2\"\6\3\2\2\2#$\7+\2\2$\b\3\2\2\2%&\7t\2\2"+
		"&\'\7g\2\2\'(\7v\2\2()\7w\2\2)*\7t\2\2*+\7p\2\2+\n\3\2\2\2,-\7?\2\2-\f"+
		"\3\2\2\2./\7k\2\2/\60\7h\2\2\60\16\3\2\2\2\61\62\7}\2\2\62\20\3\2\2\2"+
		"\63\64\7\177\2\2\64\22\3\2\2\2\65\67\t\2\2\2\66\65\3\2\2\2\678\3\2\2\2"+
		"8\66\3\2\2\289\3\2\2\29\24\3\2\2\2:C\t\3\2\2;<\7?\2\2<C\7?\2\2=C\t\4\2"+
		"\2>?\7>\2\2?C\7?\2\2@A\7@\2\2AC\7?\2\2B:\3\2\2\2B;\3\2\2\2B=\3\2\2\2B"+
		">\3\2\2\2B@\3\2\2\2C\26\3\2\2\2DF\t\5\2\2ED\3\2\2\2FG\3\2\2\2GE\3\2\2"+
		"\2GH\3\2\2\2HI\3\2\2\2IJ\b\f\2\2J\30\3\2\2\2KP\5\33\16\2LO\5\33\16\2M"+
		"O\5\35\17\2NL\3\2\2\2NM\3\2\2\2OR\3\2\2\2PN\3\2\2\2PQ\3\2\2\2Q\32\3\2"+
		"\2\2RP\3\2\2\2ST\t\6\2\2T\34\3\2\2\2UV\t\2\2\2V\36\3\2\2\2\b\28BGNP\3"+
		"\b\2\2";
	public static final ATN _ATN =
		new ATNDeserializer().deserialize(_serializedATN.toCharArray());
	static {
		_decisionToDFA = new DFA[_ATN.getNumberOfDecisions()];
		for (int i = 0; i < _ATN.getNumberOfDecisions(); i++) {
			_decisionToDFA[i] = new DFA(_ATN.getDecisionState(i), i);
		}
	}
}