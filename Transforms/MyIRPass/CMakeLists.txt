
add_llvm_library( LLVMMyIRPass MODULE BUILDTREE_ONLY
  MyIRPass.cpp

  PLUGIN_TOOL
  opt
  )
