// MinimalSlangCompilation.cpp 

#include <iostream>
#include <vector>
#include <slang.h>

int main()
{
    slang::IGlobalSession* slangGlobalSession;
    slang::createGlobalSession(&slangGlobalSession);

    slang::SessionDesc sessionDesc = {};

    // Set our compile target as SPIR-V 1.5
    slang::TargetDesc targetDesc = {};
    targetDesc.format = SLANG_SPIRV;
    targetDesc.profile = slangGlobalSession->findProfile("spirv_1_5");
    targetDesc.flags = 0;

    sessionDesc.targets = &targetDesc;
    sessionDesc.targetCount = 1;

    sessionDesc.compilerOptionEntryCount = 0; // using the default compiler options

    slang::ISession *session;
    slangGlobalSession->createSession(sessionDesc, &session);

    slang::IModule* slangModule = nullptr;

    slang::IBlob* diagnosticBlob = nullptr;
    slangModule = session->loadModule("myshader.slang", &diagnosticBlob);

    if (!slangModule)
    {
        std::cout << "Error loading Slang module" << std::endl;
        return -1;
    }
    else
        std::cout << "Loaded Slang module successfully!" << std::endl;
}
