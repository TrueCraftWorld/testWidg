// #ifndef THREADWRAPPER_H
// #define THREADWRAPPER_H

// #include <QObject>
// #include <QThread>
// #include <pathfinding.h>


// class Controller : public QObject
// {
//     Q_OBJECT
//     QThread workerThread;
// public:
//     Controller(PathSearch* path) {

//         path->moveToThread(&workerThread);
//         connect(&workerThread, &QThread::finished, path, &QObject::deleteLater);
//         connect(this, &Controller::operate, path, &PathSearch::findPath);
//         connect(path, &PathSearch::pathFound, this, &Controller::handleResults);
//         workerThread.start();
//     }
//     ~Controller() {
//         workerThread.quit();
//         workerThread.wait();
//     }
// public slots:
//     void handleResults(bool);
// signals:
//     void operate(UserMap &, QPoint, QPoint);
//     void results(bool);
// };

// #endif // THREADWRAPPER_H
