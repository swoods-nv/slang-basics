// MinimalSlangCompilation.cpp 

#include <iostream>
#include <vector>
#include <slang.h>

int main()
{
    // Create the global session - basically, instantiating the compiler & its API
    slang::IGlobalSession* slangGlobalSession;
    slang::createGlobalSession(&slangGlobalSession);

    // Now, we set some attributes to describe how we want the Slang compiler to behave 
    // when we invoke it. We think of "Uses of Slang with these particular options" as a
    // (local) session, so we store the options in a SessionDesc
    slang::SessionDesc sessionDesc = {};

    // Set our compile target as SPIR-V 1.5
    slang::TargetDesc targetDesc = {};
    targetDesc.format = SLANG_SPIRV;
    targetDesc.profile = slangGlobalSession->findProfile("spirv_1_5");
    targetDesc.flags = 0;

    sessionDesc.targets = &targetDesc;
    sessionDesc.targetCount = 1;

    sessionDesc.compilerOptionEntryCount = 0; // otherwise using the default compiler options

    // Now we create the local session, and provide the description that includes target and
    // option information
    slang::ISession *session;
    slangGlobalSession->createSession(sessionDesc, &session);

    // Lastly, we'll use the compiler session to retrive the slang module, 
    // providing the source file as input.
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
