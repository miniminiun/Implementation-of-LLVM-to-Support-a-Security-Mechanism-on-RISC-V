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

    for (auto &MBB : MF) {
        for (auto &MI : MBB) {
            MachineOpcode &code = MI.getOpcode();
            MachineOperand &ret = MI.getOperand(0);
            outs()<<code<<" "<<ret<<"\n";
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