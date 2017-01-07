#ifndef REMOTECOMPILERQTVERSIONFACTORY_H
#define REMOTECOMPILERQTVERSIONFACTORY_H

#include <qtsupport/qtversionfactory.h>

namespace RemoteCompiler {
namespace Internal {

class RemoteCompilerQtVersionFactory : public QtSupport::QtVersionFactory
{
public:
    explicit RemoteCompilerQtVersionFactory(QObject *parent = 0);

    bool canRestore(const QString &type);
    QtSupport::BaseQtVersion *restore(const QString &type, const QVariantMap &data);
    int priority() const;
    QtSupport::BaseQtVersion *create(const Utils::FileName &qmakePath, ProFileEvaluator *evaluator,
                                     bool isAutoDetected = false, const QString &autoDetectionSource = QString());
};

} //RemoteCompiler
} //Internal

#endif // REMOTECOMPILERQTVERSIONFACTORY_H
