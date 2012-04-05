// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#pragma once
#include "global.h"

#include "Basics/Task.h"

// ----- FUNCTION TASK 0 -----
template<class F>
class FunctionTask0 : public Task
{
public:
	FunctionTask0(F f) : m_f(f) {}
	virtual void execute() { m_f(); }
	F m_f;
};

template<class F>
HTask buildFunctionTask(F f)
{
	return NewObject(FunctionTask0<F>,f);
}

// ----- FUNCTION TASK 1 -----
template<class F, class P1>
class FunctionTask1 : public Task
{
public:
	FunctionTask1(F f, P1 p1) : m_f(f), m_p1(p1) {}
	virtual void execute() { m_f(m_p1); }
	F m_f; P1 m_p1;
};

template<class F, class P1>
HTask buildFunctionTask(F f, P1 p1)
{
	return NewObject(TemplateInst(FunctionTask1,F,P1),f,p1);
}

// ----- FUNCTION TASK 2 -----
template<class F, class P1, class P2 >
class FunctionTask2 : public Task
{
public:
	FunctionTask2(F f, P1 p1, P2 p2)
		: m_f(f), m_p1(p1), m_p2(p2) {}
	virtual void execute() { m_f(m_p1,m_p2); }
	F m_f; P1 m_p1; P2 m_p2;;
};

template<class F, class P1, class P2 >
HTask buildFunctionTask(F f, P1 p1, P2 p2)
{
	return NewObject(TemplateInst(FunctionTask2,F,P1,P2),f,p1,p2);
}

// ----- FUNCTION TASK 3 -----
template<class F, class P1, class P2, class P3 >
class FunctionTask3 : public Task
{
public:
	FunctionTask3(F f, P1 p1, P2 p2, P3 p3)
		: m_f(f), m_p1(p1), m_p2(p2), m_p3(p3) {}
	virtual void execute() { m_f(m_p1,m_p2,m_p3); }
	F m_f; P1 m_p1; P2 m_p2; P3 m_p3;;
};

template<class F, class P1, class P2, class P3 >
HTask buildFunctionTask(F f, P1 p1, P2 p2, P3 p3)
{
	return NewObject(TemplateInst(FunctionTask3,F,P1,P2,P3),f,p1,p2,p3);
}

// ----- FUNCTION TASK 4 -----
template<class F, class P1, class P2, class P3, class P4 >
class FunctionTask4 : public Task
{
public:
	FunctionTask4(F f, P1 p1, P2 p2, P3 p3, P4 p4)
		: m_f(f), m_p1(p1), m_p2(p2), m_p3(p3), m_p4(p4) {}
	virtual void execute() { m_f(m_p1,m_p2,m_p3,m_p4); }
	F m_f; P1 m_p1; P2 m_p2; P3 m_p3; P4 m_p4;;
};

template<class F, class P1, class P2, class P3, class P4 >
HTask buildFunctionTask(F f, P1 p1, P2 p2, P3 p3, P4 p4)
{
	return NewObject(TemplateInst(FunctionTask4,F,P1,P2,P3,P4),f,p1,p2,p3,p4);
}

// ----- FUNCTION TASK 5 -----
template<class F, class P1, class P2, class P3, class P4, class P5 >
class FunctionTask5 : public Task
{
public:
	FunctionTask5(F f, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5)
		: m_f(f), m_p1(p1), m_p2(p2), m_p3(p3), m_p4(p4), m_p5(p5) {}
	virtual void execute() { m_f(m_p1,m_p2,m_p3,m_p4,m_p5); }
	F m_f; P1 m_p1; P2 m_p2; P3 m_p3; P4 m_p4; P5 m_p5;;
};

template<class F, class P1, class P2, class P3, class P4, class P5 >
HTask buildFunctionTask(F f, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5)
{
	return NewObject(TemplateInst(FunctionTask5,F,P1,P2,P3,P4,P5),f,p1,p2,p3,p4,p5);
}

// ----- FUNCTION TASK 6 -----
template<class F, class P1, class P2, class P3, class P4, class P5, class P6 >
class FunctionTask6 : public Task
{
public:
	FunctionTask6(F f, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6)
		: m_f(f), m_p1(p1), m_p2(p2), m_p3(p3), m_p4(p4), m_p5(p5), m_p6(p6) {}
	virtual void execute() { m_f(m_p1,m_p2,m_p3,m_p4,m_p5,m_p6); }
	F m_f; P1 m_p1; P2 m_p2; P3 m_p3; P4 m_p4; P5 m_p5; P6 m_p6;;
};

