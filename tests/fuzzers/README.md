# Hikogui Fuzz Testing

Fuzz testing is a process of testing an API with generated data.
The generated input data is either random and invalid (unstructured data) or semi-valid (structured data).
The goal of fuzz testing is to increase the code coverage by finding uncovered code paths (aka "corner cases").
The technique was invented by Barton Miller, a computer science professor of the University of Wisconsin-Madison.

When testing an API endpoint there is no return value from fuzzing.
But, non-zero return values are reserved for future use by libfuzzer.

A fuzz-test run is successful, when it does not crash.
When the fuzz-test crashes, a new uncovered code path was found.
The crash report contains the random input data, which triggered the crash,
and allows the reproduction of the error and debugging.

### Input Data

There a two forms of input data.
Unstructured data, which consists of random, invalid sequences (aka fuzz) and structured data, which are semi-valid sequences.
The fuzzing of structured data needs to take a certain data format into account and adds fuzz inside of the structure.
All compression and file-formats, e.g. zlib, json, png belong into this category.

#### How to split input data

See https://github.com/google/fuzzing/blob/master/docs/split-inputs.md#fuzzed-data-provider

### How to minimize the seed corpus

1. Go to build output folder of your fuzzers
2. Create folder for the new minimized corpus:
   mkdir fuzzer_name_minimized
   For example: "hikogui\build\tests\fuzzers\RelWithDebInfo\fuzz_base_n_minimized"
3. Run fuzzer with -merge=1 flag:
   ./fuzz_base_n -merge=1 ./fuzz_base_n_minimized E:\dev\hikogui-project\hikogui\tests\fuzzers\data\_corpus\fuzz_base_n


Lets run url_parser() fuzzing for 60 seconds using the dictionary and the corpus dir:

   ./fuzz_url_parser -max_total_time=60 -dict=E:\0-dev\0-hikogui-project\hikogui\tests\fuzzers\data\dictionary\fuzz_url_parser.txt E:\0-dev\0-hikogui-project\hikogui\tests\fuzzers\data\_corpus\fuzz_url_parser

Instead of re-building the minimized corpus into a new folder and then renaming it, like this:

  mkdir E:\0-dev\0-hikogui-project\hikogui\tests\fuzzers\data\_corpus\fuzz_url_parser_minimized & fuzz_url_parser -merge=1 e:\0-dev\0-hikogui-project\hikogui\tests\fuzzers\data\_corpus\fuzz_url_parser_minimized e:\0-dev\0-hikogui-project\hikogui\tests\fuzzers\data\_corpus\fuzz_url_parser

You rename the corpus folder and append "_nomini" to it's name:

   cd E:\0-dev\0-hikogui-project\hikogui\tests\fuzzers\data\_corpus\
   ren fuzz_url_parser fuzz_url_parser_nomini
   mkdir fuzz_url_parser

Then

fuzz_url_parser -merge=1 E:\0-dev\0-hikogui-project\hikogui\tests\fuzzers\data\_corpus\fuzz_url_parser E:\0-dev\0-hikogui-project\hikogui\tests\fuzzers\data\_corpus\fuzz_url_parser_nomini