#pragma once
#include <QString>

namespace MemoLib {

class ZipCompressor {

public:
    ZipCompressor();
    static void compress(const QString& name, const QString& srcDir);
    static void decompress(const QString& name, const QString& dstDir);
};

} // MemoLib
