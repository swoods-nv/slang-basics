Configuration notes:

- This minimal Slang usage example depends on the Slang libraries. You will need to download a
Slang binary release package from https://github.com/shader-slang/slang/releases
- In Visual studio, you will need to set the following configuration properties:
  - Configuration Properties > C/C++ > Additional Include Directories (add the location of the include directory from the Slang release)
  - Configuration Properties > Linker > Additional Library Directories (add the location of the lib directory from the Slang release)
  - Configuration Properties > Debugging > Environment (prepend or append the location of the bin directory from the Slang release to PATH)