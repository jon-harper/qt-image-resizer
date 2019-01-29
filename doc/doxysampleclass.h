#ifndef DOXYSAMPLECLASS_H
#define DOXYSAMPLECLASS_H

#include <QObject>
#include <QEvent>


/*!
 * \brief The DoxySampleClass class
 * Does various things to display attributes in Doxygen documentation.
 */
class DoxySampleClass : public QObject
{
    Q_OBJECT

    Q_PROPERTY(double propExample READ propExample WRITE setPropExample NOTIFY propExampleChanged)

    Q_ENUMS( EnumExample )
    Q_FLAGS( ExampleFlags ExampleFlag )

public:
    enum EnumExample {
        valueZero,
        valueOne,
        valueTwo
    };

    enum ExampleFlag {
        FlagOne = 0x1,
        FlagTwo = 0x2,
        FlagMask = 0x4
    };
    Q_DECLARE_FLAGS(ExampleFlags, ExampleFlag)

    class ExampleClass {
        ExampleClass() {}
    };

public:
    explicit DoxySampleClass(QObject *parent = 0);
    ~DoxySampleClass();

    double propExample() const;
    bool isFinished() const;
    bool isRunning() const;

    static void publicStaticExample();

    typedef double publicExampleType;

signals:
    void propExampleChanged(double val); //! Notifier for propExample property

public slots:
    void setPropExample(double val);

protected:
    typedef double protectedExampleType;

    static void protectedStaticExample();

    void protectedExample();
    virtual bool event(QEvent *ev);

protected slots:
    void protectedSlotExample();

private:
    typedef struct {
        int total;
        int finished;
        int success;
        int skipped;
        int error;
    } FileCount;

    typedef double privateExampleType;
    void privateExample();
    static void privateStaticExample();

    double propVal; //! Private storage for propExample property

    friend class QObject;
};

#endif // DOXYSAMPLECLASS_H
