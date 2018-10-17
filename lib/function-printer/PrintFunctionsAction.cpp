#include "clang/AST/AST.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include <clang/Frontend/FrontendPluginRegistry.h>
#include "clang/AST/ASTContext.h"
#include "clang/Basic/SourceLocation.h"
#include "clang/Basic/SourceManager.h"
#include "clang/AST/ASTContext.h"

#include "PrintFunctionsAction.h"

using namespace printfunctions;

static clang::FrontendPluginRegistry::Add<PrintFunctionsAction>
  X("function-printer-demo", "Print the names of functions inside the file.");


class FunctionNameVisitor :
  public clang::RecursiveASTVisitor<FunctionNameVisitor> {

public:
  clang::ASTContext *context;

  bool
  VisitFunctionDecl(clang::FunctionDecl *f) {
    // Check to see if the function source start location is within the main file
    if(!(this->context)->getSourceManager().isInMainFile(f->getSourceRange().getBegin())) return true;
    llvm::outs() << "Function "
                 << (f->hasBody() ? "Def" : "Decl")
                 << " "
                 << f->getNameInfo().getName()
                 << "\n";
    return true;
  }
};


void
PrintFunctionsAction::EndSourceFileAction() {
  auto &ci      = getCompilerInstance();
  auto &context = ci.getASTContext();

  auto &input = getCurrentInput();
  std::string fileName = input.getFile();
  llvm::outs() << "Filename in Action: " << fileName << "\n";

  auto *unit = context.getTranslationUnitDecl();
  FunctionNameVisitor visitor;
  visitor.context = &context;
  visitor.TraverseDecl(unit);

  clang::ASTFrontendAction::EndSourceFileAction();
}
