// From qtcreator/src/plugins/clangcodemodel/raii/scopedclangoptions.h

#ifndef CLANG_UTILS_H
#define CLANG_UTILS_H

#include <QStringList>
#include <QSharedPointer>

namespace Utils {

class ScopedClangOptions
{
public:
    ScopedClangOptions(const QStringList &options);
    ~ScopedClangOptions();

    const char **data() const;
    int size() const;

private:
    void release();

    int m_size;
    const char **m_rawOptions;
};

class SharedClangOptions
{
public:
    SharedClangOptions();
    SharedClangOptions(const QStringList &options);

    void reloadOptions(const QStringList &options);

    const char **data() const;
    int size() const;

private:
    QSharedPointer<ScopedClangOptions> d;
};

} // namespace Utils

#endif // Include guard
