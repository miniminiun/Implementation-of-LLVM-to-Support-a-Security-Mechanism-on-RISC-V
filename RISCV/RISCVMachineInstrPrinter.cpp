#include "RISCV.h"
#include "RISCVInstrInfo.h"
#include "llvm/CodeGen/MachineFunctionPass.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/TargetRegisterInfo.h"

using namespace llvm;

#define RISCV_MACHINEINSTR_PRINTER_PASS_NAME "Dummy RISCV machineinstr printer pass"

namespace {

class RISCVMachineInstrPrinter : public MachineFunctionPass {
public:
    static char ID;

    RISCVMachineInstrPrinter() : MachineFunctionPass(ID) {
        initializeRISCVMachineInstrPrinterPass(*PassRegistry::getPassRegistry());
    }

    bool runOnMachineFunction(MachineFunction &MF) override;

    StringRef getPassName() const override { return RISCV_MACHINEINSTR_PRINTER_PASS_NAME; }
};

char RISCVMachineInstrPrinter::ID = 0;

bool RISCVMachineInstrPrinter::runOnMachineFunction(MachineFunction &MF) {

    const TargetInstrInfo *XII = MF.getSubtarget().getInstrInfo(); // target instruction info
    DebugLoc DL;
    for (auto &MBB : MF) {
        for (auto MI = MBB.begin(); MI != MBB.end(); MI++) {
            unsigned code = MI->getOpcode();
            if (code == 487) {
                MachineOperand &ret = MI->getOperand(0);
                Register reg = ret.getReg();
                if (reg == RISCV::X1){
                    MachineOperand &ret2 = MI->getOperand(2);
                    int offset = ret2.getImm();
                    MI++;
                    MachineBasicBlock::iterator MBBI = BuildMI(MBB, *MI ,DL, XII->get(RISCV::SD), RISCV::X5)
                    .addReg(RISCV::X2)
                    .addImm(offset);
                    MBBI = BuildMI(MBB, MBBI ,DL, XII->get(RISCV::CUSTOM_3), RISCV::X5)
                    .addReg(RISCV::X5)
                    .addReg(RISCV::X6);
                    MBBI = BuildMI(MBB, MBBI ,DL, XII->get(RISCV::ADDI), RISCV::X6)
                    .addReg(RISCV::X2)
                    .addImm(offset);
                    MBBI = BuildMI(MBB, MBBI ,DL, XII->get(RISCV::LD), RISCV::X5)
                    .addReg(RISCV::X2)
                    .addImm(offset);    
                }
            } 
            if (code == 452) {
                MachineOperand &ret = MI->getOperand(0);
                Register reg = ret.getReg();
                if (reg == RISCV::X1){
                    MachineOperand &ret2 = MI->getOperand(2);
                    int offset = ret2.getImm();
                    MachineBasicBlock::iterator MBBI = BuildMI(MBB, *MI ,DL, XII->get(RISCV::SD), RISCV::X5)
                    .addReg(RISCV::X2)
                    .addImm(offset);
                    MBBI = BuildMI(MBB, MBBI ,DL, XII->get(RISCV::CUSTOM_3), RISCV::X5)
                    .addReg(RISCV::X5)
                    .addReg(RISCV::X6);
                    MBBI = BuildMI(MBB, MBBI ,DL, XII->get(RISCV::ADDI), RISCV::X6)
                    .addReg(RISCV::X2)
                    .addImm(offset);
                    MBBI = BuildMI(MBB, MBBI ,DL, XII->get(RISCV::LD), RISCV::X5)
                    .addReg(RISCV::X2)
                    .addImm(offset);    
                }
            }
            outs()<<*MI<<" "<<code<<"\n";
        }        
    }

    return false;
}

} // end of anonymous namespace

INITIALIZE_PASS(RISCVMachineInstrPrinter, "RISCV-machineinstr-printer",
    RISCV_MACHINEINSTR_PRINTER_PASS_NAME,
    true, // is CFG only?
    true  // is analysis?
)

namespace llvm {

FunctionPass *createRISCVMachineInstrPrinterPass() { return new RISCVMachineInstrPrinter(); }

}
