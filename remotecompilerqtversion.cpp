#include "remotecompilerqtversion.h"
#include "remotecompilerconstants.h"


namespace RemoteCompiler {
namespace Internal {

using namespace ProjectExplorer;

static const char REMOTE_COMPILER_QTVERSION_COMPILATION_HOST[] = "compilationHost";
static const char REMOTE_COMPILER_QTVERSION_TARGET_ABI[]= "targetAbi";


RemoteCompilerQtVersion::RemoteCompilerQtVersion()
    : QtSupport::BaseQtVersion()
{
}

RemoteCompilerQtVersion::RemoteCompilerQtVersion(const QString &hostInfoStr, const Abi &targetAbi, const Utils::FileName &path, bool isAutodetected, const QString &autodetectionSource)
    : QtSupport::BaseQtVersion(path, isAutodetected, autodetectionSource),
      m_hostInfoStr(hostInfoStr), m_targetAbi(targetAbi)
{
    setUnexpandedDisplayName(hostInfoStr + QString::fromLatin1(" : ") + targetAbi.toString());
}

RemoteCompilerQtVersion *RemoteCompilerQtVersion::clone() const
{
    return new RemoteCompilerQtVersion(*this);
}

QString RemoteCompilerQtVersion::type() const
{
    return QLatin1String(Constants::REMOTE_COMPILER_QT);
}

bool RemoteCompilerQtVersion::isValid() const
{
    if (!BaseQtVersion::isValid())
        return false;
    if (qtAbis().isEmpty())
        return false;
    return true;
}

QString RemoteCompilerQtVersion::invalidReason() const
{
    QString tmp = BaseQtVersion::invalidReason();
    if (tmp.isEmpty() && qtAbis().isEmpty())
        return tr("Failed to detect the ABIs used by the Qt version.");
    return tmp;
}

QList<Abi> RemoteCompilerQtVersion::detectQtAbis() const
{
    QList<Abi> abis {m_targetAbi};
    return abis;
}


QString RemoteCompilerQtVersion::description() const
{
    return tr("Remote");
}

QString RemoteCompilerQtVersion::platformName() const
{
    return QLatin1String("RemotePlatform");
}

QString RemoteCompilerQtVersion::platformDisplayName() const
{
    return QLatin1String(QT_TRANSLATE_NOOP("QtSupport", "Remote platform"));
}

void RemoteCompilerQtVersion::fromMap(const QVariantMap &map)
{
    BaseQtVersion::fromMap(map);
    m_hostInfoStr = map.value(QLatin1String(REMOTE_COMPILER_QTVERSION_COMPILATION_HOST)).toString();
    m_targetAbi = Abi(map.value(QLatin1String(REMOTE_COMPILER_QTVERSION_TARGET_ABI)).toString());
}

QVariantMap RemoteCompilerQtVersion::toMap() const
{
    QVariantMap ret = BaseQtVersion::toMap();
    ret.insert(QLatin1String(REMOTE_COMPILER_QTVERSION_COMPILATION_HOST), m_hostInfoStr);
    ret.insert(QLatin1String(REMOTE_COMPILER_QTVERSION_TARGET_ABI), m_targetAbi.toString());
    return ret;
}

bool RemoteCompilerQtVersion::isMyHost(const CompilationHostInfo &host) const
{
    return m_hostInfoStr == host.hostInfoStr() && m_targetAbi == host.abi;
}

bool RemoteCompilerQtVersion::isOutdated(const CompilationHostInfo &host) const
{
    return qmakeCommand().toString() != host.qmake;
}

} // namespace Internal
} // namespace RemoteCompiler
