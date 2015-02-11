// From qtcreator/src/plugins/clangcodemodel/raii/scopedclangoptions.cpp

#include "clangutils.h"

namespace Utils {

/**
 * @class ClangCodeModel::ScopedClangOptions
 * @brief Converts QStringList to raw options, acceptable by clang-c parsing and indexing API
 */

ScopedClangOptions::ScopedClangOptions(const QStringList &options)
    : m_size(options.size())
    , m_rawOptions(new const char*[options.size()])
{
    for (int i = 0 ; i < m_size; ++i)
        m_rawOptions[i] = qstrdup(options[i].toUtf8());
}

ScopedClangOptions::~ScopedClangOptions()
{
    for (int i = 0; i < m_size; ++i)
        delete[] m_rawOptions[i];
    delete[] m_rawOptions;
}

const char **ScopedClangOptions::data() const
{
    return m_rawOptions;
}

int ScopedClangOptions::size() const
{
    return m_size;
}

/**
 * @class ClangCodeModel::SharedClangOptions
 * @brief Shared wrapper around \a {ClangCodeModel::ScopedClangOptions} ScopedClangOptions
 */

SharedClangOptions::SharedClangOptions()
    : d(0)
{
}

SharedClangOptions::SharedClangOptions(const QStringList &options)
    : d(new ScopedClangOptions(options))
{
}

/**
 * @return Replaces options with new options list
 */
void SharedClangOptions::reloadOptions(const QStringList &options)
{
    d = QSharedPointer<ScopedClangOptions>(new ScopedClangOptions(options));
}

/**
 * @return Pointer to clang raw options or NULL if uninitialized
 */
const char **SharedClangOptions::data() const
{
    return d ? d->data() : 0;
}

/**
 * @return Options count or 0 if uninitialized
 */
int SharedClangOptions::size() const
{
    return d ? d->size() : 0;
}

} // namespace Utils
