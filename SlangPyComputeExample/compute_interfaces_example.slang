// Example demonstrating Slang's generics and interfaces
// in a compute shader that processes data with different operations

// Interface for data operations
interface IOperation<T : IArithmetic>
{
    T process(T input);
}

// Concrete implementations
struct MultiplyByTwo<T : IArithmetic> : IOperation<T>
{
    T process(T input) { return input * T(2); }
}

struct Add<T : __BuiltinArithmeticType> : IOperation<T>
{
    T value;
    T process(T input) { return input + T(10); }
}

// Generic buffer processor
struct BufferProcessor<T : IArithmetic, TOperation : IOperation<T>>
{
    RWStructuredBuffer<T> buffer;
    TOperation operation;
    
    void processElement(uint index)
    {
        buffer[index] = operation.process(buffer[index]);
    }
}

[shader("compute")]
[numthreads(32, 1, 1)]
void computeMain(RWStructuredBuffer<float> a, RWStructuredBuffer<int> b, uniform uint processorChoice, uint tid: SV_DispatchThreadID)
{
    switch (processorChoice)
    {
    case 0:
        BufferProcessor<float, Add<float>> floatAdd;
        floatAdd.buffer = a;
        floatAdd.operation.value = 10.0;
        floatAdd.processElement(tid);
        break;
    case 1:
        BufferProcessor<float, MultiplyByTwo<float>> floatMult;
        floatMult.buffer = a;
        floatMult.processElement(tid);
        break;
    case 2:
        BufferProcessor<int, Add<int>> intAdd;
        intAdd.buffer = b;
        intAdd.operation.value = 5;
        intAdd.processElement(tid);
        break;
    case 3:
        BufferProcessor<int, MultiplyByTwo<int>> intMult;
        intMult.buffer = b;
        intMult.processElement(tid);
        break;
    default: // do nothing
        break;
    }
}


