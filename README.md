# Memory Emulator

How it works:

1. Restrict execution to NUMA node 0.
2. Optionally lock memory in node 0 to force allocation on remote node.
3. Start the target application.
4. While it is running, print memory usage statistics every second.

Interleaving of memory allocations can be enabled using the `-i` option.

## Restricting local memory

The amount of local memory can be restricted using the `-l` option. The argument to this option specifies how much local memory should be left free for the target application to use. Note that there is also some overhead to account for as not all memory can be used by the application. Initial results on dt1 indicate that an overhead of 1.75 GB should be added to this number.

If the emulator dies with the message `Killed`, then it probably tried to lock more memory than is available in the node. Try to increase the `-l` number.

## Command line reference

```
./emu [options] ./application arg1 arg2

Emulation parameters:
-l N    Lock local memory, leaving N bytes free. Use k/m/g suffix for KB/MB/GB.
-i      Interleave memory allocations.

Memory profiling parameters:
-m      Enable memory profiling, disable emulation.
-t      Sampling interval (seconds).
-S pat  Start profiler when pattern matches application stdout.
-E pat  Stop profiler when pattern matches application stdout.
```

## Example Applications

Applications can be found in the folder /examples. 

## Publications

```Wahlgren, J., Gokhale, M., & Peng, I. B. (2022). Evaluating Emerging CXL-enabled Memory Pooling for HPC Systems. In 2022 IEEE/ACM Workshop on Memory Centric High Performance Computing (MCHPC'22). IEEE.``` [PDF](https://arxiv.org/pdf/2211.02682)

## License

- The license is [LGPL](/LICENSE).

## Contact

- Jacob Wahlgren (jacobwah@kth.se)
- Ivy Peng  (bopeng@kth.se)

