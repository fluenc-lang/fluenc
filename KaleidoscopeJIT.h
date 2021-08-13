#ifndef KALEIDOSCOPEJIT_H
#define KALEIDOSCOPEJIT_H

#include <llvm/ADT/StringRef.h>
#include <llvm/ExecutionEngine/JITSymbol.h>
#include <llvm/ExecutionEngine/Orc/Core.h>
#include <llvm/ExecutionEngine/Orc/ExecutionUtils.h>
#include <llvm/ExecutionEngine/Orc/JITTargetMachineBuilder.h>
#include <llvm/ExecutionEngine/Orc/RTDyldObjectLinkingLayer.h>
#include <llvm/ExecutionEngine/Orc/IRCompileLayer.h>
#include <llvm/IR/DataLayout.h>
#include <llvm/IR/LLVMContext.h>

class KaleidoscopeJIT
{
	public:
		KaleidoscopeJIT(llvm::orc::JITTargetMachineBuilder targetMachineBuilder, llvm::DataLayout dataLayout);
		~KaleidoscopeJIT();

		static llvm::Expected<KaleidoscopeJIT *> create();

		llvm::Error addModule(llvm::orc::ThreadSafeModule threadSafeModule);
		llvm::Expected<llvm::JITEvaluatedSymbol> lookup(llvm::StringRef name);

	private:
		llvm::DataLayout m_dataLayout;

		llvm::orc::ExecutionSession m_session;
		llvm::orc::MangleAndInterner m_mangle;
		llvm::orc::RTDyldObjectLinkingLayer m_objectLayer;
		llvm::orc::IRCompileLayer m_compileLayer;
		llvm::orc::JITDylib &m_lib;
};


#endif // KALEIDOSCOPEJIT_H
