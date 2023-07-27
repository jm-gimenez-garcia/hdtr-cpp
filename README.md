# C++ library for HDTr

HDTr is an extension of HDT for reified triples.

## Getting Started
### Prerequisites

The implementation has the following dependencies:
- [Serd v0.28+](http://drobilla.net/software/serd/) This enables importing RDF data in the Turtle and N-Triples serialization formats specifically. The dependency is activated by default.
- [libz](http://www.zlib.net/) Enables loading N-Triples files compressed with GZIP (e.g., `file.nt.gz`) and gzipped HDTs (`file.hdt.gz`). The dependency is activated by default.
- [Kyoto Cabinet](http://fallabs.com/kyotocabinet/) (optional) Enables generating big RDF datasets on machines without much RAM memory, by creating a temporary Kyoto Cabinet database. The dependency is deactivated by default; to activate it, call `configure` with `--with-kyoto=yes` flag during installation.

The installation process has the following dependencies:

- [autoconf](https://www.gnu.org/software/autoconf/autoconf.html)
- [libtool](https://www.gnu.org/software/libtool/)

The following commands should install both packages:

    sudo apt-get update
    sudo apt-get install autoconf libtool

### Installing

To compile and install, run the following commands under the directory `hdtr-cpp`. This will generate the library and tools.

First run the following script to generate all necessary installation files with autotools:

    ./autogen.sh

Then, run:

    ./configure
    make -j2

## Running

After building, these are the typical operations that you will perform (you need to change directory to `libhdt` first):
 
- Convert your RDF data to HDT:
    
    NB: the input stream is assumed to be valid n-quads, so you should validate your data before feeding it into rdf2hdt.
    
    ```
    $ tools/rdf2hdt data/tests.nq data/tests.hdt
    ```

- Create only the index of an HDT file:

    ```
    $ tools/hdtSearch -q 0 data/tests.hdt
    ```

- Open a terminal to search quad patterns within an HDT file:

    ```
    $ tools/hdtSearch data/tests.hdt

    >> ? ? ? http://example.org/G7
    http://example.org/C1 http://example.org/link1 http://example.org/G3 http://example.org/G7

    1 results in 71
    
    >> http://example.org/G7 ? ? ?
    http://example.org/G7 http://example.org/link4 http://example.org/O1
    http://example.org/G7 http://example.org/link6 http://example.org/G9
    http://example.org/G7 http://example.org/link6 http://example.org/G2
    http://example.org/G7 http://example.org/link6 http://example.org/G3

    4 results in 366

    >> exit
    ```