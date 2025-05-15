import slangpy as spy
import numpy as np
import pathlib

# Create an SGL device -- this automatically handles setup and invocation
# of the Slang compiler.

device = spy.Device()

# Load the Slang module
#module = spy.Module.load_from_file(device, "compute_interfaces_example.slang")

program = device.load_program(module_name="compute_interfaces_example.slang", entry_point_names=["computeMain"])
kernel = device.create_compute_kernel(program=program)

buffer_a = device.create_buffer(element_count = 32,
                                struct_type=kernel.reflection.computeMain.a,
                                usage=spy.BufferUsage.unordered_access,
                                data=np.random.rand(32).astype(np.float32),)
buffer_b = device.create_buffer(element_count = 32,
                                struct_type=kernel.reflection.computeMain.b,
                                usage=spy.BufferUsage.unordered_access,
                                data=np.linspace(0,32,32, dtype=np.int32),)

print("Buffer A before dispatch: \n")
data = buffer_a.to_numpy().view(np.float32)
print(data)

kernel.dispatch(a=buffer_a, b=buffer_b, processorChoice=0,thread_count=[32,1,1])

print ("\nBuffer A after dispatch 0: \n")
data = buffer_a.to_numpy().view(np.float32)
print(data)

kernel.dispatch(a=buffer_a, b=buffer_b, processorChoice=1,thread_count=[32,1,1])

print ("\nBuffer A after dispatch 1: \n")
data = buffer_a.to_numpy().view(np.float32)
print(data)

#--------

print ("\nBuffer B before dispatch 2: \n")
data = buffer_b.to_numpy().view(np.int32)
print(data)

kernel.dispatch(a=buffer_a, b=buffer_b, processorChoice=2, thread_count=[32,1,1])

print ("\nBuffer B after dispatch 2: \n")
data = buffer_b.to_numpy().view(np.int32)
print(data)

kernel.dispatch(a=buffer_a, b=buffer_b, processorChoice=3, thread_count=[32,1,1])

print ("\nBuffer B after dispatch 3: \n")
data = buffer_b.to_numpy().view(np.int32)
print(data)