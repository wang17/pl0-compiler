#ifndef DIAG
#define DIAG(X, Y)
#endif
#ifndef DIAG_ERROR
#define DIAG_ERROR(X, Y) DIAG(X, Y)
#endif

DIAG_ERROR(C001, "应为 \"=\" 而不是 \":=\" ")
DIAG_ERROR(C002, " \"=\" 后应该是数字")
DIAG_ERROR(C003, "常量标志符后应是 \"=\" ")
DIAG_ERROR(C004, "const、var、procedure 后应是标志符")
DIAG_ERROR(C005, "缺少分号 \";\" ")
DIAG_ERROR(C006, "过程说明后的符号不正确")
DIAG_ERROR(C007, "应为语句开始符号")
DIAG_ERROR(C008, "程序体内语句部分后的符号不正确")
DIAG_ERROR(C009, "缺少句点 \".\"")
DIAG_ERROR(C010, "应为标志符")
DIAG_ERROR(C011, "标志符未声明")
DIAG_ERROR(C012, "不可向常量或过程赋值")
DIAG_ERROR(C013, "应为赋值号 \":=\" ")
DIAG_ERROR(C014, "call 后应为标志符")
DIAG_ERROR(C015, "不可调用常量或变量")
DIAG_ERROR(C016, "缺少 then")
DIAG_ERROR(C017, "缺少 end")
DIAG_ERROR(C018, "缺少 do")
DIAG_ERROR(C019, "语句后的符号不正确")
DIAG_ERROR(C020, "应为关系运算符")
DIAG_ERROR(C021, "表达式内不可有过程标志符")
DIAG_ERROR(C022, "遗漏左括号\"(\"")
DIAG_ERROR(C023, "遗漏右括号\")\"")
DIAG_ERROR(C024, "因子后不可为此符号")
DIAG_ERROR(C025, "表达式不能以此符号开始")
DIAG_ERROR(C026, "嵌套层次太大")
DIAG_ERROR(C027, "应为左括号")
DIAG_ERROR(C028, "文件内容过多")
DIAG_ERROR(C029, "非预期的标志符")
DIAG_ERROR(C030, "不可为常量或过程")

#undef DIAG_ERROR
#undef DIAG
