#ifndef REMOTECOMPILERQTVERSION_H
#define REMOTECOMPILERQTVERSION_H

#include "remotecompilerconfigurations.h"

#include <qtsupport/baseqtversion.h>
#include <QCoreApplication>
#include <projectexplorer/abi.h>

namespace RemoteCompiler {
namespace Internal {

class RemoteCompilerQtVersion : public QtSupport::BaseQtVersion
{
    Q_DECLARE_TR_FUNCTIONS(RemoteCompiler::Internal::RemoteCompilerQtVersion)

    QString m_hostInfoStr;
    ProjectExplorer::Abi m_targetAbi;

public:
    RemoteCompilerQtVersion();
    RemoteCompilerQtVersion(const QString &hostInfoStr,
                            const ProjectExplorer::Abi &targetAbi,
                            const Utils::FileName &path, bool isAutodetected = false,
                            const QString &autodetectionSource = QString());

    RemoteCompilerQtVersion *clone() const;
    QString type() const;
    bool isValid() const;
    QString invalidReason() const;
    QList<ProjectExplorer::Abi> detectQtAbis() const;
    QString platformName() const;
    QString platformDisplayName() const;
    QString description() const;
    void fromMap(const QVariantMap &map);
    QVariantMap toMap() const;

    bool isMyHost(const CompilationHostInfo &host) const;
    bool isOutdated(const CompilationHostInfo &host) const;
};

} // namespace Internal
} // namespace RemoteCompiler

#endif // REMOTECOMPILERQTVERSION_H
