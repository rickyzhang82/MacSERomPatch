What
====

The app MacSERomPatch is a C++ program that initializes the compressed dispatch table address from Macintosh SE ROM `MD5: 9fb38bdcc0d53d9d380897ee53dc1322`. It replicates the M68K logic at the offset of `0x6DA` in the ROM file.

Mac SE ROM dispatch routine initialize Toolbox trap addresses to the memory location `0xE00` and OS trap addresses to `0x400`. The file under `data` folder stores the memory dump from Basilisk II. They are served as a benchmark to verify C++ program logic.

The input `data\MacSE-Dispatch-Table.compressed` is the compressed format dipatch addresses. It dumps from Macintosh SE ROM file at offset `0x1aa9c`.

How
===

Run `build.sh` script.

- MacSERomPatch. Take the compressed format dispatch address table as input and output the trap addresse.
- utils. Format the `e00` and `400` memory dump to cross compare the output and the benchmark. You can diff between two.
