#pragma once
#include <QString>

namespace Common {

class ZipCompressor {

public:
    static void compress(const QString& name, const QString& srcDir);
    static void decompress(const QString& name, const QString& dstDir);
};
} // namespace Common
