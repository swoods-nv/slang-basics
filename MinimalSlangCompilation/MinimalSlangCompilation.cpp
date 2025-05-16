// MinimalSlangCompilation.cpp 

#include <iostream>
#include <vector>
#include <slang.h>

int main()
{
    // Create the global session - basically, instantiating the compiler & its API,
    // allowing it to load any downstream libraries or tools needed.
    // Most projects will do this only once during execution.
    slang::IGlobalSession* slangGlobalSession;
    slang::createGlobalSession(&slangGlobalSession);

    // We then need to set up a "local session", which is the context for running the 
    // Slang compiler. Think of this as packaging up all the information you need to provide
    // to the compiler about what target you want to generate code for, any compiler flags
    // you want to provide, include library paths, stage and entry point information, etc.
    // Many projects will end up creating and destroying lots of sessions during execution,
    // to compile different things for different targets or with different options.

    // We store the options for our local session in a SessionDesc
    slang::SessionDesc sessionDesc = {};

    // There are a number of options that can be provided as part of the session description, 
    // including include/import search paths, one or more compilation targets,
    // preprocessor macros, and much more. You can find detail on all of these in the
    // documentation for the compilation API at 
    // https://docs.shader-slang.org/en/latest/external/slang/docs/user-guide/08-compiling.html

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

    // Now we'll use the compiler session to retrive the slang module, 
    // providing the source file as input. This loads the module but does
    // not perform linking or compilation.
    slang::IModule* slangModule = nullptr;

    slang::IBlob* diagnosticBlob = nullptr;
    slangModule = session->loadModule("myshader.slang", &diagnosticBlob);

    if (!slangModule)
    {
        std::cout << "Error loading Slang module" << std::endl;
        return -1;
    }

    slang::IEntryPoint* entryPoint;

    std::cout << "Loaded Slang module successfully!" << std::endl;

    // Now we can link our shader program. We're just using a simple 
    // single-module compute shader -- if we had multiple stages to 
    // chain together, or our code was spread across multiple modules,
    // we would need to load them, identify entry points, and compose
    // the modules into a single composite for linking.
    SlangResult result;
    slang::IComponentType* linkedProgram = nullptr;
    result = slangModule->link(&linkedProgram, &diagnosticBlob);
    if (SLANG_FAILED(result))
    {
        std::cout << "Linking failed!" << std::endl;
        return -1;
    }
    
    slang::IBlob* spirvCode;
    diagnosticBlob = nullptr;

    // Now that our module is linked, it's finally time to get the 
    // resulting target code! This is the step that actually causes
    // compilation.
    result = linkedProgram->getTargetCode(
        0, // targetIndex
        &spirvCode,
        &diagnosticBlob);

    if (SLANG_FAILED(result))
    {
        std::cout << "Compilation failed!!" << std::endl;
        return -1;
    }

    std::cout << "Successfully compiled " << spirvCode->getBufferSize() << " bytes of SPIR-V." << std::endl;

    // Note: This program does not clean up after any of its potential allocations, and
    // will leak memory. You'll want to be tidier in your own implementation!

}
