#include "remotecompilerqtversionfactory.h"
#include "remotecompilerconstants.h"
#include "remotecompilerqtversion.h"

#include <utils/qtcassert.h>
#include <projectexplorer/abi.h>


namespace RemoteCompiler {
namespace Internal {


RemoteCompilerQtVersionFactory::RemoteCompilerQtVersionFactory(QObject *parent)
    : QtSupport::QtVersionFactory(parent)
{
}

bool RemoteCompilerQtVersionFactory::canRestore(const QString &type)
{
    return type == QLatin1String(Constants::REMOTE_COMPILER_QT);
}

QtSupport::BaseQtVersion *RemoteCompilerQtVersionFactory::restore(const QString &type, const QVariantMap &data)
{
    QTC_ASSERT(canRestore(type), return 0);
    RemoteCompilerQtVersion *v = new RemoteCompilerQtVersion;
    v->fromMap(data);
    return v;
}

int RemoteCompilerQtVersionFactory::priority() const
{
    return 91;
}

QtSupport::BaseQtVersion *RemoteCompilerQtVersionFactory::create(const Utils::FileName &qmakePath, ProFileEvaluator *evaluator, bool isAutoDetected, const QString &autoDetectionSource)
{
    Q_UNUSED(evaluator)

    QStringList data = autoDetectionSource.split(QLatin1Char(':'));
    if(data.size() != 3 || data[0] != QLatin1String(Constants::REMOTE_COMPILER_QT_VERSION_DATA_HEADER))
        return 0;
    QString hostInfoStr = data[1], targetAbi = data[2];
    return new RemoteCompilerQtVersion(hostInfoStr, ProjectExplorer::Abi(targetAbi), qmakePath, isAutoDetected, autoDetectionSource);
}

} //RemoteCompiler
} //Internal
