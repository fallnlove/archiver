#include <iostream>

#include "decode/decode.h"
#include "encode/encode.h"
#include "exceptions.h"
#include "parser/parser_args.h"

int main(int argc, char** argv) {
    ParserArgs parser(argc, argv);
    parser.SetFlag("-c", 2);
    parser.SetFlag("-d", 1);
    parser.SetFlag("-h", 0);
    try {
        if (parser.GetFlag() == "-c") {
            Encode encode(parser.GetFirstArgument(), parser.GetArgumentsFrom(2));
            encode.StartEncoding();
        } else if (parser.GetFlag() == "-d") {
            Decode decode(parser.GetFirstArgument());
            decode.StartDecoding();
        } else {
            std::cout << "Use flag -c to archive file, first argument should be archive name, after you should print"
                         " file names, you can archive more than one file in one archive. Archive will save in current"
                         " directory\n"
                         "Use flag -d to unzip file, first argument should be archive name. Files will will save in"
                         " current directory\n"
                         "Use flag -h to see this manual"
                      << std::endl;
        }
    } catch (std::exception& exc) {
        std::cout << exc.what() << "\n";
        return 111;
    } catch (...) {
        std::cout << "Undefined error" << std::endl;
        return 111;
    }
    return 0;
}