template<class F, class P1, class P2, class P3, class P4, class P5, class P6 >
HTask buildFunctionTask(F f, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6)
{
	return NewObject(TemplateInst(FunctionTask6,F,P1,P2,P3,P4,P5,P6),f,p1,p2,p3,p4,p5,p6);
}

// ----- FUNCTION TASK 7 -----
template<class F, class P1, class P2, class P3, class P4, class P5, class P6, class P7 >
class FunctionTask7 : public Task
{
public:
	FunctionTask7(F f, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7)
		: m_f(f), m_p1(p1), m_p2(p2), m_p3(p3), m_p4(p4), m_p5(p5), m_p6(p6), m_p7(p7) {}
	virtual void execute() { m_f(m_p1,m_p2,m_p3,m_p4,m_p5,m_p6,m_p7); }
	F m_f; P1 m_p1; P2 m_p2; P3 m_p3; P4 m_p4; P5 m_p5; P6 m_p6; P7 m_p7;;
};

template<class F, class P1, class P2, class P3, class P4, class P5, class P6, class P7 >
HTask buildFunctionTask(F f, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7)
{
	return NewObject(TemplateInst(FunctionTask7,F,P1,P2,P3,P4,P5,P6,P7),f,p1,p2,p3,p4,p5,p6,p7);
}

// ----- FUNCTION TASK 8 -----
template<class F, class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8 >
class FunctionTask8 : public Task
{
public:
	FunctionTask8(F f, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8)
		: m_f(f), m_p1(p1), m_p2(p2), m_p3(p3), m_p4(p4), m_p5(p5), m_p6(p6), m_p7(p7), m_p8(p8) {}
	virtual void execute() { m_f(m_p1,m_p2,m_p3,m_p4,m_p5,m_p6,m_p7,m_p8); }
	F m_f; P1 m_p1; P2 m_p2; P3 m_p3; P4 m_p4; P5 m_p5; P6 m_p6; P7 m_p7; P8 m_p8;;
};

template<class F, class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8 >
HTask buildFunctionTask(F f, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8)
{
	return NewObject(TemplateInst(FunctionTask8,F,P1,P2,P3,P4,P5,P6,P7,P8),f,p1,p2,p3,p4,p5,p6,p7,p8);
}

// ----- FUNCTION TASK 9 -----
template<class F, class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8, class P9 >
class FunctionTask9 : public Task
{
public:
	FunctionTask9(F f, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8, P9 p9)
		: m_f(f), m_p1(p1), m_p2(p2), m_p3(p3), m_p4(p4), m_p5(p5), m_p6(p6), m_p7(p7), m_p8(p8), m_p9(p9) {}
	virtual void execute() { m_f(m_p1,m_p2,m_p3,m_p4,m_p5,m_p6,m_p7,m_p8,m_p9); }
	F m_f; P1 m_p1; P2 m_p2; P3 m_p3; P4 m_p4; P5 m_p5; P6 m_p6; P7 m_p7; P8 m_p8; P9 m_p9;;
};

template<class F, class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8, class P9 >
HTask buildFunctionTask(F f, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8, P9 p9)
{
	return NewObject(TemplateInst(FunctionTask9,F,P1,P2,P3,P4,P5,P6,P7,P8,P9),f,p1,p2,p3,p4,p5,p6,p7,p8,p9);
}

// ----- FUNCTION TASK 10 -----
template<class F, class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8, class P9, class P10 >
class FunctionTask10 : public Task
{
public:
	FunctionTask10(F f, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8, P9 p9, P10 p10)
		: m_f(f), m_p1(p1), m_p2(p2), m_p3(p3), m_p4(p4), m_p5(p5), m_p6(p6), m_p7(p7), m_p8(p8), m_p9(p9), m_p10(p10) {}
	virtual void execute() { m_f(m_p1,m_p2,m_p3,m_p4,m_p5,m_p6,m_p7,m_p8,m_p9,m_p10); }
	F m_f; P1 m_p1; P2 m_p2; P3 m_p3; P4 m_p4; P5 m_p5; P6 m_p6; P7 m_p7; P8 m_p8; P9 m_p9; P10 m_p10;;
};

template<class F, class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8, class P9, class P10 >
HTask buildFunctionTask(F f, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8, P9 p9, P10 p10)
{
	return NewObject(TemplateInst(FunctionTask10,F,P1,P2,P3,P4,P5,P6,P7,P8,P9,P10),f,p1,p2,p3,p4,p5,p6,p7,p8,p9,p10);
}

