//===- Hello.cpp - Example code from "Writing an LLVM Pass" ---------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file implements two versions of the LLVM "Hello World" pass described
// in docs/WritingAnLLVMPass.html
//
//===----------------------------------------------------------------------===//

#include "llvm/ADT/Statistic.h"
#include "llvm/IR/Function.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"

#include "llvm/IR/InlineAsm.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/IRBuilder.h"


using namespace llvm;

#define DEBUG_TYPE "MyIRPass"

bool  isFunctionPointerTy(Type *type){
      // Check the type here
     if(PointerType *pointerType=dyn_cast<PointerType>(type)){
        return isFunctionPointerTy(pointerType->getElementType());
     }
        //Exit Condition
        else if(type->isFunctionTy()){
        return  true;
        }
       return false;
  }

namespace {
  // Hello - The first implementation, without getAnalysisUsage.
  struct MyIRPass : public FunctionPass {
    static char ID; // Pass identification, replacement for typeid
    MyIRPass() : FunctionPass(ID) {}

    bool runOnFunction(Function &F) override {

      for (Function::iterator bb = F.begin(), bbe = F.end(); bb != bbe; ++bb) {
        BasicBlock& b = *bb;
        for (BasicBlock::iterator i = b.begin(), ie = b.end(); i != ie; ++i) {
          errs()<<*i<<"\n";
          if (i->getOpcode() == 33) {
            Value *v2 = i->getOperand(1);
            Value *v1 = i->getOperand(0);
            Type *t = v2->getType();
            if (isFunctionPointerTy(t)) {
              IRBuilder<> Builder1(&*bb, ++i);
              llvm::SmallVector<llvm::Value*, 1> indexVector;
              llvm::Value *const_0 = llvm::ConstantInt::get(llvm::IntegerType::getInt32Ty(bb->getContext()), 0);
              indexVector.push_back(const_0);
              Value *funp = Builder1.CreateLoad(v1->getType(), v2);
              Value *address = Builder1.CreateGEP(v2, indexVector);
              
              std::vector<llvm::Type *> AsmArgTypes;
              AsmArgTypes.push_back(funp->getType());
              AsmArgTypes.push_back(address->getType());
              llvm::FunctionType *AsmFTy = llvm::FunctionType::get(funp->getType(), AsmArgTypes, false);
              llvm::InlineAsm *IA = llvm::InlineAsm::get(AsmFTy, "custom_3 $0, $1, $2", "=r,r,r", true);
              std::vector<llvm::Value *> AsmArg;

              AsmArg.push_back(funp);
              AsmArg.push_back(address);
              Value *result = Builder1.CreateCall(IA, AsmArg);
              Builder1.CreateStore(result, v2);
              --i;
            }
		
          } 
          else if (i->getOpcode() == 32) {
            Value *v1 = i->getOperand(0);
            Type *t1 = v1->getType();
            Type *t2 = i->getType();
            if (isFunctionPointerTy(t1)) {
              IRBuilder<> Builder1(&*bb, i++);
              llvm::SmallVector<llvm::Value*, 1> indexVector;
              llvm::Value *const_0 = llvm::ConstantInt::get(llvm::IntegerType::getInt32Ty(bb->getContext()), 0);
              indexVector.push_back(const_0);
              Value *funp = Builder1.CreateLoad(t2, v1);
              Value *address = Builder1.CreateGEP(v1, indexVector);

              std::vector<llvm::Type *> AsmArgTypes;
              AsmArgTypes.push_back(t2);
              AsmArgTypes.push_back(address->getType());
              llvm::FunctionType *AsmFTy = llvm::FunctionType::get(t2, AsmArgTypes, false);
              llvm::InlineAsm *IA = llvm::InlineAsm::get(AsmFTy, "custom_3 $0, $1, $2", "=r,r,r", true);
              std::vector<llvm::Value *> AsmArg;
              AsmArg.push_back(funp);
              AsmArg.push_back(address);
              Value *result = Builder1.CreateCall(IA, AsmArg);
              Builder1.CreateStore(result, v1);
              i--;
              errs()<<"where the pointer is  "<<*i<< "\n";
            }


          }
        }
      }
      return true;
    }
  };
}

char MyIRPass::ID = 0;
static RegisterPass<MyIRPass> X("MyIRPass", "My IR Pass");

static RegisterStandardPasses Y(
	PassManagerBuilder::EP_EarlyAsPossible,
	[](const PassManagerBuilder &Builder,
	legacy::PassManagerBase &PM) { PM.add(new MyIRPass());});	



















